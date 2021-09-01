if (T) {

## Integer Section

test_that("sample int database with no values", {
	open_db("test_db/b_sample_val/simple_int_sampling", create = T)
	expect_error(sample_val("integer"))
	close_db()
})

test_that("sample int database with some int values", {
	open_db("test_db/b_sample_val/simple_int_sampling")
	add_val(1L)
	expect_silent(sample_val("integer"))
	expect_equal(sample_val("integer"), 1L)
	close_db()
})

test_that("sample int database with some int values", {
	open_db("test_db/b_sample_val/int_sampling", create = T)
	add_val(c(1L, 2L, 3L))
	expect_silent(sample_val("integer"))
	expect_equal(sample_val("integer"), c(1L, 2L, 3L))
	close_db()
})

## Double Section

test_that("sample dbl database with no values", {
	open_db("test_db/b_sample_val/simple_dbl_sampling", create = T)
	expect_error(sample_val("double"))
	close_db()
})

test_that("sample dbl database with some dbl values", {
	open_db("test_db/b_sample_val/simple_dbl_sampling")
	add_val(1)
	expect_silent(sample_val("double"))
	expect_equal(sample_val("double"), 1)
	close_db()
})

test_that("sample dbl database with some dbl values", {
	open_db("test_db/b_sample_val/dbl_sampling", create = T)
	add_val(c(1, 2, 3))
	expect_silent(sample_val("double"))
	expect_equal(sample_val("double"), c(1, 2, 3))
	close_db()
})

}

if (F) {

## Other Section

test_that("sample null empty database", {
	open_db("test_db/b_sample_val/null_sampling", create = T)
	expect_error(sample_val("null"))
	close_db()
})

test_that("sample null database with null", {
	open_db("test_db/b_sample_val/null_sampling")
	add_val(NULL)
	expect_silent(sample_val("null"))
	expect_equal(sample_val("null"), NULL)
	close_db()
})

}

