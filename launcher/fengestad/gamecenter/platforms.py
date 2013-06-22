
class Platforms:

    amiga = {
        'name': 'Amiga',
        'aliases': ['Commodore Amiga'],
        'extensions': ['.adf', '.ipf'],
    }

    arcade = {
        'name': 'Arcade',
        'aliases': ['MAME'],
        'extensions': ['.game', '.zip'],
    }

    commodore64 = {
        'name': 'Commodore 64',
        'aliases': ['C64'],
        'extensions': ['.tap', '.nib', '.c64', '.d64', '.t64', '.crt'],
    }

    dos = {
        'name': 'DOS',
        'aliases': ['MS-DOS', 'PC - DOS'],
        'extensions': ['.zip', '.7z'],
    }

    supernintendo = {
        'name': 'Super Nintendo',
        'aliases': ['SNES', 'Super NES', 'Super Famicom',],
        'extensions': ['.sfc'],
    }

amiga = Platforms.amiga
arcade = Platforms.arcade
commodore64 = Platforms.commodore64
dos = Platforms.dos
supernintendo = Platforms.supernintendo
