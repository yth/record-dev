test_that("test_close_bd_1", {
	open_db_for_write()
	expect_equal(close_db(), NULL)
})

test_that("close after adding 100 elements", {
  open_db_for_write("test.txt")
  add_value(1:100)
  has_value(1:100)
  expect_equal(close_db(), NULL)
})
