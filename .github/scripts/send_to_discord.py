import json
from datetime import datetime
import os
import requests

RESULTS_FILE = os.getenv("RESULTS_FILE")
WEBHOOK = os.getenv("DISCORD_WEBHOOK")

if not WEBHOOK:
    raise SystemExit("No webhook provided.")

if not RESULTS_FILE or not os.path.isfile(RESULTS_FILE):
    raise SystemExit("No results file found.")

with open(RESULTS_FILE, "r", encoding="utf-8") as f:
    text = f.read().strip()

date = datetime.now().strftime("%d-%m-%Y")
msg = f"```ini\n# Date Posted: {date}\n\n{text}\n```"

payload = {"content": msg}

r = requests.post(WEBHOOK, json=payload)

print("Status:", r.status_code)
print("Response:", r.text)
