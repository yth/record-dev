test_that("test open non existing db", {
	expect_error(open_db("test_db/non_existing_db", create = FALSE))
})

test_that("open a new db and close it", {
	expect_silent(open_db("test_db/open", create = TRUE))
	expect_silent(close_db())
})

test_that("recreate existing db", {
	expect_error(open_db("test_db/open", create = TRUE))
})

test_that("open existing db", {
	expect_silent(open_db("test_db/open", create = FALSE))
	expect_silent(close_db())
})
