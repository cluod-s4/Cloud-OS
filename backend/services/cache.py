# backend/services/cache.py
# Real caching service

import redis
import json
from typing import Any, Optional
from datetime import timedelta
import os

REDIS_URL = os.getenv("REDIS_URL", "redis://localhost:6379/0")

class CacheService:
    def __init__(self):
        self.redis_client = redis.Redis.from_url(REDIS_URL, decode_responses=True)
    
    def get(self, key: str) -> Optional[Any]:
        value = self.redis_client.get(key)
        if value:
            return json.loads(value)
        return None
    
    def set(self, key: str, value: Any, ttl: int = 300) -> bool:
        try:
            self.redis_client.setex(key, ttl, json.dumps(value))
            return True
        except:
            return False
    
    def delete(self, key: str) -> bool:
        return bool(self.redis_client.delete(key))
    
    def exists(self, key: str) -> bool:
        return bool(self.redis_client.exists(key))
    
    def clear_pattern(self, pattern: str) -> int:
        keys = self.redis_client.keys(pattern)
        if keys:
            return self.redis_client.delete(*keys)
        return 0

cache_service = CacheService()
