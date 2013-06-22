from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui


class Book(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        self.page_titles = []
        self.pages = []
        self.current_page = None

    def add_page(self, function, title=""):
        self.page_titles.append(title)
        self.pages.append(function)

    def set_page(self, page):
        try:
            index = page + 0
        except TypeError:
            for i, p in enumerate(self.pages):
                if page == p:
                    index = i
                    break
            else:
                raise Exception("page not found")
        if self.current_page:
            self.current_page.hide()
            self.layout.remove(self.current_page)
        if callable(self.pages[index]):
            page = self.pages[index](self)
            self.pages[index] = page
        else:
            page = self.pages[index]
        self.layout.add(page, fill=True, expand=True)
        self.current_page = page
        page.show()
        if hasattr(page, "on_show"):
            page.on_show()
        self.layout.update()
