# backend/workers/threat_processor.py
# Real threat processor worker

from celery import Celery
from datetime import datetime
import logging

from backend.ai.models.threat_detector import ThreatDetector
from backend.database.connection import get_db
from backend.database.models.threat import Threat

celery_app = Celery('threat_processor', broker='redis://localhost:6379/0')
logger = logging.getLogger(__name__)

@celery_app.task
def process_threat(threat_data: dict):
    """Process detected threat"""
    try:
        detector = ThreatDetector()
        result = detector.predict(threat_data.get('features', {}))
        
        if result['is_threat']:
            db = next(get_db())
            threat = Threat(
                threat_type=threat_data.get('type', 'unknown'),
                severity='HIGH' if result['confidence'] > 0.7 else 'MEDIUM',
                source_ip=threat_data.get('source_ip'),
                destination_ip=threat_data.get('destination_ip'),
                description=f"Threat detected with score {result['threat_score']:.2%}",
                confidence=result['confidence'],
                status='ACTIVE'
            )
            db.add(threat)
            db.commit()
            
            logger.info(f"Threat processed: {threat.id}")
            return {'status': 'success', 'threat_id': threat.id}
        
        return {'status': 'no_threat'}
        
    except Exception as e:
        logger.error(f"Threat processing failed: {e}")
        return {'status': 'error', 'error': str(e)}
