import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
import glob

sns.set(style="whitegrid")
plt.rcParams["font.family"] = "DejaVu Sans"
plt.rcParams["axes.labelsize"] = 12
plt.rcParams["axes.titlesize"] = 14
plt.rcParams["legend.fontsize"] = 10

data_dir = "for_graphics"

file_paths = glob.glob(os.path.join(data_dir, "*.csv"))
files = [os.path.basename(file) for file in file_paths]

if not files:
    print(f"В директории {data_dir} не найдено CSV файлов")
    exit(1)

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(18, 16), dpi=150)

color_palette = sns.color_palette("husl", len(files))
colors = {}
marker_symbols = ["o", "s", "^", "D", "X", "P", "*", "+", "x", "d", "|", "_"]
markers = {}

for i, file in enumerate(files):
    algorithm = file.replace(".csv", "")
    colors[algorithm] = (
        f"#{int(color_palette[i][0]*255):02x}{int(color_palette[i][1]*255):02x}{int(color_palette[i][2]*255):02x}"
    )
    markers[algorithm] = marker_symbols[i % len(marker_symbols)]

marker_size = 10
line_width = 2.5

alg_data = {}

for file in files:
    file_path = os.path.join(data_dir, file)

    if not os.path.exists(file_path):
        print(f"Файл {file_path} не найден")
        continue

    algorithm = file.replace(".csv", "")

    try:
        with open(file_path, "r") as f:
            first_line = f.readline().strip()

        header = 0 if "string_len" in first_line or ";" in first_line else None

        df = pd.read_csv(
            file_path,
            sep=";",
            header=header,
        )

        if header is None:
            df.columns = ["string_len", "working_time", "symbols_comparision"]

        if len(df.columns) >= 3:
            df = df.rename(
                columns={
                    df.columns[0]: "string_len",
                    df.columns[1]: "working_time",
                    df.columns[2]: "symbols_comparision",
                }
            )

        df["string_len"] = pd.to_numeric(df["string_len"], errors="coerce")
        df["working_time"] = pd.to_numeric(df["working_time"], errors="coerce")
        df["symbols_comparision"] = pd.to_numeric(
            df["symbols_comparision"], errors="coerce"
        )

        df = df.dropna()

        df = df.sort_values("string_len")

        alg_data[algorithm] = df

        ax1.plot(
            df["string_len"],
            df["working_time"],
            label=algorithm,
            color=colors[algorithm],
            marker=markers[algorithm],
            markersize=marker_size,
            linewidth=line_width,
            alpha=0.8,
        )

        ax2.plot(
            df["string_len"],
            df["symbols_comparision"],
            label=algorithm,
            color=colors[algorithm],
            marker=markers[algorithm],
            markersize=marker_size,
            linewidth=line_width,
            alpha=0.8,
        )

        z = np.polyfit(df["string_len"], df["working_time"], 2)
        p = np.poly1d(z)
        x_new = np.linspace(df["string_len"].min(), df["string_len"].max(), 100)
        y_new = p(x_new)
        ax1.plot(
            x_new,
            y_new,
            color=colors[algorithm],
            linestyle="--",
            alpha=0.6,
            linewidth=1.5,
        )

        z2 = np.polyfit(df["string_len"], df["symbols_comparision"], 2)
        p2 = np.poly1d(z2)
        y_new2 = p2(x_new)
        ax2.plot(
            x_new,
            y_new2,
            color=colors[algorithm],
            linestyle="--",
            alpha=0.6,
            linewidth=1.5,
        )
    except Exception as e:
        print(f"Ошибка при обработке файла {file}: {e}")

ax1.set_title(
    "Зависимость времени работы от длины строки", fontsize=20, fontweight="bold"
)
ax1.set_xlabel("Средняя длина строки", fontsize=18)
ax1.set_ylabel("Время работы (мс)", fontsize=18)
ax1.tick_params(axis="both", which="major", labelsize=14)
ax1.spines["top"].set_visible(False)
ax1.spines["right"].set_visible(False)

ax2.set_title(
    "Зависимость количества сравнений от длины строки", fontsize=20, fontweight="bold"
)
ax2.set_xlabel("Средняя длина строки", fontsize=18)
ax2.set_ylabel("Количество сравнений", fontsize=18)
ax2.tick_params(axis="both", which="major", labelsize=14)
ax2.spines["top"].set_visible(False)
ax2.spines["right"].set_visible(False)

handles, labels = ax1.get_legend_handles_labels()
legend_labels = {}

for label in labels:
    parts = label.split("_")
    if len(parts) >= 2:
        algorithm_type = " ".join(parts[:-1])
        data_type = parts[-1]
        legend_labels[label] = f"{algorithm_type.title()} ({data_type})"
    else:
        legend_labels[label] = label

new_labels = [legend_labels[label] for label in labels]

n_col = min(3, len(labels))
fig.legend(
    handles,
    new_labels,
    loc="lower center",
    bbox_to_anchor=(0.5, 0.02),
    ncol=n_col,
    fontsize=16,
    frameon=True,
    fancybox=True,
    shadow=True,
)

fig.suptitle(
    "Сравнение производительности алгоритмов сортировки",
    fontsize=24,
    fontweight="bold",
    y=0.98,
)

fig.text(
    0.5,
    0.08,
    "Пунктирные линии показывают аппроксимацию полиномом второй степени",
    ha="center",
    fontsize=10,
    style="italic",
)

plt.tight_layout(rect=[0, 0.1, 1, 0.95])
plt.savefig("sorting_algorithms_comparison.png", dpi=600, bbox_inches="tight")
plt.savefig("sorting_algorithms_comparison_large.png", dpi=900, bbox_inches="tight")
print(f"Построены графики для {len(files)} файлов из директории {data_dir}")
