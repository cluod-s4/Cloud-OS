#!/bin/bash
# scripts/deploy.sh
# Real deployment script

echo "========================================"
echo "  Cloud-OS Deployment Script v1.0"
echo "========================================"

# Check if Docker is installed
if ! command -v docker &> /dev/null; then
    echo "Docker is not installed. Please install Docker first."
    exit 1
fi

# Build Docker images
echo "Building Docker images..."
docker-compose build

# Start services
echo "Starting services..."
docker-compose up -d

# Wait for services to be ready
echo "Waiting for services to be ready..."
sleep 10

# Run migrations
echo "Running database migrations..."
docker-compose exec backend alembic upgrade head

# Create admin user
echo "Creating admin user..."
docker-compose exec backend python scripts/create_admin.py

echo "========================================"
echo "  Deployment complete!"
echo "  API: http://localhost:8000"
echo "  Frontend: http://localhost:3000"
echo "========================================"
