test_that("open and close", {
	open_db_for_write("open-test.txt")
  expect_equal(file.exists("open-test.txt"), TRUE)
	expect_equal(close_db(), NULL)
})

test_that("open a new db and close it", {
	open_db("test_db", create = TRUE)
  expect_equal(dir.exists("test_db"), TRUE)
	expect_silent(close_db())
})

test_that("open existing db for read", {
	open_db("test_db", create = FALSE)
  expect_equal(file.exists("test_db/gbov.bin"), TRUE)
  expect_equal(file.exists("test_db/indices.bin"), TRUE)
	close_db()
})

test_that("open existing db", {
	expect_error(open_db("test_db", create = TRUE))
  ## close_db() =>  segfault
})

test_that("test open non existing db for read", {
	expect_error(open_db("non_existing_db", read_only = TRUE))
})
