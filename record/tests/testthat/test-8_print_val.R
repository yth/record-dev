if (T) {

test_that("print some values", {
	print("Call print_vals() before opening a new database")
	expect_error(print_vals())

	open_db("test_db/print_vals", create = TRUE)
	add_val("hello")
	add_val("a really long string should be in generic store")
	add_val(as.raw(1))
	add_val(as.raw(c(1, 1)))
	add_val(1)
	add_val(50000)
	add_val(1L)
	add_val(50000L)
	add_val(function() (1 + 1))
	print("Call print_vals() after opening a new database and doing work")
	print_vals()
	close_db()

	open_db("test_db/print_vals")
	print("Call report() after reopening the database")
	print_vals()
	close_db()
})

test_that("print no value", {
	open_db("test_db/print_vals_none", create = TRUE)
	expect_error(print_vals())
	close_db()
})

test_that("view db", {
  open_db("test_db/view_db", create = TRUE)
  add_val("1")
  add_val(2L)
  add_val(3.0000)
  viewer <- view_db()
  expect_equal(length(viewer), 3)
  close_db()
})

}
