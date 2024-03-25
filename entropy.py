import json
from math import log2

def calcular_entropia(entradas, saidas):
    prob_entradas = {valor: entradas.count(valor)/len(entradas) for valor in set(entradas)}
    prob_saidas = {valor: saidas.count(valor)/len(saidas) for valor in set(saidas)}
    
    Hx = -sum(p * log2(p) for p in prob_entradas.values())
    Hy = -sum(p * log2(p) for p in prob_saidas.values())
    
    return Hx - Hy

def somar_entropias(arquivo_json):
    # Abrir e ler o arquivo JSON
    with open(arquivo_json, 'r') as arquivo:
        portas_logicas = json.load(arquivo)
    
    # Calcular a entropia de cada porta lógica e somar todas
    soma_entropias = sum(calcular_entropia(porta['input_states'], porta['output_states']) for porta in portas_logicas)
    
    return soma_entropias

# Caminho para o arquivo JSON, substitua 'caminho_para_o_arquivo.json' pelo caminho real do arquivo
caminho_arquivo = 'log.json'
soma_entropias = somar_entropias(caminho_arquivo)
print(f"A soma das entropias de todas as portas lógicas é: {soma_entropias}")
