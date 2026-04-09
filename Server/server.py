#! /usr/bin/env python

## FLASK ##################################################
from flask import Flask, jsonify, redirect, render_template, request
app = Flask(__name__)

## CONSTANTS ##############################################
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
led_mode = 'b'
led_delay = 500
led_pattern = 0

sensor_history = []
sensor_data_label = 0

## DASHBOARD ##############################################
@app.route('/')
def hello_world():
    return render_template('index.html'), 200

@app.errorhandler(404)
def page_not_found(e):
    return redirect('/')

## API ####################################################
@app.route('/api/data', methods=['GET'])
def send_data():
    return jsonify(sensor_history), 200

@app.route('/api/data', methods=['POST'])
def receive_data():
    data = request.json

    # Data validation
    if 'temperature' not in data:
        return "Bad Request: 'temperature' value must be a number", 400
    
    extra_fields = [key for key in data if key != 'temperature'] 
    if len(extra_fields) > 0:
        print("2")
        return "Bad Request: Unrecognised value(s): " + ", ".join(extra_fields), 400

    # Add x-axis 'label' to temp value for graph
    global sensor_data_label
    data['label'] = sensor_data_label
    sensor_data_label += 1

    # Update sensor sistory
    sensor_history.append(data)
    if len(sensor_history) > MAX_SENSOR_STORAGE:
        sensor_history.pop(0)

    # Return LED commands
    return led_mode + ';' + str(led_pattern) + ';' + str(led_delay), 200

@app.route('/api/led', methods=['POST'])
def set_led_command():
    data = request.json
    errors = []

    # Data validation
    if 'mode' not in data or data['mode'] not in VALID_MODES:
        errors.append("Parameter 'mode' must be a valid character")

    if 'pattern' not in data or not isinstance(data['pattern'], int) or data['pattern'] not in range(4096):
        errors.append("Parameter 'pattern' must be an integer between 0 and 4095")

    if 'delay' not in data or not isinstance(data['delay'], int) or data['delay'] not in range(100, 1001):
        errors.append("Parameter 'delay' must be an integer between 100 and 1000")
    
    extra_fields = [key for key in data if key not in ['mode', 'pattern', 'delay']] 
    if len(extra_fields) > 0:
        errors.append("Unrecognised value(s): " + ", ".join(extra_fields))

    if errors:
        return "Bad Request: " + ", ".join(errors), 400

    # Update LED commands
    global led_mode, led_pattern, led_delay
    led_mode = data['mode']
    led_pattern = data['pattern']
    led_delay = data['delay']

    return 'OK', 200
