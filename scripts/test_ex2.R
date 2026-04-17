best.known <- read.table("../results/best_known.dat")$V1

tei <- read.table("../results/vnd_tei.dat")$V1
tie <- read.table("../results/vnd_tie.dat")$V1

cat("length(best.known) =", length(best.known), "\n")
cat("length(tei)        =", length(tei), "\n")
cat("length(tie)        =", length(tie), "\n\n")

if (length(best.known) != length(tei) || length(best.known) != length(tie)) {
  stop("Length mismatch: best_known.dat, vnd_tei.dat, and vnd_tie.dat must have the same number of lines.")
}

tei <- 100 * (tei - best.known) / best.known
tie <- 100 * (tie - best.known) / best.known

cat("Average deviation vnd-tei:", mean(tei), "\n")
cat("Average deviation vnd-tie:", mean(tie), "\n")
cat("Std deviation vnd-tei:", sd(tei), "\n")
cat("Std deviation vnd-tie:", sd(tie), "\n\n")

cat("Paired t-test p-value:\n")
print(t.test(tei, tie, paired = TRUE)$p.value)

cat("\nWilcoxon p-value:\n")
print(wilcox.test(tei, tie, paired = TRUE)$p.value)