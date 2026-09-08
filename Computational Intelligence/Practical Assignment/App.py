import streamlit as st
import os

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'  # 0=all, 1=info, 2=warning, 3=error
os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'  # desativa optimizações oneDNN
from tensorflow.keras.models import load_model
from tensorflow.keras.preprocessing.image import img_to_array
import numpy as np
from PIL import Image
from tensorflow.keras.applications.efficientnet import preprocess_input
import warnings

warnings.filterwarnings("ignore")

model_path = 'b0_model.h5'
model = load_model(model_path)

img_size = 128

fruits = ['Arbutus', 'BlackBerry', 'HollyBerry', 'RaspBerry', 'YewBerry']

files = ['arb.jpg', 'bb.jpg', 'hb.jpg', 'rb.jpg', 'yb.jpg']


def preprocess_image(image, target_size):
    img = image.resize((target_size, target_size))
    img_array = img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0)
    img_array = preprocess_input(img_array)
    return img_array


# Personalização do layout da aplicação
st.set_page_config(
    page_title="Wild Berry Classification",
    page_icon="🫐",
    layout="wide",
)

st.markdown(
    """
    <style>
    /* Fundo da página */
    .stApp {
        background: linear-gradient(to right, #e0f7fa, #e8f5e9);
        color: #333333;
        font-family: 'Arial', sans-serif;
    }

    /* Título principal */
    .title {
        color: #00695c;
        font-size: 3rem;
        font-weight: bold;
        text-align: center;
        margin-bottom: 0.5rem;
    }

    /* Instruções */
    .instructions {
        color: #004d40;
        font-size: 1.2rem;
        text-align: center;
        margin-bottom: 2rem;
    }



    /* Caixa de resultado */
    .result {
        font-size: 1.5rem;
        font-weight: bold;
        color: #ffffff;
        text-align: center;
        padding: 15px;
        border-radius: 10px;
        background-color: #00796b;
        box-shadow: 2px 2px 10px rgba(0,0,0,0.2);
        margin-top: 20px;
    }

    span[data-testid="stBaseButton-secondary"]{color:white;}
    .type_result {
        font-size: 1.5rem;
        font-weight: bold;
        color: #ffffff;
        text-align: center;
        padding: 15px;
        border-radius: 10px;
        background-color: #00796b;
        box-shadow: 2px 2px 10px rgba(0,0,0,0.2);
        margin-top: 20px;
    }



    /* Upload de ficheiro */
    .stFileUploader > label {
        font-weight: bold;
        color: #000000; 
    }



    /* Botões */
    div.stButton > button:first-child {
        background-color: #00796b;
        color: white;
        font-size: 1rem;
        padding: 8px 20px;
        border-radius: 8px;
        border: none;
    }

    div.stButton > button:hover {
        background-color: #004d40;
        color: white;
    }

    div[data-testid="stFileUploaderFileName"],
    div[data-testid="stFileUploaderFileName"] * {
        color: black !important;
    }
    </style>
    """,
    unsafe_allow_html=True,
)

# Título
st.markdown("<div class='title'>Wild Berry Classification</div>", unsafe_allow_html=True)

cols = st.columns(len(fruits))  # Cria uma coluna por fruta

for idx, file in enumerate(files):
    try:
        img = Image.open(file)
        # img = img.resize((img_size, img_size))
        cols[idx].image(img, caption=fruits[idx], width=160)
    except FileNotFoundError:
        cols[idx].write(f"Imagem {file} não encontrada")

# Instruções
st.markdown(
    "<div class='instructions'>Upload an image of a wild berry to find out its name. "
    "The possible berries are: <strong>Arbutus (Strawberry Tree Fruit)</strong>, <strong>BlackBerry</strong>, <strong>HollyBerry</strong>, "
    "<strong>RaspBerry</strong>, <strong>YewBerry</strong>.</div>",
    unsafe_allow_html=True,
)

# Upload de imagem
uploaded_file = st.file_uploader("Select an image...", type=["jpg", "png", "jpeg"])

if uploaded_file is not None:

    image = Image.open(uploaded_file)
    st.image(image, caption="Imagem sent")

    st.write("Classifying image...")

    processed_image = preprocess_image(image, img_size)

    predictions = model.predict(processed_image)

    predicted_class_index = np.argmax(predictions)
    predicted_class = fruits[predicted_class_index]
    certainty = predictions[0][predicted_class_index] * 100

    if predicted_class == 'HollyBerry' or predicted_class == 'YewBerry':
        tipofruta = 'POISONOUS'
        b_color = '#c62828'
    else:
        tipofruta = 'NOT POISONOUS'
        b_color = '#2e7d32'

    st.markdown(
        f"""
        <div class='result'>
            Fruit name: <strong>{predicted_class}</strong><br>
            Certainty: <strong>{certainty:.2f}%</strong>
        </div>
        <div class='type_result' style='background-color: {b_color};'>
            <strong>{tipofruta.upper()}</strong><br>
        </div>
        """,
        unsafe_allow_html=True
    )