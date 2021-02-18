#include "serialize.h"


#include <Rinternals.h>


#include "byte_vector.h"


// void InitOutPStream(R_outpstream_t stream, R_pstream_data_t data,
// 					SEXP (*phook)(SEXP, SEXP), SEXP pdata) {
// 	stream->data = data;
// 	stream->type = R_pstream_binary_format;
// 	stream->version = 3;
// 	stream->OutChar = outchar;
// 	stream->OutBytes = outbytes;
// 	stream->OutPersistHookFunc = NULL;
// 	stream->OutPersistHookData = pdata;
// }
