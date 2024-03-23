#!/bin/bash

# Encontrando todos os arquivos .v no diretório atual
for arquivo in *.v; do
    # Verificando se a variável 'arquivo' não está vazia para evitar execução com '*.v' quando não houver arquivos .v
    if [[ -f "$arquivo" ]]; then
        # Executando o comando para cada arquivo encontrado
        ./getValuesfromVerilog "$arquivo"
    else
        echo "Nenhum arquivo Verilog (.v) encontrado no diretório atual."
        break # Sair do loop se nenhum arquivo .v for encontrado
    fi
done

