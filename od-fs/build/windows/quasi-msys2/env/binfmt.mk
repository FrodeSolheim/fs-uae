# Configures the Linux kernel to transparently run Windows apps via Wine.
# The effects are undone on a reboot.

# We have to use bash to be able to use the nice version of `read`.
SHELL := bash

$(if $(filter-out 0 1,$(words $(MAKECMDGOALS))),$(error More than one action specified.))

# Some constants
override comma := ,
override define lf :=
$(strip)
$(strip)
endef

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

# If set, don't print any extra info.
QUIET :=

ifeq ($(QUIET),)
# Prints $1 to the terminal and asks either for confirmation (Enter) or abort (Ctrl+C)
override confirm = $(call safe_shell_exec,read -s -p '$(subst ','"'"',$1)')$(info )
# Prints $1 to the terminal
override explain = $(info $1)
else
override confirm =
override explain =
endif

# Only matters if `QUIET=1`. Set to non-empty after at least one explanation of what we're doing was given.
override quiet_explained_once :=

# Same as `safe_shell_exec`, but automatically appends `sudo` to the command, and asks for confirmation before running it.
# Also redirects `stdout` to `stderr` so you can see its output.
override sudo_exec = \
$(call explain,Will run following command$(comma) press Enter to confirm or Ctrl+C to abort.)\
$(if $(QUIET),$(if $(quiet_explained_once),,$(eval override quiet_explained_once := 1)\
$(info Trying to configure your kernel to transparently run Windows executables using Wine.\
$(lf)All changes will be undone on a reboot. Ctrl+C to skip.))\
$(info Running `sudo $1`.),$(call confirm,$$ sudo $1))\
$(call safe_shell_exec,sudo $1 1>&2)\
$(info Success.)\
$(eval override at_least_one_sudo_run := y)
# If `sudo_exec` was run at least one, this function makes sudo forget the password.
override forget_sudo_passowrd = $(if $(at_least_one_sudo_run),$(info Running `sudo -k` to forget the sudo password.)$(call safe_shell_exec,sudo -k))
# This variable is not null if we used `sudo_exec` at least once.
override at_least_one_sudo_run :=

# Calling this will make sudo ask you for the password next time you use it.
override reset_sudo = $(call safe_shell_exec,sudo -k)


# This enables the whole `binfmt_misc` and registers the right executable format.
.PHONY: enable
enable:
	$(call explain,This makefile will configure your system to transparently run `.exe` files using wine.)
	$(call explain,See link for more details: https://www.kernel.org/doc/Documentation/admin-guide/binfmt-misc.rst)
	$(call explain,)
	$(call explain,Running most of the commands will require root permissions$(comma) you'll be asked for a `sudo` password.)
	$(call explain,You'll be asked for confirmation before running each such command$(comma) restart with `QUIET=1` to disable confirmations.)
	$(call explain,In the quiet mode you also won't be asked for a root password if you recently ran `sudo`.)
	$(call explain,)
	$(call explain,All effects will be undone on a reboot.)
	$(call explain,To undo them manually$(comma) run this makefile with `unregister-format` to unregister our custom executable format.)
	$(call explain,You can also use `disable-binfmt_misc` to disable the whole system of custom executable formats.)
	$(call explain,Those actions too will be completely undone on a reboot.)
	$(call explain,)
	$(call explain,Press Enter to continue or Ctrl+C to abort.)
	$(call confirm)
	$(if $(filter-out 0,$(call shell_status,which wine)),\
		$(info Wine is not installed!)\
		$(info A native Clang can cross-compile without it, but nothing else will function.)\
		$(info If that's ok, skip this configuration script and run the remaining ones.)\
		$(error Aborted)\
	)
	$(call reset_sudo)
	$(if $(wildcard /proc/sys/fs/binfmt_misc),\
		$(info `binfmt_misc` mounted? YES)\
	,\
		$(info `binfmt_misc` mounted? NO)\
		$(call sudo_exec,mount binfmt_misc -t binfmt_misc /proc/sys/fs/binfmt_misc)\
	)
	$(if $(filter enabled,$(call safe_shell,cat /proc/sys/fs/binfmt_misc/status)),\
		$(info `binfmt_misc` enabled? YES)\
	,\
		$(info `binfmt_misc` enabled? NO)\
		$(call sudo_exec,bash -c 'echo 1 >/proc/sys/fs/binfmt_misc/status')\
	)
	$(if $(wildcard /proc/sys/fs/binfmt_misc/DOSWin),\
		$(info Executable format registered? YES)\
	,\
		$(info Executable format registered? NO)\
		$(call sudo_exec,bash -c 'echo ":DOSWin:M::MZ::$(call safe_shell,which wine):" >/proc/sys/fs/binfmt_misc/register')\
	)
	$(if $(filter enabled,$(call safe_shell,cat /proc/sys/fs/binfmt_misc/DOSWin)),\
		$(info Executable format enabled? YES)\
	,\
		$(info Executable format enabled? NO)\
		$(call sudo_exec,bash -c 'echo 1 >/proc/sys/fs/binfmt_misc/DOSWin')\
	)
	$(call forget_sudo_passowrd)
	@true

# Unregister the single executable format.
.PHONY: unregister-format
unregister-format:
	$(if $(wildcard /proc/sys/fs/binfmt_misc),\
		$(if $(wildcard /proc/sys/fs/binfmt_misc/DOSWin),\
			$(call reset_sudo)\
			$(call sudo_exec,bash -c 'echo -1 >/proc/sys/fs/binfmt_misc/DOSWin')\
		,\
			$(info The executable format is not registered, nothing to do.)\
		)\
	,\
		$(info `binfmt_misc` is not mounted, nothing to do.)\
	)
	$(call forget_sudo_passowrd)
	@true

# Disable all the formats.
.PHONY: disable-binfmt_misc
disable-binfmt_misc:
	$(if $(wildcard /proc/sys/fs/binfmt_misc),\
		$(if $(filter enabled,$(call safe_shell,cat /proc/sys/fs/binfmt_misc/status)),\
			$(call reset_sudo)\
			$(call sudo_exec,bash -c 'echo 0 >/proc/sys/fs/binfmt_misc/status')\
		,\
			$(info `binfmt_misc` is disabled, nothing to do.)\
		)\
	,\
		$(info `binfmt_misc` is not mounted, nothing to do.)\
	)
	$(call forget_sudo_passowrd)
	@true
