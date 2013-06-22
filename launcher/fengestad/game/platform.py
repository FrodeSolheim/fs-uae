from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import pkg_resources


class GamePlatform(object):

    """
    ARCADE = 'Arcade'
    AMIGA = 'Amiga'
    AMSTRAD_CPC = 'Amstrad CPC'
    APPLE_II = 'Apple II'
    APPLE_IIGS = 'Apple IIGS'
    ATARI_8BIT = 'Atari 8-Bit'
    ATARI_2600 = 'Atari 2600'
    ATARI_5200 = 'Atari 5200'
    ATARI_7800 = 'Atari 7800'
    ATARI_LYNX = 'Atari Lynx'
    ATARI_ST = 'Atari ST'
    COMMODORE_64 = 'Commodore 64'
    DOS = 'DOS'
    BBC_MICRO = 'BBC Micro'
    GAME_BOY = 'Game Boy'
    GAME_BOY_COLOR = 'Game Boy Color'
    GAME_BOY_ADVANCE = 'Game Boy Advance'
    GAME_GEAR = 'Game Gear'
    GAMECUBE = 'GameCube'
    MASTER_SYSTEM = 'Master System'
    MEGA_DRIVE = 'Mega Drive'
    NINTENDO = 'Nintendo'
    NINTENDO_64 = 'Nintendo 64'
    NINTENDO_DS = 'Nintendo DS'
    PLAYSTATION = 'PlayStation'
    PLAYSTATION_2 = 'PlayStation 2'
    SUPER_NINTENDO = 'Super Nintendo'
    TURBOGRAFX_16 = 'TurboGrafx-16'
    WII = 'Wii'
    WINDOWS = 'Windows'
    ZX_SPECTRUM = 'ZX Spectrum'

    # FIXME: REMOVE
    #DOS = 'PC - DOS'
    #WINDOWS = 'PC - Windows'
    #WII = 'Nintendo Wii'

    platforms = {
        ARCADE: (
                ['ARCADE', 'MAME'],
                ['.zip']),
        AMIGA: (
                ['AMIGA', 'COMMODORE AMIGA'],
                ['.adf', '.ipf']),
        AMSTRAD_CPC: (
                ['AMSTRAD CPC'],
                ['.dsk', '.cpr']),
        APPLE_II: (
                ['APPLE II', 'APPLE ]['],
                ['.dsk']),
        APPLE_IIGS: (
                ['APPLE IIGS', 'APPLE ][GS'],
                ['.dsk', '.2mg']),
        ATARI_2600: (
                ['ATARI 2600', 'A2600'],
                ['.a26']),
        ATARI_5200: (
                ['ATARI 5200', 'A5200'],
                ['.a52', '.bin']),
        ATARI_7800: (
                ['ATARI 7800', 'A7800'],
                ['.a78']),
        ATARI_8BIT: (
                ['ATARI 8-BIT', 'ATARI 400', 'ATARI 800', 'ATARI XL', 'ATARI XE'],
                ['.atr', 'atx', '.cas', '.rom', 'xex']),
        ATARI_LYNX: (
                ['ATARI LYNX', 'LYNX'],
                ['.lnx']),
        ATARI_ST: (
                ['ATARI ST', 'ATARI STE'],
                ['.st']),
        BBC_MICRO: (
                ['BBC MICRO'],
                ['.ssd']),
        COMMODORE_64: (
                ['COMMODORE 64', 'C64'],
                ['.c64', '.d64', '.tap', '.crt', '.t64', '.nib']),
        GAME_BOY: (
                ['GAME BOY', 'GAMEBOY', 'GB', 'NINTENDO GAMEBOY',
                'NINTENDO GAME BOY'],
                ['.gb']),
        GAME_BOY_COLOR: (
                ['GAME BOY COLOR', 'GAMEBOY COLOR', 'GB COLOR', 'GBCOLOR', 'GBC'],
                ['.gbc']),
        GAME_BOY_ADVANCE: (
                ['GAME BOY ADVANCE', 'GAMEBOY ADVANCE', 'GB ADVANCE', 'GBA'],
                ['.gba']),
        NINTENDO: (
                ['NINTENDO', 'NES'],
                ['.nes']),
        NINTENDO_64: (
                ['NINTENDO 64', 'N64'],
                ['.n64']),
        NINTENDO_DS: (
                ['NINTENDO DS', 'NDS'],
                ['.nds']),
        GAMECUBE: (
                ['GAMECUBE', 'NINTENDO GAMECUBE', 'NINTENDO GAME CUBE',
                'GAME CUBE', 'GC', 'NINTENDO GC', 'NGC', 'GCN'],
                ['.iso']), # FIXME: EXT
        WII: (
                ['WII', 'NINTENDO WII'],
                ['.iso', '.wad']),
        DOS: (
                ['DOS', 'PC - DOS', 'DOS', 'MS-DOS'],
                ['.cfg']),
        WINDOWS: (
                ['WINDOWS', 'PC - WINDOWS', 'MS-WINDOWS'],
                ['.cfg']),
        GAME_GEAR: (
                ['GAME GEAR', 'SEGA GAME GEAR', 'SGG'],
                ['.gg']),
        MASTER_SYSTEM: (
                ['MASTER SYSTEM', 'SEGA MASTER SYSTEM', 'SMS'],
                ['.sms']),
        MEGA_DRIVE: (
                ['MEGA DRIVE', 'SEGA MEGA DRIVE', 'MD', 'SMD', 'GENESIS',
                'SEGA GENESIS', 'GEN'],
                ['.md']),
        PLAYSTATION: (
                ['PLAYSTATION', 'SONY PLAYSTATION', 'PS1', 'PSX'],
                ['.cue']),
        PLAYSTATION_2: (
                ['PLAYSTATION 2', 'SONY PLAYSTATION 2', 'PS2'],
                ['.iso']),
        SUPER_NINTENDO: (
                ['SUPER NINTENDO', 'SUPER NES', 'SNES', 'SUPER FAMICOM'],
                ['.sfc', '.zip']),
        TURBOGRAFX_16: (
                ['TURBOGRAFX-16', 'NEC TURBOGRAFX-16', 'TURBOGRAFX16',
                'NEC PC-ENGINE', 'PC-ENGINE', 'PCENGINE'],
                ['.pce']),
        ZX_SPECTRUM: (
                ['ZX SPECTRUM', 'SINCLAIR ZX SPECTRUM'],
                ['.tap']),
    }
    """

    #for platform, data in platforms.iteritems():
    #    data[1].append('.game')

    #aliases = {}
    #extensions = set()
    #for platform, data in platforms.iteritems():
    #    for alias in data[0]:
    #        aliases[alias] = platform
    #    for ext in data[1]:
    #        extensions.add(ext)
    aliases = {}
    platforms = {}
    extensions = set(['.game'])

    for entry_point in pkg_resources.iter_entry_points(
            'fengestad.gamecenter.platform'):
        d = entry_point.load()
        for e in d.get('extensions', []):
            extensions.add(e)
        for a in d.get('aliases', []):
            aliases[a.upper()] = d['name']
        aliases[d['name'].upper()] = d['name']
        platforms[d['name']] = (d.get('aliases', []),
                                        d.get('extensions', []))

