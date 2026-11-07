import logging
import time
from typing import Self

from fsgui.borderlesswindow import BorderlessWindow
from fsgui.imagelabel import ImageLabel
from fsgui.label import Label
from fsgui.layout import HorizontalLayout, VerticalLayout
from fsgui.widget import Widget
from fsgui.window import Window
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
        self, id: str, text: str, description: str, icon: SvgIcon, *, duration: int = 5
    ) -> "Notification":
        notification = Notification(id, text, description, icon, duration=duration)
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
        #     window._hide_time = time.time() + notification.duration

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
        self, id: str, text: str, description: str, icon: SvgIcon, duration: int = 5
    ) -> None:
        self.id = id

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
        super().__init__(size=(320, 80))

        self.notification = notification
        self._hide_time = None

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
                    # Load icon with white tint for dark notification background
                    icon_image = notification.icon.get_image(
                        40, tint_color=(255, 255, 255, 255)
                    )  # White tinted 40x40 icon
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
                    Label(
                        notification.description,
                        font=self._get_description_font(),
                        text_color=(0xCC, 0xCC, 0xCC, 0xFF),
                    )

        self.resize_to_fit_content(320)

        # Set window layer to be on top
        self.set_layer(Window.LAYER_HIGHEST)

    def _position_window(self) -> None:
        """Position the notification window in the top-right corner."""
        try:
            from fsgui.mainwindow import MainWindow

            main_window = MainWindow.get()
            if main_window:
                main_size = main_window.get_window_size()

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
        """Schedule the notification to auto-hide after duration seconds."""
        current_time = time.time()
        self._hide_time = current_time + duration
        print("hide time:", self._hide_time)

    def update(self) -> None:
        """Update window and check for auto-hide."""
        super().update()

        # Check if we should auto-hide (only once)
        if self._hide_time and time.time() >= self._hide_time:
            print(f"[DEBUG] Auto-hiding notification at time={time.time():.2f}")
            self._auto_hide()
            self._hide_time = None  # Prevent repeated calls

    def _auto_hide(self) -> None:
        """Auto-hide this notification."""
        service = NotificationService.instance()
        service.hide_notification(self.notification.id)

    def hide(self) -> None:
        """Hide the notification window."""
        self.set_visible(False)

    def destroy(self) -> None:
        """Destroy the notification window."""
        # Clean up any resources if needed
        super().destroy()
        pass


class PlaceholderIcon(Widget):
    """A placeholder widget that displays a colored rectangle for testing layout."""

    def __init__(self):
        super().__init__()

    def calculate_min_height(self, width: int) -> int:
        return 40  # 40px height

    def calculate_min_width(self) -> int:
        return 40  # 40px width

    def on_paint(self):
        dc = self.create_dc()
        # Draw a bright colored rectangle as placeholder
        dc.set_fill_colour((0xFF, 0x66, 0x00, 0xFF))  # Orange color
        dc.draw_filled_rectangle((0, 0), (40, 40))
