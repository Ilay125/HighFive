from flask import Flask, redirect, url_for, request, render_template
app = Flask(__name__)


@app.route('/success/<name>')
def success(name):
    return f'Welcome {name}!'


@app.route('/login', methods=['POST', 'GET'])
def login():
    if request.method == 'POST':
        user = request.form['nm']
        return redirect(url_for('success', name=user))
    return render_template("index.html")
    
@app.route('/')
def main():
    return redirect(url_for('login'))


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000, debug=True)