import matplotlib.pyplot as plt

def plot_learning_curve(history):
    plt.plot(history.history["loss"], label="Train")
    plt.plot(history.history["val_loss"], label="Cross-validation")
    plt.xlabel("Epoch")
    plt.ylabel("Loss")
    plt.legend()
    plt.show() 
