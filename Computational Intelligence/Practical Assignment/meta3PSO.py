
from keras.src.applications.efficientnet import EfficientNetB0
import os

import pyswarms as ps

os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'
import numpy as np
from tensorflow.keras.preprocessing.image import load_img, img_to_array
from sklearn.preprocessing import LabelEncoder

from tensorflow.keras.layers import GlobalAveragePooling2D, Dense, Dropout
from tensorflow.keras.models import Sequential
import tensorflow as tf
from sklearn.metrics import confusion_matrix, classification_report, roc_auc_score, roc_curve, f1_score, recall_score, precision_score
from tensorflow.keras.optimizers import Adam
from tensorflow.keras import layers
from tensorflow.keras.layers import Conv2D
from tensorflow.keras.applications.efficientnet import preprocess_input
fruits = ['Arbutus','BlackBerry','HollyBerry','RaspBerry','YewBerry']
img_size = 128
le = LabelEncoder()

train_dir = 'C:/Users/david/Desktop/ISEC/3Ano/1Semestre/IC/TP/train'
val_dir = 'C:/Users/david/Desktop/ISEC/3Ano/1Semestre/IC/TP/val'
test_dir = 'C:/Users/david/Desktop/ISEC/3Ano/1Semestre/IC/TP/test'


def guardar_resultados(caminho_excel, nova_linha):
    import pandas as pd
    df_existente = pd.read_excel(caminho_excel)
    df_novo = pd.DataFrame([nova_linha])
    df_final = pd.concat([df_existente, df_novo], ignore_index=True)
    df_final.to_excel(caminho_excel, index=False)
def load_images(img_folder):
    images = []
    labels = []

    if img_folder == train_dir or img_folder == val_dir or img_folder == test_dir:
        for fruit in fruits:
            fruit_folder = os.path.join(img_folder, fruit)
            filenames = os.listdir(fruit_folder)
            for filename in filenames:
                img_path = os.path.join(fruit_folder, filename)
                img = load_img(img_path, target_size=(img_size, img_size))
                img_array = img_to_array(img)
                images.append(img_array)
                labels.append(fruit)

    return np.array(images), np.array(labels)


train_images, train_labels = load_images(train_dir)
val_images, val_labels = load_images(val_dir)
test_images, test_labels = load_images(test_dir)

train_images = train_images.astype('float32')
val_images = val_images.astype('float32')
test_images = test_images.astype('float32')

train_images = preprocess_input(train_images)
val_images = preprocess_input(val_images)
test_images = preprocess_input(test_images)

train_labels = le.fit_transform(train_labels)
val_labels = le.transform(val_labels)
test_labels = le.transform(test_labels)



#Funcao de treinar a rede
def train_network_pso(hyperparameters):

    dense = int(hyperparameters[0])
    dropout_rate = hyperparameters[1]

    data_augmentation = tf.keras.Sequential([
        layers.RandomFlip("horizontal"),
        layers.RandomRotation(0.1),
        layers.RandomZoom(0.1),
        layers.RandomTranslation(0.1, 0.1)
    ])




    base_model = EfficientNetB0(weights='imagenet', include_top=False, input_shape=(img_size, img_size, 3))
    base_model.trainable = False  # Congelar o modelo base

    model = Sequential([
        data_augmentation,
        base_model,
        Dense(128, activation="relu"),
        Dropout(0.2),
        Dense(len(fruits), activation='softmax')
    ])



    model.compile(
        optimizer=Adam(learning_rate=0.001),
        loss='sparse_categorical_crossentropy',
        metrics=['accuracy']
    )



    history = model.fit(train_images, train_labels, epochs=30,validation_data=(val_images, val_labels),
              batch_size=32,shuffle=True,verbose = 1)

    val_loss, val_accuracy = model.evaluate(val_images, val_labels, verbose=1)

    metodo_otimizacao = 'PSO'

    dados = {"Metodo_Otimizacao": metodo_otimizacao,
             "Neuronios": int(hyperparameters[0]),
             "DropoutRate": hyperparameters[1],
             "Acurácia de treino": history.history['accuracy'][-1],
             "Acurácia de validação": -val_accuracy + 1
             }

    guardar_resultados("train_results.xlsx", dados)



    return model,1 - val_accuracy






def fitness_function(x):
    global global_best_val, global_best_params, final_model

    global_best_val = float("inf")

    n_particles = x.shape[0]
    val_arr = []

    for i in range(n_particles):
        hyperparameters = x[i]
        model,val = train_network_pso(hyperparameters)
        val_arr.append(val)


    # Melhor custo da iteração
    best_idx = np.argmin(val_arr)
    best_val = val_arr[best_idx]
    best_hyperparams = x[best_idx]

    print("\n======== Iteração do PSO ========")
    print(f"Melhor loss nesta iteração: {best_val:.4f}")
    print("Hyperparâmetros correspondentes:")
    print(f"Neurónios camada 1: {int(best_hyperparams[0])}")
    print(f"Dropout: {best_hyperparams[1]:.2f}")
    print("================================\n")

    if best_val < global_best_val:
        global_best_val = best_val
        global_best_params = best_hyperparams
        final_model = model

        print("\n🌟 >>> NOVO MELHOR GLOBAL ENCONTRADO! <<< 🌟")
        print(f"Melhor ( 1- Validation Accuracy:) {global_best_val:.4f}")
        print(f"Melhor Dense: {int(global_best_params[0])}")
        print(f"Melhor Dropout: {global_best_params[1]:.2f}")

    return np.array(val_arr)

def train_PSO():
    # Configurações do PSO
    bounds = (np.array([64,0.2]), np.array([128,0.4]))
    options = {'c1': 0.5, 'c2': 0.5, 'w': 0.9}

    # Inicializar o otimizador
    optimizer = ps.single.GlobalBestPSO(n_particles=5, dimensions=2, options=options, bounds=bounds)

    # Executar o PSO para encontrar os melhores hiperparâmetros

    cost, best_pos = optimizer.optimize(
        fitness_function,
        iters=5,
        verbose=False
    )


    best_dense = int(best_pos[0])
    best_dropout_rate = best_pos[1]

    print(f"Melhores Hiperparâmetros Encontrados:")
    print(f"Neurónios na Primeira Camada: {best_dense}")
    print(f"Taxa de Dropout: {best_dropout_rate}")

    fm = final_model

    return fm,best_dense,best_dropout_rate

# Avaliar o modelo no conjunto de teste

def test_network_pso(final_model,best_dense,best_dropout_rate):

    test_loss, test_accuracy = final_model.evaluate(test_images, test_labels, verbose=0)

    # Previsões e métricas adicionais
    y_probs = final_model.predict(test_images)
    y_pred = np.argmax(y_probs, axis=1)

    confusion = confusion_matrix(test_labels, y_pred)
    print("Matriz de Confusão:")
    print(confusion)

    report = classification_report(test_labels, y_pred, target_names=fruits, zero_division=1)
    print("Relatório de Classificação:")
    print(report)

    auc_arr=[]

    # Calcular a AUC para cada classe
    for i in range(len(fruits)):
        auc = roc_auc_score(test_labels == i, y_probs[:, i])
        print(f"AUC para a fruta {fruits[i]}: {auc:.2f}")
        auc_arr.append(auc)


    sensibilidade = recall_score(test_labels, y_pred, average='macro')
    f1 = f1_score(test_labels, y_pred, average='macro')


    metodo_otimizacao = 'PSO'

    dados_teste = {
        "Metodo_Otimizacao": metodo_otimizacao,
        "Neuronios":best_dense,
        "DropoutRate":best_dropout_rate,
        "Acurácia de teste":test_accuracy,
        "Sensibilidade":sensibilidade,
        "F1-Score":f1,
        "AUC Arbutus":auc_arr[0],
        "AUC BlackBerry":auc_arr[1],
        "AUC HollyBerry":auc_arr[2],
        "AUC RaspBerry":auc_arr[3],
        "AUC YewBerry":auc_arr[4]
    }

    guardar_resultados("test_results.xlsx",dados_teste)


