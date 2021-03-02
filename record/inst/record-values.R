#!/usr/bin/env Rscript

library(signatr)
library(record)

args <- commandArgs(trailingOnly=TRUE)
if (length(args) < 1) {
  message("Usage: merge-gbov.R <dir> [... <fileN>]")
  q(status=1)
}

run_dir <- args[1]
## run_dir <- "../../../test/stringr/str_detect.Rd.R"

if (!dir.exists(run_dir)) {
  stop(run_dir, ": no such a directory")
}

values_files <- list.files(path = run_dir, pattern = "values.RDS", recursive = TRUE)

## tictoc::tic("Recording started:")
## cat(sprintf("Recording started ...\n\n"))

if(is.null(open_db_for_write("str_detect.txt"))) {
  print("db opened successfully.")
}

for (i in seq_along(values_files)) {
  values_list <- readRDS(paste0(run_dir, "/", values_files[[i]]))
}

print(paste0("recording ", toString(length(values_list)), " values."))

values <- lapply(values_list, function(x) x[[3]])

lapply(values, add_value)
lapply(values, has_value)


if(is.null(close_db())) {
  print("db closed successfully.")
}

## cat(sprintf("Recording %s values done.\n\n", length(values_list)))
## tictoc::toc()
