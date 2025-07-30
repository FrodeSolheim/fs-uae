#include "chdtypes.h"
// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/***************************************************************************

    flac.c

    FLAC compression wrappers

***************************************************************************/

#include "flac.h"
#include <assert.h>
#include <new>


//**************************************************************************
//  FLAC ENCODER
//**************************************************************************

//-------------------------------------------------
//  flac_encoder - constructors
//-------------------------------------------------

flac_encoder::flac_encoder()
{
	init_common();
}


flac_encoder::flac_encoder(void *buffer, UINT32 buflength)
{
	init_common();
	reset(buffer, buflength);
}


flac_encoder::flac_encoder(core_file &file)
{
	init_common();
	reset(file);
}


//-------------------------------------------------
//  ~flac_encoder - destructor
//-------------------------------------------------

flac_encoder::~flac_encoder()
{
	// delete the encoder
	FLAC__stream_encoder_delete(m_encoder);
}


//-------------------------------------------------
//  reset - reset state with the original
//  parameters
//-------------------------------------------------

bool flac_encoder::reset()
{
	// configure the output
	m_compressed_offset = 0;
	m_ignore_bytes = m_strip_metadata ? 4 : 0;
	m_found_audio = !m_strip_metadata;

	// configure the encoder in a standard way
	// note we do this on each reset; if we don't, results are NOT consistent!
	FLAC__stream_encoder_set_verify(m_encoder, false);
//  FLAC__stream_encoder_set_do_md5(m_encoder, false);
	FLAC__stream_encoder_set_compression_level(m_encoder, 8);
	FLAC__stream_encoder_set_channels(m_encoder, m_channels);
	FLAC__stream_encoder_set_bits_per_sample(m_encoder, 16);
	FLAC__stream_encoder_set_sample_rate(m_encoder, m_sample_rate);
	FLAC__stream_encoder_set_total_samples_estimate(m_encoder, 0);
	FLAC__stream_encoder_set_streamable_subset(m_encoder, false);
	FLAC__stream_encoder_set_blocksize(m_encoder, m_block_size);

	// re-start processing
	return (FLAC__stream_encoder_init_stream(m_encoder, write_callback_static, NULL, NULL, NULL, this) == FLAC__STREAM_ENCODER_INIT_STATUS_OK);
}


//-------------------------------------------------
//  reset - reset state with new memory parameters
//-------------------------------------------------

bool flac_encoder::reset(void *buffer, UINT32 buflength)
{
	// configure the output
	m_compressed_start = reinterpret_cast<FLAC__byte *>(buffer);
	m_compressed_length = buflength;
	m_file = NULL;
	return reset();
}


//-------------------------------------------------
//  reset - reset state with new file parameters
//-------------------------------------------------

bool flac_encoder::reset(core_file &file)
{
	// configure the output
	m_compressed_start = NULL;
	m_compressed_length = 0;
	m_file = &file;
	return reset();
}


//-------------------------------------------------
//  encode_interleaved - encode a buffer with
//  interleaved samples
//-------------------------------------------------

bool flac_encoder::encode_interleaved(const INT16 *samples, UINT32 samples_per_channel, bool swap_endian)
{
	int shift = swap_endian ? 8 : 0;

	// loop over source samples
	int num_channels = FLAC__stream_encoder_get_channels(m_encoder);
	UINT32 srcindex = 0;
	while (samples_per_channel != 0)
	{
		// process in batches of 2k samples
		FLAC__int32 converted_buffer[2048];
		FLAC__int32 *dest = converted_buffer;
		UINT32 cur_samples = MIN(ARRAY_LENGTH(converted_buffer) / num_channels, samples_per_channel);

		// convert a buffers' worth
		for (UINT32 sampnum = 0; sampnum < cur_samples; sampnum++)
			for (int channel = 0; channel < num_channels; channel++, srcindex++)
				*dest++ = INT16((UINT16(samples[srcindex]) << shift) | (UINT16(samples[srcindex]) >> shift));

		// process this batch
		if (!FLAC__stream_encoder_process_interleaved(m_encoder, converted_buffer, cur_samples))
			return false;
		samples_per_channel -= cur_samples;
	}
	return true;
}


//-------------------------------------------------
//  encode - encode a buffer with individual
//  sample streams
//-------------------------------------------------

bool flac_encoder::encode(INT16 *const *samples, UINT32 samples_per_channel, bool swap_endian)
{
	int shift = swap_endian ? 8 : 0;

	// loop over source samples
	int num_channels = FLAC__stream_encoder_get_channels(m_encoder);
	UINT32 srcindex = 0;
	while (samples_per_channel != 0)
	{
		// process in batches of 2k samples
		FLAC__int32 converted_buffer[2048];
		FLAC__int32 *dest = converted_buffer;
		UINT32 cur_samples = MIN(ARRAY_LENGTH(converted_buffer) / num_channels, samples_per_channel);

		// convert a buffers' worth
		for (UINT32 sampnum = 0; sampnum < cur_samples; sampnum++, srcindex++)
			for (int channel = 0; channel < num_channels; channel++)
				*dest++ = INT16((UINT16(samples[channel][srcindex]) << shift) | (UINT16(samples[channel][srcindex]) >> shift));

		// process this batch
		if (!FLAC__stream_encoder_process_interleaved(m_encoder, converted_buffer, cur_samples))
			return false;
		samples_per_channel -= cur_samples;
	}
	return true;
}


//-------------------------------------------------
//  finish - complete encoding and flush the
//  stream
//-------------------------------------------------

UINT32 flac_encoder::finish()
{
	// process the data and return the amount written
	FLAC__stream_encoder_finish(m_encoder);
	return (m_file != NULL) ? core_ftell(m_file) : m_compressed_offset;
}


//-------------------------------------------------
//  init_common - common initialization
//-------------------------------------------------

void flac_encoder::init_common()
{
	// allocate the encoder
	m_encoder = FLAC__stream_encoder_new();
	if (m_encoder == NULL)
		throw std::bad_alloc();

	// initialize default state
	m_file = NULL;
	m_compressed_offset = 0;
	m_compressed_start = NULL;
	m_compressed_length = 0;
	m_sample_rate = 44100;
	m_channels = 2;
	m_block_size = 0;
	m_strip_metadata = false;
	m_ignore_bytes = 0;
	m_found_audio = false;
}


//-------------------------------------------------
//  write_callback - handle writes to the
//  output stream
//-------------------------------------------------

FLAC__StreamEncoderWriteStatus flac_encoder::write_callback_static(const FLAC__StreamEncoder *encoder, const FLAC__byte buffer[], size_t bytes, unsigned samples, unsigned current_frame, void *client_data)
{
	return reinterpret_cast<flac_encoder *>(client_data)->write_callback(buffer, bytes, samples, current_frame);
}

FLAC__StreamEncoderWriteStatus flac_encoder::write_callback(const FLAC__byte buffer[], size_t bytes, unsigned samples, unsigned current_frame)
{
	// loop over output data
	size_t offset = 0;
	while (offset < bytes)
	{
		// if we're ignoring, continue to do so
		if (m_ignore_bytes != 0)
		{
			int ignore = MIN(bytes - offset, m_ignore_bytes);
			offset += ignore;
			m_ignore_bytes -= ignore;
		}

		// if we haven't hit the end of metadata, process a new piece
		else if (!m_found_audio)
		{
			assert(bytes - offset >= 4);
			m_found_audio = ((buffer[offset] & 0x80) != 0);
			m_ignore_bytes = (buffer[offset + 1] << 16) | (buffer[offset + 2] << 8) | buffer[offset + 3];
			offset += 4;
		}

		// otherwise process as audio data and copy to the output
		else
		{
			int count = bytes - offset;
			if (m_file != NULL)
				core_fwrite(m_file, buffer, count);
			else
			{
				if (m_compressed_offset + count <= m_compressed_length)
					memcpy(m_compressed_start + m_compressed_offset, buffer, count);
				m_compressed_offset += count;
			}
			offset += count;
		}
	}
	return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
}


//**************************************************************************
//  FLAC DECODER
//**************************************************************************

//-------------------------------------------------
//  flac_decoder - constructor
//-------------------------------------------------

flac_decoder::flac_decoder()
	: m_decoder(FLAC__stream_decoder_new()),
		m_file(NULL),
		m_compressed_offset(0),
		m_compressed_start(NULL),
		m_compressed_length(0),
		m_compressed2_start(NULL),
		m_compressed2_length(0)
{
}


//-------------------------------------------------
//  flac_decoder - constructor
//-------------------------------------------------

flac_decoder::flac_decoder(const void *buffer, UINT32 length, const void *buffer2, UINT32 length2)
	: m_decoder(FLAC__stream_decoder_new()),
		m_file(NULL),
		m_compressed_offset(0),
		m_compressed_start(reinterpret_cast<const FLAC__byte *>(buffer)),
		m_compressed_length(length),
		m_compressed2_start(reinterpret_cast<const FLAC__byte *>(buffer2)),
		m_compressed2_length(length2)
{
	reset();
}


//-------------------------------------------------
//  flac_decoder - constructor
//-------------------------------------------------

flac_decoder::flac_decoder(core_file &file)
	: m_decoder(FLAC__stream_decoder_new()),
		m_file(&file),
		m_compressed_offset(0),
		m_compressed_start(NULL),
		m_compressed_length(0),
		m_compressed2_start(NULL),
		m_compressed2_length(0)
{
	reset();
}


//-------------------------------------------------
//  flac_decoder - destructor
//-------------------------------------------------

flac_decoder::~flac_decoder()
{
	FLAC__stream_decoder_delete(m_decoder);
}


//-------------------------------------------------
//  reset - reset state with the original
//  parameters
//-------------------------------------------------

bool flac_decoder::reset()
{
	m_compressed_offset = 0;
	if (FLAC__stream_decoder_init_stream(m_decoder,
				&flac_decoder::read_callback_static,
				NULL,
				&flac_decoder::tell_callback_static,
				NULL,
				NULL,
				&flac_decoder::write_callback_static,
				&flac_decoder::metadata_callback_static,
				&flac_decoder::error_callback_static, this) != FLAC__STREAM_DECODER_INIT_STATUS_OK)
		return false;
	return FLAC__stream_decoder_process_until_end_of_metadata(m_decoder);
}


//-------------------------------------------------
//  reset - reset state with new memory parameters
//-------------------------------------------------

bool flac_decoder::reset(const void *buffer, UINT32 length, const void *buffer2, UINT32 length2)
{
	m_file = NULL;
	m_compressed_start = reinterpret_cast<const FLAC__byte *>(buffer);
	m_compressed_length = length;
	m_compressed2_start = reinterpret_cast<const FLAC__byte *>(buffer2);
	m_compressed2_length = length2;
	return reset();
}


//-------------------------------------------------
//  reset - reset state with new memory parameters
//  and a custom-generated header
//-------------------------------------------------

bool flac_decoder::reset(UINT32 sample_rate, UINT8 num_channels, UINT32 block_size, const void *buffer, UINT32 length)
{
	// modify the template header with our parameters
	static const UINT8 s_header_template[0x2a] =
	{
		0x66, 0x4C, 0x61, 0x43,                         // +00: 'fLaC' stream header
		0x80,                                           // +04: metadata block type 0 (STREAMINFO),
														//      flagged as last block
		0x00, 0x00, 0x22,                               // +05: metadata block length = 0x22
		0x00, 0x00,                                     // +08: minimum block size
		0x00, 0x00,                                     // +0A: maximum block size
		0x00, 0x00, 0x00,                               // +0C: minimum frame size (0 == unknown)
		0x00, 0x00, 0x00,                               // +0F: maximum frame size (0 == unknown)
		0x0A, 0xC4, 0x42, 0xF0, 0x00, 0x00, 0x00, 0x00, // +12: sample rate (0x0ac44 == 44100),
														//      numchannels (2), sample bits (16),
														//      samples in stream (0 == unknown)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // +1A: MD5 signature (0 == none)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //
														// +2A: start of stream data
	};
	memcpy(m_custom_header, s_header_template, sizeof(s_header_template));
	m_custom_header[0x08] = m_custom_header[0x0a] = block_size >> 8;
	m_custom_header[0x09] = m_custom_header[0x0b] = block_size & 0xff;
	m_custom_header[0x12] = sample_rate >> 12;
	m_custom_header[0x13] = sample_rate >> 4;
	m_custom_header[0x14] = (sample_rate << 4) | ((num_channels - 1) << 1);

	// configure the header ahead of the provided buffer
	m_file = NULL;
	m_compressed_start = reinterpret_cast<const FLAC__byte *>(m_custom_header);
	m_compressed_length = sizeof(m_custom_header);
	m_compressed2_start = reinterpret_cast<const FLAC__byte *>(buffer);
	m_compressed2_length = length;
	return reset();
}


//-------------------------------------------------
//  reset - reset state with new file parameter
//-------------------------------------------------

bool flac_decoder::reset(core_file &file)
{
	m_file = &file;
	m_compressed_start = NULL;
	m_compressed_length = 0;
	m_compressed2_start = NULL;
	m_compressed2_length = 0;
	return reset();
}


//-------------------------------------------------
//  decode_interleaved - decode to an interleaved
//  sound stream
//-------------------------------------------------

bool flac_decoder::decode_interleaved(INT16 *samples, UINT32 num_samples, bool swap_endian)
{
	// configure the uncompressed buffer
	memset(m_uncompressed_start, 0, sizeof(m_uncompressed_start));
	m_uncompressed_start[0] = samples;
	m_uncompressed_offset = 0;
	m_uncompressed_length = num_samples;
	m_uncompressed_swap = swap_endian;

	// loop until we get everything we want
	while (m_uncompressed_offset < m_uncompressed_length)
		if (!FLAC__stream_decoder_process_single(m_decoder))
			return false;
	return true;
}


//-------------------------------------------------
//  decode - decode to an multiple independent
//  data streams
//-------------------------------------------------

bool flac_decoder::decode(INT16 **samples, UINT32 num_samples, bool swap_endian)
{
	// make sure we don't have too many channels
	int chans = channels();
	if (chans > ARRAY_LENGTH(m_uncompressed_start))
		return false;

	// configure the uncompressed buffer
	memset(m_uncompressed_start, 0, sizeof(m_uncompressed_start));
	for (int curchan = 0; curchan < chans; curchan++)
		m_uncompressed_start[curchan] = samples[curchan];
	m_uncompressed_offset = 0;
	m_uncompressed_length = num_samples;
	m_uncompressed_swap = swap_endian;

	// loop until we get everything we want
	while (m_uncompressed_offset < m_uncompressed_length)
		if (!FLAC__stream_decoder_process_single(m_decoder))
			return false;
	return true;
}


//-------------------------------------------------
//  finish - finish up the decode
//-------------------------------------------------

UINT32 flac_decoder::finish()
{
	// get the final decoding position and move forward
	FLAC__uint64 position = 0;
	FLAC__stream_decoder_get_decode_position(m_decoder, &position);
	FLAC__stream_decoder_finish(m_decoder);

	// adjust position if we provided the header
	if (position == 0)
		return 0;
	if (m_compressed_start == reinterpret_cast<const FLAC__byte *>(m_custom_header))
		position -= m_compressed_length;
	return position;
}


//-------------------------------------------------
//  read_callback - handle reads from the input
//  stream
//-------------------------------------------------

FLAC__StreamDecoderReadStatus flac_decoder::read_callback_static(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data)
{
	return reinterpret_cast<flac_decoder *>(client_data)->read_callback(buffer, bytes);
}

FLAC__StreamDecoderReadStatus flac_decoder::read_callback(FLAC__byte buffer[], size_t *bytes)
{
	UINT32 expected = *bytes;

	// if a file, just read
	if (m_file != NULL)
		*bytes = core_fread(m_file, buffer, expected);

	// otherwise, copy from memory
	else
	{
		// copy from primary buffer first
		UINT32 outputpos = 0;
		if (outputpos < *bytes && m_compressed_offset < m_compressed_length)
		{
			UINT32 bytes_to_copy = MIN(*bytes - outputpos, m_compressed_length - m_compressed_offset);
			memcpy(&buffer[outputpos], m_compressed_start + m_compressed_offset, bytes_to_copy);
			outputpos += bytes_to_copy;
			m_compressed_offset += bytes_to_copy;
		}

		// once we're out of that, copy from the secondary buffer
		if (outputpos < *bytes && m_compressed_offset < m_compressed_length + m_compressed2_length)
		{
			UINT32 bytes_to_copy = MIN(*bytes - outputpos, m_compressed2_length - (m_compressed_offset - m_compressed_length));
			memcpy(&buffer[outputpos], m_compressed2_start + m_compressed_offset - m_compressed_length, bytes_to_copy);
			outputpos += bytes_to_copy;
			m_compressed_offset += bytes_to_copy;
		}
		*bytes = outputpos;
	}

	// return based on whether we ran out of data
	return (*bytes < expected) ? FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM : FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
}


//-------------------------------------------------
//  metadata_callback - handle STREAMINFO metadata
//-------------------------------------------------

void flac_decoder::metadata_callback_static(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
	// ignore all but STREAMINFO metadata
	if (metadata->type != FLAC__METADATA_TYPE_STREAMINFO)
		return;

	// parse out the data we care about
	flac_decoder *fldecoder = reinterpret_cast<flac_decoder *>(client_data);
	fldecoder->m_sample_rate = metadata->data.stream_info.sample_rate;
	fldecoder->m_bits_per_sample = metadata->data.stream_info.bits_per_sample;
	fldecoder->m_channels = metadata->data.stream_info.channels;
}


//-------------------------------------------------
//  tell_callback - handle requests to find out
//  where in the input stream we are
//-------------------------------------------------

FLAC__StreamDecoderTellStatus flac_decoder::tell_callback_static(const FLAC__StreamDecoder *decoder, FLAC__uint64 *absolute_byte_offset, void *client_data)
{
	*absolute_byte_offset = reinterpret_cast<flac_decoder *>(client_data)->m_compressed_offset;
	return FLAC__STREAM_DECODER_TELL_STATUS_OK;
}


//-------------------------------------------------
//  write_callback - handle writes to the output
//  stream
//-------------------------------------------------

FLAC__StreamDecoderWriteStatus flac_decoder::write_callback_static(const FLAC__StreamDecoder *decoder, const ::FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
	return reinterpret_cast<flac_decoder *>(client_data)->write_callback(frame, buffer);
}

FLAC__StreamDecoderWriteStatus flac_decoder::write_callback(const ::FLAC__Frame *frame, const FLAC__int32 * const buffer[])
{
	assert(frame->header.channels == channels());

	// interleaved case
	int shift = m_uncompressed_swap ? 8 : 0;
	int blocksize = frame->header.blocksize;
	if (m_uncompressed_start[1] == NULL)
	{
		INT16 *dest = m_uncompressed_start[0] + m_uncompressed_offset * frame->header.channels;
		for (int sampnum = 0; sampnum < blocksize && m_uncompressed_offset < m_uncompressed_length; sampnum++, m_uncompressed_offset++)
			for (int chan = 0; chan < frame->header.channels; chan++)
				*dest++ = INT16((UINT16(buffer[chan][sampnum]) << shift) | (UINT16(buffer[chan][sampnum]) >> shift));
	}

	// non-interleaved case
	else
	{
		for (int sampnum = 0; sampnum < blocksize && m_uncompressed_offset < m_uncompressed_length; sampnum++, m_uncompressed_offset++)
			for (int chan = 0; chan < frame->header.channels; chan++)
				if (m_uncompressed_start[chan] != NULL)
					m_uncompressed_start[chan][m_uncompressed_offset] = INT16((UINT16(buffer[chan][sampnum]) << shift) | (UINT16(buffer[chan][sampnum]) >> shift));
	}
	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}


//-------------------------------------------------
//  error_callback - handle errors (ignore them)
//-------------------------------------------------

void flac_decoder::error_callback_static(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
}
