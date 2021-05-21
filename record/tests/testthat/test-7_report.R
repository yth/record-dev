if (T) {

test_that("print some report", {
	report()
	open_db("test_db/some_report", create = TRUE)
	s1 = "hello"
	add_val(s1)
	add_val(as.raw(1))
	add_val(1)
	add_val(1L)
	expect_silent(report())
	close_db()
})

}