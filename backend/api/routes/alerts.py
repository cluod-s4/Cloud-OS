# backend/api/routes/alerts.py
# Real alert routes

from fastapi import APIRouter, HTTPException, Depends, Query
from pydantic import BaseModel
from typing import Optional
from datetime import datetime

from backend.database.models.alert import Alert
from backend.database.connection import get_db
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import select

router = APIRouter()

class AlertCreate(BaseModel):
    title: str
    message: str
    severity: str = "MEDIUM"

@router.post("/")
async def create_alert(alert_data: AlertCreate, db: AsyncSession = Depends(get_db)):
    alert = Alert(
        title=alert_data.title,
        message=alert_data.message,
        severity=alert_data.severity,
        status="pending",
        created_at=datetime.now()
    )
    
    db.add(alert)
    await db.commit()
    await db.refresh(alert)
    
    return alert

@router.get("/")
async def get_alerts(
    limit: int = Query(100, ge=1, le=1000),
    db: AsyncSession = Depends(get_db)
):
    result = await db.execute(select(Alert).limit(limit))
    alerts = result.scalars().all()
    return alerts

@router.get("/{alert_id}")
async def get_alert(alert_id: int, db: AsyncSession = Depends(get_db)):
    result = await db.execute(select(Alert).where(Alert.id == alert_id))
    alert = result.scalar_one_or_none()
    if not alert:
        raise HTTPException(status_code=404, detail="Alert not found")
    return alert
