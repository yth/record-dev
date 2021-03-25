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
  stop(run_dir, ": no such a directory")
}

val_files <- list.files(path = run_dir, pattern = "values.RDS", recursive = TRUE)

if(is.null(open_db(paste0(package_name, "_db"), create = TRUE))) {
  print(paste0(package_name, "_db", " opened successfully."))
}

tictoc::tic("Recording started:")
cat(sprintf("Recording started ...\n\n"))

gbov <- list()

for (i in seq_along(val_files)) {
  val_list <- readRDS(paste0(run_dir, "/", val_files[[i]]))
  vals <- lapply(val_list, function(x) x[[3]])

  lapply(vals, add_val)
  ## gbov <- append(gbov, vals)
}

## duplicates <- sum(duplicated(gbov))
## u_gbov <- unique(gbov)

## print(paste0(duplicates, " duplicates are found out of ", length(gbov), " values"))
## print(paste0(length(gbov) - length(u_gbov), " should equal to ", duplicates))
print(paste0("total of ", count_vals(), " saw."))
print(paste0("size of db (= num of unique values) is ", size_db()))
print(paste0("size of int_db (= num of unique int values) is ", size_ints()))

## saveRDS(u_gbov, file = "unique_vals.RDS")
if(is.null(close_db())) {
  print(paste0(package_name, "_db", " closed successfully."))
}

cat(sprintf("Recording done.\n\n"))
tictoc::toc()
