# Mini LOB (Limit Order Book) Project

## Overview
The **Mini LOB Project** is a simplified simulation and analysis of a financial **Limit Order Book (LOB)**. A LOB is a mechanism used by exchanges to match buy and sell orders for financial instruments. Understanding and modeling LOB dynamics is critical in high-frequency trading (HFT) and algorithmic trading research.  

This project is designed to:  
1. Simulate the basic behavior of a LOB.  
2. Process historical or real-time market order data.  
3. Compute key LOB features (midpoint, spread, imbalance, order flow, etc.).  
4. Provide a foundation for modeling, analysis, and ML experiments in market microstructure.  

This mini version is intended as a preparatory project before tackling a larger HFT LOB dataset.

---

## Features

### 1. LOB Simulation (To Do)
- Simplified LOB implemented with **bid and ask queues**.
- Orders can be added, modified, or canceled.
- Tracks multiple levels of the book (level 1–5).

### 2. LOB Feature Extraction
Calculates key features used in market analysis and ML modeling:  
- **Midpoint price**: `(best_bid + best_ask)/2`  
- **Spread**: `best_ask - best_bid`  
- **Relative spread**: `spread / midpoint`  
- **Imbalance**: `(bid_volume - ask_volume) / (bid_volume + ask_volume)`  
- **Weighted distances** for bid/ask levels  
- **Order flow**: Number of buys and sells over time  

### 3. Data Handling (work in progress)
- Supports CSV input of order-level data with timestamps.  
- Data is sorted and cleaned for consistent LOB processing.  
- Allows filtering and resampling for modeling purposes.

---
If you want to contact me and discuss any work please feel free to reach out at:
---
- nikhilsethu1@gmail.com
- https://www.linkedin.com/in/nikhil-sethu/ 
- https://github.com/n-sethu
