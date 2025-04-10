import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file
df = pd.read_csv('../data/miller_rabin_rand_comparision.csv') 

df[df.select_dtypes(include='number').columns] *= 1000  # Convert to milliseconds

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(df['Digits'], df['Random Time'], label='Randomized Algorithm', marker='o')
plt.plot(df['Digits'], df['Deterministic Time'], label='Deterministic Algorithm', marker='s')

# Log scale for y-axis
# plt.yscale('log')

# Customizing the plot
plt.title('Random vs Deterministic Base Choosing')
plt.xlabel('Digits')
plt.ylabel('Time (in ms)')
plt.grid(True)
plt.legend()
plt.tight_layout()


# Save plot
plt.savefig("../plots/comparision_time_plot_rand.png")

# Show plot
plt.show()
