# --- CONFIG ---

# Suffix of the package archives, such as `-any.pkg.tar.zst`.
# Can be a space-separated lists of such suffixes, those will be tried in the specified order when downloading packages.
# At some point pacman switched from `.tar.xz` to `.tar.zst`, but MSYS2 repos still have `.tar.xz` around for old packages, so we have to support both.
REPO_PACKAGE_ARCHIVE_SUFFIXES := -any.pkg.tar.zst -any.pkg.tar.xz

# Extract packages here.
ROOT_DIR := root

# Download archives here.
CACHE_DIR := cache

# How many times to retry wget operations.
# We perform retries by rerunning the command, because wget itself doesn't seem to retry after certain errors (SSL errors among others),
#   and I couldn't find a flag to make it do so.
WGET_NUM_RETRIES := 3
# Wget timeout in seconds, after which we retry. If this is empty, the default value is used, which is 900 seconds (15 minutes) for some operations,
#   and apparently infinite for some other operations.
WGET_TIMEOUT_SEC := 15
# Those flags are passed to `wget` when downloading packages and other files.
WGET_FLAGS :=
# If false, won't pass any custom flags other than `WGET_FLAGS`.
WGET_ALLOW_DEFAULT_FLAGS := 1

# The contents of this variable are called as a shell command after any changes to the packages are made.
CALL_ON_PKG_CHANGE :=

# --- REPOSITORY SETTINGS ---

# The `MSYSTEM` variable determines the MSYS2 flavor. The value is loaded from a file.
# It's recommended to change this file in a clean repo, before downloading any packages. Or at least by running make `remove-all-packages` first.
MSYSTEM := $(file <msystem.txt)
# Default to MSYSTEM=UCRT64 if the file is missing.
$(if $(MSYSTEM),,$(eval MSYSTEM := UCRT64))

# Mirror URL. You can pass a custom mirror, or pass an empty string to use the primary repo.
MIRROR_URL := https://mirror.msys2.org
ifeq ($(MIRROR_URL),)
override MIRROR_URL := https://repo.msys2.org
endif

# If true, we're running from the build shell.
override running_in_build_env := $(if $(MSYSTEM_PREFIX),y)

ifeq ($(MSYSTEM),MINGW64)
# URL of the repository database.
REPO_DB_URL := $(MIRROR_URL)/mingw/x86_64/mingw64.db
# A common prefix for all packages.
# You don't have to set this variable, as it's only used for convenience, to avoid typing long package names. (See notes at the end of `make help` for details.)
REPO_PACKAGE_COMMON_PREFIX := mingw-w64-x86_64-
# Extra stuff needed for the environment setup scripts in `env/`. The package manager itself doesn't care about those.
MSYSTEM_PREFIX := /mingw64# The top-level directory of all packages.
MSYSTEM_CARCH := x86_64
MSYSTEM_CHOST := x86_64-w64-mingw32
else ifeq ($(MSYSTEM),MINGW32)
REPO_DB_URL := $(MIRROR_URL)/mingw/i686/mingw32.db
REPO_PACKAGE_COMMON_PREFIX := mingw-w64-i686-
MSYSTEM_PREFIX := /mingw32
MSYSTEM_CARCH := i686
MSYSTEM_CHOST := i686-w64-mingw32
else ifeq ($(MSYSTEM),UCRT64)
REPO_DB_URL := $(MIRROR_URL)/mingw/ucrt64/ucrt64.db
REPO_PACKAGE_COMMON_PREFIX := mingw-w64-ucrt-x86_64-
MSYSTEM_PREFIX := /ucrt64
MSYSTEM_CARCH := x86_64
MSYSTEM_CHOST := x86_64-w64-mingw32
else ifeq ($(MSYSTEM),CLANG64)
REPO_DB_URL := $(MIRROR_URL)/mingw/clang64/clang64.db
REPO_PACKAGE_COMMON_PREFIX := mingw-w64-clang-x86_64-
MSYSTEM_PREFIX := /clang64
MSYSTEM_CARCH := x86_64
MSYSTEM_CHOST := x86_64-w64-mingw32
else
$(error Unknown MSYSTEM: $(MSYSTEM))
endif

# To add more `MSYSTEM`s:
# * Find the appropriate repository at: http://repo.msys2.org/mingw/
# * Copy variables from: https://github.com/msys2/MSYS2-packages/blob/master/filesystem/msystem

# Database signature URL.
REPO_DB_SIG_URL := $(REPO_DB_URL).sig

# The keys are downloaded from here.
KEYRING_URL := https://raw.githubusercontent.com/msys2/MSYS2-keyring/master/msys2.gpg


# --- VERSION ---
override version := 1.6.12


# --- GENERIC UTILITIES ---

# Disable parallel builds.
.NOTPARALLEL:

# This is not only for looks. The extra junk text confuses our code when we query package database.
MAKEFLAGS += --no-print-directory


# Display name of our executable, to give to the user.
ifeq ($(running_in_build_env),)
override self := make
else
# We're in a build shell, give user the name of the wrapper.
override self := pacmake
endif


# All used `make` flags that can be spelled as single letters, without any `-`s.
override makeflags_single_letters := $(filter-out -%,$(word 1,$(MAKEFLAGS)))
# Non-empty if we're being called from an autocompletion context.
override called_from_autocompletion := $(and $(findstring p,$(makeflags_single_letters)),$(findstring q,$(makeflags_single_letters)))


# Some constants.
override space := $(strip) $(strip)
override comma := ,
override define lf :=
$(strip)
$(strip)
endef

# Used to create local variables in a safer way. E.g. `$(call var,x := 42)`.
override var = $(eval override $(subst $,$$$$,$1))

# Encloses $1 in single quotes, with proper escaping for the shell.
# If you makefile uses single quotes everywhere, a decent way to transition is to manually search and replace `'(\$(?:.|\(.*?\)))'` with `$(call quote,$1)`.
override quote = '$(subst ','"'"',$1)'

# Repeats $2 string $1 times. The result is space-separated.
# You can pass multiple space-separated numbers to `$1, then it will stop at the minimum number. This is good to prevent infinite loops.
override repeat_n_times = $(call repeat_n_times_,$1,$2,)
override repeat_n_times_ = $(if $(filter $1,$(words $3)),,$2 $(call repeat_n_times_,$1,$2,$3 x))

ifeq ($(filter --trace,$(MAKEFLAGS)),)
# Same as `$(shell ...)`, but triggers a error on failure.
override safe_shell = $(shell $1)$(if $(filter-out 0,$(.SHELLSTATUS)),$(error Unable to execute `$1`, exit code $(.SHELLSTATUS)))
# Same as `$(shell ...)`, expands to the shell status code rather than the command output.
override shell_status = $(call,$(shell $1))$(.SHELLSTATUS)
else
# Same functions but with logging.
override safe_shell = $(info Shell command: $1)$(shell $1)$(if $(filter-out 0,$(.SHELLSTATUS)),$(error Unable to execute `$1`, exit code $(.SHELLSTATUS)))
override shell_status = $(info Shell command: $1)$(call,$(shell $1))$(.SHELLSTATUS)$(info Exit code: $(.SHELLSTATUS))
endif

# Same as `safe_shell`, but discards the output and expands to nothing.
override safe_shell_exec = $(call,$(call safe_shell,$1))

# Expands to non-empty string if the file `$1` exists. Can handle spaces in file names.
# Doesn't have the lame caching issues of the built-in `wildcard`.
override file_exists = $(filter 0,$(call shell_status,test -e $(call quote,$1)))

# Same as the built-in `wildcard`, but without the dumb caching issues and with more sanity checks.
# Make tends to cache the results of `wildcard`, and doesn't invalidate them when it should.
override safe_wildcard = $(foreach x,$(call safe_shell,echo $1),$(if $(filter 0,$(call shell_status,test -e $(call quote,$x))),$x))

# `--show-progress -q` for wget1 or `--progress=bar` for wget2.
# Or, if stderr is not attached to a terminal, just `-q` for wget1 or nothing for wget2 (to disable the progress output, because otherwise wget
#   uses a really verbose progress style that makes the logs hard to read).
# Wget1 without `-q` prints a lot of junk. Wget2 doesn't print junk, and `-q` makes it hide the progressbar, so we don't use it there.
# Wget2 seems to show the progressbar by default, but I'm still passing `--progress=bar` just in case.
override wget_progress_flag = $(if $(__wget_progress_flag),,$(call var,__wget_progress_flag := $(if $(filter 0,$(call shell_status,wget --version | head -1 | grep Wget2)),$(if $(MAKE_TERMERR),--progress=bar),$(if $(MAKE_TERMERR),--show-progress) -q)))$(__wget_progress_flag)

# Downloads url $1 to file $2.
# On success expands to nothing. On failure deletes the unfinished file and expands to a non-empty string.
# Need `1>&2` to see the progressbar in wget2. Seems to have no effect on wget1.
# Note `$(WGET_NUM_RETRIES) 10`, this caps the number of retries if `WGET_NUM_RETRIES` is not a number.
override use_wget = $(filter-out 0,$(call shell_status,$(call repeat_n_times,$(WGET_NUM_RETRIES) 10,wget $(call quote,$1) $(if $(call boolean,WGET_ALLOW_DEFAULT_FLAGS),-c $(wget_progress_flag)) $(if $(WGET_TIMEOUT_SEC),--timeout=$(WGET_TIMEOUT_SEC)) $(WGET_FLAGS) -O $(call quote,$2) 1>&2 ||) (rm -f $(call quote,$2) && false)))

# Prints $1 to stderr.
override print_log = $(call safe_shell_exec,echo >&2 $(call quote,$1))

# Removes the last occurence of $1 in $2, and everything that follows.
# $2 has to contain no spaces (same for $1).
# If $1 is not found in $2, returns $2 without changing it.
override remove_suffix = $(subst <<<,,$(subst $1$(lastword $(subst $1, ,$2)<<<),<<<,$2<<<))

# Removes duplicates from the list $1 without sorting it.
override uniq_no_sort = $(if $1,$(firstword $1) $(call uniq_no_sort,$(filter-out $(firstword $1),$1)))

# Given a variable name $1, returns empty if it's 0, non-empty if 1, or an error otherwise.
override boolean = $(if $(or $(findstring $($1),0),$(findstring $($1),1)),,$(error $1 must be 0 or 1))$(subst 0,,$($1))


# --- CHECK PRECONDITIONS ---

override installation_directory_marker := msys2_pacmake_base_dir

# Check if the `$(installation_directory_marker)` file exists in the current directory, otherwise stop.
# This makes sure the working directory is correct, to avoid accidentally creating files outside of the installation directory.
ifeq ($(call file_exists,./$(installation_directory_marker)),)
$(info Incorrect working directory.)
$(info Invoke `$(self)` directly from the installation directory,)
$(info or specify the installation directory using `-C <dir>` flag.)
$(error Aborted)
endif

# Check if the `QUASI_MSYS2_ROOT` environment variable exists. If it is, it means `env/vars.sh` was already invoked.
# In this case we refuse to run, because the makefile may not work correctly with MSYS2 stuff in the PATH.
ifneq ($(QUASI_MSYS2_ROOT),)
$(info Please use the `pacmake` wrapper instead.)
$(info Refuse to run directly from `make` after `env/vars.sh` was invoked.)
$(error Aborted)
endif


# --- PROCESS PARMETERS ---

# A default target.
.DEFAULT_GOAL := help
ifeq ($(words $(MAKECMDGOALS)),0)
# Note that this assignment doesn't make `make` execute the target, so we also need to set `.DEFAULT_GOAL`.
MAKECMDGOALS := help
endif

override display_help :=
ifneq ($(filter help,$(word 1,$(MAKECMDGOALS))),)
override display_help := y
endif

override p_is_set :=
override p = $(error No parameters specified)

override stop_if_have_parameters = $(if $(p_is_set),$(error This action requires no parameters))

# If more than one parameter is specified...
ifneq ($(filter-out 0 1,$(words $(MAKECMDGOALS))),)
# If it's a database query, do nothing.
# Otherwise, convert all targets after the first one to parameters. Also replace `_` with a proper prefix.
# Also create a list of fake empty recipes for those targets.
$(if $(filter __database_%,$(MAKECMDGOALS)),,\
	$(call var,p_is_set := y)\
	$(call var,p := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS)))\
	$(foreach x,$(sort $p),$(eval .PHONY: $x)$(eval $x: ; @true))\
	$(call var,p := $(patsubst _%,$(REPO_PACKAGE_COMMON_PREFIX)%,$p))\
	$(call var,p := $(strip $(call uniq_no_sort,$p)))\
)
endif


# --- DETECT A RESTART ---

$(if $(MAKE_RESTARTS),$(call var,make_was_restarted := yes))
ifneq ($(make_was_restarted),)
$(call print_log,Restaring 'make'...)
endif


# --- SIGNATURE INTERNALS ---

# Download path for the original, unfiltered keyring. Note, it might contain revoked keys!
override sig_keyring_path_orig := keyring.original_unfiltered

# A path for the filtered keyring, with revoked and irrelevant keys removed.
override sig_keyring_path_filtered := keyring.gpg

# Path for the final keyring, a binary version of `$(sig_keyring_path_filtered)`.
override sig_keyring_path := keyring.kbx

# Downloads a new keyring to `$(sig_keyring_path)`.
override sig_update_keyring = \
	$(call print_log,Updating keyring...)\
	$(call, ### Erase the old file, since we call wget in the 'continue' mode.)\
	$(call safe_shell_exec,rm -f $(call quote,$(sig_keyring_path_orig)) $(call quote,$(sig_keyring_path_filtered))) \
	$(if $(call use_wget,$(KEYRING_URL),$(sig_keyring_path_orig)),$(error Unable to download the keyring. Try again with `--trace` to debug)) \
	$(call print_log,Found keys:)\
	$(call safe_shell_exec,awk '/Comment: packager: / {x=1; print "-----BEGIN PGP PUBLIC KEY BLOCK-----"; c=$$0; sub(/Comment: packager: /,"* ",c); print c > "/dev/stderr"} x {print} /-----END/ {x=0}' $(call quote,$(sig_keyring_path_orig)) >$(call quote,$(sig_keyring_path_filtered)))\
	$(call safe_shell_exec,rm -f $(call quote,$(sig_keyring_path_orig)))\
	$(call, ### I've tested, it does remove the existing keys.)\
	$(call safe_shell_exec,LANG= gpg --batch --yes -o ./$(call quote,$(sig_keyring_path)) --dearmor $(call quote,$(sig_keyring_path_filtered)))\
	$(call safe_shell_exec,rm -f $(call quote,$(sig_keyring_path_filtered)))

# Check that gpgv is installed. We do this manually to avoid a confusing "signature check failed" when gpgv isn't installed.
override sig_check_gpgv_installed = $(if $(__sig_check_gpgv_installed_once),$(call var,__sig_check_gpgv_installed_once :=)$(call safe_shell_exec,gpgv --version))
override __sig_check_gpgv_installed_once := 1

# Verifies a signature against the current keyring. Causes an error on failure.
# $1 is the file.
# $2 is the signature.
override sig_verify = \
	$(sig_check_gpgv_installed)\
	$(if $(filter-out 0,$(call shell_status,LANG= gpgv --keyring ./$(call quote,$(sig_keyring_path)) $(call quote,$2) $(call quote,$1) $(if $(filter --trace,$(MAKEFLAGS)),,>/dev/null 2>/dev/null))),\
		$(error Signature check failed!$(lf)File: $1$(lf)Signature: $2$(lf)Try again with `--trace` to see GPG output)\
	)


# --- DATABASE INTERNALS ---

# The main database file.
override database_processed_file := database.mk

# A backup for the main database file.
override database_processed_file_bak := database.mk.bak

# Temporary database file, downloaded directly from the repo, before its signature was validated.
override database_tmp_file_unverified := database.db.unverified
# Temporary database file, downloaded directly from the repo.
override database_tmp_file := database.db
# Temporary database signature file.
override database_tmp_file_sig := database.db.sig
# A copy of `database.db`. The current processed database should be based on this file.
override database_tmp_file_original := database.current_original
# Database signature file.
override database_tmp_file_original_sig := database.current_original.sig

# Temporary database directory.
override database_tmp_dir := database

# A file (that can be created by user) specifying preferred package alternatives.
# It should contain a space-separated list of `<alias>:<package>` entries.
override database_alternatives_file := alternatives.txt


# A pattern for package desciption files.
override desc_pattern := $(database_tmp_dir)/*/desc

# Converts description file names to package names (with versions).
override desc_file_to_name_ver = $(patsubst $(subst *,%,$(desc_pattern)),%,$1)

# Strips versions from package names.
override strip_ver = $(foreach x,$1,$(call remove_suffix,-,$(call remove_suffix,-,$x)))
# Strips version conditions from package names, e.g. `=1.2` or `>=1.2`.
override strip_ver_cond = $(foreach x,$1,$(call remove_suffix,=,$(call remove_suffix,>=,$x)))

# Extracts section contents from a package description file.
# $1 is the section name, such as `%DEPENDS%`.
# $2 is the file contents, as a string.
override extract_section = $(subst <, ,$(sort $(word 1,$(subst %, ,$(word 2,$(subst $1, ,$(subst $(space),<,$(strip $2))))))))

# Prints to a file $1 a variable that contains the version of the package $2.
# $1 is a destination file.
# $2 is a package name, with version.
override code_pkg_version_var = $(file >>$1,override VERSION_OF_$(call strip_ver,$2) := $(subst $(call strip_ver,$2)-,,$2))

# Prints to a file $1 a variable that contains the dependencies of the package $1 that have a version specified.
# $1 is a destination file.
# $2 is a package name, with version.
# $3 is a list of dependencies, from a package descritpion file (extracted with `extract_section`).
#   override code_pkg_deps_versions_var = $(file >>$1,override DEP_VERS_OF_$(call strip_ver,$2) := $(sort $(foreach x,$3,$(if $(findstring =,$x),$x))))

# Prints to a file $1 a make target for a list of packages $2, that depend on packages $3.
# $1 is a destination file.
# Both $2 and $3 shouldn't contain versions.
# The first name in $2 is considered to be the actual name of the package.
override code_pkg_target = \
	$(file >>$1,.PHONY: $(addprefix PKG@@,$(sort $2)))\
	$(file >>$1,$(addprefix PKG@@,$(sort $2)): $$(if $$(__deps),$(addprefix PKG@@,$(sort $3))) ; @echo 'PKG@@$(word 1,$2)')


# We don't use `.INTERMEDIATE`, since the recipe for `$(database_processed_file)` moves this file rather than deleting it.
.SECONDARY: $(database_tmp_file)
$(database_tmp_file):
	$(call, ### Update keyring if: __no_update_keyring is 0 [default], OR if there's no keyring)\
	$(if $(and $(filter-out 0,$(__no_update_keyring)),$(call file_exists,$(sig_keyring_path))),,$(sig_update_keyring))\
	$(call, ### Delete target files to avoid wget continuing the old download, to be a bit more robust.)\
	$(call, ### We don't do this when downloading packages because for those the filename is changed with every new version.)\
	$(call safe_shell_exec,rm -f $(call quote,$(database_tmp_file_sig)))\
	$(call safe_shell_exec,rm -f $(call quote,$(database_tmp_file_unverified)))\
	$(call print_log,Downloading package database...)
	$(if $(call use_wget,$(REPO_DB_SIG_URL),$(database_tmp_file_sig)),$(error Unable to download the database signature. Try again with `--trace` to debug))\
	$(if $(call use_wget,$(REPO_DB_URL),$(database_tmp_file_unverified)),$(error Unable to download the database. Try again with `--trace` to debug))
	$(call sig_verify,$(database_tmp_file_unverified),$(database_tmp_file_sig))\
	$(call print_log,Database signature is valid.)\
	$(call safe_shell_exec,mv -f $(call quote,$(database_tmp_file_unverified)) $(call quote,$@))
	@true


# Skip downloading new keyring when updating the database. `make reparse-database` uses this internally.
__no_update_keyring := 0

# The target that parses the database info a helper makefile.
# We perform some conflict resolution on the packages here: sometimes two packages have the same alias,
#   or even a name of a package is an alias of a different one. In that case we strip the alias from one of the packages.
#   If a package gets stripped of its canonical name, it's not added to the database.
#   When two aliases conflict, the first package (alphabetically, probably) gets precedence.
#   When an alias conflicts with a canonical name, the owner of the name gets preference.
#   Both rules can be overriden.
# It seems a package is allowed to announce an alias that matches its name. We filter out such aliases.
$(database_processed_file): $(database_tmp_file)
	$(call var,_local_bad_conflict_resolutions :=)\
	$(call var,_local_database_not_changed := $(strip \
		$(if $(call file_exists,$@),$(call,$(shell cmp -s $(call quote,$(database_tmp_file)) $(call quote,$(database_tmp_file_original))))$(filter 0,$(.SHELLSTATUS)))\
	))\
	$(if $(_local_database_not_changed),\
		$(call print_log,The database has not changed.)\
		$(call safe_shell_exec,rm -f $(call quote,$(database_tmp_file)) $(call quote,$(database_tmp_file_sig)))\
	,\
		$(call print_log,Extracting package database...)\
		$(call safe_shell_exec,rm -rf $(call quote,$(database_tmp_dir)))\
		$(call safe_shell_exec,mkdir -p $(call quote,$(database_tmp_dir)))\
		$(call safe_shell_exec,tar -C $(call quote,$(database_tmp_dir)) -xf $(call quote,$(database_tmp_file)))\
		$(if $(call file_exists,$@),$(call safe_shell_exec,mv -f $(call quote,$@) $(call quote,$(database_processed_file_bak))))\
		$(call print_log,Processing package database...)\
		$(call var,_local_db_files := $(sort $(call safe_wildcard,$(desc_pattern))))\
		$(call var,_local_pkg_list :=)\
		$(call var,_local_pkg_list_with_aliases :=)\
		$(call, ### All packages mentioned as dependencies are collected here. Sometimes it can have packages from other MSYS2 environments, which we replace with stubs.)\
		$(call var,_local_dep_list :=)\
		$(call, ### A list of `name|alias` containing all known aliases and names, mapping them to canonical names, including identity mapping.)\
		$(call var,_local_dupe_check_list :=)\
		$(call, ### For each non-canonical name that has alternatives and was handled implicitly, stores `alias|discarded3/discarded2/discarded1`, listing every discarded canonical name for it.)\
		$(call var,_local_auto_discarded_alts :=)\
		$(call, ### List of `target:override` from `alternatives.txt`.)\
		$(call var,_local_conflict_resolutions := $(if $(call file_exists,$(database_alternatives_file)),$(call safe_shell,cat $(call quote,$(database_alternatives_file)))))\
		$(call, ### All canonical names that are NOT mentioned in `alternatives.txt`.)\
		$(call var,_local_non_overriden_canonical_pkg_names := $(filter-out $(foreach x,$(_local_conflict_resolutions),$(word 1,$(subst :, ,$x))),$(call strip_ver,$(call desc_file_to_name_ver,$(_local_db_files)))))\
		$(call, ### A copy of rules from `alternatives.txt`, we remove any used rules from here. Anything that remains here at the end is invalid.)\
		$(call var,_local_bad_conflict_resolutions := $(_local_conflict_resolutions))\
		$(call var,_local_had_any_conflicts :=)\
		$(foreach x,$(_local_db_files),\
			$(call var,_local_name_ver := $(call desc_file_to_name_ver,$x))\
			$(call var,_local_name := $(call strip_ver,$(_local_name_ver)))\
			$(call var,_local_file := $(call safe_shell,cat $(call quote,$x)))\
			$(call var,_local_deps := $(call strip_ver_cond,$(call extract_section,%DEPENDS%,$(_local_file))))\
			$(call var,_local_aliases := $(filter-out $(_local_name),$(call strip_ver_cond,$(call extract_section,%PROVIDES%,$(_local_file)))))\
			$(call var,_local_aliases := $(foreach y,$(_local_aliases),$(if $(filter $y,$(_local_non_overriden_canonical_pkg_names)),\
				$(call print_log,Note: package '$y' has alternative '$(_local_name)'.)$(call var,_local_had_any_conflicts := y),\
				$y)))\
			$(call var,_local_banned_names := $(foreach x,$(filter-out %:$(_local_name),$(_local_conflict_resolutions)),$(word 1,$(subst :, ,$x))))\
			$(call var,_local_lhs := $(filter-out $(_local_banned_names),$(_local_name) $(_local_aliases)))\
			$(foreach y,$(_local_lhs),\
				$(call var,_local_conflict := $(strip $(word 1,$(filter %|$y,$(_local_dupe_check_list)))))\
				$(if $(_local_conflict),\
					$(call var,_local_lhs := $(filter-out $y,$(_local_lhs)))\
					$(call var,_local_had_any_conflicts := y)\
					$(if $(filter $y|%,$(_local_auto_discarded_alts)),\
						$(call var,_local_auto_discarded_alts := $(patsubst $y|%,$y|$(_local_name)/%,$(_local_auto_discarded_alts)))\
					,\
						$(call var,_local_auto_discarded_alts += $y|$(_local_name))\
					)\
				)\
			)\
			$(call var,_local_dupe_check_list += $(addprefix $(_local_name)|,$(_local_lhs)))\
			$(if $(filter-out $(_local_banned_names),$(_local_name)),\
				$(call var,_local_bad_conflict_resolutions := $(filter-out $(addsuffix :$(_local_name),$(_local_lhs)),$(_local_bad_conflict_resolutions)))\
				$(call code_pkg_version_var,$@,$(_local_name_ver))\
				$(call code_pkg_target,$@,$(_local_lhs),$(_local_deps))\
				$(file >>$@,)\
				$(call var,_local_pkg_list += $(_local_name))\
				$(call var,_local_pkg_list_with_aliases += $(_local_lhs))\
				$(call var,_local_dep_list += $(_local_deps))\
			,\
				$(call print_log,Note: package '$(_local_name)' is inaccessible because of the selected alternatives.)\
			)\
		)\
		$(call, ### Write stubs for unknown dependencies.)\
		$(file >>$@,# Unknown deps:$(lf))\
		$(call var,_local_bad_deps := $(filter-out $(_local_pkg_list_with_aliases),$(_local_dep_list)))\
		$(file >>$@,.PHONY: $(_local_bad_deps))\
		$(file >>$@,$(addprefix PKG@@,$(_local_bad_deps)):)\
		$(file >>$@,$(lf))\
		$(file >>$@,override FULL_PACKAGE_LIST := $(sort $(_local_pkg_list)))\
		$(file >>$@,override FULL_ALIAS_LIST := $(sort $(_local_pkg_list_with_aliases)))\
		$(if $(_local_had_any_conflicts),\
			$(foreach x,$(_local_auto_discarded_alts),\
				$(call var,_local_name := $(firstword $(subst |, ,$x)))\
				$(call var,_local_default_alt := $(patsubst %|$(_local_name),%,$(filter %|$(_local_name),$(_local_dupe_check_list))))\
				$(call var,_local_other_alts := $(sort $(subst /, ,$(word 2,$(subst |, ,$x)))))\
				$(call print_log,Note: name '$(_local_name)' is ambiguous$(comma) could mean: '$(_local_default_alt)' (used by default) or $(subst $(space),$(comma) ,$(foreach y,$(_local_other_alts),'$y')))\
			)\
			$(call print_log,Note: see `$(self) help` for instructions on changing alternatives.)\
		)\
		$(call safe_shell_exec,mv -f $(call quote,$(database_tmp_file)) $(call quote,$(database_tmp_file_original)))\
		$(call safe_shell_exec,mv -f $(call quote,$(database_tmp_file_sig)) $(call quote,$(database_tmp_file_original_sig)))\
	)
	$(call safe_shell_exec,rm -rf './$(database_tmp_dir)/')
	$(if $(_local_bad_conflict_resolutions),\
		$(call print_log,Warning: following entries in '$(database_alternatives_file)' are invalid:)\
		$(foreach x,$(_local_bad_conflict_resolutions),$(call print_log,* $x))\
		$(call print_log,THIS MAY CAUSE PROBLEMS. Fix the file and run '$(self) reparse-database'.)\
	)
	@true


# Load the database if we got a query.
ifneq ($(filter __database_%,$(MAKECMDGOALS)),)
override __deps := $(if $(filter __database_nodeps,$(MAKECMDGOALS)),,y)
include $(database_processed_file)
# Also validate all package names specified in the command line.
# Note that we don't perform validation if the database file is missing. In that case it
# will be generated soon, and then Make will be restarted, and then we'll perform the checks.
ifneq ($(call safe_wildcard,$(database_processed_file)),)
ifneq ($(filter __database_allow_aliases,$(MAKECMDGOALS)),)
$(foreach x,$(patsubst PKG@@%,%,$(filter PKG@@%,$(MAKECMDGOALS))),$(if $(filter $x,$(FULL_ALIAS_LIST)),,$(error Unknown package or package alias: '$x')))
else
$(foreach x,$(patsubst PKG@@%,%,$(filter PKG@@%,$(MAKECMDGOALS))),$(if $(filter $x,$(FULL_PACKAGE_LIST)),,$(error Unknown package: '$x')))
endif
endif

endif

# Internal database interface:

# Invokes make with $1 parameters, and returns the result.
# `MAKEFLAGS=` is required here, otherwise `--trace` breaks this function, because it changes its output.
override invoke_database_process = $(call safe_shell,MAKEFLAGS= $(MAKE) $(MAKEOVERRIDES) -r $1)

# Serves as the query parameter.
__packages = $(error The parameter `__packages` is not set)

# A dummy target, simply loads the database. Good for querying package dependencies.
.PHONY: __database_load
__database_load:
	@true

# A dummy target similar to `__database_load`, but silences any information about the dependencies.
# Good for resolving package aliases into actual package names.
.PHONY: __database_nodeps
__database_nodeps:
	@true

# A dummy target similar to `__database_load`, but allows package aliases to be passed instead of actual package names.
# Good for resolving package aliases into actual package names.
.PHONY: __database_allow_aliases
__database_allow_aliases:
	@true

# Given a list of package names without versions, returns the version of each package.
# Uses `__packages`.
.PHONY: __database_get_versions
__database_get_versions:
	$(info $(strip $(foreach x,$(__packages),$(if $(VERSION_OF_$x),$(VERSION_OF_$x),??))))
	@true

# Returns a list of all available packages.
.PHONY: __database_list_all
__database_list_all:
	$(info $(FULL_PACKAGE_LIST))
	@true

# Returns a list of all available packages, including aliases.
.PHONY: __database_list_all_with_aliases
__database_list_all_with_aliases:
	$(info $(FULL_ALIAS_LIST))
	@true

# Verifies a list of packages.
# Uses `__packages`.
.PHONY: __database_verify
__database_verify:
	$(call var,_local_missing := $(sort $(filter-out $(FULL_PACKAGE_LIST), $(__packages))))
	$(if $(_local_missing),$(error Following packages are not known: $(_local_missing)))
	@true


# --- DATABASE INTERFACE ---

# Does nothing. But if the database is missing, downloads it.
override database_query_empty = $(call,$(call invoke_database_process,__database_load))

# Returns a list of all available packages.
override database_query_available = $(call invoke_database_process,__database_list_all)

# Returns a list of all available packages.
override database_query_available_with_aliases = $(call invoke_database_process,__database_list_all_with_aliases)

# Verifies a list of packages.
# Causes an error on failure, expands to nothing.
# Commented out because it doesn't respect package aliases.
# override database_query_verify = $(call invoke_database_process,__database_verify __packages=$(call quote,$1))

# Given a list of package names and/or aliases, returns their canonical names.
override database_query_resolve_aliases = $(patsubst PKG@@%,%,$(call invoke_database_process,__database_nodeps __database_allow_aliases $(addprefix PKG@@,$1)))

# $1 is a list of package names, without versions.
# Returns $1, with all dependencies added.
override database_query_deps = $(sort $(patsubst PKG@@%,%,$(filter PKG@@%,$(call invoke_database_process,__database_load $(addprefix PKG@@,$1)))))

# $1 is a list of package names, without versions.
# Returns the version of each package.
override database_query_version = $(call invoke_database_process,__database_get_versions __packages=$(call quote,$1))

# $1 is a list of package names, without versions.
# Returns the same list, but with versions added.
override database_query_full_name = $(join $1,$(addprefix -,$(call database_query_version,$1)))


# --- CACHE INTERNALS ---

ifeq ($(call file_exists,$(CACHE_DIR)),)
$(call safe_shell_exec,mkdir -p $(call quote,$(CACHE_DIR)))
endif

ifeq ($(call file_exists,$(ROOT_DIR)),)
$(call safe_shell_exec,mkdir -p $(call quote,$(ROOT_DIR)))
endif

# A prefix for unfinished downloads.
override cache_unfinished_prefix = ---

# $1 is the url, relative to the repo url. If it's a space-separated list of urls, they are tried in order until one works.
# If the file is not cached, it's downloaded.
# The signature is also downloaded automatically, and checked.
override cache_download_file_if_missing = \
	$(if $(strip $(foreach x,$1,$(call file_exists,$(CACHE_DIR)/$(notdir $x)))),,\
		$(call var,_local_continue := y)\
		$(call var,_local_first := y)\
		$(foreach x,$1,$(if $(_local_continue),\
			$(if $(_local_first),$(call var,_local_first :=),$(call print_log,Failed$(comma) trying different suffix.))\
			$(call print_log,Downloading '$(notdir $x)'...)\
			$(if $(call use_wget,$(dir $(REPO_DB_URL))$x.sig,$(CACHE_DIR)/$(cache_unfinished_prefix)$(notdir $x).sig),,\
				$(call var,_local_continue :=)\
				$(if $(call use_wget,$(dir $(REPO_DB_URL))$x,$(CACHE_DIR)/$(cache_unfinished_prefix)$(notdir $x)),$(error Downloaded the signature, but can't download the package. Try again with `--trace` to debug))\
				$(call sig_verify,$(CACHE_DIR)/$(cache_unfinished_prefix)$(notdir $x),$(CACHE_DIR)/$(cache_unfinished_prefix)$(notdir $x).sig)\
				$(call print_log,Package signature is valid.)\
				$(call safe_shell_exec,mv -f $(call quote,$(CACHE_DIR)/$(cache_unfinished_prefix)$(notdir $x)) $(call quote,$(CACHE_DIR)/$(notdir $x)))\
				$(call safe_shell_exec,mv -f $(call quote,$(CACHE_DIR)/$(cache_unfinished_prefix)$(notdir $x).sig) $(call quote,$(CACHE_DIR)/$(notdir $x).sig))\
			)\
		))\
		$(if $(_local_continue),$(error Unable to download the package. Try again with `--trace` to debug))\
	)

# Deletes unfinished downloads.
override cache_purge_unfinished = $(foreach x,$(call safe_wildcard,$(CACHE_DIR)/$(cache_unfinished_prefix)*),$(call safe_shell_exec,rm -f $(call quote,$x)))

# --- CACHE INTERFACE ---

# $1 is a list of packages, with versions.
# If they are not cached, downloads them.
override cache_want_packages = \
	$(cache_purge_unfinished)\
	$(foreach x,$1,$(if $(findstring ??,$x),\
		$(error Can't add package to cache: unknown package: '$x'),\
		$(call cache_download_file_if_missing,$(addprefix $x,$(REPO_PACKAGE_ARCHIVE_SUFFIXES)))\
	))

# $1 is a package name with version.
# Returns the file name of its archive, which must be already in the cache. If it's not cached, emits an error.
override cache_find_pkg_archive = $(call var,_local_file = $(firstword $(foreach x,$(REPO_PACKAGE_ARCHIVE_SUFFIXES),$(call safe_wildcard,$(CACHE_DIR)/$1*$x))))$(if $(_local_file),$(_local_file),$(error Can't find package in the cache: $1))

# $1 is a list of packages, with versions.
# Outputs the list of contained files, without folders, with spaces replaced with `<`.
# Note `set -o pipefail`, without it we can't detect failure of lhs of the `|` shell operator.
# Note `bash -c`. We can't use the default shell (`sh`, which is a symlink for `dash` on Ubuntu), because it doesn't support `pipefail`.
override cache_list_pkg_files = $(foreach x,$1,$(call safe_shell,bash -c "set -o pipefail && tar -tf $(call quote,$(call cache_find_pkg_archive,$x)) --exclude='.*' | (grep '[^/]$$'; (($$? >= 0 && $$? <= 1))) | sed 's| |<|g'"))

# Lists current packages sitting in the cache.
override cache_list_current = \
	$(call var,_local_files := $(call safe_wildcard,$(CACHE_DIR)/*))\
	$(foreach x,$(REPO_PACKAGE_ARCHIVE_SUFFIXES),\
		$(patsubst $(CACHE_DIR)/%$x,%,$(filter $(CACHE_DIR)/%$x,$(_local_files)))\
	)

# Lists all archives (including missing ones) used by installed packages.
override cache_list_missing = $(filter-out $(cache_list_current),$(index_list_all_installed))

# Lists all archives not used by installed packages.
override cache_list_unused = $(filter-out $(index_list_all_installed),$(cache_list_current))


# --- INDEX INTERNALS ---

override index_dir := index
override index_pattern := $(index_dir)/*

ifeq ($(call file_exists,$(index_dir)),)
$(call safe_shell_exec,mkdir -p $(call quote,$(index_dir)))
endif

# A prefix for broken packages.
override index_broken_prefix := --broken--

# Causes an error if the package $1 is already installed, or is broken.
# $1 has to include the version.
override index_stop_if_single_pkg_installed = \
	$(if $(call file_exists,$(index_dir)/$1),$(error Package '$1' is already installed))\
	$(if $(call file_exists,$(index_dir)/$(index_broken_prefix)$1),$(error Installed package '$1' is broken))

# Causes an error if the package $1 is not installed (broken counts as installed).
# $1 has to include the version.
override index_stop_if_single_pkg_not_installed = \
	$(if $(strip $(call file_exists,$(index_dir)/$1)$(call file_exists,$(index_dir)/$(index_broken_prefix)$1)),,$(error Package '$1' is not installed))\

# Removes a single broken package.
# $1 is a package name, with version. $(index_broken_prefix) is assumed and shouldn't be specified.
override index_uninstall_single_broken_pkg = \
	$(foreach x,$(call index_list_pkg_files,$(index_broken_prefix)$1),$(call safe_shell_exec,rm -f '$(ROOT_DIR)/$(subst <, ,$x)'))\
	$(call safe_shell_exec,rm -f '$(index_dir)/$(index_broken_prefix)$1')\
	$(call print_log,Removed '$1')

# Removes all empty directories in the $(ROOT_DIR).
override index_clean_empty_dirs = $(call safe_shell_exec,find $(ROOT_DIR) -mindepth 1 -type d -empty -delete)


# --- INDEX INTERFACE ---

# Removes all packages that have the $(index_broken_prefix) prefix.
override index_purge_broken = \
	$(foreach x,$(call safe_wildcard,$(index_dir)/$(index_broken_prefix)*),$(call index_uninstall_single_broken_pkg,$(patsubst $(index_dir)/$(index_broken_prefix)%,%,$x)))\
	$(index_clean_empty_dirs)\
	$(if $(CALL_ON_PKG_CHANGE),$(call safe_shell_exec,$(CALL_ON_PKG_CHANGE)))

# Installs a list of packages $1 (which has to include versions), without considering dependencies.
# Make sure the packages are not already installed.
# They are downloaded if they're not already cached.
override index_force_install = \
	$(foreach p,$1,\
		$(call index_stop_if_single_pkg_installed,$p)\
		$(call cache_want_packages,$p)\
		$(call print_log,Extracting '$p'...)\
		$(call var,_local_files := $(call cache_list_pkg_files,$p))\
		$(foreach x,$(_local_files),$(if $(call file_exists,$(ROOT_DIR)/$(subst <, ,$x)),$(error Unable to install '$p': file `$(subst <, ,$x)` already exists)))\
		$(call, ### This creates the file if it doesn't exist, even if `_local_files` is empty. This is necessary for empty packages, such as `_autotools`.)\
		$(file >$(index_dir)/$(index_broken_prefix)$p)\
		$(foreach x,$(_local_files),$(file >>$(index_dir)/$(index_broken_prefix)$p,$x))\
		$(call safe_shell_exec,tar -C $(call quote,$(ROOT_DIR)) -xf $(call quote,$(call cache_find_pkg_archive,$p)) --exclude='.*')\
		$(call safe_shell_exec,mv -f '$(index_dir)/$(index_broken_prefix)$p' '$(index_dir)/$p')\
		$(call print_log,Installed '$p')\
	)\
	$(if $(CALL_ON_PKG_CHANGE),$(call safe_shell_exec,$(CALL_ON_PKG_CHANGE)))

# Removes a list of packages $1 (which has to include versions), without considering dependencies.
override index_force_uninstall = \
	$(foreach x,$(sort $(patsubst $(index_broken_prefix)%,%,$1)),\
		$(call index_stop_if_single_pkg_not_installed,$x)\
		$(if $(call file_exists,$(index_dir)/$x),$(call safe_shell_exec,mv -f '$(index_dir)/$x' '$(index_dir)/$(index_broken_prefix)$x'))\
		$(call index_uninstall_single_broken_pkg,$x)\
	)\
	$(index_clean_empty_dirs)\
	$(if $(CALL_ON_PKG_CHANGE),$(call safe_shell_exec,$(CALL_ON_PKG_CHANGE)))

# Expands to a list of all installed packages, with versions.
override index_list_all_installed = $(patsubst $(subst *,%,$(index_pattern)),%,$(call safe_wildcard,$(index_pattern)))

# Returns a list of files that belong to installed packages listed in $1.
# $1 has to include package versions.
# Spaces in filenames in the resulting list are replaced with `<`.
override index_list_pkg_files = $(sort $(foreach x,$1,$(if $(call file_exists,$(index_dir)/$x),,$(error Package '$x' is not installed))$(call safe_shell,cat '$(index_dir)/$x')))


# --- PACKAGE MANAGEMENT INTERNALS ---

# Requested packages will be saved to this file.
override request_list_file := requested_packages.txt

ifeq ($(call file_exists,$(request_list_file)),)
$(call safe_shell_exec,touch $(call quote,$(request_list_file)))
endif

# Writes a new list of requested packages (with versions) from $1.
override pkg_set_request_list = $(call safe_shell_exec,echo >$(call quote,$(request_list_file)) $(call quote,$(sort $1)))

# $1 is a list of packages, without versions. Warns if any packages in $1 are in the requested list.
override pkg_warn_if_already_requested = \
	$(call var,_local_cur_pkgs := $(pkg_request_list))\
	$(if $(filter-out $(_local_cur_pkgs),$1),\
    	$(call var,_local_filtered_pkgs := $(sort $(filter $1,$(_local_cur_pkgs))))\
    	$(if $(_local_filtered_pkgs),$(info Following packages are already requested: $(_local_filtered_pkgs)))\
	,\
		$(info All those packages are already requested.)\
	)

# $1 is a list of packages, without versions. Warns if any packages in $1 are not in the requested list.
override pkg_warn_if_already_not_requested = \
	$(call var,_local_cur_pkgs := $(pkg_request_list))\
	$(if $(filter $(_local_cur_pkgs),$1),\
    	$(call var,_local_filtered_pkgs := $(sort $(filter-out $(_local_cur_pkgs),$1)))\
    	$(if $(_local_filtered_pkgs),$(info Following packages are already not requested: $(_local_filtered_pkgs)))\
	,\
		$(info All those packages are already not requested.)\
	)

# --- PACKAGE MANAGEMENT INTERFACE ---

# Returns the list of requested packages.
override pkg_request_list = $(call safe_shell,cat $(call quote,$(request_list_file)))

# Clears the request list.
override pkg_request_list_reset = $(call pkg_set_request_list,)

# Adds packages to the list of requested packages.
# $1 is a list of packages without versions, possibly aliases.
override pkg_request_list_add = $(call var,_local_pkgs := $(call database_query_resolve_aliases,$1))$(call pkg_warn_if_already_requested,$(_local_pkgs))$(call pkg_set_request_list,$(pkg_request_list) $(_local_pkgs))

# Adds packages to the list of requested packages.
# $1 is a list of packages without versions.
override pkg_request_list_remove = $(call var,_local_pkgs := $(call database_query_resolve_aliases,$1))$(call pkg_warn_if_already_not_requested,$(_local_pkgs))$(call pkg_set_request_list,$(filter-out $(_local_pkgs),$(pkg_request_list)))

# Computes the delta between the current state and the desired state.
# Returns a list of packages with prefixes: `>` means a package should be installed, and `<` means it should be removed.
#   Note that we do `$(foreach x,$(pkg_request_list) ...` rather than passing the entire list
#   to `database_query_deps` to reduce the length of command-line parameters that are passed around.
override pkg_compute_delta = $(strip \
	$(call var,_state_cur := $(index_list_all_installed))\
	$(call var,_state_target := $(sort $(foreach x,$(pkg_request_list),$(call database_query_full_name,$(call database_query_deps,$x)))))\
	$(addprefix <,$(filter-out $(_state_target),$(_state_cur)))\
	$(addprefix >,$(filter-out $(_state_cur),$(_state_target))))

# Prints a delta.
# $1 is the delta data.
# $2 (optional) is a message that will be printed before the delta if the delta is not empty.
# Packages that should be removed are prefixed with `- `, and packages that shoudl be installed are prefixed with `+ `.
override pkg_pretty_print_delta = \
	$(if $(and $1,$2),$(info $2))\
	$(foreach x,$(patsubst <%,%,$(filter <%,$1)),$(info - $x))\
	$(foreach x,$(patsubst >%,%,$(filter >%,$1)),$(info + $x))

# Prints a delta.
# $1 is the delta data.
# $2 (optional) is a message that will be printed before the delta if the delta is not empty.
# Same as pkg_pretty_print_delta`, but package updates are printed separately and prefixed with `> `.
override pkg_pretty_print_delta_fancy = \
	$(if $(and $1,$2),$(info $2))\
	$(call var,_local_delta := $1)\
	$(call var,_local_upd :=)\
	$(foreach x,$(_local_delta),$(if $(filter <%,$x),\
		$(call var,_local_name_ver := $(patsubst <%,%,$x))\
		$(call var,_local_name := $(call strip_ver,$(_local_name_ver)))\
		$(foreach y,$(_local_delta),$(if $(filter 1,$(words $(sort $(_local_name) $(call strip_ver,$(patsubst >%,%,$y))))),\
			$(call var,_local_upd += $(_local_name_ver)>$(subst >$(_local_name)-,,$y))\
			$(call var,_local_delta := $(filter-out $x $y,$(_local_delta)))\
		))\
	))\
	$(foreach x,$(patsubst <%,%,$(filter <%,$(_local_delta))),$(info - $x))\
	$(foreach x,$(patsubst >%,%,$(filter >%,$(_local_delta))),$(info + $x))\
	$(foreach x,$(_local_upd),$(info > $(subst >, >> ,$x)))

# Applies a delta.
# $1 is the delta data.
override pkg_apply_delta = \
	$(call cache_want_packages,$(patsubst >%,%,$(filter >%,$1)))\
	$(call index_force_uninstall,$(patsubst <%,%,$(filter <%,$1)))\
	$(call index_force_install,$(patsubst >%,%,$(filter >%,$1)))

# Fancy-prints a delta, then applies it.
# The delta will be preceeded by a disclaimer.
# $1 is the delta data.
override pkg_print_then_apply_delta = \
	$(if $1,\
		$(info Following changes will be applied:)\
		$(call pkg_pretty_print_delta_fancy,$1)\
		$(call pkg_apply_delta,$1)\
	,\
		$(info No actions needed.)\
	)


# --- TARGETS ---

# Creates a new public target.
# $1 is name.
# $2 is a user-facing parameter name, or empty if it accepts no parameters.
# #3 is human-readable descrption
# Note that the target name is uglified (unless it's the target that's being called AND unless our makefile is being examined for autocompletion).
override act = \
	$(call var,_local_target := $(if $(or $(called_from_autocompletion),$(filter $1,$(word 1,$(MAKECMDGOALS)))),,>>)$(strip $1))\
	$(eval .PHONY: $(_local_target))\
	$(if $(display_help),\
		$(info $(self) $(strip $1)$(if $2, <$2>))\
		$(info $(space)$(space)$(subst $(lf),$(lf)$(space)$(space),$3))\
		$(info )\
	)\
	$(_local_target): ; $(if $2,,$$(stop_if_have_parameters))

$(if $(display_help),\
	$(if $(p_is_set),$(error This action requires no parameters.))\
	$(info >> msys2-pacmake v$(version) <<)\
	$(info A simple makefile-based package manager that can be used instead of)\
	$(info MSYS2's pacman if it's not available (e.g. if you're not on Windows))\
	$(info )\
	$(info Usage:)\
	$(info )\
	)

# Defines a new public target section.
# $1 is name, in caps.
override act_section = $(if $(display_help),$(info -- $(strip $1) --$(lf)))


# MISC

$(call act, help \
,,Display this information and exit.)
	@true


# PACKAGE DATABASE
$(call act_section, PACKAGE DATABASE )

# Lists all available packages in the repo, without versions.
$(call act, list-all \
,,List all packages available in the repository$(comma) including their aliases.\
$(lf)Only some of the commands accept package aliases in addition to actual names.\
$(lf)Doesn't output package versions.)
	$(foreach x,$(database_query_available_with_aliases),$(info $x))
	@true

# Lists all available packages in the repo, without versions.
$(call act, list-all-canonical \
,,List all packages available in the repository.$(lf)Doesn't output package versions.)
	$(foreach x,$(database_query_available),$(info $x))
	@true

# Downloads a new database.
$(call act, update \
,,Download a new database. The existing database will be backed up.)
	$(call safe_shell_exec,$(MAKE) 1>&2 -B $(call quote,$(database_processed_file)))
	$(call pkg_pretty_print_delta_fancy,$(pkg_compute_delta),Run `$(self) apply-delta` to perform following changes:)
	@true

# Accepts a list of packages. Returns the same list, but with package versions specified.
$(call act, get-ver \
,packages,Print the specified packages with version numbers added.)
	$(foreach x,$(call database_query_full_name,$p),$(info $x))
	@true

# Accepts a list of packages. Returns the same list, with all dependencies added.
$(call act, get-deps \
,packages,Print the specified packages and all their dependencies.)
	$(foreach x,$(call database_query_deps,$p),$(info $x))
	@true

# Acceps a list of packages, prints their canonical names.
$(call act, get-canonical-name \
,packages-or-aliases,Print the canonical names of the packages.)
	$(foreach x,$(call database_query_resolve_aliases,$p),$(info $x))
	@true

# Cleans the database.
$(call act, clean-database \
,,Delete the package database$(comma) which contains the information about the repository.\
$(lf)A new database will be downloaded next time it is needed.)
	@rm -f $(call quote,$(database_processed_file)) \
		$(call quote,$(database_processed_file_bak)) \
		$(call quote,$(database_tmp_file_unverified)) \
		$(call quote,$(database_tmp_file)) \
		$(call quote,$(database_tmp_file_sig)) \
		$(call quote,$(database_tmp_file_original)) \
		$(call quote,$(database_tmp_file_original_sig))
	@rm -rf $(call quote,$(database_tmp_dir))

# Downloads a new database.
$(call act, reparse-database \
,,Reparse the database. Use this to update the database after\
$(lf)changing `$(database_alternatives_file)`$(comma) otherwise it shouldn't be necessary.)
	$(call safe_shell_exec,rm -f $(call quote,$(database_processed_file)))
	$(call safe_shell_exec,mv -f $(call quote,$(database_tmp_file_original)) $(call quote,$(database_tmp_file)) 2>/dev/null || true)
	$(call safe_shell_exec,mv -f $(call quote,$(database_tmp_file_original_sig)) $(call quote,$(database_tmp_file_sig)) 2>/dev/null || true)
	$(call safe_shell_exec,$(MAKE) 1>&2 __no_update_keyring=1 $(call quote,$(database_processed_file)))
	$(call pkg_pretty_print_delta_fancy,$(pkg_compute_delta),Run `$(self) apply-delta` to perform following changes:)
	@true


# PACKAGE MANAGEMENT
$(call act_section, PACKAGE MANAGEMENT )

# Lists all installed packages, with versions.
$(call act, list-ins \
,,List all installed packages$(comma) with versions.$(lf)The list includes automatically installed dependencies.)
	$(foreach x,$(index_list_all_installed),$(info $x))
	@true

# Lists all requested packages, without versions.
$(call act, list-req \
,,List all explicitly requested packages.)
	$(foreach x,$(pkg_request_list),$(info $x))
	@true

# Installs packages (without versions specified).
$(call act, install \
,packages-or-aliases,Install packages.$(lf)Equivalent to '$(self) request' followed by '$(self) apply-delta'.)
	$(call pkg_request_list_add,$p)
	$(call pkg_print_then_apply_delta,$(pkg_compute_delta))
	@true

# Removes packages (without versions specified).
$(call act, remove \
,packages-or-aliases,Remove packages.$(lf)Equivalent to '$(self) undo-request' followed by '$(self) apply-delta'.)
	$(call pkg_request_list_remove,$p)
	$(call pkg_print_then_apply_delta,$(pkg_compute_delta))
	@true

# Removes all packages.
$(call act, remove-all-packages \
,,Remove all packages.)
	$(info Deleting files...)
	$(call pkg_request_list_reset)
	$(call safe_shell_exec,rm -rf $(call quote,$(ROOT_DIR))/*)
	$(call safe_shell_exec,rm -rf $(call quote,$(index_dir))/*)
	@true

# Removes all packages.
$(call act, reinstall-all \
,,Reinstall all packages.)
	$(info Deleting files...)
	$(call safe_shell_exec,rm -rf '$(ROOT_DIR)/'*)
	$(call safe_shell_exec,rm -rf $(call quote,$(index_dir))/*)
	$(call pkg_apply_delta,$(pkg_compute_delta))
	@true

# Updates the database, upgrades packages, and fixes stuff.
$(call act, upgrade \
,,Update package database and upgrade all packages.)
	$(call safe_shell_exec, $(MAKE) 1>&2 upgrade-keep-cache)
	$(call safe_shell_exec, $(MAKE) 1>&2 cache-remove-unused)
	@true

# Rolls back the last upgrade.
$(call act, rollback \
,,Undo the last `$(self) upgrade`.)
	$(if $(call file_exists,$(database_processed_file_bak)),,$(error No database backup to roll back to))
	$(call safe_shell_exec,mv -f $(call quote,$(database_processed_file_bak)) $(call quote,$(database_processed_file)))
	$(call safe_shell_exec,rm -f $(call quote,$(database_tmp_file_original)) $(call quote,$(database_tmp_file_original_sig)))
	$(call pkg_print_then_apply_delta,$(pkg_compute_delta))
	@true

# Updates the database, upgrades packages, and fixes stuff. Doesn't remove old archives from the cache.
$(call act, upgrade-keep-cache \
,,Update package database and upgrade all packages.\
$(lf)Don't remove unused packages from the cache.)
	$(call safe_shell_exec,$(MAKE) -B $(call quote,$(database_processed_file)))
	$(call pkg_print_then_apply_delta,$(pkg_compute_delta))
	$(info Cleaning up...)
	$(call safe_shell_exec, $(MAKE) 1>&2 cache-purge-unfinished)
	@true

# Updates the database, upgrades packages, and fixes stuff. Cleans the cache.
$(call act, upgrade-clean-cache \
,,Update package database and upgrade all packages.\
$(lf)Clean the cache.)
	$(call safe_shell_exec, $(MAKE) 1>&2 upgrade-keep-cache)
	$(call safe_shell_exec, $(MAKE) 1>&2 clean-cache)
	@true

# Adds packages (without versions) to the request list.
$(call act, request \
,packages-or-aliases,Request packages to be installed.$(lf)The packages and their dependencies will be\
$(lf)installed next time '$(self) apply-delta' is called.)
	$(call pkg_request_list_add,$p)
	@true

# Removes packages (without versions) from the request list.
$(call act, undo-request \
,packages-or-aliases,Request packages to not be installed.$(lf)The packages and any dependencies that are no longer needed\
$(lf)will be removed next time '$(self) apply-delta' is called.)
	$(call pkg_request_list_remove,$p)
	@true

# Cleans the request list.
$(call act, clean-requests \
,,Clean the list of requested packages.)
	$(call pkg_request_list_reset)
	@true

# Prints the current delta.
$(call act, delta \
,,Prints the list of changes that should be applied to the installed packages to make sure\
$(lf)that the latest versions of the requested packages and their dependencies are installed.\
$(lf)`+` prefix means the package should be installed$(comma)\
$(lf)`-` prefix means the package should be removed$(comma) and\
$(lf)`>` prefix means the package should be updated.)
	$(call pkg_pretty_print_delta_fancy,$(pkg_compute_delta))
	@true

# Prints the current delta, in a simple form.
$(call act, simple-delta \
,,Similar to `$(self) delta`$(comma) but only displays 'install' and 'remove' actions.\
$(lf)Other actions are represented in terms of those two.)
	$(call pkg_pretty_print_delta,$(pkg_compute_delta))
	@true

# Applies the current delta.
$(call act, apply-delta \
,,Installs all requested packages and their dependencies$(comma)\
$(lf)or updates them to latest known versions.\
$(lf)Use `$(self) delta` to preview the changes before applying them.)
	$(call pkg_apply_delta,$(pkg_compute_delta))
	@true

# Accepts a list of installed packages, with versions. Returns the list of files that belong to those packages.
$(call act, list-pkg-contents \
,package-versions,List all files owned by the specified installed packages.)
	$(foreach x,$(call index_list_pkg_files,$p),$(info $(subst <, ,$x)))
	@true

# Accepts a list of packages, without versions.
# Installs those packages, without considering their dependencies.
$(call act, unmanaged-install \
,packages,Install specified packages$(comma) without dependencies.\
$(lf)Normally you don't need to use this command$(comma) since its effects\
$(lf)will be undone automatically next time you invoke a non-unmanaged command.)
	$(call index_force_install,$(call database_query_full_name,$p))
	@true

# Same as `unmanaged-install`, but you need to specify package versions manually.
$(call act, unmanaged-install-ver \
,package-versions,Install specified package versions$(comma) without dependencies.\
$(lf)Normally you don't need to use this command$(comma) since its effects\
$(lf)will be undone automatically next time you invoke a non-unmanaged command.)
	$(call index_force_install,$p)
	@true

# Accepts a list of packages, with versions.
# Removes those packages, without considering their dependencies.
$(call act, unmanaged-remove-ver \
,package-versions,Remove specified packages$(comma) without dependencies.\
$(lf)Normally you don't need to use this command$(comma) since its effects\
$(lf)will be undone automatically next time you invoke a non-unmanaged command.)
	$(call index_force_uninstall,$p)
	@true


# BROKEN PACKAGES
$(call act_section, BROKEN PACKAGES )

# Lists broken packages.
# A package can become broken if you interrupt its installation or removal.
# Run `purge-broken` to destroy those.
$(call act, list-broken \
,,List all broken packages.$(lf)A package might become broken if you interrupt its installation or removal.)
	$(foreach x,$(patsubst $(index_dir)/$(index_broken_prefix)%,%,$(call safe_wildcard,$(index_dir)/$(index_broken_prefix)*)),$(info $x))
	@true

# Destroys broken packages.
$(call act, purge-broken \
,,Destroys all broken packages.\
$(lf)Normally you don't need to call this manually$(comma) as broken packages are\
$(lf)reinstalled automatically by `$(self) apply-delta` and other commands.)
	$(index_purge_broken)
	@true


# PACKAGE CACHE
$(call act_section, PACKAGE CACHE )

# Returns a list of cached packages, with versions.
$(call act, list-cached \
,,List all cached packages archives.$(lf)Incomplete archives will be prefixed with `$(cache_unfinished_prefix)`.)
	$(foreach x,$(cache_list_current),$(info $x))
	@true

# Removes incomplete downloads from the cache.
$(call act, cache-purge-unfinished \
,,Delete incomplete cached archives$(comma) which might appear if you interrupt a download.)
	$(cache_purge_unfinished)
	@true

# Cleans the cache.
$(call act, clean-cache \
,,Clean the entire archive cache.)
	$(foreach x,$(call safe_wildcard,$(CACHE_DIR)/*),$(call safe_shell_exec,rm -f $(call quote,$x)))
	@true

# Accepts a list of packages, without versions. Downloads them, if they are not already in the cache.
$(call act, cache-download \
,packages,Download specified packages to the cache.)
	$(call cache_want_packages,$(call database_query_full_name,$p))
	@true

# Same as `cache-download`, but you need to specify package versions manually.
$(call act, cache-download-ver \
,package-versions,Download specified package versions to the cache.)
	$(call cache_want_packages,$p)
	@true

# Lists packages that are installed but not cached.
$(call act, cache-list-missing \
,,Lists all installed packages that are not cached.)
	$(foreach x,$(cache_list_missing),$(info $x))
	@true

# Lists packages that are cached but not installed.
$(call act, cache-list-unused \
,,Lists all cached packages that are not installed.)
	$(foreach x,$(cache_list_unused),$(info $x))
	@true

# Caches packages that are currently installed.
$(call act, cache-add-missing \
,,Caches all installed packages.)
	$(call cache_want_packages,$(cache_list_missing))
	@true

# Removes packages that are not currently installed from cache.
$(call act, cache-remove-unused \
,,Removes packages that are not currently installed from the cache.)
	$(foreach x,$(cache_list_unused),\
		$(foreach y,$(REPO_PACKAGE_ARCHIVE_SUFFIXES),\
			$(call safe_shell_exec,rm -f '$(CACHE_DIR)/$x$y')\
			$(call safe_shell_exec,rm -f '$(CACHE_DIR)/$x$y.sig')\
		)\
		$(info Removed '$x' from cache)\
	)
	@true

# Updates the database, upgrades packages, and fixes stuff. Doesn't remove old archives from the cache.
$(call act, cache-installed-only \
,,Make sure the cache contains all installed packages, and nothing else.)
	$(call safe_shell_exec, $(MAKE) 1>&2 cache-add-missing)
	$(call safe_shell_exec, $(MAKE) 1>&2 cache-remove-unused)
	@true

# Accepts a list of packages, without versions. Outputs the list of files contained in them.
$(call act, cache-list-pkg-contents \
,package-versions,Output a list of files contained in the specified cached packages.)
	$(foreach x,$(call cache_list_pkg_files,$p),$(info $(subst <, ,$x)))
	@true

# Help section: Package alternatives
$(call act_section, PACKAGE ALTERNATIVES )

$(if $(display_help),\
	$(info Packages have unique names. They also have aliases, which are not unique.)\
	$(info Several packages can have the same alias. The name of a package can serve)\
	$(info as an alias for other packages.)\
	$(info The choice of alternatives happens each time a new package database is downloaded.)\
	$(info During this process, each alias is disambiguated to refer to a single package.)\
	$(info By default, aliases conflicting with other packages' names are discarded,)\
	$(info giving priority to the names. Otherwise, the alias conflicts are resolved)\
	$(info by lexicographically comparing package names, with 'lesser' packages)\
	$(info getting priority.)\
	$(info The default conflict resolution can be overriden by creating a file)\
	$(info named `alternatives.txt` in the installation directory. After any change to)\
	$(info the file, you must run `$(self) reparse-database` to apply the changes.)\
	$(info Add one entry per line, in the following format:)\
	$(info $(space)   target:override)\
	$(info Where `target` is an ambiguous alias (or name), and `override` is one of)\
	$(info the packages having this alias (or name), which the alias should refer to.)\
	$(info The `target` can match the `override`, but since this is the default behavior,)\
	$(info this merely removes the notice about the ambiguity when updating the database.)\
	$(info If the specified settings cause a package to give up its name, it becomes)\
	$(info inaccessible.)\
	$(info )\
	)

# Help section: Package alternatives
$(call act_section, PLATFORMS )

$(if $(display_help),\
	$(info MSYS2 supports several different target platforms, each with its own set of packages.)\
	$(info To select a platform, create a file called `msystem.txt` and put)\
	$(info one of the following into it:)\
	$(info * `UCRT64`  (default))\
	$(info * `MINGW64`, `MINGW32` (uses legacy `msvcrt.dll`))\
	$(info * `CLANG64`, `CLANG32` (clang and libc++ only))\
	$(info For the exact list of platforms, see the beginning of the `Makefile`.)\
	$(info )\
	)

# Help section: Notes
$(call act_section, NOTES )

$(if $(display_help),\
	$(info When passing a package name to a command,)\
	$(info you can use `_` instead of the `$(REPO_PACKAGE_COMMON_PREFIX)` prefix.)\
	$(info )\
	)
