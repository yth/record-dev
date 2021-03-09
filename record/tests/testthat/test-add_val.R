test_that("add_hello", {
	open_db_for_write("add_hello.txt")
	s = "hello"
	expect_equal(add_val(s), s)
	close_db()

	f = file("add_hello.txt", "rb")
	r = readBin(f, n = 1, character())
	expect_gt(length(r), 0)
	close(f)
})

test_that("add_10_vals", {
	open_db_for_write("add_10_vals.txt")
	expect_equal(add_val(1:10), 1:10)
	close_db()
})

test_that("add_100_vals", {
	open_db_for_write("add_100_vals.txt")
	expect_equal(add_val(1:100), 1:100)
	close_db()
})

test_that("add_alphabet", {
	open_db_for_write("add_alphabet.txt")
	alphabet = c("a", "b", "c", "d", "e", "f", "g", "h", "i")
	alphabet = c(alphabet, "j", "k", "l", "m", "n", "o", "p")
	alphabet = c(alphabet, "q", "r", "s", "t", "u", "v", "w")
	alphabet = c(alphabet, "x", "y", "z")

	alphabets = c(alphabet, alphabet)

	expect_equal(length(add_val(alphabets)), length(alphabets))
	close_db()
})

test_that("add_large_vals", {
	open_db_for_write("add_large_vals.txt")
	expect_equal(add_val(c(1:500, 600:100000)), c(1:500, 600:100000))
	close_db()
})

test_that("add_val_twice", {
	open_db_for_write("add_val_twice.txt")
	once <- "1"
	twice <- "2"
	expect_equal(add_val(once), "1")
	expect_equal(add_val(twice), "2")
	close_db()
})
