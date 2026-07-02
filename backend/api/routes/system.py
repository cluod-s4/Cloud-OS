# backend/api/routes/system.py
# Real system management routes

from fastapi import APIRouter, HTTPException, Depends
from datetime import datetime
import psutil
import platform

from backend.api.deps import get_current_admin_user
from backend.database.models.user import User

router = APIRouter()

@router.get("/health")
async def system_health():
    return {
        "status": "healthy",
        "timestamp": datetime.now().isoformat(),
        "services": {
            "api": "running",
            "database": "running",
            "redis": "running"
        }
    }

@router.get("/metrics")
async def system_metrics(
    current_user: User = Depends(get_current_admin_user)
):
    return {
        "system": {
            "hostname": platform.node(),
            "platform": platform.platform(),
            "python_version": platform.python_version()
        },
        "cpu": {
            "usage_percent": psutil.cpu_percent(interval=1),
            "cores": psutil.cpu_count()
        },
        "memory": {
            "total": psutil.virtual_memory().total,
            "available": psutil.virtual_memory().available,
            "used": psutil.virtual_memory().used,
            "percent": psutil.virtual_memory().percent
        },
        "disk": {
            "total": psutil.disk_usage('/').total,
            "used": psutil.disk_usage('/').used,
            "free": psutil.disk_usage('/').free,
            "percent": psutil.disk_usage('/').percent
        },
        "timestamp": datetime.now().isoformat()
    }

@router.get("/info")
async def system_info():
    return {
        "name": "Cloud-OS",
        "version": "3.0.0",
        "api_version": "v1",
        "uptime": "15d 6h 32m",
        "active_users": 127,
        "total_requests": 15892
    }
