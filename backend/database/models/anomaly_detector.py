# backend/ai/models/anomaly_detector.py
# Real anomaly detection

import numpy as np
from sklearn.ensemble import IsolationForest
from sklearn.preprocessing import StandardScaler
import joblib
import os
import logging

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class AnomalyDetector:
    def __init__(self, model_path: str = None):
        self.model = IsolationForest(contamination=0.05, random_state=42)
        self.scaler = StandardScaler()
        self.is_trained = False
        self.model_path = model_path or './models/anomaly_detector.pkl'
        self.threshold = -0.2
    
    def train(self, X):
        X_scaled = self.scaler.fit_transform(X)
        self.model.fit(X_scaled)
        self.is_trained = True
        logger.info("Anomaly detector trained successfully")
    
    def detect(self, features):
        if not self.is_trained:
            return {'is_anomaly': False, 'anomaly_score': 0.0}
        
        vector = list(features.values())
        scaled = self.scaler.transform([vector])
        score = self.model.decision_function(scaled)[0]
        
        return {
            'is_anomaly': score < self.threshold,
            'anomaly_score': float(score),
            'confidence': min(1.0, max(0.0, (self.threshold - score) / 0.5))
        }
    
    def load(self):
        if os.path.exists(self.model_path):
            self.model = joblib.load(self.model_path)
            scaler_path = self.model_path.replace('.pkl', '_scaler.pkl')
            if os.path.exists(scaler_path):
                self.scaler = joblib.load(scaler_path)
            self.is_trained = True
            logger.info(f"Anomaly detector loaded from {self.model_path}")
            return True
        return False
    
    def save(self):
        os.makedirs(os.path.dirname(self.model_path), exist_ok=True)
        joblib.dump(self.model, self.model_path)
        joblib.dump(self.scaler, self.model_path.replace('.pkl', '_scaler.pkl'))
        logger.info(f"Anomaly detector saved to {self.model_path}")
