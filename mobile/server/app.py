import serial
from flask import Flask, request, jsonify
from flask_cors import CORS, cross_origin
import socket
import json

app = Flask(__name__)
CORS(app)

@app.route('/control', methods=['POST'])
def control_arduino():
    try:
        with serial.Serial(port='COM2', baudrate=9600, timeout=1) as ser:
            data = request.get_json()
            command = data.get('command')
            if command:
                ser.write((command + '\n').encode())
                return jsonify({'message': 'Command sent successfully'}), 200
            else:
                return jsonify({'error': 'No command provided'}), 400
    except serial.SerialException as e:
        return jsonify({'error': f'Serial communication error: {e}'}), 500

@app.route('/data', methods=['GET'])
def get_data():
    try:
        with serial.Serial(port='COM2', baudrate=9600, timeout=1) as ser:
            line = ser.readline().decode('utf-8').strip()
            if line:
                try:
                    data = json.loads(line)
                    return jsonify({'data': data}), 200
                except json.JSONDecodeError as e:
                    return jsonify({'error': f'Invalid JSON from Arduino: {e}'}), 500
            else:
                return jsonify({'error': 'No data received from Arduino'}), 404
    except serial.SerialException as e:
        return jsonify({'error': f'Serial communication error: {e}'}), 500
    except Exception as e:
        return jsonify({'error': f'An unexpected error occurred: {e}'}), 500

def get_ipv4():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))  # Kết nối đến một server DNS công cộng
        ip = s.getsockname()[0]
        s.close()
        return ip
    except Exception as e:
        return f"Error: {e}"

if __name__ == '__main__':
    app.run(debug=True, host=get_ipv4(), port=5000)
