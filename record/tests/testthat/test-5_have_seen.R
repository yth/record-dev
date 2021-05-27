if (T) {

test_that("have seen nothing", {
	open_db("test_db/nothing", create = TRUE)
	expect_equal(have_seen("hello"), FALSE)
	close_db()
})

test_that("have seen hello", {
	open_db("test_db/seen-hello", create = TRUE)
	add_val("hello")
	expect_equal(have_seen("hello"), TRUE)
	close_db()
})

test_that("have seen twice", {
	open_db("test_db/seen-twice", create = TRUE)
	add_val(TRUE)
	add_val(1)
	expect_equal(have_seen(TRUE), TRUE)
	expect_equal(have_seen(1), TRUE)
	close_db()
})

test_that("open existing db and have seen the vals", {
	open_db("test_db/seen-twice", create = FALSE)
	expect_equal(have_seen(TRUE), TRUE)
	expect_equal(have_seen(1), TRUE)
	close_db()
})

test_that("open existing db and not seen not existing val", {
	open_db("test_db/seen-twice", create = FALSE)
	expect_equal(have_seen("not existing val"), FALSE)
	close_db()
})

test_that("have seen 100 times", {
	open_db("test_db/seen-100", create = TRUE)

	for(i in 1:100) {
		add_val(i)
	}
	add_val("hello")

	for(i in 1:100) {
		expect_equal(have_seen("hello"), TRUE)
		expect_equal(have_seen(i), TRUE)
	}
	close_db()
})

test_that("have seen alphabet", {
	open_db("test_db/seen-alphabet", create = TRUE)
	alphabet <- c("a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z")
	add_val(alphabet)
	expect_equal(have_seen(alphabet), TRUE)
	close_db()
})

# real values from stringr::str_detect
val_list <- readRDS("../resource/values.RDS")
vals <- lapply(val_list, function(x) x[[3]])

test_that("have seen real vals from stringr::str_detect", {
	open_db("test_db/str_detect", create = TRUE)

	lapply(vals, add_val)

	for(val in vals) {
		expect_equal(have_seen(val), TRUE)
	}

	close_db()
})

test_that("remove all information after closing database", {
	u_vals <- unique(vals)
	expect_false(size_db() == length(u_vals))
})

}
