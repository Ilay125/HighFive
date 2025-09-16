from flask import Flask, redirect, url_for, request, render_template
import serial
import time
import threading

ser = serial.Serial('/dev/serial0', 115200, timeout=1)
app = Flask(__name__)

# DATA PASS BETWEEN THREADS
DATA = {}

# CONST
END_LINE = '|'

def process_msg(msg):
    if msg.isnumeric():
        DATA["us_dist"] = round(float(msg), 3) 

def uart_listener():
    buffer = ""

    while True:
        c = ser.read().decode()

        if not c:
            continue

        if c == END_LINE:
            process_msg(buffer)
            buffer = ""
            continue

        buffer += c 

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
    t = threading.Thread(target=uart_listener).start()

    app.run(host="0.0.0.0", port=5000, debug=True)
    ser.close()