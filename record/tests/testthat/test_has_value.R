test_that("test_rcrd_has_value_1", {
	open_db_for_write("hello.txt")
	s = "hello"
	add_value(s)
	expect_equal(has_value(s), T)
	close_db()
})

test_that("test_rcrd_has_value_2", {
  open_db_for_write("hello-goodbye.txt")
	s1 = "hello"
	s2 = "good bye"
	add_value(s1)
	add_value(s2)
	expect_equal(has_value(s1), T)
	expect_equal(has_value(s2), T)
	close_db()
})

test_that("test_rcrd_has_value_3", {
	open_db_for_write("three-string-vals.txt")
	s1 = "hello"
	s2 = "good bye"
	s3 = "arrivederci"
	add_value(s1)
	add_value(s2)
	expect_equal(has_value(s1), T)
	expect_equal(has_value(s2), T)
	expect_equal(has_value(s3), F)
	close_db()
})

test_that("test with a vector of length 100", {
  open_db_for_write("has-100-value.txt")
  add_value(1:100)
  expect_equal(has_value(1:100), TRUE)
  close_db()
})
