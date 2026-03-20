#! /usr/bin/env python

## FLASK ##################################################
from flask import Flask, jsonify, render_template, request
app = Flask(__name__)

## GLOBAL #################################################
MAX_SENSOR_STORAGE = 25
sensor_history = []

## DASHBOARD ##############################################
@app.route("/")
def hello_world():
    return render_template('index.html')

## API ####################################################
@app.route('/api/data', methods=['POST'])
def receive_data():
    sensor_history.append(request.json)

    if len(sensor_history) > MAX_SENSOR_STORAGE:
        sensor_history.pop(0)

    return "OK"

@app.route('/api/data', methods=['GET'])
def get_data():
    return jsonify(sensor_history)