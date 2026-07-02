# backend/services/queue.py
# Real queue service

import redis
import json
from typing import Any, Optional
import os

REDIS_URL = os.getenv("REDIS_URL", "redis://localhost:6379/0")

class QueueService:
    def __init__(self):
        self.redis_client = redis.Redis.from_url(REDIS_URL, decode_responses=True)
    
    def push(self, queue_name: str, data: Any) -> bool:
        try:
            self.redis_client.rpush(queue_name, json.dumps(data))
            return True
        except:
            return False
    
    def pop(self, queue_name: str) -> Optional[Any]:
        try:
            value = self.redis_client.lpop(queue_name)
            if value:
                return json.loads(value)
            return None
        except:
            return None
    
    def size(self, queue_name: str) -> int:
        return self.redis_client.llen(queue_name)

queue_service = QueueService()
