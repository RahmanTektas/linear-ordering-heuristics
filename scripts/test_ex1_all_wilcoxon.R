# Runs the wilcoxon statistical analysis for exercise 1.1, which includes 12 algorithms obtained by combining the two pivoting rules, three neighborhoods, and two initialization methods,
# using the .dat files generated from the make_r_files.sh script.
# The results are saved in stats_ex1_summary.txt

best.known <- read.table("../results/best_known.dat")$V1

files <- c(
  "lop_best_exchange_cw.dat",
  "lop_first_exchange_cw.dat",
  "lop_best_exchange_random.dat",
  "lop_first_exchange_random.dat",
  "lop_best_insert_cw.dat",
  "lop_first_insert_cw.dat",
  "lop_best_insert_random.dat",
  "lop_first_insert_random.dat",
  "lop_best_transpose_cw.dat",
  "lop_first_transpose_cw.dat",
  "lop_best_transpose_random.dat",
  "lop_first_transpose_random.dat"
)

labels <- c(
  "ex-cw-b",
  "ex-cw-f",
  "ex-r-b",
  "ex-r-f",
  "ins-cw-b",
  "ins-cw-f",
  "ins-r-b",
  "ins-r-f",
  "tr-cw-b",
  "tr-cw-f",
  "tr-r-b",
  "tr-r-f"
)

# Read data and convert to deviations
data_list <- list()

for (i in seq_along(files)) {
  x <- read.table(paste0("../results/", files[i]))$V1

  if (length(x) != length(best.known)) {
    stop(paste("Length mismatch for file:", files[i]))
  }

  x <- 100 * (x - best.known) / best.known
  data_list[[labels[i]]] <- x
}

# Pairwise Wilcoxon p-values
n <- length(labels)
p_mat <- matrix(NA, nrow = n, ncol = n)
rownames(p_mat) <- labels
colnames(p_mat) <- labels

for (i in 1:n) {
  for (j in 1:n) {
    if (i != j) {
      p_mat[i, j] <- wilcox.test(
        data_list[[labels[i]]],
        data_list[[labels[j]]],
        paired = TRUE
      )$p.value
    }
  }
}

write.csv(p_mat, "../results/wilcoxon_pvalues_ex1.csv", row.names = TRUE)

# Format numbers
format_p <- function(p) {
  if (is.na(p)) return("-")
  if (p < 1e-10) return("<1e-10")
  if (p < 1e-4)  return(format(p, scientific = TRUE, digits = 2))
  return(format(round(p, 3), nsmall = 3))
}

# Plot heatmap
png("../report/figures/wilcoxon_heatmap_ex1.png", width = 1500, height = 1200)

heat_cols <- colorRampPalette(c("#f7fbff", "#c6dbef", "#6baed6", "#2171b5", "#08306b"))(100)

plot_mat <- p_mat
plot_mat[is.na(plot_mat)] <- 0

par(mar = c(10, 10, 4, 2))
image(
  1:n, 1:n, t(plot_mat[n:1, ]),
  col = heat_cols,
  axes = FALSE,
  xlab = "",
  ylab = "",
  main = "Pairwise Wilcoxon p-values (Exercise 1.1)"
)

axis(1, at = 1:n, labels = labels, las = 2, cex.axis = 0.9)
axis(2, at = 1:n, labels = rev(labels), las = 2, cex.axis = 0.9)

for (k in 0:n) {
  abline(h = k + 0.5, col = "white", lwd = 1)
  abline(v = k + 0.5, col = "white", lwd = 1)
}

for (i in 1:n) {
  for (j in 1:n) {
    val <- p_mat[i, j]
    text(j, n - i + 1, labels = format_p(val), cex = 0.75)
  }
}

dev.off()

cat("Saved figure to ../report/figures/wilcoxon_heatmap_ex1.png\n")
cat("Saved matrix to ../results/wilcoxon_pvalues_ex1.csv\n")

