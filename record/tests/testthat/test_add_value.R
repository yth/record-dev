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
	open_db_for_write("add10.txt")
	expect_equal(length(add_value(1:10)), 10)
	close_db()
})

test_that("test_rcrd_add_value_larger", {
	open_db_for_write("add100.txt")
	expect_equal(length(add_value(1:100)), 100)
	close_db()
})

test_that("add-alphabet", {
	open_db_for_write("add-alphabet.txt")
  alphabet <- c("a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z")
	expect_equal(length(add_value(alphabet)), 26)
	close_db()
})
