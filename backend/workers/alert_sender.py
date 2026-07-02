# backend/workers/alert_sender.py
# Real alert sender worker

from celery import Celery
from datetime import datetime
import logging

from backend.services.email import email_service
from backend.services.queue import queue_service
from backend.database.connection import get_db
from backend.database.models.alert import Alert

celery_app = Celery('alert_sender', broker='redis://localhost:6379/0')
logger = logging.getLogger(__name__)

@celery_app.task
def send_alert(alert_id: int):
    """Send alert notification"""
    try:
        db = next(get_db())
        alert = db.query(Alert).filter(Alert.id == alert_id).first()
        
        if not alert:
            logger.error(f"Alert {alert_id} not found")
            return {'status': 'error', 'error': 'Alert not found'}
        
        # Send email
        email_service.send_email(
            to_emails=['admin@cloud-os.io'],
            subject=f"Alert: {alert.title}",
            body=f"{alert.message}\nSeverity: {alert.severity}"
        )
        
        alert.status = 'SENT'
        db.commit()
        
        logger.info(f"Alert {alert_id} sent")
        return {'status': 'success'}
        
    except Exception as e:
        logger.error(f"Alert send failed: {e}")
        return {'status': 'error', 'error': str(e)}
