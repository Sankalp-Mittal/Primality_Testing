import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file
df = pd.read_csv('data/miller_rabin_rand_comparision.csv')  # replace with your actual file name/path

with open('copy_stuff.txt', 'w') as file:
    for _,row in df.iterrows():
        # print(row)  # Print the entire row for debugging
        # Extract the values from the row
        digits = row['Digits']
        random_time = row['Random Time']
        deterministic_time = row['Deterministic Time']

        # Print to file
        file.write(f"{digits} & {1000*random_time} & {1000*deterministic_time} \\\\\n")
        file.write("\\hline\n")

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
plt.savefig("plots/comparision_time_plot_rand.png")

# Show plot
plt.show()
