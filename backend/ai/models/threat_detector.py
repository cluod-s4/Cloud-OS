# backend/ai/models/threat_detector.py
# Real threat detection

import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.preprocessing import StandardScaler
import joblib
import os
import json
from datetime import datetime
import logging

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class ThreatDetector:
    def __init__(self, model_path: str = None):
        self.model = RandomForestClassifier(
            n_estimators=200,
            max_depth=10,
            random_state=42,
            n_jobs=-1
        )
        self.scaler = StandardScaler()
        self.feature_names = []
        self.is_trained = False
        self.model_path = model_path or './models/threat_detector.pkl'
    
    def train(self, X, y):
        self.feature_names = [f"feature_{i}" for i in range(X.shape[1])]
        X_scaled = self.scaler.fit_transform(X)
        self.model.fit(X_scaled, y)
        self.is_trained = True
        logger.info("Threat detector trained successfully")
    
    def predict(self, features):
        if not self.is_trained:
            return {'is_threat': False, 'confidence': 0.0, 'threat_score': 0.0}
        
        vector = [features.get(name, 0.0) for name in self.feature_names]
        scaled = self.scaler.transform([vector])
        proba = self.model.predict_proba(scaled)[0]
        
        return {
            'is_threat': bool(self.model.predict(scaled)[0]),
            'confidence': float(max(proba)),
            'threat_score': float(proba[1])
        }
    
    def load(self):
        if os.path.exists(self.model_path):
            self.model = joblib.load(self.model_path)
            scaler_path = self.model_path.replace('.pkl', '_scaler.pkl')
            if os.path.exists(scaler_path):
                self.scaler = joblib.load(scaler_path)
            self.is_trained = True
            logger.info(f"Threat detector loaded from {self.model_path}")
            return True
        return False
    
    def save(self):
        os.makedirs(os.path.dirname(self.model_path), exist_ok=True)
        joblib.dump(self.model, self.model_path)
        joblib.dump(self.scaler, self.model_path.replace('.pkl', '_scaler.pkl'))
        logger.info(f"Threat detector saved to {self.model_path}")
