# Generates the summary table for the exercise 1.1, computing the average deviance
# std deviance, and copmutation time across neighborhoods, pivoting rules, initialization methods
# using exercise1_results.txt generated earlier from run_experiments_exercise1.sh

results <- read.table("../results/exercise1_results.txt", header = TRUE, stringsAsFactors = FALSE)
best_known_tbl <- read.table("../best_known/best_known.txt", header = FALSE, stringsAsFactors = FALSE)
colnames(best_known_tbl) <- c("Instance", "BestKnown")

# Merge results with best-known values
merged <- merge(results, best_known_tbl, by = "Instance")

# Deviation convention used in the report:
# 100 * (best-known - cost) / best-known
merged$Deviation <- 100 * (merged$BestKnown - merged$Cost) / merged$BestKnown

# Build algorithm label
merged$Algorithm <- paste(merged$Init, merged$Pivot, merged$Neighborhood)

# Aggregate statistics
algorithms <- unique(merged$Algorithm)

mean_dev <- sapply(algorithms, function(a) mean(merged$Deviation[merged$Algorithm == a]))
std_dev  <- sapply(algorithms, function(a) sd(merged$Deviation[merged$Algorithm == a]))
tot_time <- sapply(algorithms, function(a) sum(merged$Time[merged$Algorithm == a]))

summary_out <- data.frame(
  Algorithm = algorithms,
  AvgDeviation = mean_dev,
  StdDeviation = std_dev,
  TotalTime = tot_time,
  stringsAsFactors = FALSE
)

desired_order <- c(
  "cw best exchange",
  "cw best insert",
  "cw best transpose",
  "cw first exchange",
  "cw first insert",
  "cw first transpose",
  "random best exchange",
  "random best insert",
  "random best transpose",
  "random first exchange",
  "random first insert",
  "random first transpose"
)

summary_out$Algorithm <- factor(summary_out$Algorithm, levels = desired_order)
summary_out <- summary_out[order(summary_out$Algorithm), ]

# output
cat("\nExercise 1.1 summary table\n\n")
for (i in 1:nrow(summary_out)) {
  cat(sprintf(
    "%-24s  %.4f  %.4f  %.4f\n",
    as.character(summary_out$Algorithm[i]),
    summary_out$AvgDeviation[i],
    summary_out$StdDeviation[i],
    summary_out$TotalTime[i]
  ))
}

# Save CSV
write.csv(summary_out, "../results/summary_ex1.csv", row.names = FALSE)
