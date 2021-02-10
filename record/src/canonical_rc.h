#ifndef RCRD_CRC_H
#define RCRD_CRC_H

#include <R.h>
#include <Rinternals.h>

#ifdef __cplusplus
extern "C" {
#endif

class CanonicalRC {
public:
	SEXP test;
	CanonicalRC(SEXP r_value);
	~CanonicalRC();
};

#ifdef __cplusplus
}
#endif

#endif // RCRD_CRC_H
