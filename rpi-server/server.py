from flask import Flask, redirect, url_for, request, render_template
import serial
import time

ser = serial.Serial('/dev/serial0', 115200, timeout=1)
app = Flask(__name__)

def fancy_uart(*args):
    up_val, down_val = args
    ser.write(f"{up_val},{down_val}|".encode())
    time.sleep(0.5)
    print(f"up servo={up_val}, down servo={down_val}")

    
@app.route('/', methods=["POST", "GET"])
def main():
    if request.method == "POST":
        up_val = int(request.form["up_slider"])
        down_val = int(request.form["down_slider"])
        fancy_uart(up_val, down_val)

    else: # on GET

        # default values
        up_val = 90
        down_val = 90
        

    return render_template("index.html", up_val=str(up_val), down_val=str(down_val))


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000, debug=True)
    ser.close()