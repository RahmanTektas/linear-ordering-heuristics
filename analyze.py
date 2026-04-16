import matplotlib.pyplot as plt
import scipy.stats as stats
import os
import math

# ---------------------------------------------------------
# 1. Read best-known values
# ---------------------------------------------------------
best_known = {}
try:
    with open("best_known/best_known.txt", "r") as f:
        for line in f:
            parts = line.split()
            if len(parts) >= 2:
                instance_name = parts[0].strip()
                best_cost = float(parts[1])
                best_known[instance_name] = best_cost
except FileNotFoundError:
    print("Error: Please make sure 'best_known/best_known.txt' is in the correct folder!")
    exit()

# ---------------------------------------------------------
# 2. Prepare data structures
# ---------------------------------------------------------
rpd_trans, rpd_exch, rpd_ins = [], [], []

# Store Wilcoxon data by instance name for proper pairing
random_best_insert_by_instance = {}
cw_best_insert_by_instance = {}

# Summary stats by algorithm
summary_stats = {}

missing_best_known = set()

# ---------------------------------------------------------
# Helper functions
# ---------------------------------------------------------
def mean(values):
    if not values:
        return 0.0
    return sum(values) / len(values)

def sample_std(values):
    n = len(values)
    if n <= 1:
        return 0.0
    m = mean(values)
    variance = sum((x - m) ** 2 for x in values) / (n - 1)
    return math.sqrt(variance)

# ---------------------------------------------------------
# 3. Read experimental results
# ---------------------------------------------------------
try:
    with open("exercise1_results.txt", "r") as f:
        lines = f.readlines()
except FileNotFoundError:
    print("Error: Please make sure 'exercise1_results.txt' is in the correct folder!")
    exit()

for line in lines:
    if "Cost" in line or "Time" in line or line.strip() == "":
        continue

    clean_line = line.replace('|', ' ')
    parts = clean_line.split()

    if len(parts) < 6:
        continue

    instance = parts[0].strip()
    if "/" in instance:
        instance = os.path.basename(instance)

    init = parts[1].strip()
    pivot = parts[2].strip()
    neigh = parts[3].strip()

    try:
        cost = float(parts[4])
        time = float(parts[5])
    except ValueError:
        continue

    if instance not in best_known:
        missing_best_known.add(instance)
        continue

    best = best_known[instance]
    rpd = ((best - cost) / best) * 100.0

    # --- A. Summary table grouping ---
    algo_name = f"{init} {pivot} {neigh}"
    if algo_name not in summary_stats:
        summary_stats[algo_name] = {"rpds": [], "times": []}

    summary_stats[algo_name]["rpds"].append(rpd)
    summary_stats[algo_name]["times"].append(time)

    # --- B. Boxplot data: CW + best + neighborhood comparison ---
    if init == "cw" and pivot == "best":
        if neigh == "transpose":
            rpd_trans.append(rpd)
        elif neigh == "exchange":
            rpd_exch.append(rpd)
        elif neigh == "insert":
            rpd_ins.append(rpd)

    # --- C. Wilcoxon data: random vs CW for best+insert, paired by instance ---
    if pivot == "best" and neigh == "insert":
        if init in ("random", "rand"):
            random_best_insert_by_instance[instance] = rpd
        elif init == "cw":
            cw_best_insert_by_instance[instance] = rpd

# ---------------------------------------------------------
# 4. Output generation
# ---------------------------------------------------------

# ---- Boxplot ----
print("Generating Boxplot...")
data_to_plot = [rpd_trans, rpd_exch, rpd_ins]

plt.figure(figsize=(8, 6))
plt.boxplot(data_to_plot, tick_labels=['Transpose', 'Exchange', 'Insert'])
plt.ylabel('Relative Percentage Deviation (%)')
plt.title('Neighborhood Comparison (CW Init, Best Pivot)')
plt.savefig('boxplot.png')
print("-> Boxplot saved successfully as 'boxplot.png'!")

# ---- Wilcoxon test ----
print("\n--- Wilcoxon Test: Random vs CW (best + insert) ---")

common_instances = sorted(
    set(random_best_insert_by_instance.keys()) &
    set(cw_best_insert_by_instance.keys())
)

if len(common_instances) > 0:
    rpd_random = [random_best_insert_by_instance[inst] for inst in common_instances]
    rpd_cw = [cw_best_insert_by_instance[inst] for inst in common_instances]

    if len(rpd_random) == len(rpd_cw):
        stat, p_value = stats.wilcoxon(rpd_random, rpd_cw)

        avg_random = mean(rpd_random)
        avg_cw = mean(rpd_cw)
        med_random = stats.scoreatpercentile(rpd_random, 50)
        med_cw = stats.scoreatpercentile(rpd_cw, 50)

        print(f"Paired instances: {len(common_instances)}")
        print(f"P-value: {p_value}")
        print(f"Average RPD Random: {avg_random:.4f}")
        print(f"Average RPD CW:     {avg_cw:.4f}")
        print(f"Median RPD Random:  {med_random:.4f}")
        print(f"Median RPD CW:      {med_cw:.4f}")

        if p_value < 0.05:
            if avg_cw < avg_random:
                print("Conclusion: There is a statistically significant difference, and CW is better.")
            elif avg_random < avg_cw:
                print("Conclusion: There is a statistically significant difference, and Random is better.")
            else:
                print("Conclusion: There is a statistically significant difference, but the averages are equal.")
        else:
            print("Conclusion: There is no statistically significant difference.")
    else:
        print("Warning: Paired samples could not be aligned correctly.")
else:
    print("Warning: No common instances found for Random vs CW comparison.")

# ---- Missing best-known warning ----
if missing_best_known:
    print("\nWarning: The following instances were present in the results file but missing from best_known.txt:")
    for inst in sorted(missing_best_known):
        print(f"  - {inst}")

# ---- Final summary table ----
print("\n" + "=" * 90)
print(" FINAL RESULTS TABLE (For your Report)")
print("=" * 90)
print(f"{'Algorithm Variant':<28} | {'Average RPD (%)':<15} | {'Std RPD':<12} | {'Total Time (s)'}")
print("-" * 90)

for algo, data in sorted(summary_stats.items()):
    if len(data["rpds"]) > 0:
        avg_rpd = mean(data["rpds"])
        std_rpd = sample_std(data["rpds"])
        total_time = sum(data["times"])
        print(f"{algo:<28} | {avg_rpd:<15.4f} | {std_rpd:<12.4f} | {total_time:.4f}")

print("=" * 90 + "\n")