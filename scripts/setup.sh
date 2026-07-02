#!/bin/bash
# scripts/setup.sh
# Real setup script

echo "========================================"
echo "  Cloud-OS Setup Script v1.0"
echo "========================================"

# Check Python version
python_version=$(python3 --version 2>&1 | cut -d' ' -f2)
echo "Python version: $python_version"

# Create virtual environment
echo "Creating virtual environment..."
python3 -m venv venv
source venv/bin/activate

# Install dependencies
echo "Installing Python dependencies..."
pip install -r requirements.txt
pip install -r requirements-dev.txt

# Install frontend dependencies
echo "Installing frontend dependencies..."
cd frontend/web && npm install && cd ../..

# Create .env file
if [ ! -f .env ]; then
    echo "Creating .env file..."
    cp .env.example .env
fi

# Create directories
mkdir -p models logs data

echo "========================================"
echo "  Setup complete!"
echo "  Run 'make run' to start the system"
echo "========================================"
