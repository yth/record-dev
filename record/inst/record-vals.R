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

if(is.null(open_db_for_write(paste0(package_name, "_db.txt")))) {
  print(paste0(package_name, ".txt", " opened successfully."))
}

tictoc::tic("Recording started:")
cat(sprintf("Recording started ...\n\n"))

gbov <- list()

for (i in seq_along(val_files)) {
  val_list <- readRDS(paste0(run_dir, "/", val_files[[i]]))
  vals <- lapply(val_list, function(x) x[[3]])

  lapply(vals, add_val)
  gbov <- append(gbov, vals)
}

duplicates <- sum(duplicated(gbov))

print(paste0(duplicates, " duplicates are found."))
print(paste0(count_vals(), " - ", size_db(), " = ", count_vals() - size_db()))

if(is.null(close_db())) {
  print(paste0(package_name, ".txt", " closed successfully."))
}

cat(sprintf("Recording done.\n\n"))
tictoc::toc()
