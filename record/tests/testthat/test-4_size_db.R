test_that("add one and record one", {
	open_db_for_write("count-one-u-val.txt")
	add_val(1:10)
	expect_equal(size_db(), 1)
	close_db()
})

test_that("add two equal vals and record one", {
	open_db_for_write("count-one-u-val2.txt")
	add_val("hello")
	add_val("hello")
	expect_equal(size_db(), 1)
	close_db()
})

test_that("add two equal vals and record one2", {
	open_db_for_write("count-two.txt")
	x <- 1
	add_val(x)
	add_val(1)
	expect_equal(size_db(), 1)
	close_db()
})

test_that("add 102 vals and record 101", {
	open_db_for_write("count-101.txt")
	for(i in 1:100) {
		add_val(i)
	}
	add_val(c("a","b","c"))
	expect_equal(size_db(), 101)

	add_val(c("a","b","c"))
	expect_equal(size_db(), 101)
	close_db()
})