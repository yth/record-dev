if (T) {

test_that("merge empty ints", {
	open_db("test_db/ints1", create = T)
	close_db()

	open_db("test_db/ints2", create = T)
	add_val(3L)
	add_val(4L)
	expect_true(size_db() == 2)
	merge_db("test_db/ints1")
	expect_true(size_db() == 2)
	close_db()
})

test_that("merge ints", {
	open_db("test_db/ints1")
	add_val(1L)
	add_val(2L)
	expect_true(size_db() == 2)
	close_db()

	open_db("test_db/ints2")
	merge_db("test_db/ints1")
	expect_true(size_db() == 4)
	close_db()
})

test_that("merge ints reverse", {
	open_db("test_db/ints1")
	expect_true(size_db() == 2)
	merge_db("test_db/ints2")
	expect_true(size_db() == 4)
	close_db()
})

test_that("merge empty dbls", {
	open_db("test_db/dbls1", create = T)
	close_db()

	open_db("test_db/dbls2", create = T)
	add_val(3)
	add_val(4)
	expect_true(size_db() == 2)
	merge_db("test_db/dbls1")
	expect_true(size_db() == 2)
	close_db()
})

test_that("merge dbls", {
	open_db("test_db/dbls1")
	add_val(1)
	add_val(2)
	expect_true(size_db() == 2)
	close_db()

	open_db("test_db/dbls2")
	merge_db("test_db/dbls1")
	expect_true(size_db() == 4)
	close_db()
})

test_that("merge ints reverse", {
	open_db("test_db/dbls1")
	expect_true(size_db() == 2)
	merge_db("test_db/dbls2")
	expect_true(size_db() == 4)
	close_db()
})

test_that("merge empty raws", {
	open_db("test_db/raws1", create = T)
	close_db()

	open_db("test_db/raws2", create = T)
	add_val(as.raw(3))
	add_val(as.raw(4))
	expect_true(size_db() == 2)
	merge_db("test_db/raws1")
	expect_true(size_db() == 2)
	close_db()
})

test_that("merge raws", {
	open_db("test_db/raws1")
	add_val(as.raw(1))
	add_val(as.raw(2))
	expect_true(size_db() == 2)
	close_db()

	open_db("test_db/raws2")
	merge_db("test_db/raws1")
	expect_true(size_db() == 4)
	close_db()
})

test_that("merge raws reverse", {
	open_db("test_db/raws1")
	expect_true(size_db() == 2)
	merge_db("test_db/raws2")
	expect_true(size_db() == 4)
	close_db()
})

test_that("merge empty strs", {
	open_db("test_db/strs1", create = T)
	close_db()

	open_db("test_db/strs2", create = T)
	add_val("3")
	add_val("4")
	expect_true(size_db() == 2)
	merge_db("test_db/strs1")
	expect_true(size_db() == 2)
	close_db()
})

test_that("merge strs", {
	open_db("test_db/strs1")
	add_val("1")
	add_val("2")
	expect_true(size_db() == 2)
	close_db()

	open_db("test_db/strs2")
	merge_db("test_db/strs1")
	expect_true(size_db() == 4)
	close_db()
})

test_that("merge strs reverse", {
	open_db("test_db/strs1")
	expect_true(size_db() == 2)
	merge_db("test_db/strs2")
	expect_true(size_db() == 4)
	close_db()
})

test_that("merge empty generics", {
	open_db("test_db/generics1", create = T)
	close_db()

	open_db("test_db/generics2", create = T)
	add_val("a very long string 3")
	add_val("a very long string 4")
	expect_true(size_db() == 2)
	merge_db("test_db/generics1")
	expect_true(size_db() == 2)
	close_db()
})

test_that("merge generics", {
	open_db("test_db/generics1")
	add_val("a very long string 1")
	add_val("a very long string 2")
	expect_true(size_db() == 2)
	close_db()

	open_db("test_db/generics2")
	merge_db("test_db/generics1")
	expect_true(size_db() == 4)
	close_db()
})

test_that("merge generics reverse", {
	open_db("test_db/generics1")
	expect_true(size_db() == 2)
	merge_db("test_db/generics2")
	expect_true(size_db() == 4)
	close_db()
})

test_that("merge count", {
	open_db("test_db/generics1")
	expect_silent(report())
	close_db()
})

}

