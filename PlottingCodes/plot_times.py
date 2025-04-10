import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file
df = pd.read_csv('../data/miller_rabin_benchmark.csv')  # replace with your actual file name/path

df[df.select_dtypes(include='number').columns] *= 1000  # Convert to milliseconds


# Plotting
plt.figure(figsize=(10, 6))
plt.plot(df['Digits'], df['Random Time'], label='Randomized Algorithm', marker='o')

# Log scale for y-axis
# plt.yscale('log')

# Customizing the plot
plt.title('Expected Runtime of Miller-Rabin Algorithm')
plt.xlabel('Digits')
plt.ylabel('Time in (ms)')
plt.grid(True)
plt.legend()
plt.tight_layout()


# Save plot
plt.savefig("../plot/computation_time_plot.png")

# Show plot
plt.show()
