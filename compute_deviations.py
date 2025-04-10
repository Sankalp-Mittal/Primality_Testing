import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load the CSV file
df = pd.read_csv('data/miller_rabin_deviation.csv')  # replace with your actual file name/path

df[df.select_dtypes(include='number').columns] *= 1000  # Convert to milliseconds
df['digits'] = df['digits']//1000

# define the x% thresholds
x_values = np.arange(0, 201, 10)  # [0, 10, 20, ..., 200]

# create a table to fill
result = pd.DataFrame(index=x_values)

# group by each input size (digits)
for digits, group in df.groupby('digits'):
    mean_time = group['time'].mean()
    # for each x%, calculate percentage of times it exceeds (1 + x%) of mean
    counts = [(group['time'] > (1 + x/100) * mean_time).mean() * 100 for x in x_values]
    result[digits] = counts

# sort columns by digits (ascending input size)
result = result.sort_index(axis=1)
result.index.name = 'x'

# plot tables
x_vals = result.index

for digits in result.columns:
    y_vals = result[digits]

    plt.figure(figsize=(6, 4))
    plt.plot(x_vals, y_vals, marker='o')
    plt.title(f"Deviation from Mean Time for Input Size {digits}")
    plt.xlabel("x (%) above average")
    plt.ylabel("% of trials exceeding (1 + x%) * average")
    plt.grid(True)
    plt.tight_layout()
    filename = f"plots/deviation_plots/deviation_digits_{digits}.png"
    plt.savefig(filename)
    plt.show()
    
    plt.close()  # Close the plot to avoid memory issues