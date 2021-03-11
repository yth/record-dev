test_that("has seen hello", {
	open_db_for_write("seen-hello.txt")
	s = "hello"
	add_val(s)
	expect_equal(has_seen(s), T)
	close_db()
})

test_that("has seen twice", {
	open_db_for_write("seen-twice.txt")
	s1 = "hello"
	s2 = "good bye"
	add_val(s1)
	add_val(s2)
	expect_equal(has_seen(s1), T)
	expect_equal(has_seen(s2), T)
	close_db()
})

test_that("has not seen not added val", {
	open_db_for_write("not-seen.txt")
	s1 = "hello"
	s2 = "good bye"
	s3 = "arrivederci"
	add_val(s1)
	add_val(s2)
	expect_equal(has_seen(s1), T)
	expect_equal(has_seen(s2), T)
	expect_equal(has_seen(s3), F)
	close_db()
})

test_that("has seen 100 int vals", {
  open_db_for_write("seen-100-vals.txt")
  add_val(1:100)
  expect_equal(has_seen(1:100), TRUE)
  close_db()
})

test_that("has seen alphabet", {
  open_db_for_write("seen-alphabet.txt")
  alphabet <- c("a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z")
  add_val(alphabet)
  expect_equal(has_seen(alphabet), TRUE)
  close_db()
})
