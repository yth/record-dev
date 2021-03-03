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
	open_db_for_write("test_add_vector.txt")
	add_value(1:10)
	expect_equal(close_db(), NULL)
})

test_that("test_rcrd_add_value_larger_1", {
	open_db_for_write("test_add_larger_vector_1.txt")
	add_value(1:100)
	expect_equal(close_db(), NULL)
})

test_that("test_rcrd_add_value_larger_1", {
	open_db_for_write("test_add_larger_vector_2.txt")
	alphabet = c("a", "b", "c", "d", "e", "f", "g", "h", "i")
	alphabet = c(alphabet, "j", "k", "l", "m", "n", "o", "p")
	alphabet = c(alphabet, "q", "r", "s", "t", "u", "v", "w")
	alphabet = c(alphabet, "x", "y", "z")

	alphabets = c(alphabet, alphabet)

	add_value(alphabets)
	expect_equal(close_db(), NULL)
})

test_that("test_rcrd_add_value_large", {
	open_db_for_write("test_add_large.txt")
	add_value(c(1:500, 600:100000))
	expect_equal(close_db(), NULL)
})
