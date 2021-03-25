#define R_NO_REMAP


#include <R.h>
#include <R_ext/Rdynload.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL // Based on evil suggestion


#include "hello.h"
#include "record.h"


static const R_CallMethodDef callMethods[] = {
	//name, casted pointer to function, # of arg?
	{"hello", 			(DL_FUNC) &hello, 1},
	{"open_db", 		(DL_FUNC) &open_db, 1}, // TODO: Deprecate this
	{"close_db", 		(DL_FUNC) &close_db, 0},
	{"create_gbov", 	(DL_FUNC) &create_gbov, 1},
	{"create_indices", 	(DL_FUNC) &create_indices, 1},
	{"create_ints", 	(DL_FUNC) &create_ints, 1},
	{"load_gbov", 		(DL_FUNC) &load_gbov, 1},
	{"load_indices", 	(DL_FUNC) &load_indices, 1},
	{"load_ints", 		(DL_FUNC) &load_ints, 1},
	{"add_val", 		(DL_FUNC) &add_val, 1},
	{"add_int", 		(DL_FUNC) &add_int, 1},
	{"have_seen", 		(DL_FUNC) &have_seen, 1},
	{"have_seen_int", 	(DL_FUNC) &have_seen_int, 1},
	{"size_db", 		(DL_FUNC) &size_db, 0},
	{"size_ints", 		(DL_FUNC) &size_ints, 0},
	{"count_vals", 		(DL_FUNC) &count_vals, 0},
	{"get_random_val", 	(DL_FUNC) &get_random_val, 0},
	{"get_vals", 		(DL_FUNC) &read_vals, 2}, // TODO: Implement this
	{NULL, 				NULL, 0} // Required?
};

void R_init_record(DllInfo* dll) {
	R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
}
