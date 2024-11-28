import serial
import time
import json
from flask import Flask
import threading

app = Flask(__name__)

ser = None

def read_data():
    global ser
    while True:
        if ser is not None and ser.is_open:
            try:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    data = json.loads(line)
                    print(data)
            except json.JSONDecodeError as e:
                print(f"JSON decoding error: {e}, Line: {line}")
            except serial.SerialException as e:
                print(f"Serial communication error: {e}")
                break
            except KeyboardInterrupt:
                print("Keyboard interrupt detected. Closing serial port...")
                break
            except Exception as e:
                print(f"An unexpected error occurred in read_data(): {e}")
                break
            time.sleep(0.1)
        else:
            time.sleep(1)


if __name__ == '__main__':
    try:
        ser = serial.Serial(port='COM2', baudrate=9600, timeout=1)
        print("Serial port opened successfully.")
        
        thread = threading.Thread(target=read_data)
        thread.daemon = True
        thread.start()
        
        # Start the Flask app
        app.run(debug=True, use_reloader=False)
    except serial.SerialException as e:
        print(f'Serial communication error: {e}')
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
    finally:
        if ser is not None and ser.is_open:
            ser.close()
            print("Serial port closed.")
            
