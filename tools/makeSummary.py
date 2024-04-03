import csv

# Lista de benchmarks
benchmarks = [
    "03-adder", "04-adder", "05-adder", "06-adder", "08-adder", "alu1_split",
    "b01_C", "b01_opt_C", "b02_C", "b02_opt_C", "b06_opt_C", "cm138a", "cm151a", "cm152a", "cm163a",
    "cm42a", "cm82a", "con1", "dc1", "dc2", "decod", "dk27", "m1", "newapla1", "newcond",
    "rd53", "s208", "s27_split", "sqrt8", "sqrt8ml", "squar5", "t3", "tcon", "wim", "x2", "xor5"
]

# Propriedades
props = ["depth", "size", "entropy"]

# Abrir arquivo de saída
with open('benchmarks_summary.csv', 'w', newline='') as csvfile:
    # Definir o cabeçalho do arquivo CSV
    fieldnames = ['benchmark', 'focus', 'initial_entropy', 'initial_size', 'initial_depth',
                    'final_entropy', 'final_size', 'final_depth', 'entropy_improvement',
                    'size_improvement', 'depth_improvement']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    writer.writeheader()

    # Iterar sobre cada benchmark e propriedade (foco)
    for benchmark in benchmarks:
        for focus in props:
            filename = f"./results/overview/{benchmark}-{focus}-mean.csv"
            try:
                with open(filename, 'r') as file:
                    reader = csv.DictReader(file)
                    initial_row = None
                    final_row = None
                    for row in reader:
                        if row['generation'] == '0':
                            initial_row = row
                        elif row['generation'] == '69999':
                            final_row = row
                    
                    if initial_row and final_row:
                        # Calcular as melhorias para cada propriedade
                        improvements = {f'{prop}_improvement': (float(final_row[prop]) - float(initial_row[prop])) / float(initial_row[prop]) 
                                        for prop in props}
                        
                        # Escrever linha no arquivo CSV para o foco atual
                        writer.writerow({
                            'benchmark': benchmark,
                            'focus': focus,
                            'initial_entropy': initial_row['entropy'],
                            'initial_size': initial_row['size'],
                            'initial_depth': initial_row['depth'],
                            'final_entropy': final_row['entropy'],
                            'final_size': final_row['size'],
                            'final_depth': final_row['depth'],
                            **improvements
                        })
            except FileNotFoundError:
                print(f"Arquivo não encontrado: {filename}")

print("Resumo dos benchmarks gerado com sucesso.")
