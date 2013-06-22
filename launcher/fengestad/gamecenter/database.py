from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import sqlite3
from .gamecenter import GameCenter


class Database(object):

    VERSION = 103

    path = None
    connection = None
    cursor = None

    def __init__(self):
        #print(self.get_path())
        self.connection = sqlite3.connect(self.get_path())
        self.cursor = self.connection.cursor()

    @classmethod
    def get_path(cls):
        if cls.path:
            return cls.path
        return os.path.join(GameCenter.data_dir,
            'database-v{version}.sqlite'.format(version=cls.VERSION))

    @classmethod
    def open(cls):
        if cls.connection:
            return
        cls.connection = sqlite3.connect(cls.get_path())
        cls.cursor = cls.connection.cursor()

        create_file_sql = '''
        CREATE TABLE File (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            path TEXT,
            scanversion INTEGER NOT NULL
        );
        '''
        try:
            cls.cursor.execute('SELECT * FROM File LIMIT 1')
        except sqlite3.OperationalError:
            cls.cursor.execute(create_file_sql)


        create_game_sql = '''
        CREATE TABLE Game (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            platform TEXT,
            name TEXT,
            subtitle INT,
            config TEXT,
            year INT,
            publisher TEXT,
            developer TEXT,
            uuid TEXT,
            imageratio INTEGER,
            screenratio INTEGER,
            cmp_platform TEXT,
            cmp_name TEXT,
            cmp_config TEXT,
            script TEXT,
            scanversion INTEGER NOT NULL,
            sort_key TEXT
        );
        '''
        try:
            cls.cursor.execute('SELECT * FROM Game LIMIT 1')
        except sqlite3.OperationalError:
            cls.cursor.execute(create_game_sql)


        create_game_file_sql = '''
        CREATE TABLE GameFile (
            game INTEGER NOT NULL,
            file INTEGER NOT NULL
        );
        '''
        try:
            cls.cursor.execute('SELECT * FROM GameFile LIMIT 1')
        except sqlite3.OperationalError:
            cls.cursor.execute(create_game_file_sql)


        create_config_sql = '''
        CREATE TABLE Config (
            game INTEGER,
            key TEXT,
            value TEXT
        );
        '''
        try:
            cls.cursor.execute('SELECT * FROM Config LIMIT 1')
        except sqlite3.OperationalError:
            cls.cursor.execute(create_config_sql)
            cls.cursor.execute('CREATE INDEX Config_game on Config(game)')


        create_barcode_sql = '''
        CREATE TABLE Barcode (
            barcode TEXT PRIMARY KEY,
            cmp_platform TEXT,
            cmp_game TEXT,
            cmp_config TEXT
        );
        '''
        try:
            cls.cursor.execute('SELECT * FROM Barcode LIMIT 1')
        except sqlite3.OperationalError:
            cls.cursor.execute(create_barcode_sql)

