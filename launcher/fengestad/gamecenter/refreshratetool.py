from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import re
import subprocess
from fengestad import fs
if fs.windows:
    import win32api
    import win32con
    EDS_RAWMODE = 2


class RefreshRateTool(object):

    def __init__(self, game_platform=None, game_refresh=None):
        self.game_platform = game_platform
        self.game_refresh = game_refresh

    def set_best_mode(self):
        print("RefreshRateTool.set_best_mode")
        current = self.get_current_mode()
        print("REFRESH RATE CONTROL: current mode is", current)
        modes = self.get_all_modes()
        mode_score_list = []
        for mode in modes:
            #print(mode)
            # for now, only consider same resolution
            if mode['width'] != current['width']:
                continue
            if mode['height'] != current['height']:
                continue
            # only consider modes with the same bpp
            if mode['bpp'] != current['bpp']:
                continue
            score = self.calculate_mode_score(mode)
            if score is not None:
                mode_score_list.append((score, mode))
        if len(mode_score_list) == 0:
            print("REFRESH RATE TOOL: mode_score_list is empty")
            return
        print("REFRESH RATE TOOL:", mode_score_list)
        best_mode = mode_score_list[0][1]
        if best_mode == current:
            print("REFRESH RATE TOOL: using current mode")
        else:
            self.set_mode(best_mode)

    def calculate_mode_score(self, mode, debug=False):
        game_refresh = self.game_refresh
        if not self.game_refresh:
            if debug:
                print("REFRESH RATE TOOL: game refresh not specified")
            return None
        # in case of values such as -1 etc
        if game_refresh < 1:
            if debug:
                print("REFRESH RATE TOOL: game refresh not specified")
            return None
        display_refresh = mode['refresh']
        if not display_refresh:
            if debug:
                print("REFRESH RATE TOOL: display refresh not found")
            return None
        # in case of values such as -1 etc
        if display_refresh < 1:
            if debug:
                print("REFRESH RATE TOOL: display refresh not found")
            return None
        diff = game_refresh - display_refresh
        # FIXME: configurable?
        # FIXME: configurable per platform?
        allowable_pos_diff = 1.01
        allowable_neg_diff = 1.01
        if debug:
            print("REFRESH RATE TOOL: game refresh: {0} vs "
                    "display refresh: {1}".format(game_refresh,
                    display_refresh))
            print("REFRESH RATE TOOL: allow +{0}/-{1}".format(allowable_pos_diff,
                    allowable_neg_diff))
            print("REFRESH RATE TOOL: diff: {0}".format(diff))
        if diff < allowable_pos_diff and diff > -allowable_neg_diff:
            if debug:
                print("REFRESH RATE TOOL: allow vsync")
            return diff
        if debug:
            print("REFRESH RATE TOOL: deny vsync")
        return None

    def allow_vsync(self):
        """
        game_refresh = self.game_refresh
        if not self.game_refresh:
            print("REFRESH RATE TOOL: game refresh not specified")
            return False
        # in case of values such as -1 etc
        if game_refresh < 1:
            print("REFRESH RATE TOOL: game refresh not specified")
            return False
        display_refresh = self.get_display_refresh()
        if not display_refresh:
            print("REFRESH RATE TOOL: display refresh not found")
            return False
        # in case of values such as -1 etc
        if display_refresh < 1:
            print("REFRESH RATE TOOL: display refresh not found")
            return False
        diff = game_refresh - display_refresh
        # FIXME: configurable?
        # FIXME: configurable per platform?
        allowable_pos_diff = 1.01
        allowable_neg_diff = 1.01
        print("REFRESH RATE TOOL: game refresh: {0} vs "
                "display refresh: {1}".format(game_refresh,
                display_refresh))
        print("REFRESH RATE TOOL: allow +{0}/-{1}".format(allowable_pos_diff,
                allowable_neg_diff))
        print("REFRESH RATE TOOL: diff: {0}".format(diff))
        if diff < allowable_pos_diff and diff > -allowable_neg_diff:
            print("REFRESH RATE TOOL: allow vsync")
            return True
        print("REFRESH RATE TOOL: deny vsync")
        """
        current = self.get_current_mode()
        return self.calculate_mode_score(current, debug=True) is not None
        
    def get_display_refresh(self):
        ## FIXME:
        #if fs.macosx:
        #    return 60.0
        #elif fs.windows:
        #    import win32api
        #    device = win32api.EnumDisplayDevices()
        #    settings = win32api.EnumDisplaySettings(device.DeviceName, 0)
        #    return settings.DisplayFrequency()
        ## FIXME: 
        #return 50.0
        #return None
        return self.get_current_mode()['refresh']

    def get_current_mode(self):
        if fs.windows:
            settings = win32api.EnumDisplaySettings(None,
                    win32con.ENUM_CURRENT_SETTINGS)
            refresh = float(settings.DisplayFrequency)
            width = int(settings.PelsWidth)
            height = int(settings.PelsHeight)
            bpp = int(settings.BitsPerPel)
            flags = int(settings.DisplayFlags)
            return {'width': width, 'height': height, 'refresh': refresh,
                    'bpp': bpp, 'flags': flags}
        elif fs.macosx:
            # FIXME:
            return {'width': 0, 'height': 0, 'refresh': 60.0}
        else:
            return self._get_current_mode_x()

    def get_all_modes(self):
        modes = []
        if fs.windows:
            k = 0
            while True:
                try:
                    settings = win32api.EnumDisplaySettingsEx(None, k,
                            EDS_RAWMODE)
                except win32api.error:
                    break
                refresh = float(settings.DisplayFrequency)
                width = int(settings.PelsWidth)
                height = int(settings.PelsHeight)
                bpp = int(settings.BitsPerPel)
                flags = int(settings.DisplayFlags)
                #print(width, height, refresh, bpp, flags)
                modes.append({'width': width, 'height': height,
                        'refresh': refresh, 'bpp': bpp, 'flags': flags})
                k += 1
        elif fs.macosx:
            # FIXME:
            pass
        else:
            modes = self._get_all_modes_x()
        #modes.extend(self.get_override_modes())
        return modes
    """
    def get_override_modes(self):
        modes = []
        k = 0
        while True:
            mode_string = pyapp.user.ini.get('ForceDisplayModes/%d' % k)
            if not mode_string:
                break
            mode_string = mode_string.lower()
            mode = {}
            w, rest = mode_string.split('x')
            h, rest = rest.split('@')
            r, rest = rest.split('hz')
            mode['width'] = int(w)
            mode['height'] = int(h)
            mode['bpp'] = 32
            mode['refresh'] = int(r)
            mode['flags'] = 0
            modes.append(mode)
            k += 1
        print("get_override_modes returning", modes)
        return modes
    """

    def set_mode(self, mode):
        print("FIXME: Currently disabled set_mode")
        return

        if fs.windows:
            self._set_mode_windows(mode)
        elif fs.macosx:
            # FIXME:
            print("WARNING: mode settings is not supported on this "
                    "platform yet")
        else:
            self._set_mode_x(mode)

    def _set_mode_windows(self, mode):
        print("REFRESH RATE TOOL: setting mode", mode)
        k = 0
        while True:
            try:
                settings = win32api.EnumDisplaySettingsEx(None, k, EDS_RAWMODE)
            except win32api.error:
                break
            refresh = float(settings.DisplayFrequency)
            width = int(settings.PelsWidth)
            height = int(settings.PelsHeight)
            bpp = int(settings.BitsPerPel)
            flags = int(settings.DisplayFlags)
            #print(width, height, refresh, bpp, flags)
            #modes.append({'width': width, 'height': height,
            #        'refresh': refresh, 'bpp': bpp, 'flags': flags})
            if width == mode['width'] and \
                    height == mode['height'] and \
                    refresh == mode['refresh'] and \
                    bpp == mode['bpp'] and \
                    flags == mode['flags']:

                #print("trying to override with refresh", int(round(self.game_refresh)))
                ##refresh == mode['refresh'] and \
                #settings.DisplayFrequency = int(round(self.game_refresh))
                #result = win32api.ChangeDisplaySettings(settings,
                #        win32con.CDS_UPDATEREGISTRY) #win32con.CDS_FULLSCREEN)
                #        #0) #win32con.CDS_FULLSCREEN)
                #if result == win32con.DISP_CHANGE_SUCCESSFUL:
                #    print("display change was successful")
                #    return True
                #print("failed, falling back to ", mode['refresh'])
                #settings.DisplayFrequency = mode['refresh']
                print("found windows mode, changing display settings")
                result = win32api.ChangeDisplaySettings(settings,
                        win32con.CDS_UPDATEREGISTRY) #win32con.CDS_FULLSCREEN)
                        #0) #win32con.CDS_FULLSCREEN)
                if result == win32con.DISP_CHANGE_SUCCESSFUL:
                    print("display change was successful")
                    return True
                else:
                    print("display change failed, result =", result)
                    return False
            k += 1
        """
        for omode in self.get_override_modes():
            print("trying override mode", omode)
            if omode['width'] == mode['width'] and \
                    omode['height'] == mode['height'] and \
                    omode['refresh'] == mode['refresh'] and \
                    omode['bpp'] == mode['bpp'] and \
                    omode['flags'] == mode['flags']:
                settings.PelsWidth = omode['width']
                settings.PelsHeight = omode['height']
                settings.BitsPerPel = omode['bpp']
                settings.DisplayFlags = omode['flags']
                settings.DisplayFrequency = omode['refresh']
                result = win32api.ChangeDisplaySettings(settings,
                        win32con.CDS_UPDATEREGISTRY) #win32con.CDS_FULLSCREEN)
                        #0) #win32con.CDS_FULLSCREEN)
                if result == win32con.DISP_CHANGE_SUCCESSFUL:
                    print("display change was successful")
                    return True
                else:
                    print("display change failed, result =", result)
                    return False
        """
        return False

    def _set_mode_x(self, mode):
        print("REFRESH RATE TOOL: setting mode", mode)
        args = ['/usr/bin/env', 'xrandr', '-s', '{0}x{1}'.format(
                mode['width'], mode['height']), '-r',
                str(mode['refresh'])]
        p = subprocess.Popen(args)
        p.wait()

    def _get_all_modes_x(self):
        modes = []
        self._get_current_mode_x(modes=modes)
        return modes

    def _get_current_mode_x(self, modes=[]):
        modes[:] = []
        mode = {'width': 0, 'height': 0, 'refresh': 0.0, 'bpp': 0,
                'flags': 0} 
        args = ['/usr/bin/env', 'xrandr', '-q']
        p = subprocess.Popen(args, stdout=subprocess.PIPE)
        for line in p.stdout:
            if not line.startswith(' '):
                continue
            line = line.strip()
            line = line.replace("+", "")
            # collapse multiple spaces
            line = re.sub(' +', ' ', line)
            parts = line.split(' ')
            resolution = parts[0]
            rates = parts[1:]
            width, height = resolution.split('x')
            width = int(width)
            height = int(height)
            refresh = 0.0
            #refresh_rates = []
            for rate in rates:
                if rate[-1] == '*':
                    refresh = float(rate[:-1])
                    mode['width'] = width
                    mode['height'] = height
                    mode['refresh'] = refresh
                    #refresh_rates.append(float(rate[1:]))
                    modes.append({'width': width, 'height': height,
                            'refresh': float(rate[:-1]), 'bpp': 0,
                            'flags': 0})
                else:
                    #refresh_rates.append(float(rate))
                    modes.append({'width': width, 'height': height,
                            'refresh': float(rate), 'bpp': 0,
                            'flags': 0})
            #return {'width': width, 'height': height, 'refresh': refresh}
            #        #'rates', refresh_rates}
        return mode

