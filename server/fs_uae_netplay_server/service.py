"""
FS-UAE Netplay Service
Copyright (C) 2012 Frode Solheim

This library is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at
your option) any later version.

This library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
"""
from __future__ import print_function

import os
import sys
import uuid
import time
import random
import tempfile
import traceback
import threading
import subprocess
import BaseHTTPServer

max_games = 30
server_host = ""
server_port = 25101
ports_from = 25102
ports_to = 25499
log_dir = ""
service_password = ""
addresses = "127.0.0.1"

for arg in sys.argv[1:]:
    temp = arg.split("=", 1)
    key = temp[0]
    if len(temp) == 2:
        value = temp[1]
    else:
        value = ""
    if key == "--max-games":
        max_games = int(value)
    if key == "--port":
        server_port = int(value)
    if key == "--ports-from":
        ports_from = int(value)
    if key == "--ports-to":
        ports_to = int(value)
    if key == "--log-dir":
        log_dir = value
    if key == "--password":
        service_password = value
    if key == "--addresses":
        addresses = value

print("addresses:  ", addresses)
print("max games:  ", max_games)
print("ports from: ", ports_from)
print("ports to:   ", ports_to)
print("log dir:    ", log_dir)
print("password:   ", service_password)

password_chars = "abcdefghijklmnopqrstuvwxyz" + \
        "ABCDEFGHIJKLMNOPQRSTUVWZYX0123456789"

port_pool = [x for x in range(ports_from, ports_to)]
random.shuffle(port_pool)
games = {}
games_lock = threading.Lock()

class Game:
    def __init__(self):
        self.id = ""
        self.proc = None
        self.password = ""
        self.players = 0
        self.port = 0
        self.log_path = ""
        self.log_file = ""

def create_game(s, args):
    try:
        client_password = args["password"]
    except KeyError:
        return 400, "missing password"
    if client_password != service_password:
        return 403, "wrong password"
    try:
        players = args["players"]
    except KeyError:
        return 400, "missing players"
    try:
        players = int(players)
    except ValueError:
        return 400, "invalid value for players"
    if players < 1 or players > 6:
        return 400, "invalid value for players"

    if len(games) >= max_games:
        return 503, "max number of games running already"

    game = Game()
    game.players = players
    game.password = ""
    for i in range(10):
        game.password += random.choice(password_chars)
    game.port = port_pool.pop(0)

    proc_args = [sys.executable, "-m", "fs_uae_netplay_server.game",
                 "--players=" + str(players),
                 "--password=" + game.password,
                 "--port=" + str(game.port),
                 "--launch-timeout=300",
                 ]
    game.id = str(uuid.uuid4())

    if log_dir:
        game.log_path = os.path.join(log_dir, game.id + ".game.log")
    else:
        t = tempfile.NamedTemporaryFile()
        with t:
            pass
        game.log_path = t.name
    game.log_file = open(game.log_path, "wb")

    print("new game: {0} ({1} players)".format(game.id, game.players))
    print("     log: {0}".format(game.log_path))
    print(proc_args)
    game.proc = subprocess.Popen(proc_args, stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT)
    games[game.id] = game
    # we wait here until the game process has actually started listening
    # on the socket, so when we report the connection info back to the client,
    # connections can actually be established immediately
    data = game.proc.stdout.read(9)
    assert data == "listening"

    def cleanup():
        try:
            with games_lock:
                port_pool.append(game.port)
            game.log_file.close()
            del games[game.id]
        except Exception:
            traceback.print_exc()

    def thread_function():
        try:
            while True:
                data = game.proc.stdout.read(1024)
                if not data:
                    break
            result = game.proc.wait()
            with games_lock:
                print("game {0} done, result {1}".format(game.id, result))
        except Exception:
            traceback.print_exc()
            cleanup()
        else:
            cleanup()

    threading.Thread(target=thread_function).start()

    data = "id={0}&password={1}&port={2}&addresses={3}".format(game.id,
            game.password, game.port, addresses)
    return 200, data

class NetplayServiceHttpHandler(BaseHTTPServer.BaseHTTPRequestHandler):

    def do_GET(s):
        temp = s.path.split("?", 1)
        path = temp[0]
        if len(temp) == 2:
            query_string = temp[1]
        else:
            query_string = ""
        args = {}
        arg_strings = query_string.split("&")
        for arg_string in arg_strings:
            temp = arg_string.split("=", 1)
            key = temp[0]
            if len(temp) == 2:
                value = temp[1]
            else:
                value = ""
            args[key] = value
        response = 404
        data = ""
        if path == "/game/create":
            with games_lock:
                response, data = create_game(s, args)
        s.send_response(response)
        s.send_header("Content-Length", str(len(data)))
        s.end_headers()
        s.wfile.write(data)

def run_server(server_class=BaseHTTPServer.HTTPServer,
        handler_class=NetplayServiceHttpHandler):
    server_address = (server_host, server_port)
    httpd = server_class(server_address, handler_class)
    print("server listening on", repr((server_host, server_port)))
    print("serving forever...")
    httpd.serve_forever()

if __name__ == '__main__':
    run_server()
