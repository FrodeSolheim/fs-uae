import hashlib
import logging
import os
import time
import zlib

from fsuae.roms.rom import ROM
from fsuae.services.pathservice import PathService

logger = logging.getLogger(__name__)

logger.setLevel(logging.DEBUG)


class ROMService:
    def __init__(self, path_service: PathService) -> None:
        self.path_service = path_service
        self.roms: list[ROM] = []

        # roms_by_path
        # roms_by_sha1
        # roms_by_crc32 ?
        # roms_by_... ?

    def scan_kickstart_dirs(self) -> None:
        for dir_path in self.path_service.get_kickstart_dirs():
            self._scan_kickstart_dir(dir_path)

    def _scan_kickstart_dir(self, dir_path: str) -> None:
        logger.debug("_scan_kickstart_dir %r", dir_path)

        lowercased_rom_extensions = {
            ".a1200",
            ".a3000",
            ".a4000",
            ".a500",
            ".a600",
            ".bin",
            ".rom",
        }

        whitelisted_names: set[str] = set()
        # whitelisted_names = {
        #     "kick33180.A500",
        #     "kick33192.A500",  # Common typo? https://www.amigaforever.com/kb/15-120
        #     "kick34005.A500",
        #     "kick37175.A500",
        #     "kick39106.A1200",
        #     "kick40063.A600",
        #     "kick40068.A1200",
        #     "kick40068.A4000",
        #     "kick40071.A4000",  # Requested by ztronzo
        # }

        t1 = time.monotonic()

        for root, _dirs, files in os.walk(dir_path):
            # It is not important to sort the files, but doing it to make it
            # (a tiy bit) easier to look through logs.
            for file in sorted(files):
                path = os.path.join(root, file)
                _, ext = os.path.splitext(path)
                ext = ext.lower()
                if file not in whitelisted_names and ext not in lowercased_rom_extensions:
                    logger.debug("Ignoring %s (unknown extension)", path)
                    continue

                if os.stat(path).st_size > 8 * 1024 * 1024:
                    logger.debug("Ignoring %s (large file > 8 MB)", path)
                    continue

                logger.debug("Found %s", path)
                self._scan_rom(path)

        t2 = time.monotonic()
        logger.debug("Scanned Kickstart ROMs in %0.1f ms", (t2 - t1) * 1000)

    def _scan_rom(self, rom_path: str) -> None:
        # with rom_path.open("rb") as f:
        #     data = f.read(1024*1024)

        with open(rom_path, "rb") as f:
            data = f.read()
        crc32 = "{:08x}".format(zlib.crc32(data))
        sha1 = hashlib.sha1(data).hexdigest()
        print(crc32)
        print(sha1)

        rom = ROM(path=str(rom_path), sha1=sha1, crc32=crc32)
        # FIXME: Check if path already exists in roms list?
        self.roms.append(rom)
