import pandas as pd
import openpyxl as xl
# Lista com os nomes das colunas

colunas = [
        "Metodo_Otimizacao",
        "Neuronios",
        "DropoutRate",
        "Acurácia de treino",
        "Acurácia de validação"
    ]

# Criar DataFrame vazio com essas colunas
df = pd.DataFrame(columns=colunas)

# Guardar no Excel
df.to_excel("train_data.xlsx", index=False)
