test_that("count one val", {
	open_db_for_write("count-one.txt")
	add_val(1:10)
	expect_equal(count_val(), 1)
  close_db()
})

test_that("count two vals", {
	open_db_for_write("count-two.txt")
	add_val(1:10)
  add_val("hello")
	expect_equal(count_val(), 2)
  close_db()
})

test_that("count 101 vals", {
	open_db_for_write("count-101.txt")
  for(i in 1:100) {
    add_val(i)
  }
  add_val(c("a","b","c"))
	expect_equal(count_val(), 101)
  close_db()
})
