#!/bin/bash
# scripts/backup.sh
# Real backup script

BACKUP_DIR="/var/backups/cloud-os"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
BACKUP_FILE="$BACKUP_DIR/cloud-os_$TIMESTAMP.tar.gz"

echo "========================================"
echo "  Cloud-OS Backup Script v1.0"
echo "========================================"

# Create backup directory
mkdir -p $BACKUP_DIR

# Backup database
echo "Backing up database..."
docker-compose exec -T postgres pg_dump -U cloud_os cloud_os > /tmp/cloud_os.sql

# Backup configuration
echo "Backing up configuration..."
tar -czf /tmp/config_backup.tar.gz config/ .env

# Create final backup
echo "Creating final backup..."
tar -czf $BACKUP_FILE \
    /tmp/cloud_os.sql \
    /tmp/config_backup.tar.gz \
    logs/ \
    models/

# Cleanup
rm -f /tmp/cloud_os.sql /tmp/config_backup.tar.gz

# Delete old backups (keep last 7 days)
find $BACKUP_DIR -name "cloud-os_*.tar.gz" -mtime +7 -delete

echo "========================================"
echo "  Backup complete!"
echo "  File: $BACKUP_FILE"
echo "========================================"
