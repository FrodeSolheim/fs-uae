import logging
from typing import Callable, ClassVar, Self

logger = logging.getLogger(__name__)

class TickService:
    _instance: ClassVar[Self | None] = None

    @classmethod
    def get(cls) -> Self:
        if cls._instance is None:
            cls._instance = cls()
        # if not hasattr(cls, "_instance"):
        #     cls._instance = cls()
        return cls._instance

    def __init__(self):
        self.tick_handlers: list[Callable[[], None]]  = []
    
    def broadcast(self) -> None:
        for handler in self.tick_handlers:
            try:
                handler()
            except Exception:
                # FIXME: UI logging? An error occurred, check logs?
                logging.exception("Exception in tick handler %s", repr(handler))


    def register(self, handler: Callable[[], None]):
        self.tick_handlers.append(handler)

    def unregister(self, handler: Callable[[], None]):
        self.tick_handlers.remove(handler)

    # @classmethod
    # def _set_instance(cls, instance: Self) -> None:
    #     assert not hasattr(cls, "_instance")
    #     cls._instance = instance

    # def set_instance(self) -> Self:
    #     self._set_instance(self)
    #     return self
