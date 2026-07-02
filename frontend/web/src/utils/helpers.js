// frontend/web/src/utils/helpers.js
import { format, formatDistanceToNow } from 'date-fns';

export const formatDate = (date) => {
  if (!date) return 'N/A';
  return format(new Date(date), 'MMM dd, yyyy HH:mm:ss');
};

export const formatRelativeTime = (date) => {
  if (!date) return 'N/A';
  return formatDistanceToNow(new Date(date), { addSuffix: true });
};

export const getSeverityColor = (severity) => {
  const colors = {
    CRITICAL: '#ff0000',
    HIGH: '#ff6b00',
    MEDIUM: '#ffaa00',
    LOW: '#00aa00'
  };
  return colors[severity] || '#888888';
};

export const getStatusColor = (status) => {
  const colors = {
    ACTIVE: '#ff6b6b',
    INVESTIGATING: '#ffaa00',
    RESOLVED: '#00aa00',
    FALSE_POSITIVE: '#888888'
  };
  return colors[status] || '#888888';
};

export const truncateText = (text, maxLength = 100) => {
  if (!text) return '';
  if (text.length <= maxLength) return text;
  return text.substring(0, maxLength) + '...';
};
