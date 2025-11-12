# analyze_results.py
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# === точное значение из условия ===
S_exact = 0.25 * np.pi + 1.25 * np.arcsin(0.8) - 1.0

# === загрузка CSV ===
df = pd.read_csv("results.csv")

# === вычисление среднего и стандартного отклонения ===
grouped = df.groupby(["rect_type", "N"])["estimate"]
mean = grouped.mean().unstack(level=0)
std = grouped.std().unstack(level=0)

Ns = mean.index.values

# === График 1: приближённая площадь ===
plt.figure(figsize=(8, 5))
plt.plot(Ns, mean["narrow"], label="Узкая область", color="tab:orange")
plt.fill_between(Ns, mean["narrow"]-std["narrow"], mean["narrow"]+std["narrow"], alpha=0.2, color="tab:orange")

plt.plot(Ns, mean["wide"], label="Широкая область", color="tab:blue")
plt.fill_between(Ns, mean["wide"]-std["wide"], mean["wide"]+std["wide"], alpha=0.2, color="tab:blue")

plt.axhline(y=S_exact, color="black", linestyle="--", label="Точная площадь")

plt.xscale("log")
plt.xlabel("Количество точек N (лог. шкала)")
plt.ylabel("Оценка площади пересечения")
plt.title("Приближённая оценка площади методом Монте-Карло")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("area_vs_N.png", dpi=300)
plt.close()

# === График 2: относительная ошибка ===
rel_error = abs(mean - S_exact) / S_exact
plt.figure(figsize=(8, 5))
plt.plot(Ns, rel_error["narrow"], label="Узкая область", color="tab:orange")
plt.plot(Ns, rel_error["wide"], label="Широкая область", color="tab:blue")

plt.xscale("log")
plt.yscale("log")
plt.xlabel("Количество точек N (лог. шкала)")
plt.ylabel("Относительная ошибка |S̃ − S| / S")
plt.title("Относительная ошибка в зависимости от N")
plt.legend()
plt.grid(True, which="both", alpha=0.3)
plt.tight_layout()
plt.savefig("rel_error_vs_N.png", dpi=300)
plt.close()

print("Графики сохранены: area_vs_N.png, rel_error_vs_N.png")
