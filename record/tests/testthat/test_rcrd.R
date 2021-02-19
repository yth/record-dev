tmp <- "tmp.txt"

test_that("test_open_db_1", {
	expect_silent(open_db_for_write(tmp))
  close_db(file)
})

test_that("test_close_bd_1", {
	file = open_db_for_write(tmp)
	expect_equal(close_db(file), NULL)
})

test_that("test_rcrd_r2cd_1", {
	file = open_db_for_write(tmp)
	s = "hello"
	expect_equal(add_value(s, file), s)
	expect_equal(close_db(file), NULL)
	f = read.delim(tmp)
	expect_equal(colnames(f), s)
})

test_that("add_value twice", {
	file = open_db_for_write("twice.txt")
	s = "charlie"

  expect_equal(add_value(s, file), s)
  expect_equal(add_value(s, file), s)

  expect_equal(close_db(file), NULL)
	f = read.delim("twice.txt")
	expect_equal(colnames(f), s)
})

test_that("testthis", {
  expect_equal(testthis("hello"), NULL)
})
