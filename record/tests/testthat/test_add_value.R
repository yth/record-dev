test_that("add_hello", {
	open_db_for_write("hello.txt")
	s = "hello"
	expect_equal(add_value(s), s)
	close_db()

	f = file("hello.txt", "rb")
	r = readBin(f, n = 1, character())
	expect_gt(length(r), 0)
	close(f)
})

test_that("add 10 vals", {
	open_db_for_write("add-10.txt")
	expect_equal(length(add_value(1:10)), 10)
	close_db()
})

test_that("add 100 vals", {
	open_db_for_write("add-100.txt")
	expect_equal(length(add_value(1:100)), 100)
	close_db()
})

test_that("add alphabet", {
	open_db_for_write("add-alphabet.txt")
  alphabet <- c("a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z")
	expect_equal(length(add_value(alphabet)), 26)
	close_db()
})
