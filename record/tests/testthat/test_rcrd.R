test_that("test_open_db_1", {
	expect_silent(open_db_for_write())
})

test_that("test_close_bd_1", {
	file = open_db_for_write()
	expect_equal(close_db(file), NULL)
})

test_that("test_rcrd_r2cd_1", {
	file = open_db_for_write()
	s = "string"
	expect_equal(add_value(s, file), s)
	expect_equal(close_db(file), NULL)
	f = read.delim("tmp.db")
	expect_equal(colnames(f), s)
})
