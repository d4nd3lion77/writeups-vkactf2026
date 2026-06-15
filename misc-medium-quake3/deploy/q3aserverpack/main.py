import os
from flask import Flask, request, abort, render_template

app = Flask(__name__)

SECRET="30f3703b953a9b178f934b5b47e5115e67f17cbfbd73f4914783a02b8269221e" # DoNotShareWithAnyone

FLAG = os.getenv("FLAG", "vka{example}")
PORT = int(os.getenv("SITE_PORT", "5544"))
HOST = os.getenv("IP_HOST")
GAMEPORT = os.getenv("GAME_PORT")

@app.route("/")
def index():
    return render_template("index.html", HOST=HOST, GAMEPORT=GAMEPORT)


@app.route("/admin")
def admin():
    user_secret = request.args.get("secret")

    if not user_secret:
        return "parameter \"secret\" required", 401

    if user_secret != SECRET:
        abort(403)

    return f"{FLAG}"


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=PORT)
