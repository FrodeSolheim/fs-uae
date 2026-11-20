/*
lzhlib - lzh library modules for lhafile

Copyright (c) 2010 Hidekazu Ohnishi.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of the author nor the names of its contributors
      may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Python.h"
#include "structmember.h"

#if PY_MAJOR_VERSION >= 3
#define IS_PY3K
#endif

static char __author__[] =
"The lzhlib python module was written by:\n\
\n\
    Hidekazu Ohnishi\n\
";

#ifdef MS_WINDOWS
#define inline __inline
#endif

typedef long long Py_off_t;

#define FILE_BUFFER_SIZE (64*1024)

/* ===================================================================== */
/* Constant definitions. */

typedef enum {
    COMPRESS_TYPE_LH0 = 1,
    COMPRESS_TYPE_LH5,
    COMPRESS_TYPE_LH6,
    COMPRESS_TYPE_LH7,
} lzhlib_compress_type;

typedef enum {
    ERR_UNEXPECT_EOF = 1,
    ERR_OUT_OF_RANGE,
    ERR_VALUE_ERROR,
    ERR_OUT_OF_MEMORY,
    ERR_IO_ERROR,
    ERR_BIT_LENGTH_TABLE_ERROR,
    ERR_BIT_PATTERN_TABLE_ERROR,
    ERR_BIT_LENGTH_SIZE_ERROR,
    ERR_DATA_ERROR,
    ERR_BUFFER_OVER_FLOW,
} lzhlib_error;

const char *lzhlib_error_msg[] = {
    "Unexpehct EOF. It is seemed this file is broken",
    "It is seemd the compressed data is too short",
    "Input argument error",
    "Out of mwmoey",
    "I/O error is happend",
    "Lzh file is seemed broken",
    "Lzh file is seemed broken",
    "Lzh file is seemed broken",
    "Can't write file",
    "Buffer overflow will happened",
};

typedef enum {
    BIT_STREAM_ERR_OVERFLOW = 0x01,
    BIT_STREAM_ERR_IOERROR = 0x02,
} bit_stream_err_type;


/* ===================================================================== */
/* Structure definitions. */

typedef struct {
    PyObject *fp;
    PyObject *read_buf;
    unsigned char *buf, *end;
    unsigned int cache;
    int bit;
    int remain_bit;
    Py_off_t pos;
    int eof;
} bit_stream_reader;

typedef struct {
    PyObject *fp;
    PyObject *write_buf;
    unsigned char *start,*buf,*end;
    Py_off_t pos;
    int crc16;
    bit_stream_err_type error;
} bit_stream_writer;

typedef struct {
    int len;
    unsigned char *s;
} string;

typedef struct{
    string *table;
    int bitMax;
} bit_length_table;

typedef struct{
    int table[510];
    int len;
    int _freqTable[17];
    int _weight[17];
    int _startPattern[17];
} bit_pattern_table;

typedef struct{
    int bitMax;
    int bitlength;
    unsigned short blen_code[65536];
    bit_length_table _blt;
    bit_pattern_table _bpt;
} huffman_decoder;



/* ===================================================================== */
/* crc16 */

const static int _crc16Table[256] =
{
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040,
};

static inline int
crc16(unsigned char *data, int len, int crc){
    for(; len > 0 ; data++, len--){
    crc = (crc >> 8) ^ _crc16Table[(crc ^ *data) & 0xFF];
    }

    return crc;
}

/* ===================================================================== */
/* lzh decode */

/* STRING OPERATIONS */

static inline void
string_init(string *self, unsigned char*s, int len)
{
    self->s = s;
    self->len = len;
}

static inline int
string_len(string *self){
    return self->len;
}

static inline unsigned char *
string_str(string *self){
    return self->s;
}

static inline int
string_get(string *self, int i){
    return (int)(self->s[i]);
}

static inline void
string_set(string *self, int i, int c){
    self->s[i] = (unsigned char)c;
}

static inline void
string_clear(string *self){
    int i;
    for(i = 0; i < self->len; i++){
        self->s[i] = 0;
    }
}

static inline string*
new_string(int len){
    string *s;
    unsigned char *str;

    str = PyMem_Malloc(len * sizeof(unsigned char));
    if(str == NULL){
        return NULL;
    }

    s = PyMem_Malloc(sizeof(string));
    if(s == NULL){
        PyMem_Free(str);
        return NULL;
    }

    string_init(s, str, len);

    return s;
}

static inline void
del_string(string *s){
    if(s != NULL){
        if(s->s != NULL){
            PyMem_Free(s->s);
        }
        PyMem_Free(s);
    }
}


/* BITSTREAM OPERATIONS */

static inline int
bit_stream_reader_init_fileio(bit_stream_reader *self, PyObject *file)
{
    int error_no = 0;
    int i;
    unsigned char *buf, *end;
    unsigned int cache;
    PyObject *read_obj = NULL;

    /* Argument check */
    if(!file){ error_no = ERR_VALUE_ERROR; goto error;}

    /* Read ahead data */
    read_obj = PyObject_CallMethod(file, "read", "(i)", FILE_BUFFER_SIZE);
    if(!read_obj){ error_no = ERR_VALUE_ERROR; goto error;}

    self->fp = file;
    self->read_buf = read_obj;
    self->bit = 0;
    self->pos = 0;

#ifdef IS_PY3K
    buf = (unsigned char*)PyBytes_AsString(read_obj);
    end = buf + PyBytes_Size(read_obj);
#else
    buf = (unsigned char*)PyString_AsString(read_obj);
    end = buf + PyString_Size(read_obj);
#endif

    /* Fill Cache */
    cache = 0;
    self->remain_bit = 0;
    for(i=0; i < sizeof(unsigned int) && buf != end; i++){
        cache = (cache << 8) | *buf++;
        self->remain_bit += 8;
    }

    self->buf = buf;
    self->end = end;
    self->cache = cache;
    if(buf == end){
        self->eof = 1;
        self->cache <<= (8 * sizeof(unsigned int) - self->remain_bit);
    }else{
        self->remain_bit = 0;
        self->eof = 0;
    }

    return 0;

error:
    Py_XDECREF(read_obj);

    return error_no;
}


static inline void
bit_stream_reader_close(bit_stream_reader *self)
{
    Py_XDECREF(self->read_buf);
    self->read_buf = NULL;
}

static inline Py_off_t
bit_stream_reader_pos(bit_stream_reader *self)
{
    return self->pos;
}

static inline int
bit_stream_reader_pre_fetch(bit_stream_reader *self, int n)
{
    return (int)(self->cache >> (8 * sizeof(int) - n));
}

static inline int
bit_stream_reader_fetch(bit_stream_reader *self, int n)
{
    int ret;

    if(n > 16 || n <= 0){
        if(n == 0){
            return 0;
        }
        return -2;
    }

    ret = (int)(self->cache >> (8 * sizeof(int) - n));
    self->cache <<= n;
    self->bit += n;

    if(self->eof){
        if(self->bit > self->remain_bit){
            return -1;
        }
    }else if(sizeof(unsigned int) * 8 - self->bit <= 16){
        self->cache >>= self->bit;

        /* if remain data cahce size is under 16 then read ahead */
        while(sizeof(unsigned int) * 8 - self->bit <= 16){
            if(self->buf == self->end){
                PyGILState_STATE state;
                PyObject *read_obj = NULL;

                state = PyGILState_Ensure();

                /* free old buffer */
                Py_DECREF(self->read_buf);
                self->read_buf = NULL;

                /* read ahead data*/
                read_obj = PyObject_CallMethod(self->fp, "read", "(i)", FILE_BUFFER_SIZE);
                if(!read_obj){
                    ret = ERR_VALUE_ERROR;
                    goto error;
                }

#ifdef IS_PY3K
                self->buf = (unsigned char*)PyBytes_AsString(read_obj);
                self->end = self->buf + PyBytes_Size(read_obj);
#else
                self->buf = (unsigned char*)PyString_AsString(read_obj);
                self->end = self->buf + PyString_Size(read_obj);
#endif

                if(self->buf != self->end){
                    self->read_buf = read_obj;
                }else{
                    /* this condition means eof */
                    self->eof = 1;
                    self->remain_bit = sizeof(unsigned int) * 8;
                    Py_DECREF(read_obj);
                    PyGILState_Release(state);
                    break;
                }

                PyGILState_Release(state);

            }
            self->cache <<= 8;
            self->cache |= *self->buf++;
            self->bit -= 8;
            self->pos += 1;
        }

        self->cache <<= self->bit;
    }

error:
    return ret;
}


static inline bit_stream_reader *
new_bit_stream_reader(void)
{
    return PyMem_Malloc(sizeof(bit_stream_reader));
}


/*  */

static inline int
bit_stream_writer_init_fileio(bit_stream_writer *self, PyObject *file)
{
    int error_no = 0;

    PyObject *write_obj = NULL;
    unsigned char *buf, *end;

    /* Argument check */
    if(!file){ error_no = ERR_VALUE_ERROR; goto error;}

    /* Allocate write buffer */
#ifdef IS_PY3K
    write_obj = PyBytes_FromStringAndSize(NULL, 65556);
#else
    write_obj = PyString_FromStringAndSize(NULL, 65556);
#endif
    if(!write_obj){ error_no = ERR_OUT_OF_MEMORY; goto error;}

    self->fp = file;
    self->write_buf = write_obj;
    self->crc16 = 0;
    self->pos = 0;

#ifdef IS_PY3K
    buf = (unsigned char*)PyBytes_AsString(write_obj);
    end = buf + PyBytes_Size(write_obj);
#else
    buf = (unsigned char*)PyString_AsString(write_obj);
    end = buf + PyString_Size(write_obj);
#endif

    self->start = buf;
    self->buf = buf;
    self->end = end;

    self->error = 0;

    return 0;

error:
    Py_XDECREF(write_obj);

    return error_no;
}

static inline int
bit_stream_writer_flush(bit_stream_writer *self)
{
    int error_no = 0;
    PyObject *ret = NULL, *write_obj = NULL;
    int s;

    if(self->write_buf){
      s = (int)(self->buf - self->start);

        if(s > 0){
            self->crc16 = crc16(self->start, s, self->crc16);
#ifdef IS_PY3K
            write_obj = PyBytes_FromStringAndSize(PyBytes_AsString(self->write_buf), s);
#else
            write_obj = PyString_FromStringAndSize(PyString_AsString(self->write_buf), s);
#endif
            if(!write_obj){ error_no = ERR_OUT_OF_MEMORY; goto error; }

            ret = PyObject_CallMethod(self->fp, "write", "(O)", write_obj);

            Py_DECREF(write_obj);
            Py_DECREF(ret);

            ret = PyErr_Occurred();
            if(ret){
                PyErr_Clear();
                error_no = ERR_IO_ERROR;
                goto error;
            }
        }

        self->buf = self->start;
    }

error:

    return error_no;
}

static inline int
bit_stream_writer_close(bit_stream_writer *self)
{
    int error_no = 0;

    error_no = bit_stream_writer_flush(self);

    Py_XDECREF(self->write_buf);
    self->write_buf = NULL;

    return error_no;
}


static inline int
bit_stream_writer_overflow(bit_stream_writer *self)
{
    return ((self->error & BIT_STREAM_ERR_OVERFLOW) != 0);
}

static inline int
bit_stream_writer_ioerror(bit_stream_writer *self)
{
    return ((self->error & BIT_STREAM_ERR_IOERROR) != 0);
}

static inline Py_off_t
bit_stream_writer_pos(bit_stream_writer *self)
{
    return self->pos;
}

static inline int
bit_stream_writer_crc(bit_stream_writer *self)
{
    return self->crc16;
}

static inline void
bit_stream_writer_write(bit_stream_writer *self, int c)
{
    self->pos++;
    *self->buf++ = (unsigned char)c;

    if(self->buf == self->end){
        int s;

        s = (int)(self->buf - self->start);
        self->crc16 = crc16(self->start, s, self->crc16);

        {
            PyObject *ret;
            PyGILState_STATE state;

            state = PyGILState_Ensure();

            ret = PyObject_CallMethod(self->fp, "write", "(O)", self->write_buf);
            Py_DECREF(ret);

            ret = PyErr_Occurred();
            if(ret){
                self->error |= BIT_STREAM_ERR_OVERFLOW;
                PyErr_Clear();
            }

            PyGILState_Release(state);
        }

        self->buf = self->start;
    }
}


/* BIT LENGTH TABLE OPERATIONS */

static inline int
bit_length_table_init(bit_length_table *self, string *s)
{
    int error_no = 0;
    int bitMax = 0;
    int blen, i;

    for(i = 0; i < string_len(s); i++){
        blen = string_get(s, i);
        if(bitMax < blen){
            bitMax = blen;
        }
    }

    if( bitMax == 0 || bitMax > 16 || string_len(s) == 0){
        error_no = ERR_BIT_LENGTH_TABLE_ERROR;
        goto error;
    }

    self->table = s;
    self->bitMax = bitMax;

error:
    return error_no;
}

static inline string*
bit_length_table_table(bit_length_table *self){
    return self->table;
}

static inline int
bit_length_table_bitMax(bit_length_table *self){
    return self->bitMax;
}

static inline int
bit_length_table_table_num(bit_length_table *self, int i){
    return string_get(self->table, i);
}


/* BIT PATTERN TABLE OPERATIONS */

static inline int
bit_pattern_table_init(bit_pattern_table *self, bit_length_table *blt)
{
    int error_no = 0;
    int i, ptn, w, bl;
    int bitMax, tableMax;

    int *table = self->table;
    int *freqTable = self->_freqTable;
    int *weight = self->_weight;
    int *startPattern = self->_startPattern;

    bitMax = bit_length_table_bitMax(blt);
    tableMax = string_len(bit_length_table_table(blt));

    memset(freqTable,    0, sizeof(int) * (bitMax + 1));
    memset(weight,       0, sizeof(int) * (bitMax + 1));
    memset(startPattern, 0, sizeof(int) * (bitMax + 1));

    for(i = 0; i < string_len(bit_length_table_table(blt)); i++){
        bl = bit_length_table_table_num(blt, i);
        if(bl == 0){
            continue;
        }
        freqTable[bl] += 1;
    }

    ptn = 0;
    w = 1 << (bitMax - 1);
    for(i = 1; i <= bitMax ; i++){
        startPattern[i] = ptn;
        weight[i] = w;

        ptn += (w * freqTable[i]);
        w >>= 1;
    }

    if(ptn > (1 << bitMax)){
        error_no = ERR_BIT_PATTERN_TABLE_ERROR;
        goto error;
    }

    /* Make bit pattern table */
    for(i = 0 ; i < tableMax ; i++){
        bl = bit_length_table_table_num(blt, i);
        if(bl == 0){
            table[i] = 0;
            continue;
        }

        ptn = startPattern[bl];
        table[i] = ptn >> (bitMax - bl);
        startPattern[bl] += weight[bl];
    }

    self->len = tableMax;

error:
    return error_no;
}

static inline int
bit_pattern_table_table_num(bit_pattern_table *self, int i){
    return self->table[i];
}


/* HUFFMAN DECODER OPERATIONS */

static int
huffman_decoder_init(huffman_decoder *self, string *s)
{
    int error_no = 0;
    int i;
    int bitMax;
    int ptn, blen;
    unsigned short *blen_code = self->blen_code;

    bit_length_table *blt = &self->_blt;
    bit_pattern_table *bpt = &self->_bpt;

    error_no = bit_length_table_init(blt, s);
    if(error_no != 0){
        goto error;
    }

    error_no = bit_pattern_table_init(bpt, blt);
    if(error_no != 0){
        goto error;
    }

    bitMax = bit_length_table_bitMax(blt);

    memset(blen_code, 0, (sizeof(unsigned short) * (1 << (int)bitMax)));

    for(i = 0; i < string_len(bit_length_table_table(blt)); i++){
        blen = bit_length_table_table_num(blt, i);
        if(blen == 0){
            continue;
        }

        ptn = bit_pattern_table_table_num(bpt, i) << (bitMax - blen);

        blen_code[ptn] = (blen << 11) | i;

    }

    if(bitMax == 1){
        if(blen_code[1] == 0){
            blen_code[0] &= 0x1FF;
        }
    }

    blen = *blen_code++;
    for(i = 1; i < (1 << bitMax) ; i++, blen_code++){
        if(*blen_code == 0){
            *blen_code = blen;
        }else{
            blen = *blen_code;
        }
    }

    self->bitMax = bitMax;

error:
    return error_no;
}

static inline int
huffman_decoder_decode(huffman_decoder *self, bit_stream_reader *bs)
{
    int bits, blen, code;

    bits = bit_stream_reader_pre_fetch(bs, self->bitMax);
    blen = self->blen_code[bits] >> 11;
    code = self->blen_code[bits] & 0x1FF;
    bit_stream_reader_fetch(bs, blen);

    return code;
}

static inline huffman_decoder *
new_huffman_decoder(void){
    return (huffman_decoder *)PyMem_Malloc(sizeof(huffman_decoder));
}

static inline void
del_huffman_decoder(huffman_decoder *d){
    if(d != NULL){
        PyMem_Free(d);
    }
}

/* LZH DECODE OPERATIONS */

#define EOF_CHECK(c)                                                    \
    if(c < 0){                                                          \
        if(c == -1){                                                    \
            error_no = ERR_UNEXPECT_EOF;                                \
            goto error;                                                 \
        }else if(c == -2){                                              \
            error_no = ERR_OUT_OF_RANGE;                                \
            goto error;                                                 \
        }                                                               \
    }


static inline int
decodeUnary7(bit_stream_reader *bs, int *unary_code)
{
    int error_no = 0;
    int c, code;

    code = bit_stream_reader_fetch(bs, 3);
    EOF_CHECK(code);

    if(code == 7){
        while((c = bit_stream_reader_fetch(bs, 1)) == 1){
            code += 1;
        }
        EOF_CHECK(c);
    }

    *unary_code = code;

error:
    return error_no;
}


static inline int
decodeBitLengthDecoder(bit_stream_reader *bs, string *blenlen19)
{
    int error_no = 0;
    int i, c;
    int blenSize, blenLeafCode, nmax;


    blenSize = bit_stream_reader_fetch(bs, 5);
    EOF_CHECK(blenSize);
    if(blenSize > 19){
        error_no = ERR_BIT_LENGTH_SIZE_ERROR;
        goto error;
    }

    if(blenSize == 0){
        blenLeafCode = bit_stream_reader_fetch(bs, 5);
        EOF_CHECK(blenLeafCode);
        if(blenLeafCode >= 19){
            error_no = ERR_BIT_LENGTH_SIZE_ERROR;
            goto error;
        }
        string_clear(blenlen19);
        string_set(blenlen19, blenLeafCode, 1);
    }else{
        i = 0;

        while(i < blenSize){
            error_no = decodeUnary7(bs, &c);

            if(error_no != 0){
                goto error;
            }

            string_set(blenlen19, i, c);
            i += 1;

            if(i == 3){
                nmax = bit_stream_reader_fetch(bs, 2);
                EOF_CHECK(nmax);

                while(nmax > 0){
                    string_set(blenlen19, i, 0);
                    i += 1;
                    nmax -= 1;
                }
            }
        }

        while(i < 19){
            string_set(blenlen19, i, 0);
            i += 1;
        }
    }

error:
    return error_no;

}

static int
decodeBitLengthLiteral(bit_stream_reader *bs, string *blenlen510, huffman_decoder *bitlen_decoder)
{
    int error_no = 0;
    int i, n, code, c, leafCode;

    n = bit_stream_reader_fetch(bs, 9);
    EOF_CHECK(n);

    if(n == 0){
        leafCode = bit_stream_reader_fetch(bs, 9);
        EOF_CHECK(leafCode);

        string_clear(blenlen510);
        string_set(blenlen510, leafCode, 1);
    }else{
        i = 0;

        while(i < n){
            code = huffman_decoder_decode(bitlen_decoder, bs);

            if(code > 2){
                string_set(blenlen510, i, code - 2);
                i += 1;
                continue;
            }else if(code == 0){
                string_set(blenlen510, i, 0);
                i += 1;
                continue;
            }else if(code == 1){
                c = bit_stream_reader_fetch(bs, 4);
                EOF_CHECK(c);
                c += 3;
            }else if(code == 2){
                c = bit_stream_reader_fetch(bs, 9);
                EOF_CHECK(c);
                c += 20;
            }else{
                error_no = ERR_DATA_ERROR;
                goto error;
            }

            while(c > 0){
                c -= 1;
                string_set(blenlen510, i, 0);
                i += 1;
            }
        }

        while(i < 510){
            string_set(blenlen510, i, 0);
            i += 1;
        }
    }

error:
    return error_no;
}

static inline int
decodeBitLengthDistance(bit_stream_reader *bs, string *blenlen_distance, int dispos_bit, int dis_bit)
{
    int error_no = 0;
    int i, unary;
    int leafCode, tableSize;

    tableSize = bit_stream_reader_fetch(bs, dis_bit);
    EOF_CHECK(tableSize);

    if(tableSize == 0){
        leafCode = bit_stream_reader_fetch(bs, dis_bit);
        EOF_CHECK(leafCode);

        string_clear(blenlen_distance);
        string_set(blenlen_distance, leafCode, 1);
    }else{
        i = 0;

        while(i < tableSize){
            error_no = decodeUnary7(bs, &unary);
            if(error_no != 0){
                goto error;
            }
            string_set(blenlen_distance, i, unary);

            i += 1;
        }

        while(i <= dispos_bit){
            string_set(blenlen_distance, i, 0);
            i += 1;
        }
    }

error:
    return error_no;
}


/*
 *
 */

typedef struct {
    PyObject_HEAD
    /* */
    PyObject *fin;
    PyObject *fout;
    lzhlib_compress_type compress_type;
    Py_off_t info_compress_size;
    Py_off_t info_file_size;
    int      info_crc;


    bit_stream_reader *in;
    bit_stream_writer *out;

    huffman_decoder *bitlen_decoder;
    huffman_decoder *literal_decoder;
    huffman_decoder *distance_decoder;

    string *bitlen_distance;
    string *bitlen19;
    string *bitlen510;

    unsigned char *dic_buf;
    int dic_pos;
    int dic_size;
    int blockSize;

    int finish;
    int error_no;

    int dic_bit;
    int dispos_bit;
    int dis_bit;

    /* buffer instances */
    bit_stream_reader _in;
    bit_stream_writer _out;
    string _bitlen_distance;
    string _bitlen19;
    string _bitlen510;
    huffman_decoder _literal_decoder;
    huffman_decoder _distance_decoder;
    unsigned char _bitlen_distance_buf[18];
    unsigned char _bitlen19_buf[19];
    unsigned char _bitlen510_buf[510];
    unsigned char _dic_buf[65536];
} LZHDecodeSessionObject;


PyDoc_STRVAR(LZHDecodeSession_do_next__doc__,
"");

static PyObject *
LZHDecodeSession_do_next(LZHDecodeSessionObject *self)
{
    int error_no = 0;
    int loop, code, srcpos, mlen, bitl, dist;
    PyObject *ret;

    /* */
    if(self->error_no){
        goto exception;
    }

    if(self->finish){
        Py_INCREF(Py_True);
        return Py_True;
    }

    /* */
    Py_BEGIN_ALLOW_THREADS

    loop = 64*1024;

    if(self->compress_type == COMPRESS_TYPE_LH0){
        /* This code is slow, but this happens a little */
        while(loop > 0){
            code = bit_stream_reader_fetch(self->in, 8);
            if(code == -1){
                self->finish = 1;
                break;
            }
            bit_stream_writer_write(self->out, code);
            loop -= 1;
        }
    }else{
        while(loop > 0){
            if(self->blockSize <= 0){
                /* Delayed check for tuning */
                if(bit_stream_writer_overflow(self->out) != 0){
                    error_no = ERR_BUFFER_OVER_FLOW;
                    break;
                }

                if(bit_stream_writer_ioerror(self->out) != 0){
                    error_no = ERR_IO_ERROR;
                    break;
                }

                /* Read blockSize */
                self->blockSize = bit_stream_reader_fetch(self->in, 16);

                if(self->blockSize == -1){
                    self->finish = 1;
                    break;
                }else{
                    /* Create bitlen_decoder for literal_decoder */
                    error_no = decodeBitLengthDecoder(self->in, self->bitlen19);
                    if(error_no != 0){goto error;}

                    error_no = huffman_decoder_init(self->bitlen_decoder, self->bitlen19);
                    if(error_no != 0){goto error;}

                    /* Create literal decoder */
                    error_no = decodeBitLengthLiteral(self->in, self->bitlen510, self->bitlen_decoder);
                    if(error_no != 0){goto error;}

                    error_no = huffman_decoder_init(self->literal_decoder, self->bitlen510);
                    if(error_no != 0){goto error;}

                    /* Create distance decoder */
                    error_no = decodeBitLengthDistance(self->in, self->bitlen_distance, self->dispos_bit, self->dis_bit);
                    if(error_no != 0){goto error;}

                    error_no = huffman_decoder_init(self->distance_decoder, self->bitlen_distance);
                    if(error_no != 0){goto error;}

                }
            }

            code = huffman_decoder_decode(self->literal_decoder, self->in);
            self->blockSize -= 1;

            if(code < 256){
                self->dic_buf[self->dic_pos++] = code;
                bit_stream_writer_write(self->out, code);
                loop -=1;

                self->dic_pos &= (self->dic_size -1);
                continue;
            }

            mlen = code - 256 + 3;
            bitl = huffman_decoder_decode(self->distance_decoder, self->in);

            if(bitl == 0){
                dist = 1;
            }else{
                dist = bit_stream_reader_fetch(self->in, bitl - 1);
                EOF_CHECK(dist);
                dist += (1 << (bitl - 1));
                dist += 1;
            }

            srcpos = self->dic_pos - dist;
            if(srcpos < 0){
                srcpos += self->dic_size;
            }

            for(; mlen > 0 ; mlen--){
                code = self->dic_buf[self->dic_pos++] = self->dic_buf[srcpos++];
                bit_stream_writer_write(self->out, code);
                loop -= 1;

                self->dic_pos &= (self->dic_size -1);
                srcpos &= (self->dic_size -1);
            }
        }
    }

error:
    Py_END_ALLOW_THREADS

    if(error_no != 0){
        self->error_no = error_no;
        bit_stream_reader_close(self->in);
        bit_stream_writer_close(self->out);
        goto exception;
    }

    if(self->finish){
        bit_stream_reader_close(self->in);
        error_no = bit_stream_writer_close(self->out);
        if(error_no != 0){
            self->error_no = error_no;
            goto exception;
        }

        Py_INCREF(Py_True);
        ret = Py_True;
    }else{
        Py_INCREF(Py_False);
        ret = Py_False;
    }
    return ret;

exception:
    return PyErr_Format(PyExc_RuntimeError, "internal error code = %d", self->error_no);
}

static PyMethodDef LZHDecodeSession_methods[] = {
    {"do_next", (PyCFunction)LZHDecodeSession_do_next, METH_NOARGS,  LZHDecodeSession_do_next__doc__},
    {NULL, NULL}
};

static PyMemberDef LZHDecodeSession_members[] = {
    {"input_file_size",  T_LONGLONG, offsetof(LZHDecodeSessionObject, info_compress_size), READONLY},
    {"input_pos",        T_LONGLONG, offsetof(LZHDecodeSessionObject, _in) + offsetof(bit_stream_reader, pos), READONLY},
    {"output_file_size", T_LONGLONG, offsetof(LZHDecodeSessionObject, info_file_size), READONLY},
    {"output_pos",       T_LONGLONG, offsetof(LZHDecodeSessionObject, _out) + offsetof(bit_stream_writer, pos), READONLY},
    {"crc16",            T_INT,     offsetof(LZHDecodeSessionObject, _out) + offsetof(bit_stream_writer, crc16), READONLY},
    {NULL}
};


static long long
LhaInfo_GetAttr(PyObject *info, const char *attr_name){
    PyObject *attr, *value;
    long long num;

#ifdef IS_PY3K
    attr = PyUnicode_FromString(attr_name);
#else
    attr = PyString_FromString(attr_name);
#endif
    if(!attr){ goto error; }

    value = PyObject_GetAttr(info, attr);
    Py_DECREF(attr);
    if(!value){ goto error; }

#ifdef IS_PY3K
    if(PyLong_Check(value)){
        num = (Py_off_t)PyLong_AsLongLong(value);
#else
    if(PyInt_Check(value)){
        num = (Py_off_t)PyInt_AsLong(value);
    }else if(PyLong_Check(value)){
        num = (Py_off_t)PyLong_AsLongLong(value);
#endif
    }else{
        Py_DECREF(value);
        goto error;
    }
    Py_DECREF(value);

    return num;
error:
    return -1;
}


static int
LZHDecodeSession_init(LZHDecodeSessionObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *fin, *fout, *info, *value, *attr;
    int error_no;

    /* Initialize these so we can test them in dealloc if init fails */
    self->in = NULL;
    self->out = NULL;
    self->fin = NULL;
    self->fout = NULL;

    /* Parse arguments */
    if(!PyArg_ParseTuple(args, "OOO", &fin, &fout, &info)){
        goto error;
    }

    /* compress_type */
#ifdef IS_PY3K
    attr = PyUnicode_FromString("compress_type");
#else
    attr = PyString_FromString("compress_type");
#endif
    if(!attr){ goto error; }

    value = PyObject_GetAttr(info, attr);

    Py_DECREF(attr);
    if (!value) {
         PyErr_SetString(PyExc_RuntimeError, "Could not read compress_type");
         goto error;
    }

#ifdef IS_PY3K
    if(memcmp(PyBytes_AsString(value), "-lh0-\x00", 6) == 0){
#else
    if(memcmp(PyString_AsString(value), "-lh0-\x00", 6) == 0){
#endif
        self->compress_type = COMPRESS_TYPE_LH0;
        self->dic_size = 0;
#ifdef IS_PY3K
    }else if(memcmp(PyBytes_AsString(value), "-lh5-\x00", 6) == 0){
#else
    }else if(memcmp(PyString_AsString(value), "-lh5-\x00", 6) == 0){
#endif
        self->compress_type = COMPRESS_TYPE_LH5;
        self->dic_size = 8192;
        self->dic_bit = 13;
        self->dispos_bit = 14;
        self->dis_bit = 4;
#ifdef IS_PY3K
    }else if(memcmp(PyBytes_AsString(value), "-lh6-\x00", 6) == 0){
#else
    }else if(memcmp(PyBytes_AsString(value), "-lh6-\x00", 6) == 0){
#endif
        self->compress_type = COMPRESS_TYPE_LH6;
        self->dic_size = 32768;
        self->dic_bit = 15;
        self->dispos_bit = 16;
        self->dis_bit = 5;
#ifdef IS_PY3K
    }else if(memcmp(PyBytes_AsString(value), "-lh7-\x00", 6) == 0){
#else
    }else if(memcmp(PyString_AsString(value), "-lh7-\x00", 6) == 0){
#endif
        self->compress_type = COMPRESS_TYPE_LH7;
        self->dic_size = 65536;
        self->dic_bit = 16;
        self->dispos_bit = 17;
        self->dis_bit = 5;
    }else{
        PyErr_SetString(PyExc_RuntimeError, "Unsupported compress type");
        goto error;
    }
    Py_DECREF(value);

    /* Initialize each buffer and decoder */
    string_init(&self->_bitlen_distance, self->_bitlen_distance_buf, self->dispos_bit + 1);
    string_init(&self->_bitlen19, self->_bitlen19_buf, 19);
    string_init(&self->_bitlen510, self->_bitlen510_buf, 510);

    /* */
    self->finish = 0;
    self->error_no = 0;

    self->in = &self->_in;
    self->out = &self->_out;

    self->bitlen_distance = &self->_bitlen_distance;
    self->bitlen19 = &self->_bitlen19;
    self->bitlen510 = &self->_bitlen510;

    self->literal_decoder = &self->_literal_decoder;
    self->distance_decoder = &self->_distance_decoder;
    self->bitlen_decoder = &self->_distance_decoder;

    self->dic_buf = self->_dic_buf;
    self->dic_pos = 0;

    self->blockSize = 0;

    self->info_compress_size = (Py_off_t)LhaInfo_GetAttr(info, "compress_size");
    self->info_file_size     = (Py_off_t)LhaInfo_GetAttr(info, "file_size");
    self->info_crc           = (int)LhaInfo_GetAttr(info, "CRC");

    self->fin = fin;
    self->fout = fout;

    error_no = bit_stream_reader_init_fileio(self->in, self->fin);
    if(error_no != 0){
        PyErr_SetString(PyExc_RuntimeError, "bit_stream_reader_init_fileio");
        goto error;
    }

    error_no = bit_stream_writer_init_fileio(self->out, self->fout);
    if(error_no != 0){
        bit_stream_reader_close(self->in);
        PyErr_SetString(PyExc_RuntimeError, "bit_stream_writer_init_fileio");
        goto error;
    }

    Py_INCREF(self->fin);
    Py_INCREF(self->fout);

    return 0;

error:
    return -1;
}


static void
LZHDecodeSession_dealloc(LZHDecodeSessionObject *self)
{
    /* If decode is not finished */
    if(!self->finish && self->error_no == 0){
        if (self->in) {
            bit_stream_reader_close(self->in);
        }
        if (self->out) {
            bit_stream_writer_close(self->out);
        }
    }

    Py_XDECREF(self->fin);
    Py_XDECREF(self->fout);

    Py_TYPE(self)->tp_free((PyObject *)self);
}


static PyTypeObject LZHDecodeSession_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "lhafile.LZHDecodeSession",             /*tp_name*/
    sizeof(LZHDecodeSessionObject),     /*tp_basicsize*/
    0,                              /*tp_itemsize*/
    (destructor)LZHDecodeSession_dealloc,   /*tp_dealloc*/
    0,                                      /*tp_print*/
    0,                                      /*tp_getattr*/
    0,                                      /*tp_setattr*/
    0,                                      /*tp_compare*/
    0,                                      /*tp_repr*/
    0,                                      /*tp_as_number*/
    0,                                      /*tp_as_sequence*/
    0,                                      /*tp_as_mapping*/
    0,                                      /*tp_hash*/
    0,                                      /*tp_call*/
    0,                                      /*tp_str*/
    PyObject_GenericGetAttr,                /*tp_getattro*/
    PyObject_GenericSetAttr,                /*tp_setattro*/
    0,                                      /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE, /*tp_flags*/
    0,                                      /*tp_doc*/
    0,                                      /*tp_traverse*/
    0,                                      /*tp_clear*/
    0,                                      /*tp_richcompare*/
    0,                                      /*tp_weaklistoffset*/
    0,                                      /*tp_iter*/
    0,                                      /*tp_iternext*/
    LZHDecodeSession_methods,               /*tp_methods*/
    LZHDecodeSession_members,               /*tp_members*/
    0,                                      /*tp_getset*/
    0,                                      /*tp_base*/
    0,                                      /*tp_dict*/
    0,                                      /*tp_descr_get*/
    0,                                      /*tp_descr_set*/
    0,                                      /*tp_dictoffset*/
    (initproc)LZHDecodeSession_init,        /*tp_init*/
    PyType_GenericAlloc,                    /*tp_alloc*/
    PyType_GenericNew,                      /*tp_new*/
    PyObject_Del,                           /*tp_free*/
    0,                                      /*tp_is_gc*/
};


/*
 *
 */

static PyObject*
lzhlib_crc16(PyObject* self, PyObject* args)
{
    unsigned char *data;
    int len;
    int crc;

    crc = 0;
    if(!PyArg_ParseTuple(args, "s#|i", &data, &len, &crc)){
        return NULL;
    }

    crc = crc16(data, len, crc);

    return Py_BuildValue("i", (int)crc);
}

static PyMethodDef lzhlib_methods[] = {
    {"crc16", lzhlib_crc16, METH_VARARGS,
     "Execute crc16 function: (s, crc) -> crc"},
    { NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
    static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "lzhlib",                  /* m_name */
        "c extension for lhafile", /* m_doc */
        -1,                        /* m_size */
        lzhlib_methods,            /* m_methods */
        NULL,                      /* m_reload */
        NULL,                      /* m_traverse */
        NULL,                      /* m_clear */
        NULL,                      /* m_free */
    };
#endif

PyMODINIT_FUNC
#ifdef IS_PY3K
PyInit_lzhlib(void)
#else
initlzhlib(void)
#endif
{
    PyObject *m;

#ifdef IS_PY3K
    PyType_Ready(&LZHDecodeSession_Type);
#else
    LZHDecodeSession_Type.ob_type = &PyType_Type;
#endif

#ifdef IS_PY3K
    m = PyModule_Create(&moduledef);
#else
    m = Py_InitModule("lzhlib", lzhlib_methods);
#endif
    if (m == NULL)
#ifdef IS_PY3K
        return NULL;
#else
        return;
#endif

#ifdef IS_PY3K
    PyModule_AddObject(m, "__author__", PyUnicode_FromString(__author__));
#else
    PyModule_AddObject(m, "__author__", PyString_FromString(__author__));
#endif

    Py_INCREF(&LZHDecodeSession_Type);
    PyModule_AddObject(m, "LZHDecodeSession", (PyObject *)&LZHDecodeSession_Type);

#ifdef IS_PY3K
    return m;
#endif
}
