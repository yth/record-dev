if (T) {

test_that("print some report", {
	open_db("test_db/some_report", create = TRUE)
	s1 = "hello"
	add_val(s1)
	expect_equal(get_random_val(), s1)
	expect_silent(report())
	close_db()
})

}