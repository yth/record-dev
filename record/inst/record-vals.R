#!/usr/bin/env Rscript

library(signatr)
library(record)
library(pryr)

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

sizes_df <- data.frame(byte = numeric(), type = character(), val = list())

tictoc::tic("Recording started:")
cat(sprintf("Recording started ...\n\n"))

for (i in seq_along(val_files)) {
  val_list <- readRDS(paste0(run_dir, "/", val_files[[i]]))
  vals <- lapply(val_list, function(x) x[[3]])

  lapply(vals, add_val)

  bytes = unlist(lapply(vals, function(v) as.numeric(object_size(v))))
  types = unlist(lapply(vals, typeof))
  serialized = lapply(vals, function(v) I(list(serialize(v, connection = NULL))))

  for (i in seq_along(vals)) {
    obs <- data.frame(byte = bytes[[i]], type = types[[i]], val = serialized[[i]])
    sizes_df <- rbind(sizes_df, obs)
  }
}

cat(sprintf("Recording %s values done.\n\n", toString(count_vals())))
tictoc::toc()

print(paste0("successfully recorded  ", toString(size_db()), " values."))

if(is.null(close_db())) {
  print(paste0(package_name, ".txt", " closed successfully."))
}

saveRDS(sizes_df, file = paste0(run_dir, "/size-dist.RDS"))
