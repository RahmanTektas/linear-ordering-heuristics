# Runs the statistical analysis for exercise 1.1 with 5 selected examples 
# using the .dat files generated from the make_r_files.sh script.

best.known <- read.table("../results/best_known.dat")$V1

run_test <- function(file_a, name_a, file_b, name_b) {
  a <- read.table(paste0("../results/", file_a))$V1
  b <- read.table(paste0("../results/", file_b))$V1

  cat(name_a, "vs", name_b, "\n")
  cat("length(best.known) =", length(best.known), "\n")
  cat("length(a)          =", length(a), "\n")
  cat("length(b)          =", length(b), "\n\n")

  if (length(best.known) != length(a) || length(best.known) != length(b)) {
    stop(paste("Length mismatch for", name_a, "and", name_b))
  }

  a <- 100 * (a - best.known) / best.known
  b <- 100 * (b - best.known) / best.known

  cat("Average deviation", name_a, ":", mean(a), "\n")
  cat("Average deviation", name_b, ":", mean(b), "\n")
  cat("Std deviation", name_a, ":", sd(a), "\n")
  cat("Std deviation", name_b, ":", sd(b), "\n\n")

  cat("Paired t-test p-value:\n")
  print(t.test(a, b, paired=TRUE)$p.value)

  cat("\nWilcoxon p-value:\n")
  print(wilcox.test(a, b, paired=TRUE)$p.value)
}

run_test("lop_best_exchange_random.dat", "best exchange random",
         "lop_best_insert_random.dat",  "best insert random")

run_test("lop_best_exchange_cw.dat", "best exchange cw",
         "lop_best_insert_cw.dat",   "best insert cw")

run_test("lop_first_insert_random.dat", "first insert random",
         "lop_best_insert_random.dat",  "best insert random")

run_test("lop_first_insert_random.dat", "first insert random",
         "lop_first_insert_cw.dat",     "first insert cw")

run_test("lop_best_transpose_random.dat", "best transpose random",
         "lop_best_transpose_cw.dat",     "best transpose cw")