#define R_NO_REMAP


#include <R.h>
#include <R_ext/Rdynload.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL // Based on evil suggestion


#include "hello.h"
#include "record.h"


static const R_CallMethodDef callMethods[] = {
	//name, casted pointer to function, # of arg?
	{"hello", 			(DL_FUNC) &hello,			1},
	{"setup",			(DL_FUNC) &setup,			0},
	{"teardown",		(DL_FUNC) &teardown,		0},
	{"create_indices", 	(DL_FUNC) &create_indices, 	1},
	{"load_indices", 	(DL_FUNC) &load_indices,	1},
	{"close_indices",	(DL_FUNC) &close_indices,	0},
	{"create_gbov", 	(DL_FUNC) &create_gbov,		1},
	{"load_gbov", 		(DL_FUNC) &load_gbov,		1},
	{"close_gbov",		(DL_FUNC) &close_gbov,		0},
	{"create_ints", 	(DL_FUNC) &create_ints,		1},
	{"load_ints", 		(DL_FUNC) &load_ints,		1},
	{"close_ints",		(DL_FUNC) &close_ints,		0},
	{"create_stats", 	(DL_FUNC) &create_stats,	1},
	{"load_stats", 		(DL_FUNC) &load_stats,		1},
	{"close_stats",		(DL_FUNC) &close_stats,		0},
	{"add_val", 		(DL_FUNC) &add_val,			1},
	{"add_int", 		(DL_FUNC) &add_int,			1},
	{"have_seen", 		(DL_FUNC) &have_seen,		1},
	{"have_seen_int", 	(DL_FUNC) &have_seen_int,	1},
	{"size_db", 		(DL_FUNC) &size_db,			0},
	{"size_ints", 		(DL_FUNC) &size_ints,		0},
	{"count_vals", 		(DL_FUNC) &count_vals,		0},
	{"sample_val", 		(DL_FUNC) &sample_val,		0},
	{"get_vals", 		(DL_FUNC) &read_vals,		2}, // TODO: Implement this
	{"report",			(DL_FUNC) &report,			0},
	{NULL, 				NULL,						0}
};

void R_init_record(DllInfo* dll) {
	R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
}
