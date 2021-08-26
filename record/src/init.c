#define R_NO_REMAP

#include <R.h>
#include <R_ext/Rdynload.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL // Based on evil suggestion

#include "hello.h"

#include "stats_store.h"
#include "generic_store.h"

#include "int_store.h"
#include "simple_int_store.h"

#include "simple_dbl_store.h"
#include "simple_raw_store.h"
#include "simple_str_store.h"

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
	{"get_val",					(DL_FUNC) &get_val,					1},

	// stats store related
	{"init_stats_store",		(DL_FUNC) &init_stats_store,		1},
	{"load_stats_store",		(DL_FUNC) &load_stats_store,		1},
	{"merge_stats_store",		(DL_FUNC) &merge_stats_store,		0},		{"close_stats_store",		(DL_FUNC) &close_stats_store,		0},
	{"print_report",			(DL_FUNC) &print_report,			0},
	{"size_db",					(DL_FUNC) &size_db,					0},
	{"size_ints",				(DL_FUNC) &size_ints,				0},
	{"count_vals",				(DL_FUNC) &count_vals,				0},

	// int value store related
	{"init_int_index",			(DL_FUNC) &init_int_index,			1},
	{"init_int_store",			(DL_FUNC) &init_int_store,			1},
	{"load_int_index",			(DL_FUNC) &load_int_index,			1},
	{"load_int_store",			(DL_FUNC) &load_int_store,			1},
	{"merge_int_store",			(DL_FUNC) &merge_int_store,			2},
	{"close_int_index",			(DL_FUNC) &close_int_index,			0},
	{"close_int_store",			(DL_FUNC) &close_int_store,			0},
	{"sample_int",				(DL_FUNC) &sample_int,				0},


	// simple ints store related
	{"init_simple_int_store",	(DL_FUNC) &init_simple_int_store,	1},
	{"load_simple_int_store",	(DL_FUNC) &load_simple_int_store,	1},
	{"merge_simple_int_store",	(DL_FUNC) &merge_simple_int_store,	1},	{"close_simple_int_store",	(DL_FUNC) &close_simple_int_store,	0},

	// simple doubles store related
	{"init_simple_dbl_store",	(DL_FUNC) &init_simple_dbl_store,	1},
	{"load_simple_dbl_store",	(DL_FUNC) &load_simple_dbl_store,	1},
	{"merge_simple_dbl_store",	(DL_FUNC) &merge_simple_dbl_store,	1},		{"close_simple_dbl_store",	(DL_FUNC) &close_simple_dbl_store,	0},

	// simple raw store related
	{"init_simple_raw_store",	(DL_FUNC) &init_simple_raw_store,	1},
	{"load_simple_raw_store",	(DL_FUNC) &load_simple_raw_store,	1},	{"merge_simple_raw_store",	(DL_FUNC) &merge_simple_raw_store,	1},
	{"close_simple_raw_store",	(DL_FUNC) &close_simple_raw_store,	0},

	// simple string store related
	{"init_simple_str_index",	(DL_FUNC) &init_simple_str_index,	1},
	{"init_simple_str_store",	(DL_FUNC) &init_simple_str_store,	1},
	{"load_simple_str_index",	(DL_FUNC) &load_simple_str_index,	1},
	{"load_simple_str_store",	(DL_FUNC) &load_simple_str_store,	1},
	{"merge_simple_str_store",	(DL_FUNC) &merge_simple_str_store,	2},
	{"close_simple_str_index",	(DL_FUNC) &close_simple_str_index,	0},
	{"close_simple_str_store",	(DL_FUNC) &close_simple_str_store,	0},

	// generic R value store related
	{"init_generic_index",		(DL_FUNC) &init_generic_index,		1},
	{"init_generic_store",		(DL_FUNC) &init_generic_store,		1},
	{"load_generic_index",		(DL_FUNC) &load_generic_index,		1},
	{"load_generic_store",		(DL_FUNC) &load_generic_store,		1},
	{"merge_generic_store",		(DL_FUNC) &merge_generic_store,		2},
	{"close_generic_index",		(DL_FUNC) &close_generic_index,		0},
	{"close_generic_store",		(DL_FUNC) &close_generic_store,		0},

	// Must have at the end
	{NULL,						NULL,								0}
};

void R_init_record(DllInfo* dll) {
	R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
	R_RegisterCCallable("record", "add_val", (DL_FUNC) &add_val);
}

