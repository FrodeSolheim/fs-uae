from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import re
import sys
import ConfigParser
from fengestad import fs
from fengestad.gamecenter.changehandler import ChangeHandler
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.gamecenter.resources import resources, logger, ngettext, _


class DOSController(GameController):

    def prepare(self):
        self.config_file = ConfigParser.ConfigParser()
        self.prepare_media()

    def configure(self):
        temp_dir = self.context.temp.dir('dosbox')
        config_file = os.path.join(temp_dir, 'dosbox.cfg')
        self.args = ['-conf', config_file]
        with open(config_file, 'wb') as f:
            self._configure_emulator(f)

    def run(self):
        if fs.windows:
            self.args.append("-noconsole")
        emulator = "fs-dosbox"
        return self.run_emulator(emulator)

    def cleanup(self):
        if not self.context.config.install:
            self.changes.update(os.path.join(self.context.game.state_dir, 'C'))

    def _configure_emulator(self, f):
        self.config_file.add_section('sdl')
        if self.get_option('fullscreen'):
            self.config_file.set('sdl', 'fullscreen', 'true')
        else:
            self.config_file.set('sdl', 'fullscreen', 'false')
        #self.config_file.set('sdl', 'fulldouble', 'false')
        self.config_file.set('sdl', 'fullresolution', '{w}x{h}'.format(
                w=self.context.system.screen_size[0],
                h=self.context.system.screen_size[1]))
        #if self.get_option('install'):
        #    f.write("windowresolution=original\n")
        #else:
        #    f.write("windowresolution=original\n")
        #    #f.write("windowresolution=%dx%d\n" % Settings.windowed_size)
        if fs.windows:
            #f.write("output=ddraw\n")
            #self.config_file.set('sdl', 'output', 'overlay')
            #self.config_file.set('sdl', 'output', 'ddraw')
            self.config_file.set('sdl', 'output', 'opengl')
        else:
            self.config_file.set('sdl', 'output', 'opengl')
        #f.write("autolock=true\n")
        #f.write("sensitivity=100\n")
        #f.write("waitonerror=true\n")
        #f.write("priority=higher,normal\n")
        #f.write("mapperfile=mapper.txt\n")
        #f.write("mapperfile=%s\n" % (self.get_temp_path('mapper.txt'),))
        #f.write("usescancodes=true\n")

        self.config_file.add_section('render')
        self.config_file.set('render', 'frameskip', '0')
        if self.get_option('stretch'):
            # FIXME: this option does not stretch, merely does not correct aspect for
            # non-square pixels resolutions, e.g. 320x200
            self.config_file.set('render', 'aspect' , 'false')
        else:
            self.config_file.set('render', 'aspect' , 'true')
        if self.context.system.screen_size[1] < 960:
            self.config_file.set('render', 'scaler' , 'none')
        else:
            self.config_file.set('render', 'scaler', 'normal2x')

        self.config_file.add_section('gus')
        try:
            self.util_path = self.resource_file('UTIL')
        except Exception:
            self.util_path = None

        if fs.windows:
            midi_config = ''
        elif fs.macosx:
            midi_config = ''
        else:
            midi_config = '128:0'
        midi_config = fs.config_string('dosbox/midiconfig', midi_config)
        if midi_config:
            self.config_file.add_section('midi')
            self.config_file.set('midi', 'midiconfig', midi_config)

        self.config_file.set('gus', 'gusrate', '22050')
        self.config_file.set('gus', 'gusbase', '240')
        self.config_file.set('gus', 'irq1', '5')
        self.config_file.set('gus', 'irq2', '5')
        self.config_file.set('gus', 'dma1', '3')
        self.config_file.set('gus', 'dma2', '3')
        self.config_file.set('gus', 'ultradir', 'C:\\ULTRASND\n')
        #self.config_file.set('gus', 'ultradir', 'U:\\ULTRASND\\411\n')
        #self.config_file.set('gus', 'ultradir', 'U:\\ULTRASND\\PPL161\n')
        
        self.config_file.add_section("cpu")
        cpu_core = self.get_config("cpu_core", "auto")
        #print("cpu_core:", cpu_core)
        self.config_file.set("cpu", "core", cpu_core)
        cpu_cycles = self.get_config("cpu_cycles", "auto")
        #print("cpu_cycles:", cpu_cycles)
        try:
            cpu_cycles_int = int(cpu_cycles)
        except ValueError:
            cpu_cycles_int = 0
        if str(cpu_cycles_int) == cpu_cycles:
            self.config_file.set("cpu", "cycles", "fixed " + cpu_cycles)
        else:
            self.config_file.set("cpu", "cycles", cpu_cycles)

        self.configure_media(f)

    def prepare_media(self):
        print("\n" + "-" * 79 + "\n" + "PREPARE MEDIA")
        media_dir = os.path.dirname(self.context.game.file)
        print("directory:", repr(media_dir))
        media = self.find_media()
        media = self.sort_media(media)
        print(media)
        # unpack disks
        temp_dir = self.context.temp.dir('dosbox-drives')
        self.discs = []
        # read dosbox config from bundled cfg file
        new_media = media[:]
        for medium in media:
            print( "-", repr(medium))
            #if medium.endswith('.cfg'):
            #    print("read config file")
            #    self.config_file.read(medium)
                #try:
                #    shared_name = self.config_file.get('game', 'shared')
                #except Exception:
                #    pass
                #else:
                #    path = os.path.join(media_dir, shared_name + '.7z')
                #    if os.path.exists(path):
                #        new_media.append(path)
                #    path = os.path.join(media_dir, shared_name + '.zip')
                #    if os.path.exists(path):
                #        new_media.append(path)
                #    self.context.game.state_dir_name = shared_name
            try:
                shared_name = self.get_config('shared_name')
            except Exception:
                print("got exception")
                pass
            else:
                path = os.path.join(media_dir, shared_name + '.7z')
                if os.path.exists(path):
                    new_media.append(path)
                path = os.path.join(media_dir, shared_name + '.zip')
                if os.path.exists(path):
                    new_media.append(path)
                self.context.game.state_dir_name = shared_name
        media = new_media
        media = self.sort_media(media)
        # unpack and/or prepare media files
        for medium in media:
            if medium.endswith('.7z') or medium.endswith('.zip'):
                if not self.context.config.install:
                    # FIXME: !!
                    if self.config_file.has_option('game', 'directory'):
                        directory = self.config_file.get('game', 'directory')
                        if not os.path.normpath(os.path.join(temp_dir, 
                                directory)).startswith(temp_dir):
                            raise Exception("illegal game directory")
                        os.makedirs(os.path.join(temp_dir, 'C', directory))
                        GameCenterUtil.unpack(medium, os.path.join(temp_dir,
                                'C', directory))
                    else:
                        GameCenterUtil.unpack(medium, temp_dir)
            elif medium.endswith('.iso'):
                self.discs.append(medium)
            elif medium.endswith('.cue'):
                self.discs.append(medium)
        # check if disc drives are overriden in config
        orig_discs = self.discs[:]
        self.discs = []
        try:
            drive_d = self.get_config('drive_d')
        except Exception:
            pass
        else:
            for ext in [u'.cue', u'.iso']:
                path = os.path.join(media_dir, drive_d + ext)
                if os.path.exists(path):
                    self.discs = [path]
                    break
            else:
                # file was not found, may be a disc spesifier (substring)
                # of file name, e.g. "Disc 1"
                check = u"(" + drive_d + u")"
                for d in orig_discs:
                    if check in d:
                        # remove medium and put first in self.discs
                        self.discs = [d]
                        orig_discs.remove(d)
                        break
        for d in orig_discs:
            if d not in self.discs:
                self.discs.append(d)
        # make sure every file name is upper cased
        for dir_path, dir_names, file_names in os.walk(temp_dir):
            for name in file_names + dir_names:
                u = name.upper()
                if name == u:
                    continue
                print("renaming to uppercase:", repr(name))
                os.rename(os.path.join(dir_path, name),
                        os.path.join(dir_path, 'UPPER_' + name))
                os.rename(os.path.join(dir_path, 
                        'UPPER_' + name), os.path.join(dir_path, u))
        print("discs:")
        print(self.discs)

    def configure_media(self, f):
        temp_dir = self.context.temp.dir('dosbox-drives')
        # archive has a bundled dosbox config file
        #if os.path.exists(os.path.join(temp_dir, 'C', 'DOSBOX')):
        #    for name in sorted(os.listdir(os.path.join(temp_dir, 'C',
        #            'DOSBOX'))):
        #        self.config_file.read(os.path.join(temp_dir, 'C', 'DOSBOX', name))
        # write final dosbox configuration
        self.config_file.write(f)
        f.write('\n')
        # check for gus support and copy patches
        def init_ultrasound(dir):
            # FIXME: !!
            if self.config_file.has_option('gus', 'gus') and \
                    self.config_file.get('gus', 'gus') == 'true':
                self.gus_patches = self.resource_file('ULTRASND411.7z')
                GameCenterUtil.unpack(self.gus_patches, os.path.join(dir))
        if not self.context.config.install:
            init_ultrasound(os.path.join(temp_dir, 'C'))

        # setup change handler
        if not self.context.config.install:
            self.changes = ChangeHandler(os.path.join(temp_dir, 'C'))
            self.changes.init(os.path.join(self.context.game.state_dir, 'C'))

        # write autoexec file
        print("\n" + "-" * 79 + "\n" + "CONFIGURE AUTOEXEC")
        f.write('[autoexec]\n')
        if self.context.config.install:
            media_dir = os.path.dirname(self.context.game.file)
            for name in os.listdir(media_dir):
                if name in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ':
                    if name == 'C':
                        init_ultrasound(os.path.join(media_dir, 'C'))
                    if name in 'DEF':
                        f.write('MOUNT {0} "{1}" -t cdrom\n'.format(name, 
                                os.path.join(media_dir, name)))
                    else:
                        f.write('MOUNT {0} "{1}"\n'.format(name, 
                                os.path.join(media_dir, name)))
        else:
            for name in os.listdir(temp_dir):
                if name in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ':
                    if name in 'DEF':
                        f.write('MOUNT {0} "{1}" -t cdrom\n'.format(name, 
                                os.path.join(temp_dir, name)))
                    else:
                        f.write('MOUNT {0} "{1}"\n'.format(name, 
                                os.path.join(temp_dir, name)))

        for i, medium in enumerate(self.discs):
            letter = 'DEFGHIJKLMNOPQRSTUVWXYZ'[i]
            if i == 0:
                f.write('IMGMOUNT {0} '.format(letter))
                # mount all discs as swappable for the first drive, useful
                # for multi-disc games such as Phantasmagory which does not
                # support multiple drives
                for m in self.discs:
                    f.write('"{0}" '.format(medium))
                f.write('-t iso\n')
            else:
                # also mount all disc on sequentially labelled drives, useful
                # for multi-disc games such as Under a Killing Moon which
                # supports multiple drives
                f.write('IMGMOUNT {0} "{1}" -t iso\n'.format(letter, medium))

        if os.path.exists(os.path.join(temp_dir, 'C', 'WINDOWS')):
            f.write('SET PATH=%PATH%;C:\\WINDOWS;\n')
            f.write('SET TEMP=C:\\WINDOWS\\TEMP\n')
        if self.util_path:
            f.write('MOUNT U "{0}"\n'.format(self.util_path))
            f.write('SET PATH=%PATH%;U:\\;\n')
        # make xcopy work properly in installers (always overwrite files,
        # do not ask questions)
        f.write('SET COPYCMD=/Y\n')
        f.write('C:\n')
        f.write('CD \\\n')
        if not self.context.config.install and \
                not '--shell' in sys.argv:
            #if self.context.config.setup:
            #    for name in sorted(os.listdir(os.path.join(temp_dir, 'C',
            #            'SETUP'))):
            #        f.write('CD \\SETUP\n')
            #        f.write('CALL ' + name + '\n')
            #        f.write('EXIT\n')
            #else:
            print("config is", repr(self.config_file))
            #if self.config_file.has_option('game', 'execute'):
            try:
                command = self.get_config('execute')
                commands = command.split(';')
            except Exception, e:
                commands = self.generate_autoexec(os.path.join(temp_dir, 'C'))
                print("auto-generated command:", commands)
            for line in commands:
                f.write(line.strip())
                f.write('\n')
            #f.write('PAUSE\n')
            f.write('EXIT\n')
                
                #print("looking for RUN file...")
                #for name in sorted(os.listdir(os.path.join(temp_dir,
                #        'C', 'RUN'))):
                #    f.write('CD \\RUN\n')
                #    f.write('CALL ' + name + '\n')
                #    f.write('EXIT\n')

        #import sys
        #sys.exit(1)
        #self.set_input_options()
        #self.set_media_options()

    #def unpack(self, archive):
    #    #data_dir = self.configuration.get_data_dir()
    #    temp_dir = self.context.temp.dir('dosbox')
    #    GameCenterUtil.unpack(archive, temp_dir)

    def generate_autoexec(self, dir):
        autoexec = []
        items = os.listdir(dir)
        for item in items:
            game_dir = os.path.join(dir, item)
            if os.path.isdir(game_dir):
                autoexec.append("CD {0}".format(item))
                break
        else:
            raise Exception("could not find game dir")
        items = os.listdir(game_dir)
        bat_files = []
        com_files = []
        exe_files = []
        for item in items:
            name, ext = os.path.splitext(item.lower())
            if name in ['install', 'setup']:
                continue
            if ext == '.bat':
                bat_files.append(item)
            elif ext == '.com':
                com_files.append(item)
            elif ext == '.exe':
                exe_files.append(item)
        if len(bat_files) == 1:
            autoexec.append("CALL {0}".format(bat_files[0]))
            return autoexec
        if len(exe_files) == 1:
            autoexec.append("{0}".format(exe_files[0]))
            return autoexec
        if len(com_files) == 1:
            autoexec.append("{0}".format(com_files[0]))
            return autoexec
        raise Exception("could not (automatically) find a suitable exe file")

    def extract_match_name(self, name):
        #name = os.path.normcase(name)
        name = name.lower()
        name, ext = os.path.splitext(name)
        name = re.sub('\\([Dd][Ii][Ss][Kk][^)]*\\)', '', name)
        name = re.sub('\\([Dd][Ii][Ss][Cc][^)]*\\)', '', name)
        # FIXME: REMOVE CD? USE DISC INSTEAD?
        name = re.sub('\\([Cc][Dd][^-)]*\\)', '', name)
        #pos_list = []
        #pos_list.append(name.find('('))
        #pos_list.append(name.find('['))
        #pos_list.append(name.rfind('.'))
        #pos_list.append(len(name))
        #for pos in sorted(pos_list):
        #    if pos > -1:
        #        return name[:pos].strip().lower()
        name = re.sub('[ ]+', ' ', name)
        name = name.strip()
        print(name)
        return name

    def find_media(self):
        media = []
        media_dir = os.path.dirname(self.context.game.file)
        base_match = self.extract_match_name(os.path.basename(
                self.context.game.file))
        print("base_match is", base_match)
        for name in os.listdir(media_dir):
            match = self.extract_match_name(name)
            if base_match == match:
                media.append(os.path.join(media_dir, name))
            print("match for file", name, "is", base_match)
        print("found media", media)
        return media

    def sort_media(self, media):
        return sorted(media)

    #def insert_option_defaults(self):
    #    GameLauncher.insert_option_defaults(self)
    #    #self.options.setdefault('install', False)

    # FIXME: Disable publseaudio
    def prepare_pulseaudio(self):
        pass

    def get_joystick_config(self, device_name):
        if device_name == 'THRUSTMASTER FCS':
            return 'thrustmaster_fcs', 'fcs', 1, {
                'jaxis_%d_1-': 'UP',
                'jaxis_%d_1+': 'DOWN',
                'jaxis_%d_0-': 'LEFT',
                'jaxis_%d_0+': 'RIGHT',
                'jaxis_%d_2-': 'TWIST_LEFT',
                'jaxis_%d_2+': 'TWIST_RIGHT',
                'jhat_%d_0_0': 'HAT_UP',
                'jhat_%d_0_1': 'HAT_RIGHT',
                'jhat_%d_0_2': 'HAT_DOWN',
                'jhat_%d_0_3': 'HAT_LEFT',
                'jbutton_%d_0': '1',
                'jbutton_%d_1': '2',
                'jbutton_%d_2': '3',
                'jbutton_%d_3': '4',
            }
        if device_name == 'CH FLIGHTSTICK':
            return 'ch_flightstick', 'ch', 1, {
                'jaxis_%d_1-': 'UP',
                'jaxis_%d_1+': 'DOWN',
                'jaxis_%d_0-': 'LEFT',
                'jaxis_%d_0+': 'RIGHT',
                'jaxis_%d_2-': 'TWIST_LEFT',
                'jaxis_%d_2+': 'TWIST_RIGHT',
                'jaxis_%d_3-': 'THROTTLE-',
                'jaxis_%d_3+': 'THROTTLE+',
                'jhat_%d_0_0': 'HAT_UP',
                'jhat_%d_0_1': 'HAT_RIGHT',
                'jhat_%d_0_2': 'HAT_DOWN',
                'jhat_%d_0_3': 'HAT_LEFT',
                'jbutton_%d_0': '1',
                'jbutton_%d_1': '2',
                'jbutton_%d_2': '3',
                'jbutton_%d_3': '4',
                'jbutton_%d_4': '5',
                'jbutton_%d_5': '6',
            }
        if device_name == '4-BUTTON ANALOG':
            return '4button_analog', '2axis', 1, {
                'jaxis_%d_1-': 'UP',
                'jaxis_%d_1+': 'DOWN',
                'jaxis_%d_0-': 'LEFT',
                'jaxis_%d_0+': 'RIGHT',
                #'jaxis_%d_2-': 'TWIST_LEFT',
                #'jaxis_%d_2+': 'TWIST_RIGHT',
                #'jaxis_%d_3-': 'THROTTLE-',
                #'jaxis_%d_3+': 'THROTTLE+',
                #'jhat_%d_0_0': 'HAT_UP',
                #'jhat_%d_0_1': 'HAT_RIGHT',
                #'jhat_%d_0_2': 'HAT_DOWN',
                #'jhat_%d_0_3': 'HAT_LEFT',
                'jbutton_%d_0': '1',
                'jbutton_%d_1': '2',
                'jbutton_%d_2': '3',
                'jbutton_%d_3': '4',
        }
        raise Exception("Unknown joystick: " + device_name)

    def get_joystick_event_id(self, stick, v):
        if v.startswith('JB'):
            mapper_bind = 'button %d' % (int(v[2:]),)
        elif v.startswith('JA'):
            direction = 1 if v[3] == '+' else 0
            mapper_bind = 'axis %d %d' % (int(v[2]), direction)
        elif v.startswith('JH'):
            direction = int(v[3])
            mapper_bind = 'hat %d %d' % (int(v[2]), direction)
        else:
            raise Exception("Unknown joystick part")
        return '"stick_%d %s"' % (stick, mapper_bind)

    def map_virtual_keyboard(self, mapper):
        if not 'VIRTUAL KEYBOARD' in self.game_info.info_dict:
            return mapper
        controllers, sclist = self.get_controllers('COMPATIBLE', 0, 2)
        for i, controller in enumerate(controllers):
            os.environ['KGS_JOYSTICK_%d' % (i,)] = str(
                    controller.get_sdl_joystick_id())
        mapper = dict([x.split(' ', 1) for x in mapper.split('\n')])
        for k, v in self.game_info.info_dict.iteritems():
            if not k.startswith('VIRTUAL KEYBOARD/'):
                continue
            k = k[17:]
            dev, event = k.split(' ', 1)
            event = event.upper().replace(' ', '_').replace('-', '_')
            assert dev[:3].upper() == 'JOY'
            stick = int(dev[3]) - 1
            #assert stick == 0 or stick == 1
            if stick >= len(controllers):
                # this stick is not available
                continue
            controller = controllers[stick]
            cfg = controller.get_config()
            #cfg_action = cfg[event]
            m = self.get_joystick_event_id(stick, cfg[event])
            #v = 'key_' + v.lower()
            for v in v.split(','):
                v = v.strip().lower()
                if not v:
                    continue
                v = 'key_' + v
                mapper[v] = mapper[v] + " " + m
        return '\n'.join([' '.join(x) for x in mapper.iteritems()])

    def set_input_options(self):
        try:
            game_port = self.configuration['GAME PORT']
            game_port_devices = [x.strip().upper() \
                    for x in game_port.split('>')]
        except:
            game_port_devices = []
        mapper = ""
        if fs.linux:
            from .dosboxmapper import linux_mapper
            mapper = linux_mapper
        dosbox_name = "none"
        for device_name in game_port_devices:
            config_name, dosbox_name, count, input_config = \
                    self.get_joystick_config(device_name)
            print("Config Name", config_name)
            print("DOSBox Name", dosbox_name)
            controllers, sclist = self.get_controllers(config_name, count)
            if controllers == 0:
                print("(Found 0 eligible controllers)")
                continue
            for i, controller in enumerate(controllers):
                ctrl_config = controller.get_config()
                if controller.get_type() == "TYPE_JOYSTICK":
                    for key, value in input_config.iteritems():
                        v = ctrl_config[value]
                        m = self.get_joystick_event_id(i, v)
                        key = key % (i,)
                        mapper += '\n%s "%s" ' % (key, m)
                    os.environ['KGS_JOYSTICK_%d' % (i,)] = str(
                            controller.get_sdl_joystick_id())
                else:
                    raise Exception("Only joysticks are supported")
            break
        if not game_port_devices:
            mapper = self.map_virtual_keyboard(mapper)
            dosbox_name = "ch" # FIXME
        with open(self.get_temp_path('mapper.txt'), "wb") as f:
            f.write(mapper)
        print
        print(mapper)
        print
        with open(self.get_temp_path('config'), 'ab+') as f:
            f.write('\n[joystick]\n')
            f.write('joysticktype=%s\n' % (dosbox_name,))
            f.write('timed=true\n')
            f.write('autofire=false\n')
            f.write('swap34=false\n')
            f.write('buttonwrap=false\n')

