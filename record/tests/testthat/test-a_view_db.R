if (T) {

## Integer Section

## Other Section

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
