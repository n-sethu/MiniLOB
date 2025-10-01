import pandas as pd
import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt

# ------------------------
# 1. Load & sort
# ------------------------
df = pd.read_csv("HFTCryptoLOBdata.csv", parse_dates=["system_time"])
df = df.sort_values("system_time").reset_index(drop=True)

# ------------------------
# 2. Create labels (10s ahead)
# ------------------------
horizon = 10  # lookahead = 10 rows
df["future_midpoint"] = df["midpoint"].shift(-horizon)
df["label"] = (df["future_midpoint"] > df["midpoint"]).astype(int)
df = df.dropna().reset_index(drop=True)

# ------------------------
# 3. Build feature list dynamically
# ------------------------
exclude_cols = ["system_time", "future_midpoint", "label"]
features = [c for c in df.columns if c not in exclude_cols]

# ------------------------
# 4. Extract X and y
# ------------------------
X = df[features].values
y = df["label"].values

print(f"Feature count: {len(features)}")
print("First 10 features:", features[:10])

# ------------------------
# 3. Turn into sequences
# ------------------------
timesteps = 20  # how many past steps to use as input

def create_sequences(X, y, timesteps):
    Xs, ys = [], []
    for i in range(len(X) - timesteps):
        Xs.append(X[i:i+timesteps])
        ys.append(y[i+timesteps])
    return np.array(Xs), np.array(ys)

X_seq, y_seq = create_sequences(X, y, timesteps)

print("Input shape:", X_seq.shape)  # (samples, timesteps, features)
print("Labels shape:", y_seq.shape)

# ------------------------
# 4. Train-test split
# ------------------------
X_train, X_test, y_train, y_test = train_test_split(
    X_seq, y_seq, test_size=0.3, shuffle=False  # keep time order
)

# ------------------------
# 5. Build model (CNN + LSTM)
# ------------------------
model = models.Sequential([
    layers.Conv1D(filters=32, kernel_size=3, activation="relu", input_shape=(timesteps, len(features))),
    layers.MaxPooling1D(pool_size=2),
    layers.LSTM(64),
    layers.Dense(32, activation="relu"),
    layers.Dense(1, activation="sigmoid")
])

model.compile(optimizer="adam", loss="binary_crossentropy", metrics=["accuracy"])

# ------------------------
# 6. Train
# ------------------------
history = model.fit(
    X_train, y_train,
    validation_data=(X_test, y_test),
    epochs=10,
    batch_size=64
)

# ------------------------
# 7. Evaluate
# ------------------------
loss, acc = model.evaluate(X_test, y_test)
print("Test accuracy:", acc)

# ------------------------
# 8. Plot training history
# ------------------------
plt.plot(history.history["accuracy"], label="train_acc")
plt.plot(history.history["val_accuracy"], label="val_acc")
plt.legend()
plt.show()