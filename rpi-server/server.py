from flask import Flask, redirect, url_for, request, render_template
from flask_socketio import SocketIO
import serial
import time
import threading

ser = serial.Serial('/dev/serial0', 115200, timeout=1)
app = Flask(__name__)
socketio = SocketIO(app, async_mode="threading")

# DATA PASS BETWEEN THREADS
DATA = {"us_dist": 0}

# CONST
END_LINE = '|'

def process_msg(msg_buffer):
    for msg in msg_buffer:
        if msg.startswith("us"):
            DATA["us_dist"] = round(float(msg[2:]), 3)

    socketio.emit('update', DATA, broadcast=True)


def uart_listener():
    print("Listening...")

    while True:
        data = ser.readline().decode().strip()
        if not data:
            time.sleep(0.01)
            continue

        data_buff = data.split(END_LINE)

        if data_buff[-1] == '':
            data_buff.pop()
        
        #print(f"Got msg {data_buff}")
        process_msg(data_buff)

def fancy_uart(*args):
    up_val, down_val, fast_val = args
    ser.write(f"{up_val},{down_val},{fast_val}|".encode())
    time.sleep(0.5)
    print(f"up servo={up_val}, down servo={down_val}, fast={fast_val}")

    
@app.route('/', methods=["POST", "GET"])
def main():
    if request.method == "POST":
        up_val = int(request.form["up_slider"])
        down_val = int(request.form["down_slider"])
        fast_val = request.form.get("fast_chk") is not None
        fancy_uart(up_val, down_val, fast_val + 0)

    else: # on GET

        # default values
        up_val = 90
        down_val = 90
        fast_val = False
        

    return render_template("index.html", 
                           up_val=str(up_val),
                           down_val=str(down_val),
                           fast_val=fast_val,
                           us_dist=DATA["us_dist"])


if __name__ == '__main__':
    threading.Thread(target=uart_listener, daemon=True).start()
    socketio.run(app,
                 host="0.0.0.0",
                 port=5000,
                 debug=True,
                 allow_unsafe_werkzeug=True)