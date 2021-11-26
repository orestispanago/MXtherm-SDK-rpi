import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("output/a.csv", skiprows=7, header=None, delimiter=";")

plt.imshow(df, interpolation='none')
plt.colorbar(label="Temperature °C")
plt.show()