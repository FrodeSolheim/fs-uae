from fsapp.eventservice import Event, EventService
from fsuae.gui.notificationservice import NotificationService
from fsuae.gui.notificationtype import NotificationType


class InputGrabNotification:
    """Shows the input grab notification when the input grab is turned on, and hides it when it is
    turned off, on the app event that fsapp pushes when the mouse grab changes
    (FSAPP_MOUSE_GRAB, with intdata 1 when grabbed and 0 when released)."""

    def __init__(
        self, notification_service: NotificationService, event_service: EventService
    ) -> None:
        self._notification_service = notification_service
        self._input_grab = False
        event_service.event("FSAPP_MOUSE_GRAB").connect(self._on_mouse_grab)

    def _on_mouse_grab(self, event: Event) -> None:
        input_grab = bool(event.get("intdata", 0))
        if input_grab == self._input_grab:
            return
        if input_grab:
            self._notification_service.show_notification(NotificationType.MOUSE_GRABBED)
        else:
            # Hides the notification if it is showing.
            self._notification_service.hide_notification(NotificationType.MOUSE_GRABBED)
        self._input_grab = input_grab
