test_that("get_random_val1", {
	open_db("test_db_get_random_1", create = TRUE)
	s1 = "hello"
	add_val(s1)
	expect_equal(get_random_val(), s1)
	close_db()
})

test_that("get_random_val2", {
	open_db("test_db_get_random_2", create = TRUE)
	add_val(TRUE)
	add_val(1:10)
	add_val("hello")

  vals_added <- list(TRUE, 1:10, "hello")

  rand_vals = list()
	for (i in 1:100) {
		rand_vals = append(rand_vals, get_random_val())
	}

  for(rand_val in rand_vals) {
    expect_true(rand_val %in% vals_added)
  }

	close_db()
})

# real values from stringr::str_detect
str_detect <- "../../../../test/stringr/str_detect.Rd.R"
val_file <- list.files(path = str_detect, pattern = "values.RDS")
val_list <- readRDS(paste0(str_detect, "/", val_file))
vals <- lapply(val_list, function(x) x[[3]])

## test_that("get random val from stringr::str_detect from existing db", {
## 	open_db("test_db_get_random_3", create = TRUE)

##   lapply(vals, add_val)

## 	rand_vals = list()
## 	for (i in 1:100) {
## 		rand_vals = append(rand_vals, get_random_val())
## 	}

##   for(rand_val in rand_vals) {
##     expect_true(rand_val %in% vals)
##   }

## 	close_db()
## })
