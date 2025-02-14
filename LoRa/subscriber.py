import paho.mqtt.client as mqtt
import json
import base64
import mysql.connector

# TTN MQTT Configuration
BROKER = "eu1.cloud.thethings.network"
PORT = 1883
USERNAME = "fridge-id"  # Replace with TTN App ID
PASSWORD = "NNSXS.D5QVK4G2J6Y6HDSF27BPGGS22VZS4SI3YPEMZ5Y.SQLTC3BMAGT4VLYJLPZNTTKAXE7PMQCMGYGFB2L75T26AYM7Q6IQ"  # Replace with TTN API Key
TOPIC = "v3/#"  # Replace with your topic

# Connect to MySQL Database
db = mysql.connector.connect(
    host="localhost",
    user="root",  # Default XAMPP username
    password="",  # Default XAMPP has no password
    database="fridge_iot"
)
cursor = db.cursor()

# Callback when connected
def on_connect(client, userdata, flags, rc, properties=None):
    if rc == 0:
        print("✅ Connected to TTN MQTT Broker")
        client.subscribe(TOPIC)  # Subscribe to the correct topic
    else:
        print(f"❌ Connection failed with code {rc}")

# Callback when a message is received
def on_message(client, userdata, message):
    print("\n📥 Received MQTT Message:")
    raw_json = message.payload.decode()  # Convert to JSON format

    try:
        data = json.loads(raw_json)  # Parse JSON

        # Extract frm_payload (Base64 encoded)
        if "uplink_message" in data and "frm_payload" in data["uplink_message"]:
            frm_payload = data["uplink_message"]["frm_payload"]
            payload_bytes = base64.b64decode(frm_payload)  # Decode Base64

            # Convert bytes to a readable JSON format
            decoded_payload = json.loads(payload_bytes.decode("utf-8"))

            # Extract values
            temperature = decoded_payload.get("temperature", "N/A")
            humidity = decoded_payload.get("humidity", "N/A")
            door = decoded_payload.get("door", "N/A")

            print(f"🌡 Temperature: {temperature}°C")
            print(f"💧 Humidity: {humidity}%")
            print(f"🔘 Door: {'Closed' if door == 0 else 'Open'}")
            
            if(temperature > '25.00'):
                print("Temperature limit exceeded! Close the fucking door!")

            # Insert data into MySQL
            sql = "INSERT INTO sensor_data (temperature, humidity, door) VALUES (%s, %s, %s)"
            values = (temperature, humidity, door)
            cursor.execute(sql, values)
            db.commit()  # Save changes

            print("✅ Data inserted into MySQL")

        else:
            print("⚠ No valid payload found in message.")

    except Exception as e:
        print(f"❌ Error decoding message: {e}")

# MQTT Client Setup
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.username_pw_set(USERNAME, PASSWORD)
client.on_connect = on_connect
client.on_message = on_message

try:
    print("🔄 Connecting to TTN MQTT...")
    client.connect(BROKER, PORT, 60)
    client.loop_forever()
except Exception as e:
    print(f"❌ Connection Error: {e}")
