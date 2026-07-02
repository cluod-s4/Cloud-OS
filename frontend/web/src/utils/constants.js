// frontend/web/src/utils/constants.js
export const API_URL = import.meta.env.VITE_API_URL || 'http://localhost:8000';
export const WS_URL = import.meta.env.VITE_WS_URL || 'ws://localhost:8000/ws';

export const THREAT_SEVERITIES = {
  CRITICAL: 'CRITICAL',
  HIGH: 'HIGH',
  MEDIUM: 'MEDIUM',
  LOW: 'LOW'
};

export const THREAT_STATUSES = {
  ACTIVE: 'ACTIVE',
  INVESTIGATING: 'INVESTIGATING',
  RESOLVED: 'RESOLVED',
  FALSE_POSITIVE: 'FALSE_POSITIVE'
};

export const ALERT_SEVERITIES = {
  CRITICAL: 'CRITICAL',
  HIGH: 'HIGH',
  MEDIUM: 'MEDIUM',
  LOW: 'LOW'
};

export const PAGINATION_DEFAULTS = {
  limit: 20,
  offset: 0
};
