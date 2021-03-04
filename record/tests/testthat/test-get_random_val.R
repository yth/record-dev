if (F) {
test_that("get_random_val1", {
	open_db_for_write()
	s1 = "hello"
	add_val(s1)
	expect_equal(get_random_val(), s1)
	close_db()
})

test_that("get_random_val2", {
	open_db_for_write()
	s1 = "hello"
	s2 = "good bye"
	s3 = "arrivederci"
	add_val(s1)
	add_val(s2)
	add_val(s3)

	l = list()
	for (i in 1:100) {
		l = append(l, get_random_val())
	}

	# expect_true(s1 %in% l)
	# expect_true(s2 %in% l)
	# expect_true(s3 %in% l)

	close_db()
})
}
