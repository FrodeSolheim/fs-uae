#include "chdtypes.h"
// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/***************************************************************************

    hashing.c

    Hashing helper classes.

***************************************************************************/

#include "hashing.h"
#include "zlib.h"


//**************************************************************************
//  CONSTANTS
//**************************************************************************

const crc16_t crc16_t::null = { 0 };
const crc32_t crc32_t::null = { 0 };
const md5_t md5_t::null = { { 0 } };
const sha1_t sha1_t::null = { { 0 } };



//**************************************************************************
//  INLINE FUNCTIONS
//**************************************************************************

//-------------------------------------------------
//  char_to_hex - return the hex value of a
//  character
//-------------------------------------------------

inline int char_to_hex(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return 10 + c - 'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c - 'A';
	return -1;
}



//**************************************************************************
//  SHA-1 HELPERS
//**************************************************************************

//-------------------------------------------------
//  from_string - convert from a string
//-------------------------------------------------

bool sha1_t::from_string(const char *string, int length)
{
	// must be at least long enough to hold everything
	memset(m_raw, 0, sizeof(m_raw));
	if (length == -1)
		length = strlen(string);
	if (length < 2 * sizeof(m_raw))
		return false;

	// iterate through our raw buffer
	for (int bytenum = 0; bytenum < sizeof(m_raw); bytenum++)
	{
		int upper = char_to_hex(*string++);
		int lower = char_to_hex(*string++);
		if (upper == -1 || lower == -1)
			return false;
		m_raw[bytenum] = (upper << 4) | lower;
	}
	return true;
}


//-------------------------------------------------
//  as_string - convert to a string
//-------------------------------------------------

const char *sha1_t::as_string(astring &buffer) const
{
	buffer.reset();
	for (int i = 0; i < ARRAY_LENGTH(m_raw); i++)
		buffer.catformat("%02x", m_raw[i]);
	return buffer;
}


//**************************************************************************
//  MD-5 HELPERS
//**************************************************************************

//-------------------------------------------------
//  from_string - convert from a string
//-------------------------------------------------

bool md5_t::from_string(const char *string, int length)
{
	// must be at least long enough to hold everything
	memset(m_raw, 0, sizeof(m_raw));
	if (length == -1)
		length = strlen(string);
	if (length < 2 * sizeof(m_raw))
		return false;

	// iterate through our raw buffer
	for (int bytenum = 0; bytenum < sizeof(m_raw); bytenum++)
	{
		int upper = char_to_hex(*string++);
		int lower = char_to_hex(*string++);
		if (upper == -1 || lower == -1)
			return false;
		m_raw[bytenum] = (upper << 4) | lower;
	}
	return true;
}


//-------------------------------------------------
//  as_string - convert to a string
//-------------------------------------------------

const char *md5_t::as_string(astring &buffer) const
{
	buffer.reset();
	for (int i = 0; i < ARRAY_LENGTH(m_raw); i++)
		buffer.catformat("%02x", m_raw[i]);
	return buffer;
}



//**************************************************************************
//  CRC-32 HELPERS
//**************************************************************************

//-------------------------------------------------
//  from_string - convert from a string
//-------------------------------------------------

bool crc32_t::from_string(const char *string, int length)
{
	// must be at least long enough to hold everything
	m_raw = 0;
	if (length == -1)
		length = strlen(string);
	if (length < 2 * sizeof(m_raw))
		return false;

	// iterate through our raw buffer
	m_raw = 0;
	for (int bytenum = 0; bytenum < sizeof(m_raw) * 2; bytenum++)
	{
		int nibble = char_to_hex(*string++);
		if (nibble == -1)
			return false;
		m_raw = (m_raw << 4) | nibble;
	}
	return true;
}


//-------------------------------------------------
//  as_string - convert to a string
//-------------------------------------------------

const char *crc32_t::as_string(astring &buffer) const
{
	return buffer.format("%08x", m_raw);
}


//-------------------------------------------------
//  append - hash a block of data, appending to
//  the currently-accumulated value
//-------------------------------------------------

void crc32_creator::append(const void *data, UINT32 length)
{
	m_accum.m_raw = crc32(m_accum, reinterpret_cast<const Bytef *>(data), length);
}



//**************************************************************************
//  CRC-16 HELPERS
//**************************************************************************

//-------------------------------------------------
//  from_string - convert from a string
//-------------------------------------------------

bool crc16_t::from_string(const char *string, int length)
{
	// must be at least long enough to hold everything
	m_raw = 0;
	if (length == -1)
		length = strlen(string);
	if (length < 2 * sizeof(m_raw))
		return false;

	// iterate through our raw buffer
	m_raw = 0;
	for (int bytenum = 0; bytenum < sizeof(m_raw) * 2; bytenum++)
	{
		int nibble = char_to_hex(*string++);
		if (nibble == -1)
			return false;
		m_raw = (m_raw << 4) | nibble;
	}
	return true;
}


//-------------------------------------------------
//  as_string - convert to a string
//-------------------------------------------------

const char *crc16_t::as_string(astring &buffer) const
{
	return buffer.format("%04x", m_raw);
}


//-------------------------------------------------
//  append - hash a block of data, appending to
//  the currently-accumulated value
//-------------------------------------------------

void crc16_creator::append(const void *data, UINT32 length)
{
	static const UINT16 s_table[256] =
	{
		0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
		0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
		0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
		0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
		0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
		0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
		0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
		0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
		0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
		0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
		0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
		0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
		0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
		0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
		0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
		0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
		0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
		0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
		0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
		0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
		0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
		0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
		0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
		0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
		0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
		0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
		0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
		0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
		0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
		0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
		0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
		0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
	};

	const UINT8 *src = reinterpret_cast<const UINT8 *>(data);

	// fetch the current value into a local and rip through the source data
	UINT16 crc = m_accum.m_raw;
	while (length-- != 0)
		crc = (crc << 8) ^ s_table[(crc >> 8) ^ *src++];
	m_accum.m_raw = crc;
}
