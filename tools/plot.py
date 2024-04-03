import os
import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

props = ['entropy', 'depth', 'size']
for prop in props:
    folder_path = sys.argv[1]
    filenames = sys.argv[2]

    csv_files = [file for file in os.listdir(folder_path) if file.startswith(filenames+"-"+prop)]

    if not csv_files:
        print("Erro: nenhum arquivo encontrado.")
        continue

    print("Arquivos CSV encontrados:")
    for file in csv_files:
        print(folder_path+"/"+file)

    df_list = []
    for file in csv_files:
        df = pd.read_csv(folder_path+"/"+file)
        df_list.append(df)

    consolidated_df = pd.concat(df_list)

    if not os.path.exists(os.path.join(folder_path, "overview")):
        os.makedirs(os.path.join(folder_path, "overview"))

    grouped_df = consolidated_df.groupby('generation')[['entropy', 'size', 'depth']].mean()
    std_df = consolidated_df.groupby('generation')[['entropy', 'size', 'depth']].std()

    all_props = {"entropy":'g', "size":'r', "depth":'b'}
    other_props = {"entropy":'g', "size":'r', "depth":'b'}
    del other_props[prop]

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
    plt.savefig(folder_path+"/overview/"+filenames+"-"+prop+"-graphs.png")

    grouped_df.loc[[0, 69999]].to_csv(f"{folder_path}/overview/{filenames}-{prop}-mean.csv")