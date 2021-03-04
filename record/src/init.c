#define R_NO_REMAP


#include <R.h>
#include <R_ext/Rdynload.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL // Based on evil suggestion


#include "hello.h"
#include "record.h"


static const R_CallMethodDef callMethods[] = {
	//name, casted pointer to function, # of arg?
	{"hello", (DL_FUNC) &hello, 1},
	{"open_db", (DL_FUNC) &open_db, 1},
	{"close_db", (DL_FUNC) &close_db, 0},
	{"add_value", (DL_FUNC) &add_value, 1},
	{"has_value", (DL_FUNC) &has_value, 1},
	{"get_random_value", (DL_FUNC) &get_random_value, 0},
	{NULL, NULL, 0} // Required?
};

void R_init_record(DllInfo* dll) {
	R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
}
