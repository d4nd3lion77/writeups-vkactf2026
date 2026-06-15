FROM dockerhub.timeweb.cloud/python:3.12-alpine
WORKDIR /deploy
RUN apk add --no-cache socat gcc musl-dev


COPY requirements.txt .
RUN pip install -r requirements.txt


COPY . .

RUN chmod +x task.py

ENTRYPOINT ["socat", "TCP-LISTEN:1447,fork,reuseaddr", "EXEC:python3 ./task.py"]