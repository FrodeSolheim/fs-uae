# from fswidgets3.widget import Widget3
# Widget3 = "Widget3"  # FIXME
from typing import Any, List, Optional

# if TYPE_CHECKING:
#     from fswidgets3.container import Container3

#     # from fswidgets3.widget import Widget3
#     from fswidgets3.widgetimpl import WidgetImpl3


class ParentStack:
    stack: List[Any] = []

    @classmethod
    def parent(cls) -> Any:
        # Because init_subclass wraps the init functions in the class hierarchy
        # for widgets, all the init functions will push/pop and we will get
        # multiple of the same instance on the stack.

        top = cls.stack[-1]
        for widget in reversed(cls.stack):
            if widget != top:
                # from fswidgets3.container import Container3

                # assert isinstance(widget, Container3)
                return widget
        raise Exception("Could not find parent")

        # if len(cls.stack) < 2:
        #     return None
        # parent = cast("Container3", cls.stack[-2])
        # from fswidgets3.container import Container3
        # print(cls.stack)
        # print(parent)
        # assert isinstance(parent, Container3)
        # return parent

    @classmethod
    def parent_widget(cls) -> Any:
        from fsgui.widget import Widget

        # Because init_subclass wraps the init functions in the class hierarchy
        # for widgets, all the init functions will push/pop and we will get
        # multiple of the same instance on the stack.

        top = cls.stack[-1]
        for widget in reversed(cls.stack):
            if not isinstance(widget, Widget):
                continue
            if widget != top:
                # from fswidgets3.container import Container3

                # assert isinstance(widget, Container3)
                return widget
        raise Exception("Could not find parent")

        # if len(cls.stack) < 2:
        #     return None
        # parent = cast("Container3", cls.stack[-2])
        # from fswidgets3.container import Container3
        # print(cls.stack)
        # print(parent)
        # assert isinstance(parent, Container3)
        # return parent

    @classmethod
    def top(cls) -> Any:
        return cls.stack[-1]

    @classmethod
    def push(cls, widget: Any) -> None:
        # if len(cls.stack) > 0 and cls.stack[-1] == widget:
        #     # Already on top. This can happen because init_subclass wraps the
        #     # init functions in the class hierarchy for widgets, and so all
        #     # the init functions will push/pop.
        #     return
        cls.stack.append(widget)

    @classmethod
    def pop(cls, widget: Optional[Any] = None) -> Any:
        """Pops the topmost widget off the stack.

        If the widget parameter is specified, the function checks that the
        popped widget is identical to the parameter."""
        if widget is not None:
            assert cls.top() == widget
        return cls.stack.pop()


# class Parent:
#     def __init__(self, parent: "WidgetImpl3"):
#         self.parent = parent

#     def __enter__(self) -> None:
#         ParentStack3.push(self.parent)

#     def __exit__(
#         self,
#         exc_type: Optional[Type[BaseException]],
#         exc_val: Optional[BaseException],
#         exc_tb: Optional[TracebackType],
#     ) -> None:
#         assert ParentStack3.stack.pop() is self.parent


# class AsParent:
#     def __init__(self, parent: "WidgetImpl3"):
#         self.parent = parent

#     def __enter__(self) -> None:
#         ParentStack3.push(self.parent)

#     def __exit__(
#         self,
#         exc_type: Optional[Type[BaseException]],
#         exc_val: Optional[BaseException],
#         exc_tb: Optional[TracebackType],
#     ) -> None:
#         assert ParentStack3.stack.pop() is self.parent
