from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import re
import sys
import json
import time
import sqlite3
import hashlib
import shutil
import urllib2
import ConfigParser
from cStringIO import StringIO
from tempfile import mkstemp
from zipfile import ZipFile
#from PIL import Image
import Image
from fengestad import fs
import threading
from fengestad.base.Paths import Paths
#from fengestad.http import HTTPClient, HTTPError
from .database import Database
from .gamecenter import GameCenter
from .platform import Platform
from .nameutil import NameUtil
from .javascript import JavaScript


#scan_database = sys.argv[1]
#game_database = sys.argv[2]
#scan_dirs = sys.argv[3:]


class NotFoundException(RuntimeError):
    pass

md5_cache = {}
sha1_cache = {}
dir_cache = set()


def get_hashes(path):
    path = os.path.normcase(os.path.normpath(path))
    print(path)
    dir = os.path.dirname(path)
    if not dir in dir_cache:
        for hash_cache, hash_file in [(md5_cache, 'MD5SUMS'), 
                (sha1_cache, 'SHA1SUMS')]:
            if os.path.exists(os.path.join(dir, hash_file)):
                with open(os.path.join(dir, hash_file), 'r') as f:
                    for line in f.readlines():
                        line = line.strip()
                        if line:
                            hash, name = line.split(' ', 1)
                            name = name.strip()
                            if name[0] == '*':
                                name = os.path.normcase(name[1:])
                                hash_cache[os.path.join(dir, name)] = hash
                                print("*", os.path.join(dir, name))
        dir_cache.add(dir)
    try:
        return sha1_cache[path], md5_cache[path]
    except KeyError:
        pass
    print("read hashes from", path)
    with open(path, 'rb') as f:
        md5 = hashlib.md5()
        sha1 = hashlib.sha1()
        data = f.read(65536)
        while data:
            md5.update(data)
            sha1.update(data)
            data = f.read(65536)
        return str(sha1.hexdigest()), str(md5.hexdigest())


def filter_path(path):
    path = path.lower()
    if 'save disk' in path:
        return False
    return True


#result = scan_cursor.execute("SELECT uuid, name FROM Game WHERE "
#        "cmpname IS NULL")
#for row in result.fetchall():
#    uuid, name = row
#    scan_cursor.execute("UPDATE Game SET cmpname = ? WHERE uuid = ?",
#            (NameUtil.create_cmpname(name), uuid))
#scan_connection.commit()
#import sys
#sys.exit()


class GameScanner(object):

    #client = HTTPClient()
    status = ""
    scanning = False
    progress = 0.0
    scanner_thread = None

    @classmethod
    def is_scanning(cls):
        if cls.scanner_thread is None:
            return False
        return cls.scanner_thread.is_alive()

    @classmethod
    def get_status(cls):
        if cls.scanner_thread is None:
            return ""
        return "Scanning"

    @classmethod
    def scan(cls):
        global scan_connection, scan_cursor
        #print("GameScanner.scan is currently disabled")
        
        from fengestad.game.FileScanner import FileScanner
        from fengestad.game.GameScanner import GameScanner
        from fengestad.game.GameDatabaseUpdater import GameDatabaseUpdater
        from fengestad.game.ScannerThread import ScannerThread

        scan_directories = [
            Paths.unicode_path(os.path.join(Paths.get_home_dir(),
                    "Games", "Files")),
            Paths.unicode_path(os.path.join(Paths.get_documents_dir(),
                    "FS-UAE", "Floppies")),
            Paths.unicode_path(os.path.join(Paths.get_documents_dir(),
                    "FS-UAE", "CD-ROMs")),
            Paths.unicode_path(os.path.join(Paths.get_documents_dir(),
                    "FS-UAE", "Hard Drives")),
        ]
        scanners = [
            FileScanner(scan_directories),
            GameDatabaseUpdater(),
            GameScanner(),
        ]
        cls.scanner_thread = ScannerThread(scanners)
        cls.scanner_thread.start()

        return

        if cls.scanning:
            return
        cls.scanning = True
        cls.progress = 0.0
        cls.status = "Starting scan"
        threading.Thread(target=cls.scan_thread).start()

    @classmethod
    def scan_thread(cls):
        try:
            cls._scan_thread()
        except Exception, e:
            print(repr(e))
        cls.scanning = False

    @classmethod
    def _scan_thread(cls):
        database = Database()
        scan_dirs = GameCenter.game_dirs
        print("scan_dirs is", scan_dirs)
        # FIXME: DEFAULT VALUE FOR --scan-database..?
        #scan_database = None
        #for arg in sys.argv:
        #    if arg.startswith('--scan-database='):
        #        scan_database = arg[16:]
        #if scan_database:
        #    scan_connection = sqlite3.connect(scan_database)
        #    scan_cursor = scan_connection.cursor()

        scan_db_paths = set()
#try:
        database.cursor.execute('SELECT path FROM File')
#except sqlite3.OperationalError:
#    Database.cursor.execute(create_file_sql)
#else:
        for row in database.cursor:
            scan_db_paths.add(row[0])

        cls.scanversion = int(time.time())
        counter = 0
        scan_these_files = []
        cls.status = "Scanning for new games"
        for scan_dir in scan_dirs:
            print(scan_dir)
            norm_scan_dir = os.path.normpath(scan_dir)
            for dir_path, dir_names, file_names in os.walk(scan_dir,
                    followlinks=True):
                remove_names = []
                for name in dir_names:
                    if os.path.exists(os.path.join(dir_path, name,
                            "no-games-here")):
                        print("not scanning", os.path.join(dir_path, name))
                        remove_names.append(name)
                for name in remove_names:
                    dir_names.remove(name)
                if 'resources' in dir_path.lower():
                    continue
                for name in file_names:
                    base, ext = os.path.splitext(name)
                    #print(name, ext)
                    #if not ext:
                    #    continue
                    if ext.lower() not in Platform.extensions:
                        continue
                    path = os.path.normpath(os.path.join(dir_path, name))
                    if not filter_path(path):
                        continue
                    relative_path = path[len(norm_scan_dir)+1:]

                    if relative_path in scan_db_paths:
                        database.cursor.execute("UPDATE Game SET "
                                "scanversion = ? WHERE id IN (SELECT game "
                                "FROM GameFile, File WHERE "
                                "GameFile.file = File.id and File.path = ?)",
                                (cls.scanversion, relative_path))
                        database.cursor.execute("UPDATE File SET "
                                "scanversion = ? WHERE path = ?",
                                (cls.scanversion, relative_path))
                        continue
                    #number = NameUtil.find_number(path)
                    #if number is not None and number != 1:
                    #    # We only scan medium 1 of a multi-medium game. This is
                    #    # necessary to avoid config name confusion in TOSEC sets
                    #    # where disk 1 may have another name than disk 2+,
                    #    # e.g. crack information only for disk 1.
                    #    continue

                    #from pprint import pprint
                    #pprint(scan_db_paths)
                    #print("")
                    #print(repr(path))
                    #time.sleep(10)
                    #if scan_database:
                    #    cls.scan_file_db(path, os.path.join(dir_path, name))
                    #else:
                    scan_these_files.append((path, os.path.join(dir_path, name),
                            relative_path))

        cls.status = "Downloading game information"
        count = len(scan_these_files)
        for i, (path, original_path, relative_path) in enumerate(scan_these_files):
            cls.progress = i / count
            cls.scan_file(database, path, original_path, relative_path)
            counter += 1
            if counter == 20:
                counter = 0
                database.connection.commit()
        cls.progress = 1.0
        database.connection.commit()

        cls.status = "Updating game database"
        database.cursor.execute("DELETE FROM Config WHERE game in "
                "(SELECT id FROM game WHERE scanversion < ?)",
                (cls.scanversion,))
        database.cursor.execute("DELETE FROM GameFile WHERE game in "
                "(SELECT id FROM game WHERE scanversion < ?)",
                (cls.scanversion,))
        database.cursor.execute("DELETE FROM GameFile WHERE file in "
                "(SELECT id FROM file WHERE scanversion < ?)",
                (cls.scanversion,))
        database.cursor.execute("DELETE FROM Game WHERE scanversion < ?",
                (cls.scanversion,))
        database.cursor.execute("DELETE FROM File WHERE scanversion < ?",
                (cls.scanversion,))

        print("commiting to database...")
        database.connection.commit()
        print("scan is done")

    @classmethod
    def ignore_path(cls, database, path):
        print("adding path to ignore list")
        database.cursor.execute("INSERT INTO File (path, scanversion) "
                "VALUES (?, ?)", (path, cls.scanversion))
        #Database.cursor.execute("SELECT game, file FROM GameFile WHERE "
        #        "game = ? AND file = ?", (None, file_id,))
        #row = Database.cursor.fetchone()
        #if not row:
        #    Database.cursor.execute("INSERT INTO GameFile (game, file) VALUES "
        #            "(?, ?)", (None, file_id))

    @classmethod
    def get_config_file_for_game_file(cls, game_file):
        if game_file.endswith('.game'):
            print("\n\n\n\nWARNING: does not read config from .game yet")
            return None
        base, _ext = os.path.splitext(game_file)
        # FIXME: THIS METHOD IS NOT GOOD ENOUGH
        path = base + '.cfg'
        if os.path.exists(path):
            #with open(path, 'rb') as f:
            #    return f.read()
            return open(path, 'rb')
        path = os.path.join(os.path.dirname(game_file), "META-INF",
                            "Config.ini")
        if os.path.exists(path):
            #with open(path, 'rb') as f:
            #    return f.read()
            return open(path, 'rb')
        return None
    
    @classmethod
    def scan_file(cls, database, path, original_path, relative_path):
        print("scan file", path)
        game_uuid = None
        game_name = ''
        sort_key = ''
        imageratio = None
        screenratio = None
        year = None
        publisher = None
        developer = None
        script = None
        platform_name = ''
        subtitle = 0
        configurations = []
        config_defaults = {}
        server_configs = []
        # disabled looking up game by hash, for now
        """
        sha1, md5 = get_hashes(path)
        for hash in [sha1, md5]:
            try:
                data = cls.download('file/' + hash[0:1] + '/' + hash[1:3] + 
                        '/' + hash[3:])
            except NotFoundException:
                continue
            doc = json.loads(data)
            game_uuid = doc['uuid']
            game_name = doc['name']
            platform_name = doc['platform']
            for config in doc['configs']:
                for f in config['files']:
                    if sha1 == f.get('sha1', '') or md5 == f.get('md5', ''):
                        #c = {}
                        #for key, value in config['data'].iteritems():
                        #    c['data'][key] = value
                        configurations.append(config)
                        break
            break
        """
        if not game_name:
            info = {}
            game_name, config_name = NameUtil.extract_names(original_path)
        number = NameUtil.find_number(original_path)
        if number is not None and number != 1:
            # We only scan medium 1 of a multi-medium game. This is
            # necessary to avoid config name confusion in TOSEC sets
            # where disk 1 may have another name than disk 2+,
            # e.g. crack information only for disk 1.
            return cls.ignore_path(database, relative_path)
        if NameUtil.is_bad_dump(original_path):
            return cls.ignore_path(database, relative_path)
        if not platform_name:
            platform_name = NameUtil.extract_platform(original_path)
            if platform_name is None:
                return cls.ignore_path(database, relative_path)
        print("---- scan file from server", path, original_path)
        if game_uuid is None:
            #server_name = ""
            #for c in game_name.lower():
            #    if c in u"abcdefghijklmnopqrstuvwxyz0123456789":
            #        server_name += c
            cmp_name = NameUtil.create_cmpname(game_name)
            cmp_platform = NameUtil.create_cmpname(platform_name)
            try:
                #data = cls.download('info/{0}/{1}'.format(cmp_platform,
                #        cmp_name))
                info_file = os.path.join(GameCenter.data_dir, 'info',
                        cmp_platform, cmp_name, 'game.json')
                #print(info_file, os.path.exists(info_file))
                if os.path.exists(info_file):
                    with open(info_file, 'rb') as f:
                        data = f.read()
                else:
                    data = cls.download_info(cmp_platform, cmp_name)
            except NotFoundException:
                pass
            else:
                doc = json.loads(data)
                game_uuid = doc['uuid']
                game_name = doc['name']
                sort_key = doc.get("sort_key", "")
                subtitle = doc.get('subtitle', 0)
                imageratio = doc.get('imageratio', None)
                screenratio = doc.get('screenratio', None)
                platform_name = doc['platform']
                year = doc.get('year', None)
                #publisher = ' / '.join(
                publisher_list = doc.get('publishers', [])
                developer_list = doc.get('developers', [])
                publisher = u' / '.join(publisher_list)
                developer = u' / '.join(developer_list)
                #company_list = doc.get('companies', [])
                #companies = ' / '.join(company_list)
                config_defaults = doc.get('defaults', {})
                script = doc.get('script', None)
                server_configs = doc.get("configs", [])
        if len(configurations) == 0:
            use_name = os.path.basename(original_path)
            if script:
                print(use_name)
                print("-- has script --")
                js = JavaScript(script)
                use_name = js.rename_file(use_name)
                print(use_name)
                number = NameUtil.find_number(use_name)
                if number is not None and number != 1:
                    # if we no have a numbered disk, reject all that
                    # is not #1
                    return cls.ignore_path(database, relative_path)
            else:
                print("-- (no script) --")
            extracted_name, config_name = NameUtil.extract_names(use_name)
            configuration = {
                'name': config_name,
            }
            
            for server_config in server_configs:
                for scf in server_config["files"]:
                    if scf["name"].lower() == os.path.basename(path).lower():
                        configuration["config_data"] = \
                                server_config["data"].copy()
                        break
                else:
                    continue
                break
            configurations.append(configuration)
            #configurations = set([(None, config_name)])
        
        variants = []
        config_file = cls.get_config_file_for_game_file(path)
        print("config_file:", config_file)
        if config_file:
            cp = ConfigParser.ConfigParser()
            cp.readfp(config_file)
            for section in cp.sections():
                if section == 'default':
                    continue
                variants.append(section)
        
        print("variants: ", variants)
        if len(variants) > 0:
            original = configurations
            configurations = []
            for org_c in original:
                for variant in variants:
                    new_c = org_c.copy()
                    new_c['name'] = u"{0}, {1}".format(variant, new_c['name'])
                    print(" -- config variant --", new_c['name'])
                    configurations.append(new_c)

        print("configuration defaults:", config_defaults)
        print("game", game_name, "platform", platform_name, "configurations",
                configurations)
        for configuration in configurations:
            config_name = configuration['name']
            #game_data = []
            database.cursor.execute('SELECT id FROM Game WHERE name = ? '
                    'AND config = ? AND platform = ?',
                    (game_name, config_name, platform_name))
            row = database.cursor.fetchone()
            if row:
                game_id = int(row[0])
                database.cursor.execute("UPDATE Game SET scanversion = ? "
                        "WHERE id = ?", (cls.scanversion, game_id))
            else:
                #cls.download_image_for_game(game_name,
                #        platform_name)
                #cls.download_screens_for_game(game_name,
                #        platform_name)
                #image_ratio = cls.download_image_for_game(game_name,
                #        platform_name)
                #screen_ratio = cls.download_screens_for_game(game_name,
                #        platform_name)
                cmp_platform = NameUtil.create_cmpname(platform_name)
                cmp_name = NameUtil.create_cmpname(game_name)
                cmp_config = NameUtil.create_cmpname(config_name)
                database.cursor.execute("INSERT INTO Game (platform, name, "
                        "subtitle, " 
                        "config, uuid, imageratio, screenratio, "
                        "year, publisher, developer, cmp_platform, "
                        "cmp_name, cmp_config, script, "
                        "scanversion, sort_key) VALUES (?, ?, ?, ?, ?, ?, ?, "
                        "?, ?, ?, ?, ?, ?, ?, ?, ?)", (
                        platform_name, game_name, subtitle, config_name, game_uuid,
                        imageratio, screenratio, year, publisher, developer,
                        cmp_platform, cmp_name, cmp_config, script,
                        cls.scanversion, sort_key or game_name))
                def add_barcode(platform=None, game=None, config=None):
                    data = []
                    if platform:
                        data.append(platform)
                    if game:
                        data.append(game)
                    if config:
                        data.append(config)
                    data = ':'.join(data)
                    m = hashlib.md5()
                    m.update(data)
                    barcode = str(int(m.hexdigest(), 16))[:12]
                    print(barcode)
                    try:
                        database.cursor.execute("INSERT INTO Barcode (barcode, "
                                "cmp_platform, cmp_name, cmp_config) VALUES "
                                "(?, ?, ?, ?)", (barcode, platform, game, config))
                    except sqlite3.OperationalError:
                        # No error if barcode already exists..
                        pass
                if True: # FIXME: MAKE CONFIGURABLE?
                    add_barcode(cmp_platform, cmp_name, cmp_config)
                    add_barcode(cmp_platform, cmp_name)
                    add_barcode(cmp_name)
                database.cursor.execute("SELECT id FROM Game WHERE rowid = ?",
                        (database.cursor.lastrowid,))
                row = database.cursor.fetchone()
                game_id = int(row[0])
                config_data = configuration.get('config_data', {})
                for key, value in config_defaults.iteritems():
                    if key not in config_data:
                        config_data[key] = value
                for key, value in config_data.iteritems():
                    database.cursor.execute("INSERT INTO Config (game, key, value) "
                            "VALUES (?, ?, ?)", (game_id, key, value))
            print(game_id)
            database.cursor.execute("SELECT id, scanversion FROM File WHERE path = ?",
                    (path,))
            row = database.cursor.fetchone()
            if row:
                file_id = row[0]
                if row[1] != cls.scanversion:
                    database.cursor.execute("UPDATE File SET scanversion = ? "
                            "WHERE id = ?", (cls.scanversion, file_id))
            else:
                database.cursor.execute("INSERT INTO File (path, scanversion) "
                        "VALUES (?, ?)", (relative_path, cls.scanversion))
                database.cursor.execute("SELECT id FROM File WHERE rowid = ?",
                        (database.cursor.lastrowid,))
                row = database.cursor.fetchone()
                file_id = int(row[0])
            database.cursor.execute("SELECT game, file FROM GameFile WHERE "
                    "game = ? AND file = ?", (game_id, file_id,))
            row = database.cursor.fetchone()
            if not row:
                database.cursor.execute("INSERT INTO GameFile (game, file) VALUES "
                        "(?, ?)", (game_id, file_id))

    @classmethod
    def download_info(cls, cmp_platform, cmp_name):
        zip_file = os.path.join('c:\\data\\games\\info', cmp_platform,
                cmp_name + '.zip')
        if not os.path.exists(zip_file):
            handle, zip_file = mkstemp()
            os.close(handle)
            cls.download('info/{0}/{1}.zip'.format(cmp_platform, cmp_name),
                    zip_file)

        def extract_screen(zf, number):
            name = 'screen{0}.png'.format(number)
            #data = zf.read(name)
            try:
                #f = zf.open(name)
                data = zf.read(name)
            except KeyError:
                try:
                    data = zf.read('META-INF/' + name)
                except KeyError:
                    return
            f = StringIO(data)
            im = Image.open(f)
            if im.mode != 'RGB':
                im = im.convert('RGB')
            if im.size != (512, 512):
                im = im.resize((512, 512), Image.ANTIALIAS)
            dest = prepare_dest_file(name)
            im.save(dest)

        def prepare_dest_file(name):
            dest = os.path.join(GameCenter.data_dir, 'info',
                    cmp_platform, cmp_name, name)
            if not os.path.exists(os.path.dirname(dest)):
                os.makedirs(os.path.dirname(dest))
            return dest

        def extract_file(zf, name):
            dest = prepare_dest_file(name)
            try:
                data = zf.read(name)
            except KeyError:
                try:
                    data = zf.read('META-INF/' + name)
                except KeyError:
                    return
            with open(dest, 'wb') as f:
                f.write(data)

        zf = ZipFile(zip_file, 'r')
        data = zf.read('game.json')
        dest = prepare_dest_file('game.json')
        with open(dest, 'wb') as f:
            f.write(data)
        old_cmp_name = cmp_name
        # read corrected game name, the name used until now
        # may not be the final name -and we also save a copy
        # of the game.json file under both the old _and_ the
        # new name
        doc = json.loads(data)
        cmp_name = NameUtil.create_cmpname(doc["name"])
        if cmp_name != old_cmp_name:
            print("name change:", old_cmp_name, "=>", cmp_name)
            dest = prepare_dest_file('game.json')
            with open(dest, 'wb') as f:
                f.write(data)
        extract_file(zf, 'front.jpg')
        for i in range(1, 6):
            extract_screen(zf, i)
        return data

    @classmethod
    def download(cls, path, save=None):
        # check against local development files first
        #if os.path.exists('../fs-game-database/game'):
        #    path = '../fs-game-database/game/' + path
        #    print(path)
        #    if not os.path.exists(path):
        #        raise NotFoundException(path)
        #    if save:
        #        if not os.path.exists(os.path.dirname(save)):
        #            os.makedirs(os.path.dirname(save))
        #        shutil.copy(path, save)
        #        return
        #    else:
        #        with open(path, 'rb') as f:
        #            return f.read()
        # try to download from server
        for i in range(10):
            try:
                return cls._download(path, save)
            except NotFoundException, e:
                raise e
            except Exception, e:
                import traceback
                traceback.print_exc()
                time.sleep(i / 2)
                continue

    @classmethod
    def _download(cls, path, save=None):
        t1 = time.time()
        full_url = 'http://www.fengestad.no/game/' + path
        print(full_url)
        #cls.client = HTTPClient()
        try:
            print(cls.client.get(full_url))
            data = cls.client.get(full_url).body
            #f = urllib2.urlopen(full_url)
            #data = f.read()
            #f.close()
            t2 = time.time()
            print("downloaded in", t2 - t1)
            if save:
                if not os.path.exists(os.path.dirname(save)):
                    os.makedirs(os.path.dirname(save))
                with open(save, 'wb') as f:
                    f.write(data)
                return
            else:
                return data
        except HTTPError, e:
            print(repr(e))
            if e.response.status == 404:
                print("http not found(404):", repr(full_url))
                raise NotFoundException(full_url)
            raise e

    @classmethod
    def download_image_for_game(cls, game_name, platform_name):
        cmp_name = NameUtil.create_cmpname(game_name)
        cmp_platform = NameUtil.create_cmpname(platform_name)
        dest = os.path.join(GameCenter.data_dir, 'texture',
                cmp_platform, cmp_name, 'front-1024.jpg')
        if not os.path.exists(os.path.dirname(dest)):
            os.makedirs(os.path.dirname(dest))
        # FIXME: READ FROM LOCAL CACHE / INSTALLATION
        """
        for dir in GameCenter.game_dirs:
            source = os.path.join(dir, 'Resources', 'Images', 'Medium',
                    platform_name, cmp_name + '.jpg')
            if os.path.exists(source):
                if not os.path.exists(dest):
                    shutil.copy(source, dest)
                im = Image.open(dest)
                return im.size[0] / im.size[1]
        """
        try:
            cls.download('texture/' + cmp_platform + '/' +
                    cmp_name + '/front-1024.jpg', save=dest)
        except NotFoundException:
            return None
        im = Image.open(dest)
        return im.size[0] / im.size[1]

    @classmethod
    def download_screens_for_game(cls, game_name, platform_name):
        for i in range(2):
            n = i + 1
            cmp_name = NameUtil.create_cmpname(game_name)
            cmp_platform = NameUtil.create_cmpname(platform_name)
            dest = os.path.join(GameCenter.data_dir, 'texture',
                    cmp_platform, cmp_name,
                    'screen{0}-512.png'.format(n))
            if not os.path.exists(os.path.dirname(dest)):
                os.makedirs(os.path.dirname(dest))
            # FIXME: READ FROM LOCAL CACHE / INSTALLATION
            """
            for dir in GameCenter.game_dirs:
                source = os.path.join(dir, 'Resources', 'Images', 'Medium',
                        platform_name, cmp_name + '.jpg')
                if os.path.exists(source):
                    if not os.path.exists(dest):
                        shutil.copy(source, dest)
                    im = Image.open(dest)
                    return im.size[0] / im.size[1]
            """
            try:
                cls.download('texture/' + cmp_platform + '/' +
                        cmp_name + '/screen{0}-512.png'.format(n), save=dest)
            except NotFoundException:
                return None
            im = Image.open(dest)
            return im.size[0] / im.size[1]



