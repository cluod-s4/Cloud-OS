# backend/api/routes/threats.py
# Real threat routes

from fastapi import APIRouter, HTTPException, Depends, Query
from pydantic import BaseModel
from typing import List, Optional
from datetime import datetime

from backend.database.models.threat import Threat
from backend.database.connection import get_db
from backend.ai.models.threat_detector import ThreatDetector
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import select

router = APIRouter()

class ThreatDetectionRequest(BaseModel):
    type: str
    data: dict

@router.post("/detect")
async def detect_threat(request: ThreatDetectionRequest, detector: ThreatDetector = Depends()):
    if request.type == "network_anomaly":
        features = request.data
        result = detector.predict(features)
        
        if result['is_threat']:
            return {
                "threat_id": "thr_123456",
                "threat_type": "network_anomaly",
                "severity": "HIGH" if result['confidence'] > 0.7 else "MEDIUM",
                "confidence": result['confidence'],
                "description": f"Network anomaly detected with confidence {result['confidence']:.2%}",
                "timestamp": datetime.now().isoformat()
            }
    
    return {"threat_id": "", "threat_type": "none", "severity": "LOW", "confidence": 0.0}

@router.get("/")
async def get_threats(
    limit: int = Query(100, ge=1, le=1000),
    offset: int = Query(0, ge=0),
    db: AsyncSession = Depends(get_db)
):
    result = await db.execute(select(Threat).limit(limit).offset(offset))
    threats = result.scalars().all()
    return threats

@router.get("/{threat_id}")
async def get_threat(threat_id: int, db: AsyncSession = Depends(get_db)):
    result = await db.execute(select(Threat).where(Threat.id == threat_id))
    threat = result.scalar_one_or_none()
    if not threat:
        raise HTTPException(status_code=404, detail="Threat not found")
    return threat
