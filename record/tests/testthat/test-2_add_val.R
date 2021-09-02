if (T) {

## Integer Section

test_that("add simple int", {
	open_db("test_db/2_add_val/simple_int_1", create = T)
	expect_equal(add_val(1L), 1L)
	close_db()
})

test_that("add scalar int", {
	open_db("test_db/2_add_val/int_1", create = T)
	expect_equal(add_val(10000L), 10000L)
	close_db()
})

test_that("add vector int", {
	open_db("test_db/2_add_val/int_1")
	expect_equal(add_val(c(1L, 2L, 3L)), c(1L, 2L, 3L))
	close_db()
})

test_that("add -5000L", {
	open_db("test_db/2_add_val/add--5000L", create = TRUE)
	expect_equal(add_val(-5000L), -5000L)
	close_db()
})

test_that("add 5000L", {
	open_db("test_db/2_add_val/add-5000L", create = TRUE)
  expect_equal(add_val(5000L), 5000L)
	close_db()
})

test_that("add 5000L again", {
	open_db("test_db/2_add_val/add-5000L", create = FALSE)
	expect_equal(add_val(5000L), NULL)
	close_db()
})

test_that("add 5001L", {
	open_db("test_db/2_add_val/add-5001L", create = TRUE)
	expect_equal(add_val(5001L), 5001L)
	close_db()
})

test_that("add 10 vals", {
	open_db("test_db/2_add_val/add-10-vals", create = T)
	expect_equal(add_val(1:10), 1:10)
	close_db()
})

test_that("add 100 vals", {
	open_db("test_db/2_add_val/add-100-vals", create = T)
	expect_equal(add_val(1:100), 1:100)
	close_db()
})

test_that("add large vals", {
	open_db("test_db/2_add_val/add-large-vals", create = T)
	expect_equal(add_val(c(1:500, 600:100000)), c(1:500, 600:100000))
	close_db()
})

test_that("add attributed int", {
	open_db("test_db/2_add_val/add-attributed-0L", create = TRUE)

	x <- 0L
	attr(x, "name") <- "attributed 0"

	expect_equal(add_val(x), x)
	expect_equal(size_db(), 1)
	close_db()
})

## Double Section

test_that("add simple dbl", {
	open_db("test_db/2_add_val/simple_dbl_1", create = T)
	expect_equal(add_val(1), 1)
	close_db()
})

test_that("add scalar int", {
	open_db("test_db/2_add_val/dbl_1", create = T)
	expect_equal(add_val(10000), 10000)
	close_db()
})

test_that("add vector dbl", {
	open_db("test_db/2_add_val/dbl_1")
	expect_equal(add_val(c(1, 2, 3)), c(1, 2, 3))
	close_db()
})

test_that("add attributed dbl", {
	open_db("test_db/2_add_val/add-attributed-0", create = TRUE)

	x <- 0
	attr(x, "name") <- "attributed 0"

	expect_equal(add_val(x), x)
	expect_equal(size_db(), 1)
	close_db()
})

## Raw Section

test_that("add simple raw", {
	open_db("test_db/2_add_val/simple_raw_1", create = T)
	expect_equal(add_val(as.raw(1)), as.raw(1))
	close_db()
})

test_that("add scalar raw", {
	open_db("test_db/2_add_val/raw_1", create = T)
	expect_equal(add_val(as.raw(2)), as.raw(2))
	close_db()
})

test_that("add vector raw", {
	open_db("test_db/2_add_val/raw_1")
	expect_equal(add_val(c(as.raw(1), as.raw(2), as.raw(3))),
				c(as.raw(1), as.raw(2), as.raw(3)))
	close_db()
})

test_that("add attributed dbl", {
	open_db("test_db/2_add_val/add-attributed-raw", create = TRUE)

	x <- as.raw(0)
	attr(x, "name") <- "attributed 0"

	expect_equal(add_val(x), x)
	expect_equal(size_db(), 1)
	close_db()
})

## Other Section

if (F) {

test_that("add hello", {
	open_db("test_db/add-hello", create = T)
	s = "hello"
	expect_equal(add_val(s), s)
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

test_that("add value again", {
	open_db("test_db/add-large-vals")
	expect_equal(add_val(1), 1)
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

test_that("add double 1", {
	open_db("test_db/add-1", create = TRUE)
	expect_equal(add_val(1), 1)
	close_db()
})

test_that("add simple double", {
	open_db("test_db/add-simple-double", create = TRUE)
	expect_equal(add_val(5), 5)
	close_db()
})

test_that("add few strings", {
	open_db("test_db/add-few-strings", create = TRUE)
	for (i in 1:10) {
		expect_equal(add_val(paste(as.character(i))), as.character(i))
	}
	report()
	close_db()
})

test_that("add many strings", {
	open_db("test_db/add-many-strings", create = TRUE)
	for (i in 1:3000) {
		expect_equal(add_val(paste(as.character(i))), as.character(i))
	}
	report()
	close_db()
})

test_that("add huge number of strings", {
	open_db("test_db/add-many-many-strings", create = TRUE)
	for (i in 99990000:100002000) {
		expect_equal(add_val(paste(as.character(i))), as.character(i))
	}
	report()
	close_db()
})

test_that("add empty string", {
	open_db("test_db/empty-string", create = TRUE)
	expect_equal(add_val(""), "")
	report()
	close_db()
})

}

}
