import logging
from typing import Self

from fsapp.surfaceapi.v1 import Role
from fsapp.timerservice import Timer, TimerService
from fsgui.borderlesswindow import BorderlessWindow
from fsgui.imagelabel import ImageLabel
from fsgui.label import Label
from fsgui.layout import HorizontalLayout, VerticalLayout
from fsgui.scale import scaled
from fsgui.widget import Widget
from fsuae.svgicon import SvgIcon

logger = logging.getLogger()


class NotificationService:
    # ----------------------------------------------------------------------------------------------
    _instance: "NotificationService"

    @classmethod
    def instance(cls) -> "NotificationService":
        try:
            return cls._instance
        except AttributeError:
            cls._instance = NotificationService()
            return cls._instance

    @classmethod
    def _set_instance(cls, instance: Self) -> None:
        assert not hasattr(cls, "_instance")
        cls._instance = instance

    def set_instance(self) -> Self:
        self._set_instance(self)
        return self

    # ----------------------------------------------------------------------------------------------

    def __init__(self) -> None:
        self._notifications: list[Notification] = []
        self._active_windows: dict[str, NotificationWindow] = {}

    def create_notification(
        self,
        id: str,
        text: str,
        description: str,
        icon: SvgIcon,
        *,
        duration: int = 5,
        width: int | None = None,
    ) -> "Notification":
        """Create a notification (shown with `show_notification`).

        A `duration` of 0 means the notification stays until it is hidden. With `width`, the
        notification has that width and the description is wrapped to fit.
        """
        notification = Notification(id, text, description, icon, duration=duration, width=width)
        self._notifications.append(notification)
        return notification

    def show_notification(self, id: str) -> None:
        """Show a notification by ID."""
        notification = self._find_notification(id)
        if notification is None:
            logger.warning(f"Notification with id '{id}' not found")
            return

        try:
            window = self._active_windows[id]
        except KeyError:
            window = NotificationWindow(notification)
            self._active_windows[id] = window
            window.show()
        # else:
        #     window.schedule_auto_hide(notification.duration)

        # Create and show notification window

        # Schedule auto-hide if duration is set
        if notification.duration > 0:
            window.schedule_auto_hide(notification.duration)

    def hide_notification(self, id: str) -> None:
        """Hide a notification by ID."""
        if id in self._active_windows:
            window = self._active_windows[id]
            window.hide()
            window.destroy()
            del self._active_windows[id]

    def _find_notification(self, id: str) -> "Notification | None":
        """Find notification by ID."""
        for notification in self._notifications:
            if notification.id == id:
                return notification
        return None


class Notification:
    def __init__(
        self,
        id: str,
        text: str,
        description: str,
        icon: SvgIcon,
        duration: int = 5,
        width: int | None = None,
    ) -> None:
        self.id = id
        # In design units.
        self.width = width

        self.default_text = text
        self.default_description = description
        self.default_icon = icon
        self.default_duration = duration

        self.text = text
        self.description = description
        self.icon = icon
        self.duration = duration


class NotificationWindow(BorderlessWindow):
    def __init__(self, notification: "Notification") -> None:
        # Above every band, taking no pointer input: a click on it reaches what is below.
        super().__init__(size=(320, 80), role=Role.OVERLAY)
        # A notification never takes the keys.
        self.set_focusable(False)

        self.notification = notification
        # Hides the notification when it is due (schedule_auto_hide).
        self._hide_timer: Timer | None = None

        # Style the window
        # self.style.background_color = (0x20, 0x20, 0x20, 0xE0)  # Semi-transparent dark
        self.style.background_color = (0x50, 0x50, 0x50, 0xFF)
        self.padding = (24, 24, 24, 24)

        # Position window in top-right corner
        self._position_window()

        # Create layout
        with HorizontalLayout(gap=12):
            # Icon
            if notification.icon:
                try:
                    print(f"Loading notification icon: {notification.icon.name}")
                    # Load icon with white tint for dark notification background, 40x40 in
                    # design units, rasterised in pixels.
                    icon_image = notification.icon.get_image(
                        scaled(40), tint_color=(255, 255, 255, 255)
                    )
                    if icon_image:
                        print(f"Icon loaded successfully: {icon_image.width}x{icon_image.height}")
                        ImageLabel(icon_image)  # Create directly in layout context
                        print("ImageLabel created and added to layout")
                    else:
                        print("Icon loading returned None - creating placeholder")
                        # Create a placeholder widget to test layout
                        PlaceholderIcon()
                except (FileNotFoundError, Exception) as e:
                    # If icon can't be loaded, create a placeholder
                    print(f"Failed to load notification icon: {e}")
                    import traceback

                    traceback.print_exc()
                    PlaceholderIcon()

            # Text content
            with VerticalLayout(gap=2).fill():
                Label(
                    notification.text,
                    font=self._get_title_font(),
                    text_color=(0xFF, 0xFF, 0xFF, 0xFF),
                )
                if notification.description:
                    description_font = self._get_description_font()
                    if notification.width is None:
                        lines = [notification.description]
                    else:
                        text_width = (
                            notification.width - self.padding[1] - self.padding[3] - (40 + 12)
                        )
                        lines = wrap_text(notification.description, description_font, text_width)
                    # One label per line (wrapped labels do not work in horizontal layouts yet).
                    for line in lines:
                        Label(
                            line,
                            font=description_font,
                            text_color=(0xCC, 0xCC, 0xCC, 0xFF),
                        )

        self.resize_to_fit_content(notification.width or 320)

    def _position_window(self) -> None:
        """Position the notification window in the top-right corner."""
        try:
            from app.mainwindow import MainWindow

            main_window = MainWindow.get()
            if main_window:
                # Position 24px from left edge and top (upper left corner)
                x = 24
                y = 24

                self.move((x, y))
            else:
                # Fallback positioning
                self.move((100, 100))
        except Exception:
            # Fallback positioning if MainWindow is not available
            self.move((100, 100))

    def _get_title_font(self):
        """Get font for notification title."""
        from fsgui.font import Font

        return Font("UI", 16, Font.BOLD)

    def _get_description_font(self):
        """Get font for notification description."""
        from fsgui.font import Font

        return Font("UI", 14)

    def schedule_auto_hide(self, duration: int) -> None:
        """Schedule the notification to auto-hide after duration seconds (a timer, which wakes the
        UI thread then, also when nothing else happens). Replaces an earlier schedule."""
        self._cancel_auto_hide()
        self._hide_timer = TimerService.get().call_after(duration, self._auto_hide)

    def _cancel_auto_hide(self) -> None:
        if self._hide_timer is not None:
            self._hide_timer.cancel()
            self._hide_timer = None

    def _auto_hide(self) -> None:
        """Auto-hide this notification."""
        self._hide_timer = None
        logger.debug("Auto-hiding notification %s", self.notification.id)
        service = NotificationService.instance()
        service.hide_notification(self.notification.id)

    def hide(self) -> None:
        """Hide the notification window."""
        self.set_visible(False)

    def destroy(self) -> None:
        """Destroy the notification window."""
        self._cancel_auto_hide()
        # Clean up any resources if needed
        super().destroy()
        pass


def wrap_text(text: str, font, width: float) -> list[str]:
    """Split text into lines no wider than width when drawn with font.

    Breaks at spaces; a word that is too wide on its own (such as a long path) is broken after
    a "/" or "\\" if possible, otherwise anywhere.
    """

    def fits(s: str) -> bool:
        return font.measure_text(s)[0] <= width

    lines: list[str] = []
    line = ""
    for word in text.split():
        candidate = f"{line} {word}" if line else word
        if fits(candidate):
            line = candidate
            continue
        if line:
            lines.append(line)
        line = word
        while not fits(line):
            # Break the word at the last fitting separator, or else at the last fitting character.
            end = len(line) - 1
            while end > 1 and not fits(line[:end]):
                end -= 1
            separator = max(line.rfind("/", 0, end), line.rfind("\\", 0, end))
            if separator > 0:
                end = separator + 1
            lines.append(line[:end])
            line = line[end:]
    if line:
        lines.append(line)
    return lines


class PlaceholderIcon(Widget):
    """A placeholder widget that displays a colored rectangle for testing layout."""

    def __init__(self):
        super().__init__()

    def calculate_min_height(self, width: float) -> float:
        return 40

    def calculate_min_width(self) -> float:
        return 40

    def on_paint(self):
        dc = self.create_dc()
        # Draw a bright colored rectangle as placeholder
        dc.set_fill_colour((0xFF, 0x66, 0x00, 0xFF))  # Orange color
        dc.draw_filled_rectangle((0, 0), (40, 40))
