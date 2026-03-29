from flask import Flask, jsonify
import requests
import threading
import time

app = Flask(__name__)

latest_data = {
    "crypto": {},
    "metals": {},
    "fx": {}
}

# ---------- CRYPTO ----------
def fetch_crypto():
    try:
        url = "https://api.coingecko.com/api/v3/simple/price"
        params = {
            "ids": "bitcoin,ethereum",
            "vs_currencies": "usd"
        }
        r = requests.get(url, params=params, timeout=5)
        data = r.json()

        return {
            "bitcoin": data.get("bitcoin", {}),
            "ethereum": data.get("ethereum", {})
        }

    except Exception as e:
        print("Crypto error:", e)
        return None


# ---------- METALS ----------
def fetch_metals():
    try:
        gold_url = "https://api.gold-api.com/price/XAU"
        silver_url = "https://api.gold-api.com/price/XAG"

        gold_data = requests.get(gold_url, timeout=5).json()
        silver_data = requests.get(silver_url, timeout=5).json()

        return {
            "gold": gold_data.get("price", 0),
            "silver": silver_data.get("price", 0)
        }

    except Exception as e:
        print("Metals error:", e)
        return None


# ---------- FX ----------
def fetch_fx():
    try:
        url = "https://api.exchangerate-api.com/v4/latest/USD"
        data = requests.get(url, timeout=5).json()

        return {
            "GBP": data["rates"]["GBP"],
            "EUR": data["rates"]["EUR"]
        }

    except Exception as e:
        print("FX error:", e)
        return None


# ---------- BACKGROUND LOOP ----------
def update_loop():
    global latest_data

    while True:
        crypto = fetch_crypto()
        metals = fetch_metals()
        fx = fetch_fx()

        if crypto:
            latest_data["crypto"] = crypto

        if metals:
            latest_data["metals"] = metals

        if fx:
            latest_data["fx"] = fx

        print("Updated data:", latest_data)

        time.sleep(10)


# ---------- API ENDPOINT ----------
@app.route("/price")
def price():
    return jsonify(latest_data)


# ---------- START ----------
if __name__ == "__main__":
    threading.Thread(target=update_loop, daemon=True).start()
    app.run(host="0.0.0.0", port=5000)
