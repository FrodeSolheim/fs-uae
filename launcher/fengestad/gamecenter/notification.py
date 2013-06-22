from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import time


class Notification(object):
    DEFAULT_DURATION = 5.0
    notifications = []
    new = False

    def __init__(self, text, duration=DEFAULT_DURATION):
        self.start = 0.0
        self.end = 0.0
        self.duration = duration
        self.text = text
        self.add(self)

    def show(self):
        if self.start > 0:
            return
        self.start = time.time()
        self.end = self.start + self.duration

    def dismiss(self):
        self.start = 1
        self.end = 1

    @classmethod
    def add(cls, notification):
        cls.notifications.append(notification)
        cls.new = True
        print("\n" + "-" * 79 + "\n" + "NOTIFICATION")
        print(notification.text)

    @classmethod
    def all(cls):
        notifications = []
        for notification in cls.notifications:
            if notification.end == 0 or notification.end > time.time():
                notifications.append(notification)
        if len(cls.notifications) != len(notifications):
            cls.notifications = notifications[:]
        return notifications

    @classmethod
    def count(cls):
        return len(cls.notifications)

