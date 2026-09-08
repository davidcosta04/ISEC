from matplotlib import pyplot as plt
from numpy.ma import product
from tensorflow.keras.utils import load_img, img_to_array
from tensorflow.keras.applications.efficientnet import preprocess_input
from sklearn.preprocessing import LabelEncoder
from sklearn.metrics import confusion_matrix, classification_report, roc_auc_score, roc_curve, f1_score, recall_score, precision_score
import seaborn as sns
import tensorflow as tf

import numpy as np
import os
import random
from tensorflow.keras.applications import EfficientNetB0
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv2D, MaxPooling2D, GlobalAveragePooling2D, Dense, Dropout
from tensorflow.keras.optimizers import Adam
from tensorflow.keras import layers

os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'

fruits = ['Arbutus','BlackBerry','HollyBerry','RaspBerry','YewBerry']
img_size = 128
le = LabelEncoder()

train_dir = 'C:/Users/david/Desktop/ISEC/3Ano/1Semestre/IC/TP/train'
val_dir = 'C:/Users/david/Desktop/ISEC/3Ano/1Semestre/IC/TP/val'
test_dir = 'C:/Users/david/Desktop/ISEC/3Ano/1Semestre/IC/TP/test'


def save_results(caminho_excel, nova_linha):
    import pandas as pd
    import os
    if os.path.exists(caminho_excel):
        df_existente = pd.read_excel(caminho_excel)
        df_novo = pd.DataFrame([nova_linha])
        df_final = pd.concat([df_existente, df_novo], ignore_index=True)
    else:
        df_final = pd.DataFrame([nova_linha])
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


def train_network_b0():
    data_augmentation = tf.keras.Sequential([
        layers.RandomFlip("horizontal"),
        layers.RandomRotation(0.1),
        layers.RandomZoom(0.1),
        layers.RandomTranslation(0.1, 0.1)
    ])


    best_val_accuracy = 0

    best_dense = None
    best_dropout = None




    param_grid = {
        'dense': [64, 128, 256],
        'dropout': [0.2, 0.3, 0.4]
    }



    for dense, dropout_rate in product(param_grid['dense'], param_grid['dropout']):


        base_model = EfficientNetB0(
            weights='imagenet',
            include_top=False,
            input_shape=(img_size, img_size, 3)
        )
        base_model.trainable = False

        model = Sequential([
            data_augmentation,
            base_model,
            GlobalAveragePooling2D(),
            Dense(dense, activation="relu"),
            Dropout(dropout_rate),
            Dense(len(fruits), activation='softmax')
        ])

        model.compile(
            optimizer=Adam(learning_rate=0.001),
            loss='sparse_categorical_crossentropy',
            metrics=['accuracy']
        )

        history = model.fit(
            train_images, train_labels,
            epochs=30,
            validation_data=(val_images, val_labels),
            batch_size=32,
            shuffle=True,
            verbose=1
        )

        val_loss, val_accuracy = model.evaluate(val_images, val_labels, verbose=1)
        print(f"Validação: Loss={val_loss:.4f}, Accuracy={val_accuracy:.4f}")

        dados = {
            "Metodo_Otimizacao": "GridSearch",
            "Neuronios": int(dense),
            "DropoutRate": dropout_rate,
            "Acurácia de treino": history.history['accuracy'][-1],
            "Acurácia de validação": val_accuracy
        }

        save_results("train_results.xlsx", dados)

        if val_accuracy > best_val_accuracy:
            best_val_accuracy = val_accuracy
            best_dense = dense
            best_dropout = dropout_rate

    return model, best_dense, best_dropout


def test_network_b0(final_model,best_dense,best_dropout):
    test_images, test_labels = load_images(test_dir)
    test_images = test_images.astype('float32')
    test_images = preprocess_input(test_images)
    test_labels = le.transform(test_labels)

    test_loss, test_accuracy = final_model.evaluate(test_images, test_labels, verbose=0)

    # Previsões e métricas adicionais
    y_probs = final_model.predict(test_images)
    y_pred = np.argmax(y_probs, axis=1)



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

    cm = confusion_matrix(test_labels, y_pred)


    plt.figure(figsize=(6, 4))
    sns.heatmap(cm,annot=True,cmap="Blues",fmt="d",xticklabels=["Arbutus", "BlackBerry","HollyBerry","RaspBerry","YewBerry"],yticklabels=["Arbutus", "BlackBerry","HollyBerry","RaspBerry","YewBerry"])


    plt.xlabel("Predict")
    plt.ylabel("Real")
    plt.title("Matriz de Confusão")
    plt.show()

    metodo_otimizacao = 'GridSearch'

    dados_teste = {
        "Metodo_Otimizacao": metodo_otimizacao,
        "Neuronios": best_dense,
        "DropoutRate": best_dropout,
        "Acurácia de teste": test_accuracy,
        "Sensibilidade": sensibilidade,
        "F1-Score": f1,
        "AUC Arbutus": auc_arr[0],
        "AUC BlackBerry": auc_arr[1],
        "AUC HollyBerry": auc_arr[2],
        "AUC RaspBerry": auc_arr[3],
        "AUC YewBerry": auc_arr[4]
    }

    save_results("test_results.xlsx", dados_teste)


def save_model(final_model):

    final_model.save('b0_model.h5')
