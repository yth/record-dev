## usethis namespace: start
#' @useDynLib record, .registration = TRUE, .fixes = "RCRD_"
## usethis namespace: end
record <- function(rvalue) {
  .Call(C_record, rvalue)
}
