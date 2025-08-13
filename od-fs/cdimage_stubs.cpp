#include "sysconfig.h"
#include "sysdeps.h"

#include "mp3decoder.h"
#include "FLAC/stream_decoder.h"
#include "cda_play.h"

#if 0

FLAC_API FLAC__StreamDecoder *FLAC__stream_decoder_new(void) {
    return NULL;
}

FLAC_API FLAC__bool FLAC__stream_decoder_set_md5_checking(
        FLAC__StreamDecoder *decoder, FLAC__bool value) {
    return 0;
}

FLAC_API FLAC__bool FLAC__stream_decoder_set_metadata_respond(FLAC__StreamDecoder *decoder, FLAC__MetadataType type)
{
    return 0;
}

FLAC_API FLAC__StreamDecoderInitStatus FLAC__stream_decoder_init_stream(
    FLAC__StreamDecoder *decoder,
    FLAC__StreamDecoderReadCallback read_callback,
    FLAC__StreamDecoderSeekCallback seek_callback,
    FLAC__StreamDecoderTellCallback tell_callback,
    FLAC__StreamDecoderLengthCallback length_callback,
    FLAC__StreamDecoderEofCallback eof_callback,
    FLAC__StreamDecoderWriteCallback write_callback,
    FLAC__StreamDecoderMetadataCallback metadata_callback,
    FLAC__StreamDecoderErrorCallback error_callback,
    void *client_data) {
    return FLAC__STREAM_DECODER_INIT_STATUS_UNSUPPORTED_CONTAINER;
}

FLAC_API FLAC__bool FLAC__stream_decoder_process_until_end_of_metadata(FLAC__StreamDecoder *decoder) {
    return 0;
}

FLAC_API FLAC__bool FLAC__stream_decoder_process_until_end_of_stream(FLAC__StreamDecoder *decoder) {
    return 0;
}

FLAC_API void FLAC__stream_decoder_delete(FLAC__StreamDecoder *decoder) {
}

#endif

#if 0

mp3decoder::~mp3decoder() {
}

mp3decoder::mp3decoder() {
}

uae_u8 *mp3decoder::get (struct zfile *zf, uae_u8 *outbuf, int maxsize) {
    return NULL;
}

uae_u32 mp3decoder::getsize (struct zfile *zf) {
    return 0;
}

#endif