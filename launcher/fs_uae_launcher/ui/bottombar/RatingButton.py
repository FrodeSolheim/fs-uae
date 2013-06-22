from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from fsui import ImageButton, Image
from fsgs.OAGDClient import OAGDClient
from ...Config import Config
from ...I18N import _
from ...Database import Database
from ..IconButton import IconButton


class RatingButton(ImageButton):

    def __init__(self, parent, rating):
        self.rating = rating
        self.icons = [None, None]
        if self.rating == 5:
            self.icons[0] = Image(
                "fs_uae_launcher:res/16/rating_fav_2.png")
            tooltip = _("Mark this game variant as personal favorite")
        elif self.rating == 4:
            self.icons[0] = Image(
                "fs_uae_launcher:res/16/thumb_up_2.png")
            tooltip = _("Like this game variant")
        elif self.rating == 1:
            self.icons[0] = Image(
                "fs_uae_launcher:res/16/thumb_down_2.png")
            tooltip = _("Dislike this game variant")
        else:
            raise Exception("ImageButton: unknown rating")
        self.icons[1] = self.icons[0].grey_scale()
        self.active_icon = 1
        ImageButton.__init__(self, parent, self.icons[1])
        self.set_min_width(IconButton.BUTTON_WIDTH)
        self.set_tooltip(tooltip)
        Config.add_listener(self)
        self.on_config("variant_rating", "")

    def on_destroy(self):
        Config.remove_listener(self)
        pass

    def on_activate(self):
        variant_uuid = Config.get("variant_uuid", "")
        if not variant_uuid:
            return
        client = OAGDClient()
        variant_rating = Config.get("variant_rating", "")
        if self.is_active(variant_rating):
            like = 0
        else:
            like = self.rating
        client.rate_variant(variant_uuid, like=like)

        like_rating = client.get("like", 0)
        work_rating = client.get("work", 0)
        database = Database.instance()
        cursor = database.cursor()
        #cursor.execute("UPDATE rating SET like_rating = ?, "
        #               "work_rating = ? WHERE game_uuid = ?",
        #               (like_rating, work_rating, variant_uuid))

        cursor.execute(
            "DELETE FROM rating WHERE game_uuid = ?", (variant_uuid,))
        cursor.execute(
            "INSERT INTO rating (game_uuid, work_rating, like_rating) "
            "VALUES (?, ?, ?)", (variant_uuid, work_rating, like_rating))
        database.commit()
        Config.set("variant_rating", str(like_rating))

    def is_active(self, value):
        try:
            rating = int(value)
        except ValueError:
            rating = 0
        if self.rating == 5:
            if rating == 5:
                return 1
            else:
                return 0
        elif self.rating == 4:
            if 3 <= rating <= 4:
                return 1
            else:
                return 0
        elif self.rating == 1:
            if 1 <= rating <= 2:
                return 1
            else:
                return 0

    def on_config(self, key, value):
        if key == "variant_rating":
            active_icon = not self.is_active(value)
            if active_icon != self.active_icon:
                self.set_image(self.icons[active_icon])
                self.active_icon = active_icon
                self.refresh()
        elif key == "variant_uuid":
            if value:
                if not self.is_enabled():
                    self.enable()
            else:
                if self.is_enabled():
                    self.disable()
