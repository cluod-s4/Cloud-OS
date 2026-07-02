# Makefile
.PHONY: help install run test build clean docker-up docker-down

help:
	@echo "Available commands:"
	@echo "  install      - Install all dependencies"
	@echo "  run          - Run development environment"
	@echo "  test         - Run all tests"
	@echo "  build        - Build the system"
	@echo "  clean        - Clean temporary files"
	@echo "  docker-up    - Start Docker containers"
	@echo "  docker-down  - Stop Docker containers"

install:
	pip install -r requirements.txt
	pip install -r requirements-dev.txt
	cd frontend/web && npm install

run:
	docker-compose up -d
	@echo "Services running:"
	@echo "  Backend: http://localhost:8000"
	@echo "  Frontend: http://localhost:3000"

test:
	pytest tests/ -v --cov=src

build:
	docker-compose build

clean:
	find . -type d -name "__pycache__" -exec rm -rf {} +
	find . -type f -name "*.pyc" -delete
	find . -type d -name ".pytest_cache" -exec rm -rf {} +

docker-up:
	docker-compose up -d

docker-down:
	docker-compose down
