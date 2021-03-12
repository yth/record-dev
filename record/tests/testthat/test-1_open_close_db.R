test_that("open a new db and close it", {
	expect_silent(open_db("test_db", create = TRUE))
	expect_silent(close_db())
})

test_that("open existing db", {
	expect_error(open_db("test_db", create = TRUE))
	close_db()
})

test_that("test open non existing db for read", {
	expect_error(open_db("non_existing_db", read_only = TRUE))
})
