# COM3505-IOT Assignment
![The Dashboard](./Server/static/images/dashboard.png)
<!-- ^ Crunchy image -->
<!-- Maybe a picture of the board too -->

## Client
### Layout
![Circuit Diagram](./Client/assets/circuit.png)

### Setup
Wire your circuit on an breadboard according to the circuit diagram above.

Add the Adafruit ESP32 board package to the Arduino IDE. Find the preferences menu in the IDE and add `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json` to the list of "Additional Board Manager URLs".

Then install the ESP32 board support package from the board manager, and select an ESP32-S3 feather from the board list.

Use the Arduino IDE to upload the [firmware](./Client/COM3505_Firmware/COM3505_Firmware.ino) to an ESP32-S3 Feather.

### Provisioning
Using the inbuilt serial monitor, provision the WiFi credentials and server IP address when prompted.

## Server
### Setup
Create a virtual environment:
```bash
python -m venv .venv
```

Once your venv is created, you can activate it by running the following command:

```sh
source .venv/bin/activate # On Linux/Mac
```

```powershell
.venv\Scripts\Activate # On Windows
```

Install Flask
```
pip install Flask
```

Run the Flask Server
```
flask --app .\Server\server.py run --host=0.0.0.0
```

Visit the Dashboard

http://127.0.0.1:5000

### Routes

#### GET '/'
The Dashboard. Used to view the live temperature readings on a graph and send LED commands. Employs a mobile first design.

#### POST '/api/led'
Updating the LED commands. Requires a json object:
```bash
{
  "mode": "b", # valid pattern code
  "pattern": 0, # 0 - 4095
  "delay": 500 # 100 - 1000
}
```

#### POST '/api/data'
The designated route for the ESP32 to send temperature readings using this JSON format: 
```bash
{
  "temperature": 23.1
}
```
The server attaches its own incrementing 'label' value to each data packet for when it is displayed on the graph.

#### GET '/api/data'
The AJAX route for the dashboard and graph to get the most recent readings.
Returns the most recent 150 readings as a JSON object.

## Contributors
- Edouard Levasseur (Client)
- Max Manning (Server)