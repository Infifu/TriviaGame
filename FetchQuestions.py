import requests
import json

url = "https://opentdb.com/api.php?amount=20"
response = requests.get(url)

print(response.text)