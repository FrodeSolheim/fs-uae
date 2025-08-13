// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/***************************************************************************

	flac.h

	FLAC compression wrappers

***************************************************************************/

#ifndef MAME_UTIL_FLAC_H
#define MAME_UTIL_FLAC_H

#pragma once

#include "utilfwd.h"

#include <FLAC/all.h>

#include <cstdint>


//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> flac_encoder

class flac_encoder
{
public:
	// construction/destruction
	flac_encoder();
	flac_encoder(void* buffer, uint32_t buflength);
	flac_encoder(util::random_write& file);
	~flac_encoder();

	// configuration
	void set_sample_rate(uint32_t sample_rate) { m_sample_rate = sample_rate; }
	void set_num_channels(uint8_t num_channels) { m_channels = num_channels; }
	void set_block_size(uint32_t block_size) { m_block_size = block_size; }
	void set_strip_metadata(bool strip) { m_strip_metadata = strip; }

	// getters (valid after reset)
	FLAC__StreamEncoderState state() const { return FLAC__stream_encoder_get_state(m_encoder); }
	const char* state_string() const { return FLAC__stream_encoder_get_resolved_state_string(m_encoder); }

	// reset
	bool reset();
	bool reset(void* buffer, uint32_t buflength);
	bool reset(util::random_write& file);

	// encode a buffer
	bool encode_interleaved(const int16_t* samples, uint32_t samples_per_channel, bool swap_endian = false);
	bool encode(int16_t* const* samples, uint32_t samples_per_channel, bool swap_endian = false);

	// finish up
	uint32_t finish();

private:
	// internal helpers
	void init_common();
	static FLAC__StreamEncoderWriteStatus write_callback_static(const FLAC__StreamEncoder* encoder, const FLAC__byte buffer[], size_t bytes, unsigned samples, unsigned current_frame, void* client_data);
	FLAC__StreamEncoderWriteStatus write_callback(const FLAC__byte buffer[], size_t bytes, unsigned samples, unsigned current_frame);

	// internal state
	FLAC__StreamEncoder* m_encoder;              // actual encoder
	util::random_write* m_file;                 // output file
	uint32_t                m_compressed_offset;    // current offset with the compressed stream
	FLAC__byte* m_compressed_start;     // start of compressed data
	uint32_t                m_compressed_length;    // length of the compressed stream

	// parameters
	uint32_t                m_sample_rate;          // sample rate
	uint8_t                 m_channels;             // number of channels
	uint32_t                m_block_size;           // block size

	// header stripping
	bool                    m_strip_metadata;       // strip the metadata?
	uint32_t                m_ignore_bytes;         // how many bytes to ignore when writing
	bool                    m_found_audio;          // have we hit the audio yet?
};


// ======================> flac_decoder

class flac_decoder
{
public:
	// construction/destruction
	flac_decoder();
	flac_decoder(const void* buffer, uint32_t length, const void* buffer2 = nullptr, uint32_t length2 = 0);
	flac_decoder(util::read_stream& file);
	~flac_decoder();

	// getters (valid after reset)
	uint32_t sample_rate() const { return m_sample_rate; }
	uint8_t channels() const { return m_channels; }
	uint8_t bits_per_sample() const { return m_bits_per_sample; }
	uint32_t total_samples() const { return FLAC__stream_decoder_get_total_samples(m_decoder); }
	FLAC__StreamDecoderState state() const { return FLAC__stream_decoder_get_state(m_decoder); }
	const char* state_string() const { return FLAC__stream_decoder_get_resolved_state_string(m_decoder); }

	// reset
	bool reset();
	bool reset(const void* buffer, uint32_t length, const void* buffer2 = nullptr, uint32_t length2 = 0);
	bool reset(uint32_t sample_rate, uint8_t num_channels, uint32_t block_size, const void* buffer, uint32_t length);
	bool reset(util::read_stream& file);

	// decode to a buffer; num_samples must be a multiple of the block size
	bool decode_interleaved(int16_t* samples, uint32_t num_samples, bool swap_endian = false);
	bool decode(int16_t** samples, uint32_t num_samples, bool swap_endian = false);

	// finish up
	uint32_t finish();

private:
	// internal helpers
	static FLAC__StreamDecoderReadStatus read_callback_static(const FLAC__StreamDecoder* decoder, FLAC__byte buffer[], size_t* bytes, void* client_data);
	FLAC__StreamDecoderReadStatus read_callback(FLAC__byte buffer[], size_t* bytes);
	static void metadata_callback_static(const FLAC__StreamDecoder* decoder, const FLAC__StreamMetadata* metadata, void* client_data);
	static FLAC__StreamDecoderTellStatus tell_callback_static(const FLAC__StreamDecoder* decoder, FLAC__uint64* absolute_byte_offset, void* client_data);
	static FLAC__StreamDecoderWriteStatus write_callback_static(const FLAC__StreamDecoder* decoder, const ::FLAC__Frame* frame, const FLAC__int32* const buffer[], void* client_data);
	FLAC__StreamDecoderWriteStatus write_callback(const ::FLAC__Frame* frame, const FLAC__int32* const buffer[]);
	static void error_callback_static(const FLAC__StreamDecoder* decoder, FLAC__StreamDecoderErrorStatus status, void* client_data);

	// output state
	FLAC__StreamDecoder* m_decoder;              // actual decoder
	util::read_stream* m_file;                 // input file
	uint32_t                m_sample_rate;          // decoded sample rate
	uint8_t                 m_channels;             // decoded number of channels
	uint8_t                 m_bits_per_sample;      // decoded bits per sample
	uint32_t                m_compressed_offset;    // current offset in compressed data
	const FLAC__byte* m_compressed_start;     // start of compressed data
	uint32_t                m_compressed_length;    // length of compressed data
	const FLAC__byte* m_compressed2_start;    // start of compressed data
	uint32_t                m_compressed2_length;   // length of compressed data
	int16_t* m_uncompressed_start[8];// pointer to start of uncompressed data (up to 8 streams)
	uint32_t                m_uncompressed_offset;  // current position in uncompressed data
	uint32_t                m_uncompressed_length;  // length of uncompressed data
	bool                    m_uncompressed_swap;    // swap uncompressed sample data
	uint8_t                 m_custom_header[0x2a];  // custom header
};

#endif // MAME_UTIL_FLAC_H
