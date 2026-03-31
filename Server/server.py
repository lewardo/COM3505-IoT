#! /usr/bin/env python

## FLASK ##################################################
from flask import Flask, jsonify, render_template, request
app = Flask(__name__)

## GLOBAL #################################################
MAX_SENSOR_STORAGE = 150
VALID_MODES = {
    'b', # blink
    'w', # twinkle
    'l', # linear
    'i', # ease in
    'o', # ease out
    'x', # ease in out
    'r', # rainbow
    'f', # flame
    'c', # binary
    't', # temperature
    'm'  # manual
}

## DEFAULTS ###############################################
led_mode = "b"
led_delay = 500
led_pattern = 0

sensor_history = []
sensor_data_label = 0

## DASHBOARD ##############################################
@app.route("/")
def hello_world():
    return render_template('index.html')

## API ####################################################
@app.route('/api/data', methods=['POST'])
def receive_data():
    global sensor_data_label

    data = request.json
    data["label"] = sensor_data_label

    sensor_history.append(data)
    sensor_data_label += 1
    
    if len(sensor_history) > MAX_SENSOR_STORAGE:
        sensor_history.pop(0)

    return led_mode + ";" + str(led_pattern) + ";" + str(led_delay)

@app.route('/api/data', methods=['GET'])
def get_data():
    return jsonify(sensor_history)

@app.route('/api/led/', methods=['POST'])
def set_led_command():
    global led_mode, led_pattern, led_delay

    data = request.json

    if data['mode'] in VALID_MODES:
        led_mode = data['mode']
        led_pattern = int(data['pattern'])
        led_delay = int(data['delay'])

        return 'Ok'
    else:
        return 'Error'
