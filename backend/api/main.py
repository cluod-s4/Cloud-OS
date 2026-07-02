# backend/api/main.py
# Real FastAPI application

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from contextlib import asynccontextmanager
import uvicorn
import logging
from datetime import datetime

from backend.api.routes import auth, threats, alerts, analytics
from backend.database.connection import init_db, engine
from backend.database.models import Base
from backend.ai.models.threat_detector import ThreatDetector
from backend.ai.models.anomaly_detector import AnomalyDetector

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

@asynccontextmanager
async def lifespan(app: FastAPI):
    logger.info("Starting Cloud-OS API...")
    
    async with engine.begin() as conn:
        await conn.run_sync(Base.metadata.create_all)
    logger.info("Database initialized")
    
    app.state.threat_detector = ThreatDetector()
    app.state.threat_detector.load()
    
    app.state.anomaly_detector = AnomalyDetector()
    app.state.anomaly_detector.load()
    logger.info("AI models loaded")
    
    yield
    
    logger.info("Shutting down Cloud-OS API...")

app = FastAPI(
    title="Cloud-OS API",
    description="Cloud-OS Backend API",
    version="1.0.0",
    lifespan=lifespan,
    docs_url="/api/docs",
    redoc_url="/api/redoc"
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(auth.router, prefix="/api/v1/auth", tags=["Authentication"])
app.include_router(threats.router, prefix="/api/v1/threats", tags=["Threats"])
app.include_router(alerts.router, prefix="/api/v1/alerts", tags=["Alerts"])
app.include_router(analytics.router, prefix="/api/v1/analytics", tags=["Analytics"])

@app.get("/")
async def root():
    return {"name": "Cloud-OS API", "version": "1.0.0", "status": "running"}

@app.get("/health")
async def health():
    return {"status": "healthy", "timestamp": datetime.now().isoformat()}

if __name__ == "__main__":
    uvicorn.run("backend.api.main:app", host="0.0.0.0", port=8000, reload=True)
