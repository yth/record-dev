#define R_NO_REMAP


#include <R.h>
#include <R_ext/Rdynload.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL // Based on evil suggestion


#include "hello.h"

#include "stats_store.h"
#include "generic_store.h"
#include "simple_int_store.h"
#include "simple_dbl_store.h"

#include "record.h"


static const R_CallMethodDef callMethods[] = {
	/* name						casted ptr to function			# of args */
	// Testing
	{"hello",					(DL_FUNC) &hello,					1},

	// Generic record related
	{"setup",					(DL_FUNC) &setup,					0},
	{"teardown",				(DL_FUNC) &teardown,				0},
	{"add_val",					(DL_FUNC) &add_val,					1},
	{"have_seen",				(DL_FUNC) &have_seen,				1},
	{"sample_val",				(DL_FUNC) &sample_val,				0},

	// stats store related
	{"init_stats_store",		(DL_FUNC) &init_stats_store,		1},
	{"load_stats_store",		(DL_FUNC) &load_stats_store,		1},
	{"close_stats_store",		(DL_FUNC) &close_stats_store,		0},
	{"print_report",			(DL_FUNC) &print_report,			0},
	{"size_db",					(DL_FUNC) &size_db,					0},
	{"size_ints",				(DL_FUNC) &size_ints,				0},
	{"count_vals",				(DL_FUNC) &count_vals,				0},

	// simple ints store related
	{"init_simple_int_store",	(DL_FUNC) &init_simple_int_store,	1},
	{"load_simple_int_store",	(DL_FUNC) &load_simple_int_store,	1},
	{"close_simple_int_store",	(DL_FUNC) &close_simple_int_store,	0},
	{"is_simple_int",			(DL_FUNC) &is_simple_int,			1},
	{"add_simple_int",			(DL_FUNC) &add_simple_int,			1},
	{"have_seen_simple_int",	(DL_FUNC) &have_seen_simple_int,	1},
	{"get_simple_int",			(DL_FUNC) &get_simple_int,			1},

	// simple doubles store related
	{"init_simple_dbl_store",	(DL_FUNC) &init_simple_dbl_store,	1},
	{"load_simple_dbl_store",	(DL_FUNC) &load_simple_dbl_store,	1},
	{"close_simple_dbl_store",	(DL_FUNC) &close_simple_dbl_store,	0},
	{"is_simple_dbl",			(DL_FUNC) &is_simple_dbl,			1},
	{"add_simple_dbl",			(DL_FUNC) &add_simple_dbl,			1},
	{"have_seen_simple_dbl",	(DL_FUNC) &have_seen_simple_dbl,	1},
	{"get_simple_dbl",			(DL_FUNC) &get_simple_dbl,			1},

	// generic R value store related
	{"create_indices",			(DL_FUNC) &create_indices, 			1},
	{"load_indices",			(DL_FUNC) &load_indices,			1},
	{"close_indices",			(DL_FUNC) &close_indices,			0},
	{"create_gbov",				(DL_FUNC) &create_gbov,				1},
	{"load_gbov",				(DL_FUNC) &load_gbov,				1},
	{"close_gbov",				(DL_FUNC) &close_gbov,				0},

	// Must have at the end
	{NULL,						NULL,								0}
};

void R_init_record(DllInfo* dll) {
	R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
}
