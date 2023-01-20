import PySimpleGUI as sg
import pandas as pd
import matplotlib.pyplot as plt

def graph_data(x,y):
    plt.plot(x,y)
    plt.show()
# Open file dialog to select text file
file_path = sg.popup_get_file('Select text file', default_extension='txt')

# Read the text file into a pandas dataframe
df = pd.read_csv(file_path)

# Separate the data into x and y groups
x = df.iloc[:,0]
y = df.iloc[:,1]

# Display the table in a window
graph_data(x,y)
