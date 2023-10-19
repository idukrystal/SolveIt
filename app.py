from flask import request, send_file, render_template, Flask

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')


@app.route('/solve')
def solve():
    type = request.args.get('calculation')
    return render_template('solver.html', type=type)

@app.route('/solver/<type>')
def get_bin_file(type):
    return send_file(
        f'./static/bin/{type}.wasm'
    )

@app.route('/hop.js')
def hop():
    return send_file('./hop.js');
