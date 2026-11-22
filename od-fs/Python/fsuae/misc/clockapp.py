"""
Clock widget app - AI generated as an experiment.
"""

import logging
import math
import time
from typing import ClassVar, Self

import _fsapp
from fsapp.surfaceapi.v1 import Keycode
from fsapp.timerservice import Timer, TimerService
from fsgui.drawingcontext import Colour
from fsgui.layout import VerticalLayout
from fsgui.reactive import Var
from fsgui.widget import Widget
from fsgui.window import Window

from fsuae.servicecontainer import ServiceContainer

logger = logging.getLogger(__name__)


class AnalogClockWidget(Widget):
    def __init__(self, *, parent: Widget | None = None):
        super().__init__(parent=parent)

        # Current time
        self.hours = Var(0)
        self.minutes = Var(0)
        self.seconds = Var(0)

        # Listen to time changes and refresh display
        self.on(self.hours, lambda _: self.refresh())
        self.on(self.minutes, lambda _: self.refresh())
        self.on(self.seconds, lambda _: self.refresh())

    def calculate_min_height(self, width: float) -> float:
        return width  # Make it square

    def calculate_min_width(self) -> float:
        return 100  # Minimum reasonable size

    def on_paint(self) -> None:
        dc = self.create_dc()

        # Clock dimensions - use available space
        center_x = self.width // 2
        center_y = self.height // 2
        radius = min(center_x, center_y) - 5  # Less margin for better filling

        # center_x += 100
        # center_y -= 100

        # Draw white background circle
        dc.set_fill_colour(Colour.WHITE)
        dc.draw_filled_circle((center_x, center_y), radius)

        # Draw clock face outline (black circle)
        dc.set_line_colour(Colour.BLACK)
        dc.set_line_width(2)
        dc.draw_circle((center_x, center_y), radius)

        # Draw hour marks
        dc.set_line_width(3)
        for hour in range(12):
            angle = (hour * 30 - 90) * math.pi / 180  # -90 to start at 12 o'clock
            outer_x = center_x + (radius - 5) * math.cos(angle)
            outer_y = center_y + (radius - 5) * math.sin(angle)
            inner_x = center_x + (radius - 15) * math.cos(angle)
            inner_y = center_y + (radius - 15) * math.sin(angle)

            dc.draw_line((inner_x, inner_y), (outer_x, outer_y))

        # Draw minute marks
        dc.set_line_width(1)
        for minute in range(60):
            if minute % 5 != 0:  # Skip hour marks
                angle = (minute * 6 - 90) * math.pi / 180
                outer_x = center_x + (radius - 5) * math.cos(angle)
                outer_y = center_y + (radius - 5) * math.sin(angle)
                inner_x = center_x + (radius - 10) * math.cos(angle)
                inner_y = center_y + (radius - 10) * math.sin(angle)

                dc.draw_line((inner_x, inner_y), (outer_x, outer_y))

        # Get current time values
        hours = self.hours.value
        minutes = self.minutes.value
        seconds = self.seconds.value

        # Draw hour hand
        hour_angle = ((hours % 12) * 30 + minutes * 0.5 - 90) * math.pi / 180
        hour_length = radius * 0.5
        hour_x = center_x + hour_length * math.cos(hour_angle)
        hour_y = center_y + hour_length * math.sin(hour_angle)

        dc.set_line_width(4)
        dc.set_line_colour(Colour.BLACK)
        dc.draw_line((center_x, center_y), (hour_x, hour_y))

        # Draw minute hand
        minute_angle = (minutes * 6 - 90) * math.pi / 180
        minute_length = radius * 0.7
        minute_x = center_x + minute_length * math.cos(minute_angle)
        minute_y = center_y + minute_length * math.sin(minute_angle)

        dc.set_line_width(3)
        dc.draw_line((center_x, center_y), (minute_x, minute_y))

        # Draw second hand
        second_angle = (seconds * 6 - 90) * math.pi / 180
        second_length = radius * 0.8
        second_x = center_x + second_length * math.cos(second_angle)
        second_y = center_y + second_length * math.sin(second_angle)

        dc.set_line_width(1)
        dc.set_line_colour(Colour.RED)
        dc.draw_line((center_x, center_y), (second_x, second_y))

        # Draw center dot
        dc.set_fill_colour(Colour.BLACK)
        dc.draw_filled_circle((center_x, center_y), 4)

    def set_time(self, hours: int, minutes: int, seconds: int) -> None:
        """Update the clock time"""
        self.hours.set(hours)
        self.minutes.set(minutes)
        self.seconds.set(seconds)


class ClockWindow(Window):
    _instance: ClassVar[Self | None] = None

    @classmethod
    def has_instance(cls) -> bool:
        return cls._instance is not None

    @classmethod
    def instance(cls) -> Self:
        def on_destroy():
            cls._instance = None

        if cls._instance is None:
            cls._instance = cls().on_destroy(on_destroy)
        return cls._instance

    @classmethod
    def toggle(cls) -> None:
        """Open the clock window, or close it if it is open."""
        if cls._instance is not None:
            cls._instance.close()
        else:
            cls.instance().center_window().show()

    @classmethod
    def setup(cls, services: ServiceContainer):
        _fsapp.register_shortcut(Keycode.T, _fsapp.SHORTCUT_MOD, cls.toggle)

    def __init__(self) -> None:
        super().__init__("Clock", size=(200, 200), padding=16)

        # Set up layout
        VerticalLayout()

        # Create the analog clock widget and make it fill and expand
        self.clock_widget = AnalogClockWidget()
        self.clock_widget.fill().expand()  # Make the widget fill and expand to use available space

        # The timer that updates the time at the start of the next second.
        self._timer: Timer | None = None

        # Initial update, which sets the timer
        self._update_time()

    def _update_time(self) -> None:
        """Update the current time, and set the timer for the start of the next second."""
        now = time.time()
        local = time.localtime(now)
        self.clock_widget.set_time(local.tm_hour, local.tm_min, local.tm_sec)
        # A little after the second starts, so that the wall clock has passed it when the timer
        # runs (it runs on fsapp's monotonic clock).
        delay = 1.0 - now % 1.0 + 0.002
        self._timer = TimerService.get().call_after(delay, self._update_time)

    def close(self) -> None:
        """Close the clock window"""
        if self._timer is not None:
            self._timer.cancel()
            self._timer = None
        super().close()

    def center_window(self) -> Self:
        """Center the window on screen"""
        # Position it at a reasonable location
        self.move((300, 200))
        return self


# Convenience function to set up the clock app
def setup_clock_app(services: ServiceContainer) -> None:
    """Set up the analog clock app with its shortcut, Mod+T"""
    ClockWindow.setup(services)
