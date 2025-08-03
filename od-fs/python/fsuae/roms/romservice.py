import hashlib
import logging
import time
import zlib
from pathlib import Path

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

    def _scan_rom(self, rom_path: Path) -> None:
        # with rom_path.open("rb") as f:
        #     data = f.read(1024*1024)

        data = rom_path.read_bytes()
        crc32 = "{:08x}".format(zlib.crc32(data))
        sha1 = hashlib.sha1(data).hexdigest()
        print(crc32)
        print(sha1)

        rom = ROM(path=str(rom_path), sha1=sha1, crc32=crc32)
        # FIXME: Check if path already exists in roms list?
        self.roms.append(rom)

    def scan_kickstarts_dir(self):
        logger.debug(
            "scan_kickstarts_dir: %s", self.path_service.kickstart_dir
        )

        kickstart_dir = Path(self.path_service.kickstart_dir)
        rom_extensions = [".rom"]
        rom_extensions = [".rom", ".bin"]

        t1 = time.monotonic()

        for root, _dirs, files in kickstart_dir.walk():
            # It is not important to sort the files, but doing it to make it
            # (a tiy bit) easier to look through logs.
            for file in sorted(files):
                path = root / file
                if path.suffix.lower() not in rom_extensions:
                    logger.debug("Ignoring %s (unknown extension)", path)
                    continue

                if path.stat().st_size > 8 * 1024 * 1024:
                    logger.debug("Ignoring %s (large file > 8 MB)", path)
                    continue

                logger.debug("Found %s", path)
                self._scan_rom(path)

        t2 = time.monotonic()
        logger.debug("Scanned Kickstart ROMs in %0.1f ms", (t2 - t1) * 1000)
