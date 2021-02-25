test_that("test_open_db_1", {
	expect_silent(open_db_for_write())
})

test_that("test_close_bd_1", {
	file = open_db_for_write()
	expect_equal(close_db(file), NULL)
})

test_that("test_rcrd_add_value_1", {
	success = open_db_for_write("tmp.txt")
	expect_equal(success, NULL)
	s = "hello"
	expect_equal(add_value(s), s)
	expect_equal(close_db(), NULL)
	f = file("tmp.txt", "rb")
	r = readBin(f, n = 1, character())
	expect_gt(length(r), 0)
})

test_that("test_rcrd_has_value_1", {
	success = open_db_for_write("tmp.txt")
	expect_equal(success, NULL)
	s = "hello"
	expect_equal(add_value(s), s)
	expect_equal(has_value(s), T)
	expect_equal(close_db(), NULL)
})

test_that("test_rcrd_has_value_2", {
	success = open_db_for_write("tmp.txt")
	expect_equal(success, NULL)
	s1 = "hello"
	s2 = "good bye"
	expect_equal(add_value(s1), s1)
	expect_equal(add_value(s2), s2)
	expect_equal(has_value(s1), T)
	expect_equal(has_value(s2), T)
	expect_equal(close_db(), NULL)
})

test_that("test_rcrd_has_value_3", {
	success = open_db_for_write("tmp.txt")
	expect_equal(success, NULL)
	s1 = "hello"
	s2 = "good bye"
	s3 = "arrivederci"
	expect_equal(add_value(s1), s1)
	expect_equal(add_value(s2), s2)
	expect_equal(has_value(s1), T)
	expect_equal(has_value(s2), T)
	expect_equal(has_value(s3), F)
	expect_equal(close_db(), NULL)
})
