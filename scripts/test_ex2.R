best.known <- read.table("best_known.dat")$V1

tei <- read.table("vnd_tei.dat")$V1
tie <- read.table("vnd_tie.dat")$V1

cat("length(best.known) =", length(best.known), "\n")
cat("length(tei)        =", length(tei), "\n")
cat("length(tie)        =", length(tie), "\n\n")

if (length(tei) != length(best.known) || length(tie) != length(best.known)) {
  stop("Length mismatch between best_known.dat, vnd_tei.dat, and vnd_tie.dat")
}

tei.dev <- 100 * (tei - best.known) / best.known
tie.dev <- 100 * (tie - best.known) / best.known

cat("Average deviation vnd-tei:", mean(tei.dev), "\n")
cat("Average deviation vnd-tie:", mean(tie.dev), "\n")
cat("Std deviation vnd-tei:", sd(tei.dev), "\n")
cat("Std deviation vnd-tie:", sd(tie.dev), "\n\n")

cat("Paired t-test p-value:\n")
t_p <- t.test(tei.dev, tie.dev, paired = TRUE)$p.value
print(t_p)

cat("\nWilcoxon p-value:\n")
w_p <- wilcox.test(tei.dev, tie.dev, paired = TRUE)$p.value
print(w_p)

# save summary to a txt file
sink("../results/stats_ex2_summary.txt")
cat("Exercise 1.2 statistical comparison\n\n")
cat("Average deviation vnd-tei:", mean(tei.dev), "\n")
cat("Average deviation vnd-tie:", mean(tie.dev), "\n")
cat("Std deviation vnd-tei:", sd(tei.dev), "\n")
cat("Std deviation vnd-tie:", sd(tie.dev), "\n\n")
cat("Paired t-test p-value:\n")
print(t_p)
cat("\nWilcoxon p-value:\n")
print(w_p)
sink()