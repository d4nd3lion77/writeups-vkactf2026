#!/usr/bin/python3
# -*- coding: utf-8 -*-

import io
import os
import lzma
import secrets
import tarfile
import datetime
import humanize
from flask_socketio import SocketIO
from werkzeug.utils import secure_filename
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from flask import Flask, render_template, request, send_file, flash, redirect, url_for, jsonify


app = Flask(__name__)
app.config['SECRET_KEY'] = secrets.token_hex(32)
socketio = SocketIO(app)

ALLOWED_EXTENSIONS = {'txt', 'pdf', 'png', 'jpg', 'jpeg', 'gif', 'doc', 'docx', 'xls', 'xlsx', 'csv', 'py', 'html', 'css', 'js'}
MAX_FILE_SIZE = 50 * 1024 * 1024

app.config['MAX_CONTENT_LENGTH'] = MAX_FILE_SIZE



def encrypt(content: bytes) -> bytes:
    key = os.urandom(32)
    nonce = os.urandom(12)
    
    algorithm = algorithms.AES(key)
    cipher = Cipher(algorithm, mode=modes.GCM(nonce))
    encryptor = cipher.encryptor()
    
    encrypted = encryptor.update(content) + encryptor.finalize()
    
    return encrypted


def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS


def get_file_size(file):
    file.seek(0, io.SEEK_END)
    size = file.tell()
    file.seek(0)
    return size


def format_size(size):
    return humanize.naturalsize(size)


@app.route('/')
def index():
    return render_template('index.html', 
                         max_size=MAX_FILE_SIZE,
                         max_size_readable=format_size(MAX_FILE_SIZE),
                         allowed_extensions=', '.join(sorted(ALLOWED_EXTENSIONS)))


@app.route('/check-file', methods=['POST'])
def check_file():
    if 'file' not in request.files:
        return jsonify({'error': 'No file selected'}), 400
    
    file = request.files['file']
    
    if file.filename == '':
        return jsonify({'error': 'No file selected'}), 400
    
    file_size = get_file_size(file)
    if file_size > MAX_FILE_SIZE:
        return jsonify({
            'error': f'File too large. Maximum size is {format_size(MAX_FILE_SIZE)}',
            'size': file_size,
            'size_readable': format_size(file_size),
            'max_size': MAX_FILE_SIZE,
            'max_size_readable': format_size(MAX_FILE_SIZE)
        }), 400
    
    if not allowed_file(file.filename):
        return jsonify({
            'error': f'File type not allowed. Allowed types: {", ".join(sorted(ALLOWED_EXTENSIONS))}'
        }), 400
    
    return jsonify({
        'success': True,
        'filename': file.filename,
        'size': file_size,
        'size_readable': format_size(file_size)
    })


@app.route('/archive', methods=['POST'])
def archive_file():
    if 'file' not in request.files:
        flash('Пожалуйста, выберите файл')
        return redirect(url_for('index'))
    
    file = request.files['file']
    
    if file.filename == '':
        flash('Пожалуйста, выберите файл')
        return redirect(url_for('index'))
    
    try:
        file_data = file.read()
        filename = secure_filename(file.filename)
        
        tar_buffer = io.BytesIO()
        
        with tarfile.open(fileobj=tar_buffer, mode='w') as tar:
            tar.add("flag.txt")
            info = tarfile.TarInfo(name=filename)
            info.size = len(file_data)
            
            tar.addfile(info, io.BytesIO(file_data))
            
        tar.close()
        tar_data = tar_buffer.getvalue()
        
        compressed = lzma.compress(
            tar_data, 
            format=lzma.FORMAT_XZ,
            preset=9)
        
        encrypted = encrypt(compressed)
        
        timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
        out_filename = f"{os.path.splitext(filename)[0]}_{timestamp}.tar.lzma.enc"
        
        encrypted_buffer = io.BytesIO(encrypted)
        encrypted_buffer.seek(0)
        
        response = send_file(
            encrypted_buffer,
            as_attachment=True,
            download_name=out_filename,
            mimetype='application/octet-stream',
        )

        response.headers['X-Tar-Size'] = str(len(tar_data))
        response.headers['X-Encryption'] = 'AES-256-GCM'
        response.headers['Content'] = encrypted.hex()
        return response
        
    except Exception as e:
        flash(f'Ошибка: {str(e)}')
        return redirect(url_for('index'))


if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5001, debug=False, allow_unsafe_werkzeug=True)