# -*- coding: utf-8 -*-
"""starter-notebook.ipynb

Automatically generated by Colab.

Original file is located at
    https://colab.research.google.com/drive/1n5UGk9kmUQ8nzM04-PPQPKlB0N5sikAU

# Starter notebook for the competition "Russian military vehicle classification"

This notebook contains code that helps you get started with:
* Read and visualize the data.
* Split the full training dataset into training and validation sets.
* Create a dummy model.
* Visualize the confusion matrix on validation data.
* Submit the solution file.

First, let's import some stuff and define our directories
"""

import os
import random

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from skimage.io import imread
from sklearn.metrics import confusion_matrix, ConfusionMatrixDisplay
from sklearn.model_selection import train_test_split
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv2D, MaxPooling2D, Flatten, Dense, Dropout
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.preprocessing.image import ImageDataGenerator

# Input data files.
INPUT_DIR = 'rmv_data'


# Let's see what files we have
from google.colab import files
files.upload()  # Upload kaggle.json here
!mkdir -p ~/.kaggle
!cp kaggle.json ~/.kaggle/
!chmod 600 ~/.kaggle/kaggle.json

# Now download the competition data
!kaggle competitions download -c russian-military-vehicle-classification

# Unzip it
!unzip -q russian-military-vehicle-classification.zip -d rmv_data

for filename in os.listdir(INPUT_DIR):
    print(os.path.join(INPUT_DIR, filename))



"""## Dataset class"""

dft=pd.read_csv(os.path.join(INPUT_DIR, 'train.csv'))
dfs=pd.read_csv(os.path.join(INPUT_DIR, 'test.csv'))

dft

# prompt: remove id column

dft = dft.drop('id', axis=1)
print(dft.head())

dfs = dfs.drop('id', axis=1)
print(dfs.head())

# prompt: replace the filename column path with the images from the pathes

dft['filename'] = dft['filename'].apply(lambda x: os.path.join(INPUT_DIR, x))
dfs['filename'] = dfs['filename'].apply(lambda x: os.path.join(INPUT_DIR, x))

print("\nUpdated training dataframe with full image paths:")
print(dft.head())

print("\nUpdated test dataframe with full image paths:")
print(dfs.head())

# prompt: grayscale dft and dfs

from skimage.color import rgb2gray

def load_and_grayscale(filepath):
    img = imread(filepath)
    if img.ndim == 3:
        return rgb2gray(img)
    return img

dft['image'] = dft['filename'].apply(load_and_grayscale)
dfs['image'] = dfs['filename'].apply(load_and_grayscale)

print("\nTraining dataframe with grayscale images:")
print(dft.head())

print("\nTest dataframe with grayscale images:")
print(dfs.head())

# prompt: split data to x,y training and test

# Splitting data into training and validation sets
# We will use the original dft dataframe for splitting since we don't load images into it yet
X = dft['filename']  # Features (image file paths)
y = dft['class']      # Labels (vehicle classes)

# Split the data
# stratify=y ensures that the split maintains the same class distribution in both train and test sets
X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, random_state=42, stratify=y)

print("Training set size:", len(X_train))
print("Validation set size:", len(X_val))
print("\nTraining labels distribution:")
print(y_train.value_counts())
print("\nValidation labels distribution:")
print(y_val.value_counts())

# prompt: 0 - this is not a military vehicle
# 1 - BM-21
# 2 - BMD-2
# 3 - BMP-1
# 4 - BMP-2
# 5 - BTR-70
# 6 - BTR-80
# 7 - MT-LB
# 8 - T-64
# 9 - T-72
# 10 - T-80 replace class in y

y_train = y_train.replace({
    1: 'BM-21', 2: 'BMD-2', 3: 'BMP-1', 4: 'BMP-2', 5: 'BTR-70',
    6: 'BTR-80', 7: 'MT-LB', 8: 'T-64', 9: 'T-72', 10: 'T-80', 0: 'not a military vehicle'
})

y_val = y_val.replace({
    1: 'BM-21', 2: 'BMD-2', 3: 'BMP-1', 4: 'BMP-2', 5: 'BTR-70',
    6: 'BTR-80', 7: 'MT-LB', 8: 'T-64', 9: 'T-72', 10: 'T-80', 0: 'not a military vehicle'
})

print("\nTraining labels after replacing class numbers with names:")
print(y_train.value_counts())
print("\nValidation labels after replacing class numbers with names:")
print(y_val.value_counts())

IMG_WIDTH = 416
IMG_HEIGHT = 416

# prompt: apply resnet50V2 greyscale  reduce trainable data

import pandas as pd
import numpy as np
from tensorflow.keras.applications import ResNet50V2
from tensorflow.keras.layers import Input, GlobalAveragePooling2D, Dense
from tensorflow.keras.models import Model
from tensorflow.keras.preprocessing.image import ImageDataGenerator

# Create DataFrames for train and validation sets using the file paths and converted labels
train_df = pd.DataFrame({'filename': X_train, 'class': y_train})
val_df = pd.DataFrame({'filename': X_val, 'class': y_val})

# Get the list of unique classes for the output layer
classes = y_train.unique().tolist()
num_classes = len(classes)

# Create data generators
# For grayscale, ensure color_mode='grayscale'
train_datagen = ImageDataGenerator(rescale=1./255) # Rescale pixels to [0, 1]
val_datagen = ImageDataGenerator(rescale=1./255)

# Flow from DataFrame
# target_size should match the input size expected by ResNet50V2 (typically 224x224 or larger)
# Since we are working with grayscale, the input shape will be (height, width, 1)
# ResNet50V2 was trained on RGB (3 channels). We need to adapt or use a different approach
# for grayscale. A common approach is to duplicate the grayscale channel 3 times.
# However, Keras generators for grayscale will output (height, width, 1).
# We'll need to handle the channel expansion either in the generator or before the model.
# Let's adapt the model to accept 1 channel and then add a Conv2D layer to expand to 3 channels
# at the beginning of the model, or adjust the data loading to triplicate the channel.

# Let's modify the data loading function to triplicate the grayscale channel to fake an RGB image
def load_and_grayscale_and_expand(filepath):
    img = imread(filepath)
    if img.ndim == 3:
        gray_img = rgb2gray(img)
    else:
        gray_img = img
    # Expand dimensions to add channel dimension (height, width) -> (height, width, 1)
    gray_img = np.expand_dims(gray_img, axis=-1)
    # Triplicate the channel: (height, width, 1) -> (height, width, 3)
    rgb_like_img = np.repeat(gray_img, 3, axis=-1)
    return rgb_like_img

# We need to apply this new loading function within the generator pipeline,
# or better, load the images into memory first, which is not ideal for large datasets.
# A more standard approach is to use the `color_mode='rgb'` in the generator and
# provide the grayscale images with the channel tripled *before* passing them to the generator,
# or handle it inside the model.

# Let's create a custom generator function that loads and processes the images
def image_generator(dataframe, target_size, batch_size, class_mode='categorical', shuffle=True):
    datagen = ImageDataGenerator(rescale=1./255) # Apply other augmentations here if needed
    for i in range(0, len(dataframe), batch_size):
        batch_df = dataframe.iloc[i:i+batch_size]
        batch_images = []
        batch_labels = []
        for index, row in batch_df.iterrows():
            img_path = row['filename']
            label = row['class']
            img = load_and_grayscale_and_expand(img_path) # Load, grayscale, and expand
            # Resize image to target size if needed, using PIL's resize function via Keras util
            img = tf.keras.preprocessing.image.smart_resize(img, target_size)
            batch_images.append(img)
            batch_labels.append(label)

        batch_images = np.array(batch_images)
        # Convert labels to one-hot encoding
        label_map = {cls: idx for idx, cls in enumerate(classes)}
        batch_labels_encoded = np.array([label_map[label] for label in batch_labels])
        if class_mode == 'categorical':
            batch_labels_encoded = tf.keras.utils.to_categorical(batch_labels_encoded, num_classes=num_classes)

        yield batch_images, batch_labels_encoded

# Define batch size
BATCH_SIZE = 32

# Create generators using the custom function
train_generator = image_generator(
    train_df,
    target_size=(IMG_HEIGHT, IMG_WIDTH),
    batch_size=BATCH_SIZE,
    class_mode='categorical',
    shuffle=True
)

val_generator = image_generator(
    val_df,
    target_size=(IMG_HEIGHT, IMG_WIDTH),
    batch_size=BATCH_SIZE,
    class_mode='categorical',
    shuffle=False # No need to shuffle validation data
)


# Load the ResNet50V2 model pre-trained on ImageNet, excluding the top classification layer
# input_shape should be (height, width, 3) for ResNet50V2
base_model = ResNet50V2(weights='imagenet', include_top=False, input_shape=(IMG_HEIGHT, IMG_WIDTH, 3))

# Reduce trainable data by freezing early layers
# Iterate through layers and set them to non-trainable
for layer in base_model.layers:
    layer.trainable = False # Freeze all layers in the base model initially

# Optionally, unfreeze some later layers for fine-tuning if needed
# For example, unfreeze the last few convolutional blocks
# num_layers_to_unfreeze = 30 # Adjust this number as needed
# for layer in base_model.layers[-num_layers_to_unfreeze:]:
#    layer.trainable = True


# Add a new classification head on top of the base model
x = base_model.output
x = GlobalAveragePooling2D()(x) # Add a global average pooling layer
x = Dense(256, activation='relu')(x) # Add a dense layer
x = Dropout(0.5)(x) # Add dropout for regularization
predictions = Dense(num_classes, activation='softmax')(x) # Add the output layer with softmax

# Create the full model
model = Model(inputs=base_model.input, outputs=predictions)

# Compile the model
# Using Adam optimizer
optimizer = Adam(learning_rate=0.001)
model.compile(optimizer=optimizer, loss='categorical_crossentropy', metrics=['accuracy'])

# Print model summary
model.summary()

# Calculate steps per epoch for generators
steps_per_epoch_train = len(train_df) // BATCH_SIZE
steps_per_epoch_val = len(val_df) // BATCH_SIZE

# Train the model using the generators
history = model.fit(
    train_generator,
    steps_per_epoch=steps_per_epoch_train,
    epochs=10, # Adjust number of epochs
    validation_data=val_generator,
    validation_steps=steps_per_epoch_val
)

# You can then evaluate the model and make predictions
