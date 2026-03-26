#! /usr/bin/env python

## FLASK ##################################################
from flask import Flask, jsonify, render_template, request
app = Flask(__name__)

## GLOBAL #################################################
MAX_SENSOR_STORAGE = 50
sensor_history = []
sensor_index = 0

VALID_MODES = {'b', 'l', 'i', 'o', 'x', 'r', 'f', 'c', 't', 'm'}
# {blink, linear, ease in, ease out, 'ease in out', 'rainbow', 'flame', 'binary', 'temperature', 'manual'}

LED_COUNT = 6 # update when breadboard is laid out
led_mode = "b"
led_delay = 500
manual_pattern = 0

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

    return led_mode + ";" + str(manual_pattern) + ";" + str(led_delay)

@app.route('/api/data', methods=['GET'])
def get_data():
    return jsonify(sensor_history)

@app.route('/api/led/', methods=['POST'])
def set_led_command():
    global led_mode, led_delay

    data = request.json
    mode = data['mode']
    delay = int(data['delay'])

    if mode in VALID_MODES:
        led_mode = mode
        led_delay = delay
        return 'Ok'
    else:
        return 'Error'