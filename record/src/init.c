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
	{"add_val", (DL_FUNC) &add_val, 1},
	{"has_seen", (DL_FUNC) &has_seen, 1},
	{"count_vals", (DL_FUNC) &count_vals, 0},
	{"size_db", (DL_FUNC) &size_db, 0},
	{"get_vals", (DL_FUNC) &read_vals, 2},
	{"get_random_val", (DL_FUNC) &get_random_val, 0},
	{"load_gbov", (DL_FUNC) &load_gbov, 1},
	{"load_indices", (DL_FUNC) &load_indices, 1},
	{"create_gbov", (DL_FUNC) &create_gbov, 1},
	{"create_indices", (DL_FUNC) &create_indices, 1},
	{NULL, NULL, 0} // Required?
};

void R_init_record(DllInfo* dll) {
	R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
}
