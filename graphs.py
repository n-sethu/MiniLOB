import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier

df = pd.read_csv("metrics.csv")

# Basic stats
print(df.describe())

# Plot imbalance over time
df.plot(x="system_time", y="imbalance")

# Train ML model to predict price move


X = df[["rel_spread", "imbalance", "weighted_bid_distance"]]
y = (df["midpoint"].shift(-1) > df["midpoint"]).astype(int)  # next price up?

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3)
model = RandomForestClassifier().fit(X_train, y_train)
print("Accuracy:", model.score(X_test, y_test))    