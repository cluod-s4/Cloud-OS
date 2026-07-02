# backend/database/models/alert.py
from sqlalchemy import Column, Integer, String, DateTime, Boolean, JSON, Enum
from sqlalchemy.sql import func
from backend.database.connection import Base
import enum

class AlertSeverity(str, enum.Enum):
    LOW = "LOW"
    MEDIUM = "MEDIUM"
    HIGH = "HIGH"
    CRITICAL = "CRITICAL"

class AlertStatus(str, enum.Enum):
    PENDING = "PENDING"
    SENT = "SENT"
    ACKNOWLEDGED = "ACKNOWLEDGED"
    RESOLVED = "RESOLVED"

class Alert(Base):
    __tablename__ = "alerts"

    id = Column(Integer, primary_key=True, index=True)
    title = Column(String(200), nullable=False)
    message = Column(String(500), nullable=False)
    severity = Column(Enum(AlertSeverity), default=AlertSeverity.MEDIUM)
    source = Column(String(100), nullable=True)
    metadata = Column(JSON, nullable=True)
    status = Column(Enum(AlertStatus), default=AlertStatus.PENDING)
    created_at = Column(DateTime, server_default=func.now())
    acknowledged_at = Column(DateTime, nullable=True)
    resolved_at = Column(DateTime, nullable=True)
    user_id = Column(Integer, nullable=True)
