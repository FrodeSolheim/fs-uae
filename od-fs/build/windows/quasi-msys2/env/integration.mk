$(if $(filter-out 0 1,$(words $(MAKECMDGOALS))),$(error More than one action specified.))

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

# Encloses $1 in single quotes, with proper escaping for the shell.
# If you makefile uses single quotes everywhere, a decent way to transition is to manually search and replace `'(\$(?:.|\(.*?\)))'` with `$(call quote,$1)`.
override quote = '$(subst ','"'"',$1)'


override target_file := $(HOME)/.local/share/applications/quasi-msys2.desktop

override installation_root := $(call safe_shell,realpath $(call quote,$(dir $(word 1,$(MAKEFILE_LIST)))..))
$(if $(wildcard $(installation_root)/msys2_pacmake_base_dir),,$(error Looks like this makefile is in a wrong directory. Refuse to continue))

override define file_contents :=
[Desktop Entry]
Version=1.1
Type=Application
Name=Quasi-MSYS2
Comment=Windows cross-compilation environment
Icon=$(installation_root)/env/internal/quasi-msys2-icon.png
Exec=bash $(installation_root)/env/shell.sh
Path=bash $(installation_root)
Terminal=true
Actions=
Categories=Development;Utility;
endef

install:
	$(info Creating file: $(target_file))
	$(call safe_shell_exec,mkdir -p $(call quote,$(dir $(target_file))))
	$(file >$(target_file),$(file_contents))
	$(info Created a menu entry for the Quasi-MSYS2 shell.)
	$(info Run with `uninstall` flag to undo.)
	@true

uninstall:
	$(call safe_shell_exec,rm -f $(call quote,$(target_file)))
	$(info Removed file: $(target_file))
	@true
