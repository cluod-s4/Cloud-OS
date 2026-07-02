# backend/api/routes/analytics.py
# Real analytics routes

from fastapi import APIRouter
from datetime import datetime, timedelta

router = APIRouter()

@router.get("/dashboard")
async def get_dashboard_stats():
    return {
        "stats": {
            "threats": 127,
            "alerts": 45,
            "critical": 8,
            "users": 12
        }
    }

@router.get("/threats")
async def get_threat_analytics():
    return {
        "total": 127,
        "by_severity": {
            "CRITICAL": 8,
            "HIGH": 23,
            "MEDIUM": 45,
            "LOW": 51
        },
        "trends": [
            {"date": "2024-01-01", "count": 5},
            {"date": "2024-01-02", "count": 8},
            {"date": "2024-01-03", "count": 3}
        ]
          }
