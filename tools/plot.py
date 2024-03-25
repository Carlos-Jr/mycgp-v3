import os
import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
# plt.errorbar(grouped_df.index, grouped_df['entropy'], yerr=std_df['entropy'], fmt='-')

if len(sys.argv) == 3:
    filenames = sys.argv[1]
    prop = sys.argv[2]
else:
    print("Usage: script.py <filenames> <prop>")
    sys.exit(1)

csv_files = [file for file in os.listdir('.') if file.startswith(filenames+"-"+prop)]

if not csv_files:
    print("Erro: nenhum arquivo encontrado.")
    sys.exit(1)

all_props = {"entropy":'g', "size":'r', "depth":'b'}
other_props = {"entropy":'g', "size":'r', "depth":'b'}
del other_props[prop]

print("Arquivos CSV encontrados:")
for file in csv_files:
    print(file)

df_list = []
for file in csv_files:
    df = pd.read_csv(file)
    df_list.append(df)

consolidated_df = pd.concat(df_list)
grouped_df = consolidated_df.groupby('generation')[['entropy', 'size', 'depth']].mean()
std_df = consolidated_df.groupby('generation')[['entropy', 'size', 'depth']].std()

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8), sharex=True)
grouped_df[prop].plot(ax=ax1, color=all_props[prop],)
ax1.set_ylabel(prop.capitalize(),color=all_props[prop])
ax1.set_xlabel('Generations')
ax1.grid(True)

grouped_df[list(other_props.keys())[0]].plot(kind='line', color=other_props[list(other_props.keys())[0]], ax=ax2)
ax2.set_ylabel(list(other_props.keys())[0].capitalize(), color=other_props[list(other_props.keys())[0]])
ax2.grid(True)
ax3 = ax2.twinx()
grouped_df[list(other_props.keys())[1]].plot(kind='line', color=other_props[list(other_props.keys())[1]], ax=ax3)
ax3.set_ylabel(list(other_props.keys())[1].capitalize(), color=other_props[list(other_props.keys())[1]])
ax2.set_xlabel('Generations')
plt.subplots_adjust(hspace=0.1)
plt.savefig(filenames+"-"+prop+"graphs.png")
