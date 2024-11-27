import serial
from flask import Flask, request, jsonify
from flask_cors import CORS, cross_origin

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

if __name__ == '__main__':
    app.run(debug=True, host='192.168.2.9', port=5000)
