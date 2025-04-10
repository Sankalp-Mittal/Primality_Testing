import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file
df = pd.read_csv('data/miller_rabin_deviation.csv')  # replace with your actual file name/path

# with open('copy_stuff.txt', 'w') as file:
#     for _,row in df.iterrows():
#         # print(row)  # Print the entire row for debugging
#         # Extract the values from the row
#         digits = row['Digits']
#         random_time = row['Random Time']

#         # Print to file
#         file.write(f"{digits} & {1000*random_time} \\\\\n")
#         file.write("\\hline\n")

df[df.select_dtypes(include='number').columns] *= 1000  # Convert to milliseconds
df['digits'] = df['digits']//1000


stats = df.groupby('digits')['time'].agg(['mean', 'std']).reset_index()

print(stats)

# 2. Plot discrete points with error bars
plt.errorbar(stats['digits'], stats['mean'],
             yerr=stats['std'],
             fmt='o',             # 'o' means circular markers only (no line)
             capsize=5,           # adds small horizontal lines at the ends of the error bars
             ecolor='gray',       # color of error bars
             markerfacecolor='blue',
             label='Mean ± Std Dev')

plt.xlabel('Number of Digits')
plt.ylabel('Time Taken (in ms)')
plt.title('Time vs Number of Digits')
plt.grid(True)
plt.legend()
# plt.show()

# # Save plot
plt.savefig("plot/deviation_plot.png")

# # Show plot
plt.show()
