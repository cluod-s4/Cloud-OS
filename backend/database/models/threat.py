# backend/database/models/threat.py
from sqlalchemy import Column, Integer, String, DateTime, Float, JSON, Enum
from sqlalchemy.sql import func
from backend.database.connection import Base
import enum

class ThreatSeverity(str, enum.Enum):
    CRITICAL = "CRITICAL"
    HIGH = "HIGH"
    MEDIUM = "MEDIUM"
    LOW = "LOW"

class ThreatStatus(str, enum.Enum):
    ACTIVE = "ACTIVE"
    INVESTIGATING = "INVESTIGATING"
    RESOLVED = "RESOLVED"
    FALSE_POSITIVE = "FALSE_POSITIVE"

class Threat(Base):
    __tablename__ = "threats"

    id = Column(Integer, primary_key=True, index=True)
    threat_type = Column(String(50), nullable=False)
    severity = Column(Enum(ThreatSeverity), default=ThreatSeverity.MEDIUM)
    source_ip = Column(String(50), nullable=True)
    destination_ip = Column(String(50), nullable=True)
    description = Column(String(500), nullable=False)
    details = Column(JSON, nullable=True)
    confidence = Column(Float, nullable=True)
    detected_at = Column(DateTime, server_default=func.now())
    resolved_at = Column(DateTime, nullable=True)
    status = Column(Enum(ThreatStatus), default=ThreatStatus.ACTIVE)
    user_id = Column(Integer, nullable=True)
    assigned_to = Column(Integer, nullable=True)
    notes = Column(String(500), nullable=True)
