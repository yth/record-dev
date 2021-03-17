test_that("add hello", {
	open_db("test_db/add-hello", create = T)
	s = "hello"
	expect_equal(add_val(s), s)
	close_db()
})

test_that("add 10 vals", {
	open_db("test_db/add-10-vals", create = T)
	expect_equal(add_val(1:10), 1:10)
	close_db()
})

test_that("add 100 vals", {
	open_db("test_db/add-100-vals", create = T)
	expect_equal(add_val(1:100), 1:100)
	close_db()
})

test_that("add alphabet", {
	open_db("test_db/add-alphabet", create = T)
	alphabet = c("a", "b", "c", "d", "e", "f", "g", "h", "i")
	alphabet = c(alphabet, "j", "k", "l", "m", "n", "o", "p")
	alphabet = c(alphabet, "q", "r", "s", "t", "u", "v", "w")
	alphabet = c(alphabet, "x", "y", "z")

	alphabets = c(alphabet, alphabet)

	expect_equal(length(add_val(alphabets)), length(alphabets))
	close_db()
})

test_that("add large vals", {
	open_db("test_db/add-large-vals", create = T)
	expect_equal(add_val(c(1:500, 600:100000)), c(1:500, 600:100000))
	close_db()
})

test_that("add val twice", {
	open_db("test_db/add-val-twice", create = T)
	once <- "1"
	twice <- "2"
	expect_equal(add_val(once), "1")
	expect_equal(add_val(twice), "2")
	close_db()
})

test_that("add duplicate simple val", {
	open_db("test_db/add-duplicate-simple-val", create = T)
	expect_equal(add_val(1), 1)
	expect_equal(add_val(1), NULL)
	close_db()
})

test_that("add value again", {
	open_db("test_db/add-large-vals")
	expect_equal(add_val(1), 1)
	close_db()
})

test_that("add duplicate simple val again", {
	open_db("test_db/add-duplicate-simple-val")
	expect_equal(add_val(1), NULL)
	expect_equal(add_val("new value string"), "new value string")
	close_db()
})

test_that("add duplicate logicals", {
	open_db("test_db/logical", create = TRUE)
	for(i in 1:10) {
    add_val(TRUE)
    add_val(FALSE)
  }
  expect_equal(size_db(), 2)
	close_db()
})
