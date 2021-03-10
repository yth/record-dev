#!/usr/bin/env Rscript

library(signatr)
library(record)

args <- commandArgs(trailingOnly=TRUE)
if (length(args) < 1) {
  message("Usage: record-vals.R <dir> [... <fileN>]")
  q(status=1)
}

run_dir <- args[1]
package_name <- args[2]


if (!dir.exists(run_dir)) {
  stop(run_dir, ": no such a directory")b

}

val_files <- list.files(path = run_dir, pattern = "values.RDS", recursive = TRUE)

tictoc::tic("Recording started:")
cat(sprintf("Recording started ...\n\n"))

if(is.null(open_db_for_write(paste0(package_name, ".txt")))) {
  print(paste0(package_name, ".txt", " opened successfully."))
}

num_total_val <- 0

for (i in seq_along(val_files)) {
  val_list <- readRDS(paste0(run_dir, "/", val_files[[i]]))
  vals <- lapply(val_list, function(x) x[[3]])
  num_total_val <- num_total_val + length(vals)

  lapply(vals, add_val)
}

print(paste0("the total number of val encountered is ", toString(num_total_val)))

print(paste0("successfully counted  ", toString(count_vals()), " many values."))
print(paste0("successfully recorded  ", toString(size_db())))

stopifnot(num_total_val == count_vals())

if(is.null(close_db())) {
  print(paste0(package_name, ".txt", " closed successfully."))
}

cat(sprintf("Recording %s values done.\n\n", num_total_val))
tictoc::toc()
