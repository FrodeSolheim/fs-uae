# Simple ADF file extraction
# Support for ADos OFS/FFS disks
# Author: TheCyberDruid
# Notes: Internal functions prefaced with adf_ for now.
# ToDo: Convert functions to a class.
# Version: 1.0

BLOCK_SIZE = 512        # Size of a block on an ADos disk
AMIGA_ULONG_SIZE = 4    # Size of an Amiga ULong variable

def convert_big_endian(big_endian):
    length = len(big_endian)
    c = 1
    sum = 0
    for digit in big_endian:
        sum += ord(digit) * 2**((length - c) * 8)
        c += 1
    return sum

def adf_check_ffs(data):
    return ord(data[3]) & 1 == 1    # Check if bit 1 is set = FFS

def adf_get_block(data, block_number):
    return data[block_number * BLOCK_SIZE:block_number * BLOCK_SIZE + BLOCK_SIZE]

def adf_get_block_name(block):
    length = ord(block[432])        # The length of the block (file) name is stored at offset 432
    return block[433:433 + length]

def adf_get_data_blocks(data, block, amount, ffs):
    c = 0
    offset = 24        # For OFS disks we need to skip the data header
    if (ffs):
        offset = 0    # For FFS disks there is no header in a data block
    filedata = ""
    for i in range(amount):
        data_block_number = convert_big_endian(block[308 - c:308 - c + AMIGA_ULONG_SIZE])    # The data block numbers are stored from offset 308 backwards
        t_data = adf_get_block(data, data_block_number)
        filedata += t_data[offset:]
        c += 4
    return filedata
    
def adf_get_file_block_count(block):
    return convert_big_endian(block[8:8 + AMIGA_ULONG_SIZE])

def adf_get_file_size(block):
    return convert_big_endian(block[324:324 + AMIGA_ULONG_SIZE])    # The next file size is stored at offset 324

def adf_get_hashtable(block):
    return block[24:24 + 72 * AMIGA_ULONG_SIZE]                        # The hashtable starts at offset 24 and is 72 * 4 (size_of ulong) big
    
def adf_get_next_hash(block):
    return convert_big_endian(block[496:496 + AMIGA_ULONG_SIZE])    # The next file/directory/link info block hash is stored at offset 496

def adf_get_rootblock(data):
    return data[880 * BLOCK_SIZE:880 * BLOCK_SIZE + BLOCK_SIZE]        # Rootblock is block 880

def adf_hash_name(name):
    l = hash = len(name)
    for char in name:
        hash = hash * 13
        hash = hash + ord(char.upper())
        hash = hash & 0x7ff;
    hash = hash % ((BLOCK_SIZE / AMIGA_ULONG_SIZE) - 56)
    return hash

def adf_find_file(name, hashtable, data):
    hash = adf_hash_name(name)
    name = name.upper()
    sector_number = convert_big_endian(hashtable[hash * AMIGA_ULONG_SIZE:hash * AMIGA_ULONG_SIZE + AMIGA_ULONG_SIZE]);
    if (sector_number != 0):
        sector = adf_get_block(data, sector_number)
        sector_name = adf_get_block_name(sector)
        sector_name = sector_name.upper();
        while (sector_name != name and adf_get_next_hash(sector) != 0):
            sector_number = adf_get_next_hash(sector)
            sector = adf_get_block(data, sector_number)
            sector_name = adf_get_block_name(sector)
            sector_name = sector_name.upper()
        if (sector_name != name):
            raise KeyError("file/dir not found")
            return -1
        return sector
    else:
        raise KeyError("file/dir not found")
        return -1

def adf_parse_name(name):
    return name.split("/")

def extract_file_from_adf_data(data, name):
    ffs = adf_check_ffs(data)
    name_parts = adf_parse_name(name)
    block = adf_get_rootblock(data)
    for name_part in name_parts:
        block = adf_find_file(name_part, adf_get_hashtable(block), data)
        if block == -1:
            return -1
    fs = adf_get_file_size(block)
    filedata = adf_get_data_blocks(data, block, adf_get_file_block_count(block), ffs)
    return filedata[:fs]

class ADFFileExtractor:
    
    def __init__(self, data):
        self.data = data

    def extract_file(self, name):
        return extract_file_from_adf_data(self.data, name)
