from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import zlib
import shutil
import traceback
import pkg_resources
import fsui as fsui
from ..I18N import _
from ..FSUAEDirectories import FSUAEDirectories


class DiskFileCreationDialog(fsui.Dialog):

    def __init__(self, parent):
        fsui.Dialog.__init__(self, parent, _("ADF & HDF Creator"))
        self.layout = fsui.VerticalLayout()
        self.layout.padding_top = 10
        self.layout.padding_bottom = 10
        self.layout.padding_left = 10
        self.layout.padding_right = 10

        self.list_view = fsui.ListView(self)
        self.list_view.set_min_width(600)
        self.list_view.set_min_height(130)
        self.list_view.set_default_icon(fsui.Image(
            "fs_uae_launcher:res/file_16.png"))
        self.layout.add(self.list_view, expand=True, fill=True, margin=10)

        self.list_view.set_items([
            _("ADF - Standard Floppy Disk Image"),
            _("ADF - Extended Floppy Disk Image (MFM)"),
            _("HDF - Single Partition Hard Disk File"),
            _("HDF - Partitionable Hard Drive Image (RDB)"),
        ])
        self.list_view.on_select_item = self.on_select_item

        text = _("Disk Image Name:")
        label = fsui.Label(self, text)
        self.layout.add(label, margin=10)

        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True, margin=10)
        self.name_field = fsui.TextField(
            self, _("New Disk Image"), read_only=False)
        hori_layout.add(self.name_field, expand=True)
        text = _("Size:")
        label = fsui.Label(self, text)
        hori_layout.add(label, margin_left=20)
        self.size_field = fsui.TextField(self, "")
        hori_layout.add(self.size_field, expand=False, margin_left=10)
        text = _("MB")
        label = fsui.Label(self, text)
        hori_layout.add(label, margin_left=10)

        text = _("Destination Directory:")
        label = fsui.Label(self, text)
        self.layout.add(label, margin=10)

        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True, margin=10)
        self.dir_field = fsui.TextField(self, "", read_only=True)
        hori_layout.add(self.dir_field, expand=True)
        self.browse_button = fsui.Button(self, _("Browse"))
        self.browse_button.on_activate = self.on_browse_button
        hori_layout.add(self.browse_button, margin_left=10)

        self.created_label = fsui.Label(self, " ")
        self.layout.add(self.created_label, margin=10)

        self.layout.add_spacer(10)
        hori_layout = fsui.HorizontalLayout()
        hori_layout.add_spacer(10, expand=True)
        self.layout.add(hori_layout, fill=True)

        self.create_button = fsui.Button(self, _("Create"))
        self.create_button.on_activate = self.on_create_button
        hori_layout.add(self.create_button)

        self.close_button = fsui.Button(self, _("Close"))
        self.close_button.on_activate = self.on_close_button
        hori_layout.add(self.close_button, margin_left=10)
        hori_layout.add_spacer(10)
        self.layout.add_spacer(10)

        self.set_size(self.layout.get_min_size())
        self.center_on_parent()

        self.list_view.select_item(0)

    def on_select_item(self, index):
        if index < 2:
            self.set_path(FSUAEDirectories.get_floppies_dir())
        else:
            self.set_path(FSUAEDirectories.get_hard_drives_dir())
        if index == 0:
            size = "0.86"
        elif index == 1:
            size = "2.00"
        elif index == 2:
            size = "256"
        elif index == 3:
            size = "1024"
        else:
            raise Exception("Unexpected index " + repr(index))
        self.size_field.set_text(size)
        self.size_field.enable(index >= 2)

    def set_path(self, path):
        self.path = path
        self.dir_field.set_text(path)

    def on_browse_button(self):
        dialog = fsui.DirDialog(
            self.get_window(),_("Select Destination Directory"))
        if dialog.show_modal():
            self.set_path(dialog.get_path())
        dialog.destroy()

    def on_close_button(self):
        self.end_modal(False)

    def on_create_button(self):
        try:
            self.create_disk_file()
        except Exception as e:
            traceback.print_exc()
            self.show_error(repr(e))

    def show_error(self, message):
        self.created_label.set_text(message)
        fsui.show_error(message)

    def show_success(self, message):
        self.created_label.set_text(message)

    def create_disk_file(self):
        self.created_label.set_text("")
        disk_type = self.list_view.get_index()
        path = self.dir_field.get_text().strip()
        if not os.path.isdir(path):
            return self.show_error(_("Specified directory does not exist"))
        name = self.name_field.get_text().strip()
        ext = ".adf" if disk_type < 2 else ".hdf"
        if not name.lower().endswith(ext):
            name += ext
        path = os.path.join(path, name)

        if disk_type == 0:
            size = 901120
        elif disk_type == 1:
            size = 2104892
        else:
            try:
                size = int(self.size_field.get_text())
            except ValueError:
                return self.show_error(_("Invalid size specified"))
            else:
                size = size * 1024 * 1024
        if disk_type == 2:
            if size >= 512 * 1024 * 1024:
                return self.show_error(
                    _("Use RDB disk images for size >= {0}".format(512)))

        if os.path.exists(path):
            return self.show_error(_("File already exists"))

        path_partial = path + ".partial"
        try:
            f = open(path_partial, "wb")
        except Exception:
            print("error opening", path)
            traceback.print_exc()
            return self.show_error(_("Could not open file for writing"))

        try:
            if disk_type == 0:
                self.create_adf(f)
            elif disk_type == 1:
                self.create_extended_adf(f)
            elif disk_type == 2:
                self.create_hdf(f, size)
            elif disk_type == 3:
                self.create_rdb(f, size)
        except Exception:
            traceback.print_exc()
            try:
                f.close()
            except Exception:
                pass
            try:
                os.unlink(path_partial)
            except Exception:
                pass
            return self.show_error(_("Error writing disk image"))
        else:
            f.close()
            try:
                shutil.move(path_partial, path)
            except Exception:
                print("error moving", path_partial, path)
                traceback.print_exc()
                return self.show_error(_("Error moving file into place"))
        self.show_success(_("Disk image created") + ": " + name)

    def create_adf(self, f):
        s = pkg_resources.resource_stream(
            str("fs_uae_launcher"), str("res/adf.dat"))
        data = s.read()
        data = zlib.decompress(data)
        f.write(data)

    def create_extended_adf(self, f):
        # Workbench 3.1 does not like the adf_extended file, created by
        # WinUAE (must check why)
        #s = pkg_resources.resource_stream(str("fs_uae_launcher"),
        #        str("res/adf_extended.dat"))
        s = pkg_resources.resource_stream(
            str("fs_uae_launcher"), str("res/adf_save_disk.dat"))
        data = s.read()
        data = zlib.decompress(data)
        f.write(data)

    def create_hdf(self, f, size):
        print("create_hdf", f, size)
        assert size % (1024 * 1024) == 0
        block = "\0" * 1024 * 1024
        for i in range(size // (1024 * 1024)):
            f.write(block)

    def create_rdb(self, f, size):
        print("create_rdb", f, size)
        assert size % (1024 * 1024) == 0
        block = "\0" * 1024 * 1024
        for i in range(size // (1024 * 1024)):
            f.write(block)
        f.seek(0)
        f.write("rdsk")
