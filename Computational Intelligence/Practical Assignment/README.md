# Computational Intelligence — Forest Berry Classifier (Fase III)

## Overview
This project implements a **Convolutional Neural Network (CNN)** to classify five species of forest berries — **Arbutus (medronho), Holly Berry, BlackBerry, RaspBerry, and YewBerry** — distinguishing poisonous from non-poisonous species and alerting the user accordingly. A dataset of 2,000 images per class was used.

## Methodology
A **custom CNN** was first built and trained from scratch. The final model, however, uses **EfficientNetB0**, pre-trained on ImageNet, as its backbone. The convolutional layers were kept frozen during training (transfer learning), while additional **dense and dropout layers** were added on top to adapt the network to this specific 5-class problem.

Images were resized to **128x128 pixels** for performance reasons, even though the base model supports 224x224x3 (RGB) inputs. **Data augmentation** was applied to improve generalization and reduce the risk of overfitting.

The dataset split was adjusted from an initial 15% test set to a final **65% train / 15% validation / 20% test** ratio, to ensure a more robust evaluation.

**Hyperparameter optimization** (number of neurons in the dense layer and dropout rate) was performed in two stages: an initial **Grid Search**, followed by **Particle Swarm Optimization (PSO)**, testing multiple configurations to identify the setup that yielded the best validation accuracy.

## Evaluation
The model was evaluated across the train, validation, and test sets to ensure the results reflected the network's true generalization capability. The configuration with the highest validation accuracy was applied to the independent test set and saved to file. Performance was assessed using **accuracy, F1-score, sensitivity, and AUC**, along with **confusion matrix analysis**, to provide a complete picture of classification performance.

## Deployment
The trained model was deployed as a web application using **[Streamlit](https://streamlit.io/)**, an open-source Python framework for building interactive data and AI/ML apps. The front-end was adapted to match the model's input requirements, allowing users to upload or capture an image and receive a real-time classification, including a poisonous/non-poisonous alert.

## Tech Stack
Python, TensorFlow/Keras, EfficientNetB0 (Transfer Learning), Grid Search, PSO (Particle Swarm Optimization), Streamlit
