from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
import math
import time
from .Color import Color
from .common import update_class
from .System import System

# FIXME: remove this dummy class
class wxgui:

    @staticmethod
    def mixColors (a, b, alpha):
        return Color.mix_colors(a, b, alpha)

    @staticmethod
    def getHighlightPen():
        return None

    def getHighlightBrush(self):
        return None

class RenderData:
    pass

class BaseItemView(wx.ScrolledWindow):
    """Acts as super class for several ItemView classes."""

    def __init__(self, parent, id=wx.ID_ANY, pos=wx.DefaultPosition,
            size=wx.DefaultSize):
        if System.windows:
            style = wx.BORDER_THEME
        else:
            style = wx.BORDER_SUNKEN
        wx.ScrolledWindow.__init__(self, parent.get_container(), id, pos,
                size, style)
        self.SetBackgroundColour(wx.WHITE)
        self.SetBackgroundStyle(wx.BG_STYLE_CUSTOM)
        self.numrows = 0
        self.numcolumns = 1
        self.numitems = 0
        self.xpadding = 0
        self.ypadding = 0
        self.xspacing = 0
        self.yspacing = 0
        self.itemwidth = 100
        self.itemheight = 26
        self.selectmode = 0 # FIXME: not used? always 0?
        self.selected_items = []
        self.canreselect_items = False # FIXME: not used? can select selected.
        self.trydragdrop = False
        self.selectonmouseup = False
        self.typingsearchstr = ""
        self.typingsearchtime = 0
        self.keyendindex = -1
        # HorizontalItemView should set this to True
        self.horizontalscrolling = False # FIXME: not implemented (always)
        self.SetScrollRate(10, 10)
        self.calculateColors()
        self.multi_select = False
        self.mouselastclickonitem = None
        self.Bind(wx.EVT_CHAR, self.onKeyPressEvent)
        self.Bind(wx.EVT_PAINT, self.onPaintEvent)
        self.Bind(wx.EVT_SIZE, self.onResizeEvent)
        self.Bind(wx.EVT_LEFT_DOWN, self.onLeftDownEvent)
        self.Bind(wx.EVT_RIGHT_DOWN, self.onRightDownEvent)
        self.Bind(wx.EVT_LEFT_DCLICK, self.onLeftDoubleClickEvent)
        self.Bind(wx.EVT_MOTION, self.onMouseMoveEvent)
        self.Bind(wx.EVT_SET_FOCUS, self.onSetFocusEvent)
        self.Bind(wx.EVT_KILL_FOCUS, self.onKillFocusEvent)
        self.Bind(wx.EVT_LEFT_UP, self.__on_left_up)

    def search_for_text(self, text):
        text = text.lower()
        start = 0
        stop = len(self.items) -1
        while start <= stop:
            if self.get_item_text(start).lower().startswith(text):
                return start
            mid = (start + stop) // 2
            midval = self.get_item_text(mid).lower()
            #print(start, stop, mid, midval, text)
            if midval > text:
                stop = mid
            elif not midval.startswith(text):
                start = mid + 1
        return None

    def setItemHeight(self, height):
        self.itemheight = height

    def setPadding(self, x, y=None):
        self.xpadding = x
        if y == None:
            self.ypadding = x
        else:
            self.ypadding = y

    def setItemSpacing(self, x, y=None):
        self.xspacing = x
        if y == None:
            self.yspacing = x
        else:
            self.yspacing = y

    def calculateColors(self):
        self.selecteditembackgroundcolor = wx.SystemSettings.GetColour (
                wx.SYS_COLOUR_HIGHLIGHT)
        self.selecteditembordercolor = wxgui.mixColors (
                self.selecteditembackgroundcolor, wx.WHITE, 0.2)
        self.selecteditemtextcolor = wx.SystemSettings.GetColour (
                wx.SYS_COLOUR_HIGHLIGHTTEXT)
        self.itembordercolor = wx.Colour (0xc4, 0xc4, 0xc4)
        self.itembackgroundcolor = wx.Colour (0xf2, 0xf2, 0xf2)
        self.itemtextcolour = wx.Colour (0xff, 0xff, 0xff)

    def on_select_item(self, index):
        """Virtual method"""
        # FIXME: Send event
        pass

    def on_activate_item(self, index):
        """Virtual method"""
        # FIXME: Send event
        pass

    def onRightClickItem(self, index):
        pass

    def get_item_count(self):
        """Must be overriden to return the number of items"""
        return 0

    def reset(self):
        self.selected_items = []
        self.calculateVirtualSize()
        self.Scroll(0, 0)

    def update(self):
        #print("Itemview update")
        self.calculateVirtualSize ();
        self.Refresh ()
        #self.numitems = self.get_item_count ()
        #print("- Items: " + str(self.numitems))
        #self.Refresh ()
        #int itemcount = get_item_count ();
        #if (m_selecteditem >= itemcount) {
        #    removeIndex (m_selecteditem);
        #    m_selecteditem = -1;
        #}
        #
        #CalculateVirtualSize ();
        #Refresh ();

    def doStartDragDrop(self):
        """Override this to implement dragging items"""
        pass

    def scrollToItem(self, index):
        if index < 0:
            return
        if index >= self.get_item_count ():
            return
        startx, starty = self.GetViewStart ()
        sunitx, sunity = self.GetScrollPixelsPerUnit ()
        scrolltox = -1
        scrolltoy = -1
        startx = startx * sunitx
        starty = starty * sunity
        pos = self.getItemCoordinates (index)
        size = self.GetClientSize ()
        if self.horizontalscrolling:
            # FIXME: NOT IMPLEMENTED
            print("Not implemented")
#            if (pos.x - m_padding < startx) {
#                scrolltox = pos.x - m_padding;
#                if (scrolltox < m_padding) scrolltox = 0;
#            }
#            else if (pos.x + m_cellwidth + m_padding > startx + size.x) {
#                scrolltox = (pos.x + m_cellwidth - size.x) + m_padding;
#                if (scrolltox > GetVirtualSize ().x - size.x - m_padding) {
#                    scrolltox = GetVirtualSize ().x;
#                }
#            }
#
#            if (scrolltox > -1) {
#                Scroll (scrolltox // sunitx, -1);
#            }
        else:
            if pos[1] - self.ypadding < starty:
                #scrolltoy = pos[1] - self.ypadding
                #if scrolltoy < self.ypadding: scrolltoy = 0
                scrolltoy = pos[1] - (size.y - self.itemheight) // 2
            elif pos[1] + self.itemheight + self.ypadding > starty + size.y:
                #scrolltoy = (pos[1] + self.itemheight - size.GetHeight ()) + \
                #        sunity + self.ypadding
                #if scrolltoy > self.GetVirtualSize ().y - size.GetHeight () - \
                #        self.ypadding:
                #    scrolltoy = GetVirtualSize ().y - size.GetHeight ()
                scrolltoy = pos[1] - (size.y - self.itemheight) // 2
            else:
                return
            print("scrolltoy", scrolltoy)
            if scrolltoy < 0: scrolltoy = 0
            #if scrolltoy > -1:
            self.Scroll (-1, scrolltoy // sunity)

    def refreshItem(self, index, updatefocusonly=False,
            selected=False, dc=None):
        """Schedules refresh of a particular item."""
        coord = self.getItemCoordinates(index)
        if dc is None:
            x, y = coord
            start_x, start_y = self.GetViewStart()
            scroll_x, scroll_y = self.GetScrollPixelsPerUnit()
            x -= start_x * scroll_x
            y -= start_y * scroll_y
            self.RefreshRect(wx.Rect(x - 5, y - 5,
                    self.itemwidth + 10, self.itemheight + 10), False)
            return
            #dc = wx.ClientDC(self) # Not good on mac.
            #self.PrepareDC(dc)
        #print("Refreshing item " + str (index))
        renderdata = RenderData()
        renderdata.itemrect = wx.Rect(coord[0], coord[1], self.itemwidth,
                self.itemheight)
        renderdata.dc = dc
        #renderdata.wxdc =
        #renderdata.cliprect = cliprect;
        #renderdata.dc = &dc;
        renderdata.dragover = False
        # FIXME m_dragover && m_dragoveritem == index;
        #renderdata.itemfocus = focused && m_selecteditem == index;
        #renderdata.index = index;
        renderdata.widgetfocus = wx.Window.FindFocus() == self
        renderdata.itemfocus = False
        if len(self.selected_items) > 0:
            if index == self.selected_items[0]:
                renderdata.selected = True
                renderdata.itemfocus = renderdata.widgetfocus
            elif index in self.selected_items:
                renderdata.selected = True
            else:
                renderdata.selected = False
        else:
            renderdata.selected = False
        #renderdata.updatefocusonly = updatefocusonly;
        #renderdata.widgetfocus = focused;
        self.render_item(index, renderdata)

    def render_item(self, index, renderdata):
        """Must be overriden to render an item"""
        print("Must override render_item " + str (index))

    #def select_item(self, index):
    #    """Select the item with the specified index"""
    #
    #    pass

    # FIXME: not used?
    def _moveindexUp(self, index):
        #"""Must be extended by descendants"""
        if index <= 0:
            return
        pass

    # FIXME: not used?
    def _moveIndexDown(self, index):
        #"""Must be extented by descendants"""
        if index >= self.get_item_count () - 1:
            return
        pass

    def _insertIndex(self, index):
        pass

    def _removeIndex(self, index):
        pass

    def renderEmptyView(self, dc):
        """Override this method to custom render the empty view"""
        pass

    def clearSelection(self):
        # FIXME: IMPLEMENT!
        pass

        #count = len(self.selected_items)
        #for (int i = count - 1; i >= 0; i--) {
        #    DeselectCell (m_selecteditems.Item (i), false, true, true, i == 0);
        #    //RefreshCell(m_selecteditems.Item(i), true, false);
        #}
        #
        #m_selecteditems.Clear ();
        #/* FIXME: Need not refresh the entire thing.. */
        #//Refresh();
        #//sendSelectionUpdateEvent ();

    def select_item(self, index, addtoselection=False, sendevent=True, d2=0,
            d3=0, forceallowreselect=False):
        if index >= self.get_item_count():
            return False
        if index < 0:
            self.clearSelection()
            return False
        itemcount = len(self.selected_items)
        if not self.canreselect_items and not forceallowreselect:
            # Scroll to item if it is outside the view.
            if self.GetScrollRange(wx.VERTICAL) > 1:
                scrollrate = self.GetScrollPixelsPerUnit()[1]
                virtual_top_y = self.getItemCoordinates(index)[1]
                virtual_bottom_y = virtual_top_y + self.itemheight
                virtual_win_top_y = self.GetViewStart()[1] * scrollrate
                virtual_win_bottom_y = virtual_win_top_y + \
                        self.GetClientSize()[1]
                if virtual_top_y < virtual_win_top_y:
                    new_scroll_pos = virtual_top_y // scrollrate
                    self.Scroll(0, new_scroll_pos - 4)
                elif virtual_bottom_y > virtual_win_bottom_y:
                    new_scroll_pos = math.ceil((virtual_bottom_y -
                            self.GetClientSize()[1]) // scrollrate)
                    self.Scroll(0, new_scroll_pos + 4)
            #for i in range (0, itemcount):
            #    if(self.selected_items.Item (i) == index) return False;
            #}

        if addtoselection:
            #//wxLogDebug("A");
            #if (m_selecteditems.Index (index) == wxNOT_FOUND) {
            #    m_selecteditems.Add (index);
            #    RefreshCell (index, true, true);
            #}
            self.keyendindex = index
            if not index in self.selected_items:
                self.selected_items.append(index)
                self.refreshItem(index, True, True)
        else:
            #m_selecteditem = index;
            self.keyendindex = index
            olditems = self.selected_items
            self.selected_items = [index]
            self.refreshItem(index, True, True)
            for i in range(0, itemcount):
                if olditems[i] != index:
                    self.refreshItem(olditems[i], True, False)
            #wxLogDebug("ADDING TO SELECTED ITEMS..!");
            #m_selecteditems.Add (index);
        if sendevent:
            self.on_select_item(index)
        self.mouselastclickonitem = index
        #if (sendevent) {
        #    SendEvent (EVENT_ITEMVIEW_SELECT, index, -1, -1, batch, last);
        #}
        #
        #/* FIXME: Need not refresh the entire thing.. */
        #
        #
        return False

    def findItemAt(self, x, y):
        if x == -1 or y == -1:
            return -1;
        # FIXME: Optimize, should not need to loop through all elements !
        sx = self.xpadding
        sy = self.ypadding
        insidecell = -1
        for i in range (0, self.numitems):
            if i % self.numcolumns == 0:
                sx = self.xpadding;
                if i > 0:
                    sy = sy + self.itemheight + self.yspacing
            if x >= sx and y >= sy and x <= sx + self.itemwidth \
                     and y <= sy + self.itemheight:
                insidecell = i
                break
            sx = sx + self.itemwidth + self.xspacing
        #if (insidecell > -1) {
        #//      wxLogDebug("Inside cell %d", insidecell);
        #} else {
        #//      wxLogDebug("Not inside anything...");
        #}
        return insidecell

    def getItemCoordinates(self, index):
        if self.numcolumns == 0 or self.numrows == 0:
            return (0, 0)
        row = index // self.numcolumns
        column = index % self.numcolumns
        totalcolspacing = (column) * self.xspacing
        totalrowspacing = (row) * self.yspacing
        if totalcolspacing < 0:
            totalcolspacing = 0
        if totalrowspacing < 0:
            totalrowspacing = 0
        return(self.xpadding + column * self.itemwidth + totalcolspacing,
                self.ypadding + row * self.itemheight + totalrowspacing)

    def calculateRows(self):
        pass

    def calculateColumns(self):
        pass

    def calculateVirtualSize(self):
        raise Exception, "Must override calculateVirtualSize"

    def absoluteToRelative(self, pos):
        return (absoluteToRelativeX (pos[0]), absoluteToRelativeY (pos[1]))

    def relativeToAbsolute(self, pos):
        return (relativeToAbsoluteX (pos[0]), relativeToAbsoluteY (pos[1]))

    def relativeToAbsoluteX(self, coord):
        sunitx, sunity = self.GetScrollPixelsPerUnit ()
        startx, starty = self.GetViewStart ()
        startx = startx * sunitx
        return startx + coord

    relToAbsX = relativeToAbsoluteX

    def relativeToAbsoluteY(self, coord):
        sunitx, sunity = self.GetScrollPixelsPerUnit ()
        startx, starty = self.GetViewStart ()
        starty = starty * sunity
        return starty + coord

    relToAbsY = relativeToAbsoluteY

    def absoluteToRelativeX(self, coord):
        sunitx, sunity = self.GetScrollPixelsPerUnit ()
        startx, starty = self.GetViewStart ()
        startx = startx * sunitx
        return coord - startx

    absToRelX = absoluteToRelativeX

    def absoluteToRelativeY(self, coord):
        sunitx, sunity = self.GetScrollPixelsPerUnit ()
        startx, starty = self.GetViewStart ()
        starty = starty * sunity
        return coord - starty

    absToRelY = absoluteToRelativeX

    def onPaintEvent(self, event):
        #logger.debug('painting')
        dc = wx.AutoBufferedPaintDC(self)
        dc.SetBackground(wx.Brush(self.GetBackgroundColour()))
        dc.Clear()
#        dc = wx.PaintDC(self)
        self.PrepareDC(dc)
#        dc.BeginDrawing()
        #print("BASEITEMVIEW: onPaintEvent")
        #int startx, starty;   // Start of client area
        #int sunitx, sunity;   // Scrolling units
        #int ux, uy, uw, uh;   // Update rectangle
        sunitx, sunity = self.GetScrollPixelsPerUnit()
        startx, starty = self.GetViewStart()
        #//  wxBrush brush(*wxWHITE);
        #//  dc.SetBrush(brush);
        # FIXME: RENDERBACKGROUND SUPPORT
        #renderBackground (dc);
        if self.numcolumns == 0 or self.numrows == 0:
            self.renderEmptyView(dc)
        region = wx.RegionIterator(self.GetUpdateRegion())
        # Keep a list of which items are rendered so we avoid rendering
        # items twice (or more) because of items overlapping regions
        rendereditems = []
        while region:
            ux = self.relativeToAbsoluteX(region.GetX())
            uy = self.relativeToAbsoluteY(region.GetY())
            uw = region.GetW()
            uh = region.GetH()
            rowstart, rowend = -1, -1
            columnstart, columnend = -1, -1
            # This is not very efficient...
            x = self.xpadding
            for i in range (0, self.numcolumns):
                if columnstart == -1 and x + self.itemwidth >= ux:
                    columnstart = i
                if x < ux + uw:
                    columnend = i
                x = x + self.itemwidth + self.xspacing
            # This is not very efficient...
            y = self.ypadding
            for i in range (0, self.numrows):
                if rowstart == -1 and y + self.itemheight >= uy:
                    rowstart = i
                if y < uy + uh:
                    rowend = i
                y = y + self.itemheight + self.yspacing
            #logger.debug('%s rows, %s cols' %(rowend, columnend))
            # Refresh the items overlapping with the update region
            for i in range(columnstart, columnend + 1):
                for j in range(rowstart, rowend + 1):
                    if rowstart == -1 or columnstart == -1:
                        continue
                    index = i + j * self.numcolumns
                    #logger.debug('painting index %s' %index)
                    if index >= self.numitems:
                        break
                    #if index in self.selected_items: selected = True
                    #else: selected = False
                    # Do not render this item again (if already rendered)
                    if index in rendereditems:
                        continue
                    rendereditems.append(index)
                    self.refreshItem(index, dc=dc)
            region.Next()
#        dc.EndDrawing()

    def onResizeEvent(self, event):
        self.calculateRows ()
        self.calculateColumns ()
        self.calculateVirtualSize ()
        self.Refresh()

    def onMouseMoveEvent(self, event):
        if self.trydragdrop and event.Dragging() and event.LeftIsDown() \
                and self.mouseclickonitem > -1:
            mouseend = wx.Size()
            mouseend.x = self.relativeToAbsoluteX(event.GetX())
            mouseend.y = self.relativeToAbsoluteY(event.GetY())
            distance = math.sqrt((mouseend.x - self.mousestartx) *
                    (mouseend.x - self.mousestartx) +
                    (mouseend.y - self.mousestarty) *
                    (mouseend.y - self.mousestarty))
            if distance > 15:
                print("START DRAG AND DROP")
                self.trydragdrop = False
                if self.HasCapture():
                    # Allow drop outside the itemview.
                    self.ReleaseMouse()
                self.doStartDragDrop()
            self.selectonmouseup = False
#        if (m_selectmode == SELECT_MODE_SINGLE) return;
#        wxClientDC dc(this);
#        PrepareDC (dc);
#        dc.BeginDrawing ();
#        if (event.Dragging() && event.LeftIsDown() &&
#                m_mouseclickonitem == -1 &&
#                m_drawrubberband == false) {
#            //CaptureMouse ();
#            m_drawrubberband = true;
#        }
#        if (m_drawrubberband) {
#            DrawRubberBand (dc);
#        }
#        m_mouseend.x = RelativeToAbsoluteX (event.GetX ());
#        m_mouseend.y = RelativeToAbsoluteY (event.GetY ());
#        if (m_drawrubberband) {
#            RefreshRubberCells ();
#            DrawRubberBand (dc);
#        }
#        /* FIXME: Use timeout instead... ! */
#        if (abs (m_mouseend.x - m_mousestart.x) > 10 ||
#                abs (m_mouseend.y - m_mousestart.y) > 10) {
#            m_mouseclick = false;
#        }
#        int startx, starty;
#        int sunitx, sunity;
#        int dy = 0;
#        GetScrollPixelsPerUnit (&sunitx, &sunity);
#        GetViewStart (&startx, &starty);
#        starty = starty * sunity;
#        wxSize size = GetClientSize ();
#    //  wxLogDebug("Timer event");
#        if (m_mouseend.y < starty) dy = m_mouseend.y - starty;
#        if (m_mouseend.y > starty + size.GetY ())
#            dy = m_mouseend.y - (starty + size.GetY ());
#        if (dy != 0) {
#            m_rubberscroll = dy // sunity;
#            //Scroll(-1, scrolltoy);
#        }
#        else {
#            m_rubberscroll = 0;
#        }
#        dc.EndDrawing ();

    def onLeftDoubleClickEvent(self, event):
        cellindex = -1
        mousestartx = self.relativeToAbsoluteX (event.GetX ())
        mousestarty = self.relativeToAbsoluteY (event.GetY ())
        cellindex = self.findItemAt (mousestartx, mousestarty)
        if cellindex > -1 and cellindex in self.selected_items:
            self.on_activate_item (cellindex)

    def onRightDownEvent(self, event):
        cellindex = -1
        mousestartx = self.relativeToAbsoluteX (event.GetX ())
        mousestarty = self.relativeToAbsoluteY (event.GetY ())
        cellindex = self.findItemAt (mousestartx, mousestarty)
        if cellindex > -1:
            if self.multi_select and cellindex not in self.selected_items:
                self.select_item(cellindex)
            self.onRightClickItem (cellindex)

    def onLeftDownEvent(self, event):
        cellindex = -1
        #CaptureMouse ();
        self.mousestartx = self.relativeToAbsoluteX(event.GetX())
        self.mousestarty = self.relativeToAbsoluteY(event.GetY())
        self.mouseclick = True
        cellindex = self.findItemAt(self.mousestartx, self.mousestarty)
        if cellindex > -1:
            cellpos = self.getItemCoordinates(cellindex)
            #if (onItemMouseDown (cellindex, m_mousestart.x - cellpos.x,
            #        m_mousestart.y - cellpos.y) == false) return;
        self.mouseclickonitem = cellindex
        #last_clicked_on = self.mouselastclickonitem
        #self.mouselastclickonitem = cellindex
        self.trydragdrop = True
        self.selectonmouseup = False
        if self.selectmode == 0:
            if cellindex > -1:
                add_to_selection = False
                shift_down = False
                if self.multi_select:
                    if event.ControlDown():
                        add_to_selection = True
                    if event.ShiftDown():
                        shift_down = True
                if self.canreselect_items:
                    self.select_item(cellindex, add_to_selection)
                elif shift_down:
                    if not self.mouselastclickonitem is None and \
                            cellindex != self.mouselastclickonitem:
                        add = [i for i in range(min(cellindex,
                                self.mouselastclickonitem), max(
                                cellindex, self.mouselastclickonitem) + 1)]
                        update = self.selected_items + add
                        if add_to_selection:
                            self.selected_items += add
                        else:
                            self.selected_items = add
                        for i in update:
                            self.refreshItem(i)
                elif not cellindex in self.selected_items:
                    # The item was not already selected
                    #//SendEvent (EVENT_ITEMVIEW_SELECT, cellindex);
                    self.select_item(cellindex, add_to_selection)
                elif cellindex in self.selected_items:
                    # FIXME: if more than one selected, wait for drag?
                    #if len(self.selected_items) > 1:
                    self.selectonmouseup = cellindex
#                    if add_to_selection:
#                        self.selected_items.remove(cellindex)
#                        self.refreshItem(cellindex, True, False)
#                    else:
#                        # Remove selection on other items.
#                        self.select_item(cellindex, False, True)
        #else {
        #
        #    m_rubberaddeditems.Clear ();
        #
        #    if (cellindex > -1 && event.ControlDown ()) {
        #        if (m_selecteditems.Index(cellindex) == wxNOT_FOUND) {
        #            SelectCell(cellindex, true);
        #            //SendEvent (EVENT_ITEMVIEW_SELECT, cellindex);
        #        } else {
        #            DeselectCell(cellindex, true);
        #        }
        #
        #    }
        #    else if (cellindex > -1 && event.ShiftDown ()) {
        #        SelectRange(m_selecteditem, cellindex);
        #
        #        /* FIXME: Should send some kind of select range event */
        #        //SendEvent (EVENT_ITEMVIEW_SELECT, cellindex);
        #
        #    }
        #
        #    else if (m_selecteditems.Index (cellindex) != wxNOT_FOUND) {
        #
        #        // We want to "select" the cell in any case, to unselect any
        #        // other selected cells
        #        //m_keyendindex = cellindex;
        #
        #        m_selectonmouseup = true;
        #        //SelectCell(cellindex, false);
        #
        #        if (m_selecteditems.Index (cellindex) == wxNOT_FOUND) {
        #            // The item was not already selected
        #            //SendEvent (EVENT_ITEMVIEW_SELECT, cellindex);
        #        }
        #    }
        #    else if (cellindex > -1) {
        #
        #        SelectCell (cellindex, false);
        #    }
        #    else {
        #
        #        ClearSelection ();
        #    }
        #
        #     m_rubberscroll = 0;
        #     m_timer.Start(SCROLLDELAY);
        #}
        # Allow wxWidgets to process the event (focus the widget, etc..)
        self.mouselastclickonitem = cellindex
        event.Skip ()

    def __on_left_up(self, event):
        # FIXME: deselect if not drag-n-drop.
        if self.selectonmouseup is not False:
            cellindex = self.selectonmouseup
            add_to_selection = False
            if self.multi_select:
                if event.ControlDown():
                    add_to_selection = True
            if add_to_selection:
                if cellindex in self.selected_items:
                    self.selected_items.remove(cellindex)
                self.refreshItem(cellindex, True, False)
            else:
                # Remove selection on other items.
                self.select_item(cellindex, False, True)
        event.Skip()

    def onSetFocusEvent(self, event):
        if len(self.selected_items) > 0:
            self.refreshItem(self.selected_items[0])

    def onKillFocusEvent(self, event):
        if len(self.selected_items) > 0:
            self.refreshItem(self.selected_items[0])

    def onKeyPressEvent(self, event):
        cellindex = -1
        skip = False
        handlenavigation = False
        if self.numitems == 0:
            return
        if event.GetKeyCode () == 9:
            if event.ShiftDown():
                self.Navigate (0)
            else:
                self.Navigate (wx.NavigationKeyEvent.IsForward)
            return
        if event.GetKeyCode() == wx.WXK_NEXT:
            skip = False
            self.Scroll(-1, self.relativeToAbsoluteY (0) +
                    self.GetClientSize().y)
        elif event.GetKeyCode () == wx.WXK_PRIOR:
            skip = False
            self.Scroll(-1, self.relativeToAbsoluteY(0) -
                    self.GetClientSize().y)
        elif event.GetKeyCode() == wx.WXK_LEFT:
            if len(self.selected_items) == 0:
                cellindex = 0
            else:
                cellindex = self.selected_items[0] - 1
            self.keyendindex = self.keyendindex - 1
            skip = False
            handlenavigation = True
        elif event.GetKeyCode() == wx.WXK_RIGHT:
            if len(self.selected_items) == 0:
                cellindex = 0
            else:
                cellindex = self.selected_items[0] + 1
            self.keyendindex = self.keyendindex + 1
            skip = False
            handlenavigation = True
        elif event.GetKeyCode () == wx.WXK_UP:
            if len(self.selected_items) == 0:
                cellindex = 0
            else:
                cellindex = self.selected_items[0] - self.numcolumns
            self.keyendindex = self.keyendindex - self.numcolumns
            skip = False
            handlenavigation = True
        elif event.GetKeyCode () == wx.WXK_DOWN:
            if len(self.selected_items) == 0:
                cellindex = 0
            else:
                cellindex = self.selected_items[0] + self.numcolumns
            self.keyendindex = self.keyendindex + self.numcolumns
            skip = False
            handlenavigation = True
        elif event.GetKeyCode () == wx.WXK_RETURN:
            # FIXME: IMPLEMENT
            if len(self.selected_items) == 1:
            #    self.sendEvent (EVENT_ITEMVIEW_ACTIVATE, m_selecteditem)
                self.on_activate_item(self.selected_items[0])
            skip = True
        else:
            if self.numitems == 0:
                if skip: event.Skip ()
                return
            #if event.GetKeyCode () == wx.WXK_BACK:
            #    self.typingsearchstr = ""
            #    if skip: event.Skip ()
            #    return
            char = event.GetUnicodeKey()
            #print(chr (char))
            t = time.time ()
            if t - self.typingsearchtime > 1:
                self.typingsearchstr = ""
            self.typingsearchtime = t
            if event.GetKeyCode() == wx.WXK_BACK:
                self.typingsearchstr = self.typingsearchstr[:-1]
            else:
                self.typingsearchstr = self.typingsearchstr + chr (char)
            print(self.typingsearchstr)
            pos = self.search_for_text(self.typingsearchstr)
            if len(self.selected_items) == 0 or self.selected_items[0] != pos \
                    and pos != None and len(self.typingsearchstr) > 0:
                self.scrollToItem (pos)
                self.select_item (pos)
        if handlenavigation:
            if not event.ShiftDown() or len(self.selected_items) == 0:
                self.keyendindex = cellindex
            else:
                cellindex = self.selected_items[0]
            if cellindex < 0:
                cellindex = 0
            if cellindex > self.numitems - 1:
                cellindex = self.numitems - 1
            if self.keyendindex < 0:
                self.keyendindex = 0
            if self.keyendindex > self.numitems - 1:
                self.keyendindex = self.numitems - 1
            #wxLogDebug("New cellindex: %d, keyendindex: %d, selecteditem: %d",
            #         cellindex, m_keyendindex, m_selecteditem);
            #if False:
            #    pass
            #if (m_selectmode == SELECT_MODE_MULTIPLE && event.ShiftDown ()) {
            #    SelectRange (m_selecteditem, m_keyendindex);
            #    ScrollToCell (m_keyendindex);
            #}
            #else {
            #else:
            self.select_item(cellindex, False)
                #//SendEvent (EVENT_ITEMVIEW_SELECT, cellindex);
            self.scrollToItem(cellindex)
        if skip:
            event.Skip ()

    #def onResizeEvent(self, event):
    #    self.Refresh (False)

class VerticalItemView (BaseItemView):

    def __init__(self, parent, id=wx.ID_ANY, pos=wx.DefaultPosition,
            size=wx.DefaultSize):
        self.numcolumns = 1 # Default number of columns
        self.wantcolumns = 1
        self.expanditemwidth = True
        self.minitemwidth = 20
        self._singletextlineitemheight = 26
        self._doubletextlineitemheight = 40
        BaseItemView.__init__(self, parent, id, pos, size)

    def render_item (self, index, rd):
        # FIXME: remove direct dependency on wx from this method
        import wx
        dc = rd.dc
        dc.SetFont(self.GetFont())

        c = dc.GetTextForeground()
        old_foreground = wx.Colour(c.Red(),c.Green(),c.Blue())
        reset_color = False

        if rd.selected:
            flags = wx.CONTROL_SELECTED
            flags |= wx.CONTROL_FOCUSED
            wx.RendererNative_Get().DrawItemSelectionRect(self, dc,
                    rd.itemrect, flags=flags)
            #dc.SetTextForeground(wx.SystemSettings.GetColour(
            #        wx.SYS_COLOUR_HIGHLIGHTTEXT))
            dc.SetTextForeground(wx.WHITE)
            reset_color = True

        x, y, w, h = rd.itemrect
        icon = self.get_item_icon(index)
        if icon is not None:
            bitmap = icon.bitmap
            if bitmap is not None and bitmap.Ok():
                dc.DrawBitmap(bitmap, x + 6, y + (h - 16) // 2, True)
                x += 16 + 6
                w -= 16 + 6
        x += 6
        w -= 6

        primary = self.get_item_text(index)
        primary = primary.replace(u"\nAmiga \u00b7 ", "\n")
        secondary = ""
        SPLIT = u"\n"
        if SPLIT in primary:
            primary, secondary = primary.split(SPLIT, 1)
            primary = primary.rstrip()
            secondary = u"   " + secondary.lstrip()

        itemRect = wx.Rect(x, y, w, h)
        dw, dh = dc.GetTextExtent(primary)
        dc.DrawText(primary, x, y + (h - dh) // 2)
        x += dw
        w -= dw

        if secondary:
            if not rd.selected:
                dc.SetTextForeground(wx.Colour(0xa0, 0xa0, 0xa0))
            dc.DrawText(secondary, x, y + (h - dh) // 2)
            reset_color = True
        if reset_color:
            dc.SetTextForeground(old_foreground)

    def setupSingleTextLineItems(self):
        self.itemheight = self._singletextlineitemheight

    def setupDoubleTextLineItems(self):
        self.itemheight = self._doubletextlineitemheight

    def getTextLinePosition (textlineindex):
        if textlineindex == 1:
            if self.itemheight == self._singletextlineitemheight:
                return (6, 6)
            elif self.itemheight == self._doubletextlineitemheight:
                return (6, 6)
        elif textlineindex == 2:
            if self.itemheight == self._doubletextlineitemheight:
                return (6, 24)
        return (0, 0)

    def getTextLineRect(self, textlineindex, icon=False):
        """
        Assumed icon size: 32x32 for doubleline items with text
        Assumed icon size: 16x16 for singleline items with text
        """
        pos = (0, 0)
        if textlineindex == 1:
            if self.itemheight == self._singletextlineitemheight:
                pos = (6, 6)
            elif self.itemheight == self._doubletextlineitemheight:
                pos = (6, 6)
        elif textlineindex == 2:
            if self.itemheight == self._doubletextlineitemheight:
                pos = (6, 20)

        if pos == (0, 0):
            return wx.Rect (0, 0, self.itemwidth, self.itemheight)

        if self.itemheight == self._singletextlineitemheight:
            if icon: x = pos[0] + 16 + 6
            else: x = pos[0]
        else:
            if icon: x = pos[0] + 32 + 6
            else: x = pos[0]

        # FIXME: 20 here represents the line height..
        return wx.Rect (x, pos[1], self.itemwidth - x - 6, 20)

    def getIconPosition(self):
        if self.itemheight == self._singletextlineitemheight:
            return (7, 5)
        else:
            return (7, 7)

    def setColumns(self, numcolumns, minitemwidth, expand):
        self.wantcolumns = numcolumns
        self.minitemwidth = minitemwidth
        self.expanditemwidth = expand

    def calculateColumns(self):
        #print("VerticalItemView.calculateColums ()")
        width = self.GetClientSize ().GetWidth ()

        if self.wantcolumns == -1:
            self.numcolumns = 1 + (width - self.minitemwidth - \
                    2 * self.xpadding) // (self.minitemwidth + self.xspacing)
        else:
            self.numcolumns = self.wantcolumns

        if self.expanditemwidth:
            self.itemwidth = (width - self.xpadding * 2 -
                self.xspacing * (self.numcolumns - 1)) // (self.numcolumns)
        else:
            self.itemwidth = self.minitemwidth
        #wxLogDebug("Width: %d, Cell width: %d", width, m_cellwidth);
        return self.numcolumns;

    def calculateVirtualSize(self):
        self.numitems = self.get_item_count()
        if self.numitems > 0:
            #print(self.numcolumns)
            if self.numcolumns < 1:
                self.numcolumns = 1
            self.numrows = self.numitems // self.numcolumns
                    #(self.numitems % 1) # KH: doesn't work if odd number.
            if self.numitems - self.numrows * self.numcolumns != 0:
                self.numrows += 1
        else:
            self.numrows = 0
            #self.numrows = self.numitems // self.numcolumns + \
                    #(self.numitems % 0)
        #self.numrows = self.numitems // self.numcolumns +
        #       (self.numitems % self.numcolumns > 0 ? 1 : 0)
        width = -1;
        if self.numrows > 0:
            height = self.ypadding * 2 + self.numrows * self.itemheight + \
                   (self.numrows - 1) * self.yspacing
        else:
            height = self.ypadding * 2 + self.numrows * self.itemheight
        #height = self.ypadding * 2 + self.numrows * self.itemheight + \
        #       (self.numrows > 0 ?(self.numrows - 1) * self.yspacing : 0)
        #print("Setting virtualsize to %dx%d" % (width, height))
        self.SetVirtualSize ((width, height));
        #self.SetScrollRate(self.cellwidth + self.yspacing, self.itemheight +
        #        self.yspacing);
        return (width, height)

    def renderStdListItemBackground(self, renderdata):
        dc = renderdata.dc
        if renderdata.dragover:
            dc.SetPen (wx.RED_PEN)
            dc.SetBrush (wx.RED_BRUSH)
            dc.SetTextForeground (wx.WHITE)
        elif renderdata.selected:
            dc.SetPen (wxgui.getHighlightPen ())
            dc.SetBrush (wxgui.getHighlightBrush ())
            dc.SetTextForeground (wx.SystemSettings.GetColour (
                    wx.SYS_COLOUR_HIGHLIGHTTEXT));
        else:
            dc.SetPen (wx.Pen(self.GetBackgroundColour ()))
            dc.SetBrush (wx.Brush(self.GetBackgroundColour ()))
            dc.SetTextForeground (wx.BLACK)
        #dc.DrawRectangleRect (renderdata.itemrect)
        dc.DrawRectangle (renderdata.itemrect.x,
                renderdata.itemrect.y, renderdata.itemrect.width,
                renderdata.itemrect.height - 1)
        if not renderdata.dragover and not renderdata.selected:
            dc.SetPen (wx.Pen (wx.Color (0xe8, 0xe8, 0xe8)))
            dc.DrawLine (renderdata.itemrect.x, renderdata.itemrect.y +
                    renderdata.itemrect.height - 1, renderdata.itemrect.x +
                    renderdata.itemrect.width, renderdata.itemrect.y +
                    renderdata.itemrect.height - 1);
        # Draw item focus if item has focus
        if renderdata.itemfocus:
            dc.SetPen (wx.Pen (wxgui.mixColors (dc.GetBrush ().GetColour (),
                dc.GetTextForeground (), 0.6), 1, wx.DOT))
            brush = dc.GetBrush ()
            dc.SetBrush (wx.TRANSPARENT_BRUSH)
            dc.DrawRectangle (renderdata.itemrect.x + 1,
                    renderdata.itemrect.y + 1,
                    renderdata.itemrect.width - 2,
                    renderdata.itemrect.height - 3);
            dc.SetBrush (brush)

class SimpleListItemView (VerticalItemView):

    def __init__(self, parent, id=wx.ID_ANY, doublelinemode=False,
            pos=wx.DefaultPosition, size=wx.DefaultSize):

        VerticalItemView.__init__(self, parent, id, pos, size)

        self.doublelinemode = doublelinemode
        self.items = []

        if doublelinemode:
            self.setupDoubleTextLineItems ()
        else:
            self.setupSingleTextLineItems ()

    def setItems(self, items):
        """Sets the full content of the itemview

        items is a python list of item(s). See addItem for format of item
        """

        self.items = items
        self.reset ()

    def addItem(self, item):
        """
        item is a list with the following content
        0: text (string)
        1: 16 small icon (wxBitmap) - used in singlelinemode
        2: subtext (string) - only used in doublelinemode
        3: 32 large icon (wxBitmap) - used in doublelinemode
        4: data (any type, content is not used)

        """
        self.items.append (item)
        self.update ()

    def removeItem(self, index):

        self.items[index:index] = []
        self._removeIndex (index)
        self.update ()

    def insertItem(self, index, item):

        self.items[index:index] = item
        self._insertIndex (index)
        # FIXME: UPDATE indexes
        self.update ()

    def moveItemUp(self, index):
        self._moveIndexUp (index)

    def moveItemDown(self, index):
        self._moveIndexDown (index)

    def get_item_count(self):
        return len(self.items)

    def render_item(self, index, renderdata):
        dc = renderdata.dc
        ndc = wxgui.DrawingContext (dc, (self.relToAbsX (0),
                self.relToAbsY (0)))

        item = self.items[index]

        if len (item) < 4:
            if len (item) < 3:
                if len (item) < 2:
                    item[1] = None
                item[2] = ""
            item[3] = None

        # Set colors and draw background

        self.renderStdListItemBackground (renderdata)

        # Draw icon for album

        if self.doublelinemode: icon = item[3]
        else: icon = item[1]

        if icon and icon.Ok ():
            pos = self.getIconPosition ()
            pos = (pos[0] + renderdata.itemrect.x,
                    pos[1] + renderdata.itemrect.y)
            dc.DrawBitmap (icon, pos[0], pos[1], True)

        # Draw text

        if self.doublelinemode:

            name = item[0]
            pos = name.find (" -- ")

            if pos == -1: # Old, standard display

                font = self.GetFont ()
                #font.SetWeight (wx.BOLD)
                dc.SetFont (font)

                textarea = self.getTextLineRect (1, True)
                ypadd = textarea.y
                textarea.x = textarea.x + renderdata.itemrect.x
                textarea.y = textarea.y + renderdata.itemrect.y
                textarea.height = renderdata.itemrect.height - ypadd

                drawnsize = ndc.drawTextArea (name, textarea)

                dc.SetTextForeground (wxgui.mixColors (
                        dc.GetBrush ().GetColour (),
                        dc.GetTextForeground (), 0.4))

                if (ypadd + drawnsize[1]) < 20 and item[2]:
                    count = folder.getFileCount ()
                    textarea = self.getTextLineRect (2, True)
                    textarea.x = textarea.x + renderdata.itemrect.x
                    textarea.y = textarea.y + renderdata.itemrect.y
                    dc.DrawText (item[2], textarea.x, textarea.y)


            else: # Display with group name

                group = name[0:pos]
                name = name[pos + 4:]

                orgfg = dc.GetTextForeground ()
                font = self.GetFont ()
                #font.SetWeight (wx.BOLD)
                dc.SetFont (font)

                textarea = self.getTextLineRect (1, True)
                textarea.x = textarea.x + renderdata.itemrect.x
                textarea.y = textarea.y + renderdata.itemrect.y

                ndc.drawTextLine (name, textarea)

                dc.SetTextForeground (wxgui.mixColors (
                        dc.GetBrush ().GetColour (), orgfg, 0.4))

                textarea = self.getTextLineRect (2, True)
                textarea.x = textarea.x + renderdata.itemrect.x
                textarea.y = textarea.y + renderdata.itemrect.y

                tw, th = dc.GetTextExtent (group);
                dc.DrawText (group, textarea.x, textarea.y)

                textarea.x = textarea.x + tw

                if item[2]:
                    dc.DrawText (" - " + item[2], textarea.x, textarea.y)
        else:

            name = item[0]
            pos = name.find (" -- ")

            if pos == -1: # Old, standard display

                font = self.GetFont ()
                #font.SetWeight (wx.BOLD)
                dc.SetFont (font)

                textarea = self.getTextLineRect (1, True)
                ypadd = textarea.y
                textarea.x = textarea.x + renderdata.itemrect.x
                textarea.y = textarea.y + renderdata.itemrect.y
                textarea.height = renderdata.itemrect.height - ypadd

                drawnsize = ndc.drawTextArea (name, textarea)


#class CheckListItemView (SimpleListItemView):
#
#    def __init__(self, parent, id = wx.ID_ANY, pos = wx.DefaultPosition,
#            size = wx.DefaultSize, style = 0):
#
#        SimpleListItemView.__init__(self, parent, id, pos, size, style)

update_class(BaseItemView)
