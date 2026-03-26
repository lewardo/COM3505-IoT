#! /usr/bin/env python

## FLASK ##################################################
from flask import Flask, jsonify, render_template, request
app = Flask(__name__)

## GLOBAL #################################################
MAX_SENSOR_STORAGE = 50
VALID_MODES = {'blink', 'linear', 'ease in', 'ease out', 'ease in out', 'rainbow', 'flame', 'binary'}
LED_COUNT = 6 #update when breadboard is laid out

sensor_history = []
sensor_index = 0
led_mode = "blink"

## DASHBOARD ##############################################
@app.route("/")
def hello_world():
    return render_template('index.html')

## API ####################################################
@app.route('/api/data', methods=['POST'])
def receive_data():
    global sensor_index

    data = request.json
    data["label"] = sensor_index

    sensor_history.append(data)

    if len(sensor_history) > MAX_SENSOR_STORAGE:
        sensor_history.pop(0)

    sensor_index += 1

    return led_mode

@app.route('/api/data', methods=['GET'])
def get_data():
    return jsonify(sensor_history)

@app.route('/api/led/<mode>', methods=['POST'])
def set_led_command(mode):
    global led_mode

    if mode in VALID_MODES:
        led_mode = mode
        return 'Ok'
    else:
        try:
            # auto led mode will take an int as a binary representation
            mode = int(mode)
            if mode >= 0 and mode <= 2**LED_COUNT:
                led_mode = str(mode)
                return 'Ok'
        finally:
            return 'Error'