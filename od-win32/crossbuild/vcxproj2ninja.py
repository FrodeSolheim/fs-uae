#!/usr/bin/env python3
"""Generate a ninja build file from WinUAE's MSVC .vcxproj files.

Two toolchains are supported, both running on a Linux host:

  clang   clang-cl / lld-link / llvm-lib / llvm-rc, with the MSVC CRT and Windows SDK from an xwin
          "splat" directory.
  msvc    the real Microsoft cl / link / lib / rc running under Wine, as installed by msvc-wine
          (https://github.com/mstorsjo/msvc-wine).

The CustomBuild steps use a native nasm, and the third party libraries come from WinUAE's
winuaeinclibs.zip (normally C:\\dev).

The .vcxproj files stay the single source of truth for the file lists and most compiler settings;
this script only translates them.
"""

import argparse
import glob
import os
import re
import shlex
import sys
import xml.etree.ElementTree as ET

NS = {"m": "http://schemas.microsoft.com/developer/msbuild/2003"}
MS = "{http://schemas.microsoft.com/developer/msbuild/2003}"

ARCHES = {
    # Platform -> (clang target, lld machine, xwin arch dir, nasm format)
    "x64": ("x86_64-pc-windows-msvc", "x64", "x86_64", "win64"),
    "Win32": ("i686-pc-windows-msvc", "x86", "x86", "win32"),
    "ARM64": ("aarch64-pc-windows-msvc", "arm64", "aarch64", None),
}


def tag(el):
    return el.tag[len(MS) :] if el.tag.startswith(MS) else el.tag


class Cond:
    """Tiny evaluator for the MSBuild conditions used in WinUAE's projects."""

    def __init__(self, props):
        self.props = props

    def expand(self, s):
        return re.sub(r"\$\((\w+)\)", lambda m: self.props.get(m.group(1), ""), s)

    def __call__(self, cond):
        if not cond:
            return True
        c = self.expand(cond).strip()
        if re.match(r"(?i)^!?exists\(", c):
            return False
        for op, joiner in ((r"\s+[Oo]r\s+", any), (r"\s+[Aa]nd\s+", all)):
            parts = re.split(op, c)
            if len(parts) > 1:
                return joiner(self(p) for p in parts)
        m = re.match(r"^'([^']*)'\s*(==|!=)\s*'([^']*)'$", c)
        if not m:
            raise ValueError("unsupported condition: %r" % cond)
        eq = m.group(1).lower() == m.group(3).lower()
        return eq if m.group(2) == "==" else not eq


class Project:
    def __init__(self, path, config, platform, extra_props):
        self.path = os.path.abspath(path)
        self.dir = os.path.dirname(self.path)
        self.root = ET.parse(path).getroot()
        self.props = {
            "Configuration": config,
            "Platform": platform,
            "ProjectDir": self.dir + "\\",
            "SolutionDir": self.dir + "\\",
            "ProjectName": os.path.splitext(os.path.basename(path))[0],
            "IntDir": "",
            "OutDir": "",
        }
        self.props.update(extra_props)
        self.cond = Cond(self.props)
        self._read_properties()
        self.defaults = self._read_item_definitions()

    def _read_properties(self):
        for pg in self.root.iterfind("m:PropertyGroup", NS):
            if not self.cond(pg.get("Condition")):
                continue
            for ch in pg:
                if not self.cond(ch.get("Condition")):
                    continue
                self.props[tag(ch)] = self.cond.expand((ch.text or "").strip())

    def _read_item_definitions(self):
        d = {}
        for g in self.root.iterfind("m:ItemDefinitionGroup", NS):
            if not self.cond(g.get("Condition")):
                continue
            for item in g:
                meta = d.setdefault(tag(item), {})
                for ch in item:
                    if self.cond(ch.get("Condition")):
                        meta[tag(ch)] = (ch.text or "").strip()
        return d

    def items(self, kind):
        """Yield (include, metadata) for active items, metadata merged with defaults."""
        for ig in self.root.iterfind("m:ItemGroup", NS):
            if not self.cond(ig.get("Condition")):
                continue
            for it in ig.iterfind("m:" + kind, NS):
                if not self.cond(it.get("Condition")):
                    continue
                meta = dict(self.defaults.get(kind, {}))
                for ch in it:
                    if not self.cond(ch.get("Condition")):
                        continue
                    name = tag(ch)
                    val = (ch.text or "").strip()
                    # %(Name) refers to the inherited value
                    inherited = meta.get(name, "")
                    val = val.replace("%%(%s)" % name, inherited)
                    meta[name] = val
                if meta.get("ExcludedFromBuild", "").lower() == "true":
                    continue
                yield it.get("Include"), meta

    def expand(self, s):
        return self.cond.expand(s)


# fmt: off
DEFAULT_LIBS = ["kernel32.lib", "user32.lib", "gdi32.lib", "winspool.lib", "comdlg32.lib",
                "advapi32.lib", "shell32.lib", "ole32.lib", "oleaut32.lib", "uuid.lib",
                "odbc32.lib", "odbccp32.lib"]
# fmt: on


def split_list(s):
    return [
        x.strip() for x in re.split(r";", s or "") if x.strip() and not x.strip().startswith("%(")
    ]


class Generator:
    def __init__(self, args):
        self.a = args
        self.target, self.machine, self.xarch, self.nasmfmt = ARCHES[args.platform]
        self.msvc = args.toolchain == "msvc"
        self.out = []
        self.build = os.path.abspath(args.build_dir)
        self.sln_map = {}
        self.include_dirs = set()
        self.sources = []
        self.rc_files = []
        self.pragma_libs = set()
        self.all_libdirs = []
        self.built_libs = {}  # lower-case lib name -> path of a lib built by this ninja file

    def read_solution(self, sln):
        """Map project paths to the configuration the solution selects for them."""
        text = open(sln, encoding="utf-8-sig").read()
        sdir = os.path.dirname(os.path.abspath(sln))
        guids = {}
        for m in re.finditer(
            r'^Project\("[^"]*"\)\s*=\s*"[^"]*",\s*"([^"]+)",\s*"(\{[^}]+\})"', text, re.M
        ):
            guids[m.group(2).upper()] = os.path.normcase(
                os.path.normpath(os.path.join(sdir, m.group(1).replace("\\", "/")))
            )
        want = "%s|%s" % (self.a.config, self.a.platform)
        for m in re.finditer(r"^\s*(\{[^}]+\})\.([^.]+)\.ActiveCfg\s*=\s*(\S+)\|(\S+)", text, re.M):
            if m.group(2) == want and m.group(1).upper() in guids:
                self.sln_map[guids[m.group(1).upper()]] = (m.group(3), m.group(4))

    # ---- path helpers ----------------------------------------------------------------------------
    def winpath(self, p, base):
        """Translate a Windows path from a project file to a host path."""
        p = p.strip().strip('"')
        if not p:
            return None
        p = p.replace("\\", "/")
        low = p.lower()
        if low.startswith("c:/dev/"):
            return os.path.join(self.a.dev, p[len("c:/dev/") :])
        if re.match(r"^[a-z]:/", low) or "windows kits" in low or "$(" in p:
            return None  # host specific directory that we can't map
        return os.path.normpath(os.path.join(base, p))

    def sdk_includes(self):
        if self.msvc:
            return []  # cl.exe gets INCLUDE from msvc-wine's msvcenv.sh
        x = self.a.xwin
        return [os.path.join(x, "crt", "include")] + [
            os.path.join(x, "sdk", "include", d) for d in ("ucrt", "um", "shared", "winrt")
        ]

    def sdk_libdirs(self):
        if self.msvc:
            # Only used to locate libraries for dependency tracking; link.exe itself gets LIB from
            # msvcenv.sh.
            m = self.a.msvc
            arch = self.machine
            return sorted(
                glob.glob(os.path.join(m, "vc", "tools", "msvc", "*", "lib", arch))
                + glob.glob(os.path.join(m, "vc", "tools", "msvc", "*", "atlmfc", "lib", arch))
                + glob.glob(os.path.join(m, "kits", "10", "lib", "*", "um", arch))
                + glob.glob(os.path.join(m, "kits", "10", "lib", "*", "ucrt", arch))
            )
        x = self.a.xwin
        return [
            os.path.join(x, "crt", "lib", self.xarch),
            os.path.join(x, "sdk", "lib", "um", self.xarch),
            os.path.join(x, "sdk", "lib", "ucrt", self.xarch),
        ]

    def find_lib(self, name, dirs):
        """Case-insensitive library lookup (Windows lib names are case-insensitive)."""
        key = name.lower()
        if not key.endswith(".lib"):
            key += ".lib"
        if key in self.built_libs:
            return self.built_libs[key]
        for d in dirs:
            if not os.path.isdir(d):
                continue
            for f in os.listdir(d):
                if f.lower() == key:
                    return os.path.join(d, f)
        return None

    # ---- flag translation ------------------------------------------------------------------------
    def cl_flags(self, meta, proj, src):
        f = []
        opt = {"Disabled": "/Od", "MinSpace": "/O1", "MaxSpeed": "/O2", "Full": "/Ox"}
        if meta.get("Optimization") in opt:
            f.append(opt[meta["Optimization"]])
        f += {"AnySuitable": ["/Ob2"], "OnlyExplicitInline": ["/Ob1"], "Disabled": ["/Ob0"]}.get(
            meta.get("InlineFunctionExpansion"), []
        )
        if meta.get("IntrinsicFunctions") == "true":
            f.append("/Oi")
        f += {"Speed": ["/Ot"], "Size": ["/Os"]}.get(meta.get("FavorSizeOrSpeed"), [])
        if meta.get("OmitFramePointers") == "true" and self.a.platform == "Win32":
            f.append("/Oy")
        if meta.get("StringPooling") == "true":
            f.append("/GF")
        f += {"Sync": ["/EHsc"], "Async": ["/EHa"], "SyncCThrow": ["/EHs"]}.get(
            meta.get("ExceptionHandling"), []
        )
        f += {
            "MultiThreaded": ["/MT"],
            "MultiThreadedDLL": ["/MD"],
            "MultiThreadedDebug": ["/MTd"],
            "MultiThreadedDebugDLL": ["/MDd"],
        }.get(meta.get("RuntimeLibrary"), ["/MT"])
        if meta.get("BufferSecurityCheck") == "false":
            f.append("/GS-")
        if meta.get("FunctionLevelLinking") == "true":
            f.append("/Gy")
        if self.a.platform == "Win32":
            f += {
                "StreamingSIMDExtensions2": ["/arch:SSE2"],
                "StreamingSIMDExtensions": ["/arch:SSE"],
            }.get(meta.get("EnableEnhancedInstructionSet"), [])
            f += {"FastCall": ["/Gr"], "StdCall": ["/Gz"], "Cdecl": ["/Gd"]}.get(
                meta.get("CallingConvention"), []
            )
        f += {
            "AdvancedVectorExtensions": ["/arch:AVX"],
            "AdvancedVectorExtensions2": ["/arch:AVX2"],
        }.get(meta.get("EnableEnhancedInstructionSet"), [])
        # MSVC /fp:fast does not assume "no NaN/Inf", clang's /fp:fast does (-ffast-math). The FPU
        # emulation depends on NaN/Inf semantics, so keep clang's default (precise) model for MSVC's
        # fast one.
        fp = meta.get("FloatingPointModel")
        if fp == "Fast":
            if self.msvc:
                f.append("/fp:fast")
        elif fp == "Strict":
            f.append("/fp:strict")
        elif fp == "Precise":
            f.append("/fp:precise")
        if meta.get("TreatWChar_tAsBuiltInType") == "false":
            f.append("/Zc:wchar_t-")
        if meta.get("RuntimeTypeInfo") == "false":
            f.append("/GR-")
        compile_as = meta.get("CompileAs", "Default")
        is_c = compile_as == "CompileAsC" or (
            compile_as == "Default" and src.lower().endswith(".c")
        )
        if compile_as == "CompileAsC":
            f.append("/TC")
        elif compile_as == "CompileAsCpp":
            f.append("/TP")
        if is_c:
            std = {"stdc11": "/std:c11", "stdc17": "/std:c17", "stdclatest": "/std:clatest"}.get(
                meta.get("LanguageStandard_C")
            )
        else:
            # clang's c++latest is C++26, which turns deprecations WinUAE relies on into errors
            std = {
                "stdcpp14": "/std:c++14",
                "stdcpp17": "/std:c++17",
                "stdcpp20": "/std:c++20",
                "stdcpplatest": "/std:c++latest" if self.msvc else "/std:c++23preview",
            }.get(meta.get("LanguageStandard"))
        if std:
            f.append(std)
        if meta.get("WholeProgramOptimization") == "true" and self.a.lto:
            f.append("/GL" if self.msvc else "-flto=thin")
        if self.msvc:
            f += {
                "Level1": ["/W1"],
                "Level2": ["/W2"],
                "Level3": ["/W3"],
                "Level4": ["/W4"],
                "TurnOffAllWarnings": ["/W0"],
            }.get(meta.get("WarningLevel"), [])
        charset = proj.props.get("CharacterSet")
        if charset == "Unicode":
            f += ["/DUNICODE", "/D_UNICODE"]
        elif charset == "MultiByte":
            f.append("/D_MBCS")
        for d in split_list(proj.expand(meta.get("PreprocessorDefinitions", ""))):
            f.append("/D" + d)
        for w in split_list(meta.get("DisableSpecificWarnings", "")):
            f.append("/wd" + w)
        for inc in split_list(proj.expand(meta.get("AdditionalIncludeDirectories", ""))):
            p = self.winpath(inc, proj.dir)
            if p:
                f.append("/I" + p)
        for fi in split_list(proj.expand(meta.get("ForcedIncludeFiles", ""))):
            f.append("/FI" + fi.replace("\\", "/"))
        for o in proj.expand(meta.get("AdditionalOptions", "")).split():
            if o.startswith("%("):
                continue
            if o.lower() == "/zc:strictstrings-" and not self.msvc:
                f.append("-Wno-writable-strings")  # clang-cl's equivalent
                continue
            f.append(o)
        return f

    # ---- ninja helpers ---------------------------------------------------------------------------
    @staticmethod
    def esc(p):
        return p.replace("$", "$$").replace(" ", "$ ").replace(":", "$:")

    @staticmethod
    def q(args):
        return " ".join(shlex.quote(a) for a in args)

    def emit(self, s=""):
        self.out.append(s)

    def rsp(self, paths):
        """Response file contents. The msvc-wine wrappers translate absolute paths on the command
        line, but not inside response files, so give link.exe/lib.exe Windows paths (Wine maps / to
        drive Z:)."""
        if self.msvc:
            return " ".join('"z:%s"' % p.replace("$", "$$") for p in paths)
        return " ".join(self.esc(p) for p in paths)

    # ---- projects --------------------------------------------------------------------------------
    def project(self, vcxproj):
        config, platform = self.sln_map.get(
            os.path.normcase(os.path.abspath(vcxproj)), (self.a.config, self.a.platform)
        )
        if platform != self.a.platform:
            # Generator tools are mapped to Win32 in the solution; build the requested architecture
            # of the project configuration instead.
            platform = self.a.platform
        proj = Project(vcxproj, config, platform, {})
        name = proj.props.get("TargetName") or proj.props["ProjectName"]
        ctype = proj.props.get("ConfigurationType", "Application")
        objdir = os.path.join(self.build, "obj", proj.props["ProjectName"])
        objs = []

        common_inc = []
        for inc in split_list(proj.expand(proj.props.get("IncludePath", ""))):
            p = self.winpath(inc, proj.dir)
            if p:
                common_inc.append("/I" + p)

        for src, meta in proj.items("ClCompile"):
            spath = os.path.normpath(os.path.join(proj.dir, src.replace("\\", "/")))
            rel = os.path.relpath(spath, self.a.srcroot)
            obj = os.path.join(
                objdir,
                re.sub(r"\.(c|cc|cpp|cxx)$", "", rel.replace("..", "__"), flags=re.I) + ".obj",
            )
            flags = self.cl_flags(meta, proj, src) + common_inc
            self.include_dirs.update(x[2:] for x in flags if x.startswith("/I"))
            self.sources.append(spath)
            self.emit("build %s: cc %s" % (self.esc(obj), self.esc(spath)))
            self.emit("  flags = %s" % self.q(flags))
            objs.append(obj)

        for src, meta in proj.items("CustomBuild"):
            spath = os.path.normpath(os.path.join(proj.dir, src.replace("\\", "/")))
            cmd = meta.get("Command", "")
            if not cmd:
                continue
            base = os.path.splitext(os.path.basename(spath))[0]
            obj = os.path.join(objdir, "custom", base + ".obj")
            if cmd.split()[0].lower() == "nasm":
                if not self.nasmfmt:
                    continue
                self.emit("build %s: nasm %s" % (self.esc(obj), self.esc(spath)))
                self.emit("  fmt = %s" % self.nasmfmt)
                objs.append(obj)
            else:
                print(
                    "warning: ignoring custom build step for %s: %s" % (src, cmd), file=sys.stderr
                )

        for src, meta in proj.items("ResourceCompile"):
            spath = os.path.normpath(os.path.join(proj.dir, src.replace("\\", "/")))
            res = os.path.join(objdir, os.path.splitext(os.path.basename(spath))[0] + ".res")
            flags = ["/D" + d for d in split_list(meta.get("PreprocessorDefinitions", ""))]
            flags += ["/I" + os.path.dirname(spath)]
            for inc in split_list(proj.expand(meta.get("AdditionalIncludeDirectories", ""))):
                p = self.winpath(inc, proj.dir)
                if p:
                    flags.append("/I" + p)
            self.rc_files.append(spath)
            self.emit("build %s: rc %s" % (self.esc(res), self.esc(spath)))
            self.emit("  flags = %s" % self.q(flags))
            objs.append(res)

        link = proj.defaults.get("Link", {})
        if ctype == "StaticLibrary":
            out = os.path.join(self.build, "lib", name + ".lib")
            self.emit("build %s: lib %s" % (self.esc(out), " ".join(self.esc(o) for o in objs)))
            self.emit("  rsp = %s" % self.rsp(objs))
            self.emit()
            self.built_libs[os.path.basename(out).lower()] = out
            # winuae links "prowizard.lib" (the project name) while the x64 project writes
            # prowizard_x64.lib to C:\dev\lib; with Visual Studio that picks up whatever prebuilt
            # copy is there. Link the library built from this source tree instead.
            self.built_libs[proj.props["ProjectName"].lower() + ".lib"] = out
            return out

        target_ext = proj.props.get("TargetExt", ".exe")
        out = os.path.join(self.build, name + target_ext)
        libdirs = [os.path.join(self.build, "lib")]
        extra = split_list(proj.expand(link.get("AdditionalLibraryDirectories", "")))
        extra += split_list(proj.expand(proj.props.get("LibraryPath", "")))
        for d in extra:
            p = self.winpath(d, proj.dir)
            if p:
                libdirs.append(p)
        libdirs += self.sdk_libdirs()
        if not self.msvc:
            libdirs.append(os.path.join(self.build, "lib-shim"))
        self.all_libdirs += [d for d in libdirs if d not in self.all_libdirs]

        lflags = ["/machine:" + self.machine, "/nologo"]
        subsystem = link.get("SubSystem", "Windows")
        lflags.append("/subsystem:" + subsystem.lower())
        if link.get("LargeAddressAware") == "true":
            lflags.append("/largeaddressaware")
        if link.get("RandomizedBaseAddress") != "false":
            lflags.append("/dynamicbase")
        if link.get("DataExecutionPrevention") != "false":
            lflags.append("/nxcompat")
        if link.get("OptimizeReferences") == "true":
            lflags.append("/opt:ref")
        if link.get("EnableCOMDATFolding") == "true":
            lflags.append("/opt:icf")
        if ctype == "DynamicLibrary":
            lflags.append("/dll")
        if (
            self.msvc
            and self.a.lto
            and link.get("LinkTimeCodeGeneration") == "UseLinkTimeCodeGeneration"
        ):
            lflags.append("/ltcg")
        for lib in split_list(link.get("IgnoreSpecificDefaultLibraries", "")):
            lflags.append("/nodefaultlib:" + lib)
        delay = split_list(link.get("DelayLoadDLLs", ""))
        for dll in delay:
            lflags.append("/delayload:" + dll)
        if self.a.debug_info:
            lflags += ["/debug", "/pdb:" + os.path.splitext(out)[0] + ".pdb"]
        for d in libdirs:
            if self.msvc and d in self.sdk_libdirs():
                continue  # already in LIB
            lflags.append("/libpath:" + d)

        manifest = proj.defaults.get("Manifest", {})
        for mf in split_list(manifest.get("AdditionalManifestFiles", "")):
            p = self.winpath(mf, proj.dir)
            if p:
                lflags += ["/manifest:embed", "/manifestinput:" + p]

        deplist = split_list(link.get("AdditionalDependencies", ""))
        if "%(AdditionalDependencies)" in link.get(
            "AdditionalDependencies", "%(AdditionalDependencies)"
        ):
            deplist += DEFAULT_LIBS  # what Microsoft.Cpp props inherit
        libs = []
        missing = []
        for lib in deplist + (["delayimp.lib"] if delay else []):
            path = self.find_lib(lib, libdirs)
            if path:
                libs.append(path)
            else:
                missing.append(lib)
        if missing:
            print(
                "warning: %s: libraries not found: %s" % (name, ", ".join(missing)), file=sys.stderr
            )

        deps = [o for o in objs] + libs
        self.emit("build %s: link %s" % (self.esc(out), " ".join(self.esc(o) for o in deps)))
        self.emit("  flags = %s" % self.q(lflags))
        self.emit("  rsp = %s" % self.rsp(deps))
        self.emit()
        return out

    PRAGMA_LIB_RE = re.compile(r'#\s*pragma\s+comment\s*\(\s*lib\s*,\s*"([^"]+)"', re.I)
    INCLUDE_RE = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.M)

    @staticmethod
    def resolve_icase(base, rel):
        """Resolve rel below base ignoring case; returns the real path or None."""
        cur = base
        for comp in rel.split("/"):
            if comp in (".", ""):
                continue
            if comp == "..":
                cur = os.path.dirname(cur)
                continue
            if os.path.exists(os.path.join(cur, comp)):
                cur = os.path.join(cur, comp)
                continue
            try:
                hits = [e for e in os.listdir(cur) if e.lower() == comp.lower()]
            except OSError:
                return None
            if not hits:
                return None
            cur = os.path.join(cur, hits[0])
        return cur

    def case_shim(self):
        """Windows file systems are case-insensitive and the sources (and some SDK headers) rely on
        that. Find every #include that only resolves when ignoring case and expose it under the
        spelling used, via symlinks in a directory that is searched last."""
        shim = os.path.join(self.build, "case-shim")
        dirs = [d for d in self.sdk_includes() + sorted(self.include_dirs) if os.path.isdir(d)]
        scan, seen = list(self.sources), set()
        for d in self.sdk_includes():
            for root, _, files in os.walk(d):
                scan += [os.path.join(root, f) for f in files]
        links = {}
        while scan:
            path = scan.pop()
            if path in seen:
                continue
            seen.add(path)
            try:
                text = open(path, encoding="latin-1").read()
            except OSError:
                continue
            if path.startswith(self.a.srcroot):
                self.pragma_libs.update(self.PRAGMA_LIB_RE.findall(text))
            for name in self.INCLUDE_RE.findall(text):
                name = name.replace("\\", "/")
                here = [os.path.dirname(path)] + dirs
                if any(os.path.exists(os.path.join(d, name)) for d in here):
                    hit = next(
                        os.path.join(d, name) for d in here if os.path.exists(os.path.join(d, name))
                    )
                    if hit.startswith(self.a.srcroot) and hit not in seen:
                        scan.append(os.path.realpath(hit))
                    continue
                for d in here:
                    real = self.resolve_icase(d, name)
                    if real and os.path.isfile(real):
                        links.setdefault(name, real)
                        if real.startswith(self.a.srcroot):
                            scan.append(real)
                        break
        # Files referenced by resource scripts (icons, cursors, bitmaps, ...)
        for rc in self.rc_files:
            text = open(rc, encoding="latin-1").read()
            for name in re.findall(
                r'"([^"\n]+\.(?:cur|ico|bmp|wav|png|manifest|bin|txt))"', text, re.I
            ):
                name = name.replace("\\\\", "/").replace("\\", "/")
                base = os.path.dirname(rc)
                if not os.path.exists(os.path.join(base, name)):
                    real = self.resolve_icase(base, name)
                    if real:
                        links.setdefault(name, real)
        for name, real in links.items():
            dst = os.path.join(shim, name)
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            if os.path.lexists(dst):
                os.remove(dst)
            os.symlink(real, dst)
        # #pragma comment(lib, "Name.lib") is looked up case-sensitively by lld-link
        libshim = os.path.join(self.build, "lib-shim")
        os.makedirs(libshim, exist_ok=True)
        for name in sorted(self.pragma_libs):
            dirs = [d for d in self.all_libdirs if d != libshim]
            if any(os.path.exists(os.path.join(d, name)) for d in dirs):
                continue
            real = self.find_lib(name, dirs)
            if real:
                dst = os.path.join(libshim, name)
                if os.path.lexists(dst):
                    os.remove(dst)
                os.symlink(real, dst)
        return shim, len(links)

    def header(self):
        if self.msvc:
            return self.header_msvc()
        inc = []
        for d in self.sdk_includes():
            inc += ["/imsvc", d]
        inc += ["/imsvc", os.path.join(self.build, "case-shim")]
        base = ["--target=" + self.target, "/nologo", "/c", "/Brepro"]
        if self.a.debug_info:
            base.append("/Z7")
        # MSVC does not exploit strict aliasing or signed overflow; neither should we.
        base += ["/clang:-fno-strict-aliasing", "/clang:-fwrapv"]
        # Things MSVC accepts that clang-cl rejects by default.
        base += ["-Wno-c++11-narrowing"]
        # Falling off the end of a non-void function is "just" garbage in MSVC, clang would treat it
        # as unreachable.
        base += ["/clang:-fno-strict-return"]
        # C++20 deprecation noise MSVC doesn't emit
        base += ["-Wno-deprecated-volatile"]
        base += self.a.cflags
        rc_inc = ["/I", os.path.join(self.build, "case-shim")]
        for d in self.sdk_includes():
            rc_inc += ["/I", d]
        e = self.emit
        e("# Generated by od-win32/crossbuild/vcxproj2ninja.py - do not edit")
        e("ninja_required_version = 1.10")
        e("cc = %s" % self.a.clang_cl)
        e("base_flags = %s" % self.q(base + inc))
        e("rc_flags = %s" % self.q(rc_inc))
        e()
        e("rule cc")
        e("  command = $cc $base_flags $flags /showIncludes:user /Fo$out $in")
        e("  deps = msvc")
        e("  msvc_deps_prefix = Note: including file:")
        e("  description = CC $in")
        e()
        e("rule nasm")
        e("  command = %s -f $fmt $in -o $out" % self.a.nasm)
        e("  description = NASM $in")
        e()
        e("rule rc")
        # winuae.rc is UTF-8 (#pragma code_page(65001)); llvm-rc wants it up front too
        e("  command = %s /nologo /C 65001 $rc_flags $flags /FO $out $in" % self.a.llvm_rc)
        e("  description = RC $in")
        e()
        self.emit_lib_link_rules(self.a.llvm_lib, self.a.lld_link)

    def emit_lib_link_rules(self, lib, link):
        e = self.emit
        e("rule lib")
        e("  command = %s /nologo /out:$out @$out.rsp" % lib)
        e("  rspfile = $out.rsp")
        e("  rspfile_content = $rsp")
        e("  description = LIB $out")
        e()
        e("rule link")
        e("  command = %s $flags /out:$out @$out.rsp" % link)
        e("  rspfile = $out.rsp")
        e("  rspfile_content = $rsp")
        e("  description = LINK $out")
        e()

    def header_msvc(self):
        bindir = os.path.join(self.a.msvc, "bin", self.machine)
        base = ["/nologo", "/c", "/Brepro"]
        if self.a.debug_info:
            base.append("/Z7")
        base += self.a.cflags
        e = self.emit
        e("# Generated by od-win32/crossbuild/vcxproj2ninja.py - do not edit")
        e("ninja_required_version = 1.10")
        e("cc = %s" % shlex.quote(os.path.join(bindir, "cl")))
        e("base_flags = %s" % self.q(base))
        e()
        e("rule cc")
        e("  command = $cc $base_flags $flags /showIncludes /Fo$out $in")
        e("  deps = msvc")
        e("  msvc_deps_prefix = Note: including file:")
        e("  description = CL $in")
        e()
        e("rule nasm")
        e("  command = %s -f $fmt $in -o $out" % self.a.nasm)
        e("  description = NASM $in")
        e()
        e("rule rc")
        e("  command = %s /nologo $flags /fo $out $in" % shlex.quote(os.path.join(bindir, "rc")))
        e("  description = RC $in")
        e()
        self.emit_lib_link_rules(
            shlex.quote(os.path.join(bindir, "lib")), shlex.quote(os.path.join(bindir, "link"))
        )


def main():
    here = os.path.dirname(os.path.abspath(__file__))
    srcroot = os.path.normpath(os.path.join(here, "..", ".."))
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    ap.add_argument("--config", default="FullRelease")
    ap.add_argument("--platform", default="x64", choices=sorted(ARCHES))
    ap.add_argument("--build-dir", required=True)
    ap.add_argument("--toolchain", default="clang", choices=["clang", "msvc"])
    ap.add_argument(
        "--xwin", help="xwin splat output directory (MSVC CRT + Windows SDK), for --toolchain clang"
    )
    ap.add_argument("--msvc", help="msvc-wine installation directory, for --toolchain msvc")
    ap.add_argument("--dev", required=True, help="winuaeinclibs directory (the C:\\dev equivalent)")
    ap.add_argument(
        "--lto",
        action="store_true",
        help="honour WholeProgramOptimization (ThinLTO with clang, /GL + /LTCG with MSVC)",
    )
    ap.add_argument("--debug-info", action="store_true", help="emit CodeView debug info and a PDB")
    ap.add_argument("--cflags", action="append", default=[], help="extra compiler flags")
    ap.add_argument("--clang-cl", default="clang-cl")
    ap.add_argument("--lld-link", default="lld-link")
    ap.add_argument("--llvm-lib", default="llvm-lib")
    ap.add_argument("--llvm-rc", default="llvm-rc")
    ap.add_argument("--nasm", default="nasm")
    args = ap.parse_args()
    args.srcroot = srcroot
    if args.toolchain == "clang" and not args.xwin:
        ap.error("--toolchain clang needs --xwin")
    if args.toolchain == "msvc" and not args.msvc:
        ap.error("--toolchain msvc needs --msvc")

    g = Generator(args)
    g.read_solution(os.path.join(srcroot, "od-win32", "winuae_msvc15", "winuae_msvc.sln"))
    g.header()
    g.project(os.path.join(srcroot, "od-win32", "prowizard", "prowizard.vcxproj"))
    g.project(os.path.join(srcroot, "od-win32", "winuae_msvc15", "winuae_msvc.vcxproj"))
    os.makedirs(g.build, exist_ok=True)
    if not g.msvc:  # Wine already gives cl.exe a case-insensitive file system
        shim, n = g.case_shim()
        print("case shim: %d include(s) only resolve case-insensitively" % n)
    out = os.path.join(g.build, "build.ninja")
    with open(out + ".tmp", "w") as fp:
        fp.write("\n".join(g.out) + "\n")
    os.replace(out + ".tmp", out)
    print("wrote", out)


if __name__ == "__main__":
    main()
