test_that("count one val", {
	open_db("test_db_count-one", create = T)
	add_val(1:10)
	expect_equal(count_vals(), 1)
	close_db()
})

test_that("count two vals", {
	open_db("test_db_count-two", create = T)
	add_val(1:10)
	add_val("hello")
	expect_equal(count_vals(), 2)
	close_db()
})

test_that("count 101 vals", {
	open_db("test_db_count-101", create = T)
	for(i in 1:100) {
		add_val(i)
	}
	add_val(c("a","b","c"))
	expect_equal(count_vals(), 101)
	close_db()
})
