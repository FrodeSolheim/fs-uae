import logging
from collections import Counter
from typing import Self

from fsapp.eventservice import (
    Event,
    EventService,
)

from fsuae.uaeconfig import UAEConfig
from fsuae.uaeconfig2 import UAEConfig2

logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


class Config:
    def __init__(self):
        self.cpu_model = ""
        self.cpu_type = ""
        self.fpu_model = ""
        self.mmu_model = ""

    def keys(self):
        return self.__dict__.keys()


# FIXME: Maybe queue up config changes, wait until next UAE_CONFIG update
# and send changes *after that* - maybe... (maybe not, maybe instead check if
# we're doing an UAE config copy before or after applying options?)


class UAEConfigService:
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        return cls._instance

    @classmethod
    def _set_instance(cls, instance: Self) -> None:
        assert not hasattr(cls, "_instance")
        cls._instance = instance

    def set_instance(self) -> Self:
        self._set_instance(self)
        return self

    def __init__(self, event_service: EventService) -> None:
        self.event_service = event_service
        self.event_service.add_listener("UAE_CONFIG", self.__uae_config)

        self.uae_config = UAEConfig()

        # FIXME: TypedDict?
        # FIXME: Or maybe a dataclass?
        # self._config = {
        #     "cpu_model": "",
        #     "cpu_type": "",  # ignore
        #     "fpu_model": "",
        #     "mmu_model": "",
        # }

        self.config = Config()
        self.config2 = UAEConfig2()

    def warning(self, message) -> None:
        logger.warning("%s", message)

    def __uae_config(self, event: Event) -> None:
        changed_keys: set[str] = set()
        # removed_keys: set[str] = set(self.config.keys())
        # Keys starting with x_ are derived by the UAEConfig2 hooks (such as x_mmu_type) and are
        # never in the snapshot, so they must not be treated as removed.
        removed_keys: set[str] = {k for k in self.config2._config if not k.startswith("x_")}

        items: list[tuple[str, str]] = []
        for line in event["strdata"].split("\n"):
            if line == "" or line.startswith(";"):
                continue
            try:
                key, value = line.split("=", 1)
            except ValueError as e:
                logger.warning("%s %r", line, e)
                continue
            items.append((key, value))

        # Some keys (such as filesystem2) can occur more than once. They are stored as one key here,
        # so each occurrence overwrites the previous one and is seen as a change on every snapshot.
        # Don't log those, to avoid flooding the log.
        key_counts = Counter(key for key, _ in items)

        for key, value in items:
            log_key = key_counts[key] == 1
            if self.config2._config.get(key) != value:
                # if getattr(self.config, key, None) != value:
                # self._config[key] = value

                setattr(self.config, key, value)
                self.config2._config[key] = value

                if log_key:
                    logger.debug("-----> %r <- %r", key, value)
                changed_keys.add(key)
            try:
                removed_keys.remove(key)
            except KeyError:
                if log_key:
                    logger.debug("  (key %r was not in config key set)", key)

        # Also look at config keys that was NOT included?

        for key in removed_keys:
            if self.config2._config.get(key) != "":
                setattr(self.config, key, "")
                self.config2._config[key] = ""

                logger.debug('-rem-> %r <- ""', key)
                changed_keys.add(key)

        # We do this after all config changes have been registered
        for key in sorted(changed_keys):
            # function = getattr(self, f"_{key}_changed", None)
            # if function is not None:
            #     function()

            function = getattr(self.config2, f"_{key}_changed", None)
            if function is not None:
                function()

        self.config2._update()

    # The following functions are called whenever the corresponding config key
    # has a value change. The functions are called in asciibetical order after
    # *all* config key/values have been changed

    # def _fpu_model_changed(self) -> None:
    #     # self._config["fpu_model"]
    #     pass

    # def _mmu_model_changed(self) -> None:
    #     mmu_model = self.config.mmu_model
    #     cpu_model = self.config.cpu_model

    #     if "ec" in mmu_model:
    #         mmu_model = mmu_model.replace("ec", "")
    #         ec = True
    #     else:
    #         ec = False

    #     MMU_NONE = 0
    #     MMU_FULL = 1
    #     MMU_EC = 2

    #     mmu_type = MMU_NONE

    #     if mmu_model == "":
    #         mmu_type = MMU_NONE
    #     elif mmu_model != cpu_model:
    #         self.warning(f"CPU ({cpu_model}) and MMU ({mmu_model})  mismatch")
    #         mmu_type = MMU_NONE  # Or maybe True. Invalid anyway.
    #     else:
    #         if ec:
    #             mmu_type = MMU_EC
    #         else:
    #             mmu_type = MMU_FULL

    #     # FIXME: Update GUI state
