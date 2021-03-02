test_that("test_rcrd_add_value_simple", {
	success = open_db_for_write("tmp.txt")
	expect_equal(success, NULL)
	s = "hello"
	expect_equal(add_value(s), s)
	expect_equal(close_db(), NULL)
	f = file("tmp.txt", "rb")
	r = readBin(f, n = 1, character())
	expect_gt(length(r), 0)
	close(f)
})

test_that("test_rcrd_add_value_vector", {
	open_db_for_write("test-close.txt")
	add_value(1:10)
	expect_equal(close_db(), NULL)
})

test_that("test_rcrd_add_value_larger", {
	open_db_for_write("test-close2.txt")
	add_value(1:100)
	expect_equal(close_db(), NULL)
})
