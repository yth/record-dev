if (T) {

test_that("sample null empty database", {
	open_db("test_db/null_sampling", create = T)
	expect_error(sample_val("null"))
	close_db()
})

test_that("sample null database with null", {
	open_db("test_db/null_sampling")
	add_val(NULL)
	expect_silent(sample_val("null"))
	expect_equal(sample_val("null"), NULL)
	close_db()
})

test_that("sample int database with no values", {
	open_db("test_db/simple_int_sampling", create = T)
	expect_error(sample_val("integer"))
	close_db()
})

test_that("sample int database with some int values", {
	open_db("test_db/simple_int_sampling")
	add_val(1L)
	expect_silent(sample_val("integer"))
	expect_equal(sample_val("integer"), 1L)
	close_db()
})

test_that("sample int database with some int values", {
	open_db("test_db/int_sampling", create = T)
	add_val(c(1L, 2L, 3L))
	expect_silent(sample_val("integer"))
	expect_equal(sample_val("integer"), c(1L, 2L, 3L))
	close_db()
})

}

