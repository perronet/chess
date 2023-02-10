# Dataset
DATASET = "dataset/chessData.csv" 
N_ROWS = 200_000
TRAINING_SET_SIZE = 0.8

# NN architecture
N_FEATURES = 8*8*12
HIDDEN_ACTIVATION = "relu"
OUTPUT_ACTIVATION = "relu"

# Training
LEARNING_RATE = 0.0001
REGULARIZATION_RATE = 0
EPOCHS = 50
BATCH_SIZE = 32
EARLY_STOPPING = True
PATIENCE = 3

# Performance evaluation
K_FOLD = False
N_FOLDS = 3

# Save/load models
SAVE_MODEL = True
MODEL_NAME = "models/latest"
