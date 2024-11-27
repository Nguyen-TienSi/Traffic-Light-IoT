import serial
from flask import Flask, request, jsonify
from flask_cors import CORS, cross_origin
import socket

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
