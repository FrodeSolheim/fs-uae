from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import subprocess
from fengestad import fs
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.gamecenter.refreshratetool import RefreshRateTool
from fengestad.gamecenter.resources import resources, logger, ngettext, _


class PlayStation2Controller(GameController):

    def configure(self):
        self.args = []
        config_dir = self.context.temp.dir('pcsx2')
        config_dir = os.path.join(config_dir, 'inis')
        if not os.path.exists(config_dir):
            os.makedirs(config_dir)
        with open(os.path.join(config_dir, 'PCSX2_ui.ini'), 'wb') as f:
            self.configure_pcsx2(f)
        #self.args.extend(['--cfgpath=' + config_dir])
        #self.args.extend(['--cfgpath=C:\Users\frode\Documents\PCSX2\inis'])
        if fs.windows:
            pass
        else:
            self.args.extend(['--cfg=' + os.path.join(config_dir,
                    'PCSX2.ini')])
        self.args.extend(['--fullboot', self.context.game.file])

    def run(self):
        plugin = pyapp.plug.get_plugin('no.fengestad.emulator.pcsx2')
        args = self.args[:]
        if fs.windows:
            temp_dir = self.context.temp.dir('pcsx2')
            GameCenterUtil.copy_folder_tree(plugin.get_bin_dir(), temp_dir)
            args = self.args[:]
            args.insert(0, os.path.join(temp_dir, 'pcsx2-r4600.exe'))
            return subprocess.Popen(args, cwd=temp_dir, env=self.env,
                    close_fds=True)
        cwd = '/'
        return plugin.pcsx2(args, cwd=cwd, env=self.env)

    def configure_pcsx2(self, f):
        def fp(path):
            return path.replace('\\', '\\\\')
        state_dir = self.context.game.state_dir
        temp_dir = self.context.temp.dir('pcsx2-temp')
        
        f.write('EnableSpeedHacks=enabled\n')

        f.write('[Folders]\n')
        f.write('UseDefaultPlugins=disabled\n')
        f.write('UseDefaultSettings=disabled\n')
        f.write('UseDefaultBios=disabled\n')
        f.write('UseDefaultSnapshots=disabled\n')
        f.write('UseDefaultSavestates=disabled\n')
        f.write('UseDefaultMemoryCards=disabled\n')
        f.write('UseDefaultLogs=disabled\n')
        f.write('Savestates={path}\n'.format(path=fp(state_dir)))
        f.write('MemoryCards={path}\n'.format(path=fp(state_dir)))
        f.write('Logs={path}\n'.format(path=fp(temp_dir)))
        f.write('Snapshots={path}\n'.format(path=fp(temp_dir)))

        bios_dir = self.resource_file('PlayStation 2')
        f.write('Bios={path}\n'.format(path=fp(bios_dir)))
        plugin = pyapp.plug.get_plugin('no.fengestad.emulator.pcsx2')
        plugins_dir = os.path.join(plugin.get_bin_dir(), 'plugins')
        f.write('Plugins={path}\n'.format(path=fp(plugins_dir)))

        gs_file = 'gsdx-sse2-r4600.dll'
        pad_file = 'lilypad-r4600.dll'
        spu2_file = 'SPU2-X.dll'
        cdvd_file = 'cdvdGigaherz.dll'
        usb_file = 'USBnull.dll'
        fw_file = 'FWnull.dll'
        dev9_file = 'DEV9null.dll'
        bios_file = 'scph39001.bin'

        f.write('[Filenames]\n')
        f.write('GS={0}\n'.format(fp(os.path.join(plugins_dir, gs_file))))
        f.write('PAD={0}\n'.format(fp(os.path.join(plugins_dir, pad_file))))
        f.write('SPU2={0}\n'.format(fp(os.path.join(plugins_dir, spu2_file))))
        f.write('CDVD={0}\n'.format(fp(os.path.join(plugins_dir, cdvd_file))))
        f.write('USB={0}\n'.format(fp(os.path.join(plugins_dir, usb_file))))
        f.write('FW={0}\n'.format(fp(os.path.join(plugins_dir, fw_file))))
        f.write('DEV9={0}\n'.format(fp(os.path.join(plugins_dir, dev9_file))))
        f.write('BIOS={0}\n'.format(fp(os.path.join(bios_dir, bios_file))))

        f.write('[GSWindow]\n')
        if self.options.fullscreen:
            f.write('DefaultToFullScreen=enabled\n')
        else:
            f.write('DefaultToFullScreen=disabled')
        f.write('AspectRatio=16:9')



        """
GS=C:\\git\\fs-game-center\\plugins\\no.fengestad.emulator.pcsx2-windows\\bin-windows-i686\\Plugins\\gsdx-sse2-r3878.dll
PAD=C:\\git\\fs-game-center\\plugins\\no.fengestad.emulator.pcsx2-windows\\bin-windows-i686\\Plugins\\lilypad-r3878.dll
SPU2=C:\\git\\fs-game-center\\plugins\\no.fengestad.emulator.pcsx2-windows\\bin-windows-i686\\Plugins\\spu2-x-r3878.dll
CDVD=C:\\git\\fs-game-center\\plugins\\no.fengestad.emulator.pcsx2-windows\\bin-windows-i686\\Plugins\\cdvdGigaherz.dll
USB=C:\\git\\fs-game-center\\plugins\\no.fengestad.emulator.pcsx2-windows\\bin-windows-i686\\Plugins\\USBnull.dll
FW=C:\\git\\fs-game-center\\plugins\\no.fengestad.emulator.pcsx2-windows\\bin-windows-i686\\Plugins\\FWnull.dll
DEV9=C:\\git\\fs-game-center\\plugins\\no.fengestad.emulator.pcsx2-windows\\bin-windows-i686\\Plugins\\DEV9null.dll
BIOS=C:\\A\\bios\\scph39001.bin"""


        #f.write(
        """MainGuiPosition=694,5
SysSettingsTabName=Speedhacks
McdSettingsTabName=none
AppSettingsTabName=BIOS
GameDatabaseTabName=none
LanguageId=0
RecentIsoCount=12
DeskTheme=default
Listbook_ImageSize=32
Toolbar_ImageSize=24
Toolbar_ShowLabels=enabled
CurrentIso=M:\\Games\\PlayStation 2\\Final Fantasy X (USA).iso
CurrentELF=
EnableSpeedHacks=disabled
EnableGameFixes=disabled
McdCompressNTFS=enabled
CdvdSource=Iso
[MemoryCards]
Slot1_Enable=enabled
Slot1_Filename=Mcd001.ps2
Slot2_Enable=enabled
Slot2_Filename=Mcd002.ps2
Multitap1_Slot2_Enable=disabled
Multitap1_Slot2_Filename=Mcd-Multitap1-Slot02.ps2
Multitap1_Slot3_Enable=disabled
Multitap1_Slot3_Filename=Mcd-Multitap1-Slot03.ps2
Multitap1_Slot4_Enable=disabled
Multitap1_Slot4_Filename=Mcd-Multitap1-Slot04.ps2
Multitap2_Slot2_Enable=disabled
Multitap2_Slot2_Filename=Mcd-Multitap2-Slot02.ps2
Multitap2_Slot3_Enable=disabled
Multitap2_Slot3_Filename=Mcd-Multitap2-Slot03.ps2
Multitap2_Slot4_Enable=disabled
Multitap2_Slot4_Filename=Mcd-Multitap2-Slot04.ps2
[ProgramLog]
Visible=enabled
AutoDock=enabled
DisplayPosition=1079,5
DisplaySize=680,560
FontSize=8
Theme=Default
[Folders]
UseDefaultPlugins=enabled
UseDefaultSettings=enabled
UseDefaultBios=disabled
UseDefaultSnapshots=enabled
UseDefaultSavestates=enabled
UseDefaultMemoryCards=enabled
UseDefaultLogs=enabled
Plugins=plugins
Bios=C:\\Users\\frode\\Desktop\\pcsx2\\bios
Snapshots=C:\\Users\\frode\\Documents\\PCSX2\\snaps
Savestates=C:\\Users\\frode\\Documents\\PCSX2\\sstates
MemoryCards=C:\\Users\\frode\\Documents\\PCSX2\\memcards
Logs=C:\\Users\\frode\\Documents\\PCSX2\\logs
RunIso=M:\\Games\\PlayStation 2
RunELF=C:\\Users\\frode\\Documents\\PCSX2
[Filenames]
GS=C:\\Users\\frode\\Desktop\\pcsx2\\Plugins\\gsdx-sse2-r3878.dll
PAD=C:\\Users\\frode\\Desktop\\pcsx2\\Plugins\\lilypad-r3878.dll
SPU2=C:\\Users\\frode\\Desktop\\pcsx2\\Plugins\\spu2-x-r3878.dll
CDVD=C:\\Users\\frode\\Desktop\\pcsx2\\Plugins\\cdvdGigaherz.dll
USB=C:\\Users\\frode\\Desktop\\pcsx2\\Plugins\\USBnull.dll
FW=C:\\Users\\frode\\Desktop\\pcsx2\\Plugins\\FWnull.dll
DEV9=C:\\Users\\frode\\Desktop\\pcsx2\\Plugins\\DEV9null.dll
BIOS=C:\\Users\\frode\\Desktop\\pcsx2\\bios\\scph39001.bin
[GSWindow]
CloseOnEsc=enabled
DefaultToFullscreen=disabled
AlwaysHideMouse=disabled
DisableResizeBorders=disabled
DisableScreenSaver=enabled
WindowSize=1229,845
WindowPos=225,71
IsMaximized=enabled
IsFullscreen=disabled
AspectRatio=4:3
[Framerate]
NominalScalar=1.0
TurboScalar=2.0
SlomoScalar=0.50
SkipOnLimit=disabled
SkipOnTurbo=disabled
[EmuCore]
CdvdVerboseReads=disabled
CdvdDumpBlocks=disabled
EnablePatches=enabled
EnableCheats=disabled
ConsoleToStdio=disabled
HostFs=disabled
McdEnableEjection=enabled
MultitapPort0_Enabled=disabled
MultitapPort1_Enabled=disabled
[EmuCore/Speedhacks]
EECycleRate=0
VUCycleSteal=0
fastCDVD=disabled
IntcStat=enabled
WaitLoop=enabled
vuFlagHack=enabled
vuBlockHack=disabled
vuMinMax=disabled
[EmuCore/CPU]
FPU.DenormalsAreZero=enabled
FPU.FlushToZero=enabled
FPU.Roundmode=3
VU.DenormalsAreZero=enabled
VU.FlushToZero=enabled
VU.Roundmode=3
[EmuCore/CPU/Recompiler]
EnableEE=enabled
EnableIOP=enabled
EnableVU0=enabled
EnableVU1=enabled
UseMicroVU0=enabled
UseMicroVU1=enabled
vuOverflow=enabled
vuExtraOverflow=disabled
vuSignOverflow=disabled
vuUnderflow=disabled
fpuOverflow=enabled
fpuExtraOverflow=disabled
fpuFullMode=disabled
StackFrameChecks=disabled
PreBlockCheckEE=disabled
PreBlockCheckIOP=disabled
[EmuCore/GS]
SynchronousMTGS=disabled
DisableOutput=disabled
VsyncQueueSize=2
FrameLimitEnable=enabled
FrameSkipEnable=disabled
VsyncEnable=disabled
LimitScalar=1.0
FramerateNTSC=59.94
FrameratePAL=50.0
DefaultRegionMode=ntsc
FramesToDraw=2
FramesToSkip=2
[EmuCore/Gamefixes]
VuAddSubHack=disabled
VuClipFlagHack=disabled
FpuCompareHack=disabled
FpuMulHack=disabled
FpuNegDivHack=disabled
XgKickHack=disabled
IPUWaitHack=disabled
EETimingHack=disabled
SkipMPEGHack=disabled
OPHFlagHack=disabled
[EmuCore/Profiler]
Enabled=disabled
RecBlocks_EE=enabled
RecBlocks_IOP=enabled
RecBlocks_VU0=enabled
RecBlocks_VU1=enabled
[EmuCore/TraceLog]
Enabled=disabled
EE.bitset=0
IOP.bitset=0
[ConsoleLogSources]
Devel=disabled
.EEout=disabled
.IOPout=disabled
.EErecPerf=disabled
.ELF=disabled
.SysEvents=disabled
.pxThread=disabled
[TraceLogSources]
.SIF=disabled
EE.Bios=disabled
EE.Memory=disabled
EE.Disasm.R5900=disabled
EE.Disasm.COP0=disabled
EE.Disasm.FPU=disabled
EE.Disasm.VUmacro=disabled
EE.Disasm.Cache=disabled
EE.Registers.HwRegs=disabled
EE.Registers.UnknownRegs=disabled
EE.Registers.DmaRegs=disabled
EE.Registers.IPU=disabled
EE.GIFtags=disabled
EE.VIFcodes=disabled
EE.Events.DmaCtrl=disabled
EE.Events.Counters=disabled
EE.Events.MFIFO=disabled
EE.Events.VIF=disabled
EE.Events.GIF=disabled
IOP.Bios=disabled
IOP.Memorycards=disabled
IOP.Pad=disabled
IOP.Disasm.R3000A=disabled
IOP.Disasm.Memory=disabled
IOP.Registers.HwRegs=disabled
IOP.Registers.UnknownRegs=disabled
IOP.Registers.DmaRegs=disabled
IOP.Events.DmaCrl=disabled
IOP.Events.Counters=disabled
IOP.Events.CDVD=disabled
[TraceLogSources/IOP.Disasm.COP2]
GPU=disabled
[DialogPositions]
CoreSettings_Pos=912,311
[RecentIso]
Filename00=M:\\Games\\PlayStation 2\\Final Fantasy X (USA).iso
"""


