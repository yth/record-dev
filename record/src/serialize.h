#include <Rinternals.h>

#ifndef RCRD_SERIALIZE_H
#define RCRD_SERIALIZE_H

void InitOutPStream(R_outpstream_t stream, R_pstream_data_t data,
					void (*outchar)(R_outpstream_t, int),
					void (*outbytes)(R_outpstream_t, void *, int),
					SEXP (*phook)(SEXP, SEXP), SEXP pdata);

SEXP serialize(SEXP object, R_outpstream_t stream);

#endif // RCRD_SERIALIZE_H
