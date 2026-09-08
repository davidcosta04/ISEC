import pandas as pd
import openpyxl as xl
# Lista com os nomes das colunas

colunas = [
        "Metodo_Otimizacao",
        "Neuronios",
        "DropoutRate",
        "Acurácia de teste",
        "Sensibilidade",
        "F1-Score",
        "AUC Arbutus",
        "AUC BlackBerry",
        "AUC HollyBerry",
        "AUC RaspBerry",
        "AUC YewBerry"
    ]

# Criar DataFrame vazio com essas colunas
df = pd.DataFrame(columns=colunas)

# Guardar no Excel
df.to_excel("test_data.xlsx", index=False)
