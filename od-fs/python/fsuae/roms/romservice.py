import hashlib
import logging
import os
import time
import zlib

from fsuae.pathservice import PathService
from fsuae.roms.rom import ROM

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

    def scan_kickstarts_dir(self) -> None:
        for dir_ in self.path_service.get_roms_dirs():
            self.scan_roms_dir_2(dir_)

    def scan_roms_dir_2(self, roms_dir: str) -> None:
        logger.debug("scan_kickstarts_dir: %s", roms_dir)

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

        for root, _dirs, files in os.walk(roms_dir):
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
