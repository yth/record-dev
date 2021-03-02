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
