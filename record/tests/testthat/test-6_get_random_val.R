test_that("get random val hello after adding hello", {
	open_db("test_db/get_random_1", create = TRUE)
	s1 = "hello"
	add_val(s1)
	expect_equal(get_random_val(), s1)
	close_db()
})

test_that("get random val hello after adding hello redux", {
	open_db("test_db/get_random_1")
	s1 = "hello"
	expect_equal(get_random_val(), s1)
	close_db()
})

test_that("get all three added vals by get_random_val in a loop", {
	open_db("test_db/get_random_2", create = TRUE)
	add_val(TRUE)
	add_val(1)
	add_val("hello")

  vals_added <- list(TRUE, 1, "hello")

  rand_vals = vector('list', 100)
	for (i in seq_along(rand_vals)) {
		rand_vals[[i]] <- get_random_val()
	}

  for(rand_val in rand_vals) {
    expect_true(list(rand_val) %in% vals_added)
  }

	close_db()
})

test_that("get all three added vals by get_random_val in a loop redux", {
	open_db("test_db/get_random_2")
  vals_added <- list(TRUE, 1, "hello")

  rand_vals = vector('list', 100)
	for (i in seq_along(rand_vals)) {
		rand_vals[[i]] <- get_random_val()
	}

  for(rand_val in rand_vals) {
    expect_true(list(rand_val) %in% vals_added)
  }

	close_db()
})

test_that("get random val from stringr::str_detect from existing db", {
  val_list <- readRDS("../resource/values.RDS")
  vals <- lapply(val_list, function(x) x[[3]])

	open_db("test_db/str_detect")

	rand_vals <- vector('list', 100)
	for (i in seq_along(rand_vals)) {
		rand_vals[[i]] <- get_random_val()
	}

  for(rand_val in rand_vals) {
    expect_true(list(rand_val) %in% vals)
  }

	close_db()
})

