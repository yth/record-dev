#define R_NO_REMAP


#include <R.h>
#include <R_ext/Rdynload.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL // Based on evil suggestion


#include "hello.h"


static const R_CallMethodDef callMethods[] = {
	//name, casted pointer to function, # of arg?
	{"hello", (DL_FUNC) &hello, 1},
	{NULL, NULL, 0} // Required?
};

void R_init_record(DllInfo* dll) {
    R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
}
