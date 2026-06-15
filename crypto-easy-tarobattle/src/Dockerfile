FROM python:3.11-slim

RUN apt-get update && apt-get install -y socat && rm -rf /var/lib/apt/lists/*
RUN useradd -m chaluser
COPY chal.py /app/chal.py
COPY flag /app/flag
WORKDIR /app
USER chaluser

EXPOSE 31337
CMD ["socat", "TCP-LISTEN:31337,reuseaddr,fork", "EXEC:python3 /app/chal.py"]
