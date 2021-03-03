test_that("test_rcrd_has_value_1", {
	open_db_for_write("has-hello.txt")
	s = "hello"
	add_value(s)
	expect_equal(has_value(s), T)
	close_db()
})

test_that("test_rcrd_has_value_2", {
	open_db_for_write("has-hello-goodbye.txt")
	s1 = "hello"
	s2 = "good bye"
	add_value(s1)
	add_value(s2)
	expect_equal(has_value(s1), T)
	expect_equal(has_value(s2), T)
	close_db()
})

test_that("has 3 string vals", {
	open_db_for_write("has-3-vals.txt")
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

test_that("has 100 int vals", {
  open_db_for_write("has-100-value.txt")
  add_value(1:100)
  expect_equal(has_value(1:100), TRUE)
  close_db()
})
