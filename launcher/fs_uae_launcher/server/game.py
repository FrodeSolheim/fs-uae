"""
FS-UAE Netplay Game Server
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

import sys
import time
from collections import deque
import socket
import traceback
import threading
import random
from hashlib import sha1

SERVER_PROTOCOL_VERSION = 1
MAX_PLAYERS = 6

if sys.version > '3':
    PYTHON3 = True
else:
    PYTHON3 = False

if PYTHON3:
    def int_to_bytes(number):
        return bytes([(number & 0xff000000) >> 24, (number & 0x00ff0000) >> 16,
                (number & 0x0000ff00) >> 8, (number & 0x000000ff)])
    def bytes_to_int(m):
        return m[0] << 24 | m[1] << 16 | m[2] << 8 | m[3]
    def byte_ord(v):
        #print("byte_ord", v)
        try:
            return v[0]
        except TypeError:
            return v
    def byte(v):
        return bytes([v])
    server_protocol_version = byte(SERVER_PROTOCOL_VERSION)

else:
    def int_to_bytes(number):
        return chr((number & 0xff000000) >> 24) + \
                chr((number & 0x00ff0000) >> 16) + \
                chr((number & 0x0000ff00) >> 8) + \
                chr((number & 0x000000ff))
    def bytes_to_int(m):
        return ord(m[0]) << 24 | ord(m[1]) << 16 | ord(m[2]) << 8 | ord(m[3])
    def byte_ord(v):
        return ord(v)
    def byte(v):
        return v
    server_protocol_version = chr(SERVER_PROTOCOL_VERSION)

max_drift = 25
num_clients = 2
port = 25100
host = "0.0.0.0"
game = None
game_password = 0
launch_timeout = 0

def create_game_password(value):
    # for python 2 + 3 compatibility
    #if not isinstance(value, unicode):
    value = value.encode("UTF-8")
    #print(repr(value))
    h = sha1()
    h.update(b"FSNP")
    val = b""
    for v in value:
        if byte_ord(v) < 128:
            val += byte(v)
    #print("update:", repr(val))
    h.update(val)
    return bytes_to_int(h.digest()[:4])

for arg in sys.argv:
    if arg.startswith("--"):
        parts = arg[2:].split("=", 1)
        if len(parts) == 2:
            key, value = parts
            key = key.lower()
            if key == "port":
                port = int(value)
            elif key == "players":
                num_clients = int(value)
            elif key == "password":
                #game_password = crc32(value) & 0xffffffff
                game_password = create_game_password(value)
                #print("game password (numeric) is", game_password)
            elif key == "launch-timeout":
                launch_timeout = int(value)


MESSAGE_READY             =  0
MESSAGE_MEM_CHECK         =  5
MESSAGE_RND_CHECK         =  6
MESSAGE_PING              =  7
MESSAGE_PLAYERS           =  8
MESSAGE_PLAYER_TAG_0      =  9
MESSAGE_PLAYER_TAG_1      = 10
MESSAGE_PLAYER_TAG_2      = 11
MESSAGE_PLAYER_TAG_3      = 12
MESSAGE_PLAYER_TAG_4      = 13
MESSAGE_PLAYER_TAG_5      = 14

MESSAGE_PLAYER_PING       = 15
MESSAGE_PLAYER_LAG        = 16
MESSAGE_SET_PLAYER_TAG    = 17
MESSAGE_PROTOCOL_VERSION  = 18
MESSAGE_EMULATION_VERSION = 19
MESSAGE_ERROR             = 20
MESSAGE_TEXT              = 21
MESSAGE_SESSION_KEY       = 22

#MESSAGE_MEM_CHECK = 5
#MESSAGE_RND_CHECK = 6
#MESSAGE_PING = 7

MESSAGE_MEMCHECK_MASK = (0x80000000 | (MESSAGE_MEM_CHECK << 24))
MESSAGE_RNDCHECK_MASK = (0x80000000 | (MESSAGE_RND_CHECK << 24))

ERROR_PROTOCOL_MISMATCH    =  1
ERROR_WRONG_PASSWORD       =  2
ERROR_CANNOT_RESUME        =  3
ERROR_GAME_ALREADY_STARTED =  4
ERROR_PLAYER_NUMBER        =  5
ERROR_EMULATOR_MISMATCH    =  6
ERROR_CLIENT_ERROR         =  7
ERROR_MEMORY_DESYNC        =  8
ERROR_RANDOM_DESYNC        =  9
ERROR_SESSION_KEY          = 10
ERROR_GAME_STOPPED         = 99

def create_ext_message(ext, data):
    return 0x80000000 | ext << 24 | (data & 0xffffff)

class Client:

    def __init__(self, socket, address):
        #self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket = socket
        self.messages = []
        self.lock = threading.Lock()
        self.address = address
        self.ready = 0
        self.tag = b"PLY"
        self.player = 0
        self.playing = False
        self.frame = 0
        self.frame_times = [0.0 for x in range(100)]
        self.lag = 0.0
        self.out_seq = 0
        self.memcheck = [(0, 0) for x in range(100)]
        self.rndcheck = [(0, 0) for x in range(100)]
        self.ping_sent_at = 0
        self.pings = deque([0 for x in range(10)])
        self.pings_sum = 0
        self.pings_avg = 0
        #self.protocol_version = 0
        self.emulator_version = b""
        self.session_key = 0


        self.temp_a = 0
        self.temp_b = 0
        self.temp_c = 0

        threading.Thread(target=self.__thread_function).start()

    def send_error_message(self, error_num):
        print(self, "error", error_num)
        message = 0x80000000 | (MESSAGE_ERROR) << 24 | error_num
        self.send_message(message)

    def send_message(self, message):
        with self.lock:
            #if message == 0x87000000:
            #    #print("queueing %08x" % (message,))
            #    #traceback.print_stack()
            #    self.temp_c += 1
            self.__send_data(int_to_bytes(message))

    def __send_data(self, data):
        #if data[0] == '\x87':
        #    #print("queueing ping")
        #    #traceback.print_stack()
        #    self.temp_c += 1
        self.socket.sendall(data)

    def queue_message(self, message):
        with self.lock:
            #if message == 0x87000000:
            #    #print("queueing %08x" % (message,))
            #    #traceback.print_stack()
            #    self.temp_c += 1
            #print("queueing %08x" % (message,))
            #if message & 0x20000000:
            #    traceback.print_stack()
            self.messages.append(int_to_bytes(message))
            if len(self.messages) == 100:
                self.__send_queued_messages()

    def queue_bytes(self, message):
        with self.lock:
            self.messages.append(message)
            if len(self.messages) == 100:
                self.__send_queued_messages()

    def send_queued_messages(self):
        with self.lock:
            self.__send_queued_messages()

    def __send_queued_messages(self):
        data = b"".join(self.messages)
        self.messages[:] = []
        #print("sending", repr(data))
        self.__send_data(data)

    def initialize_client(self):
        print("initialize", self)
        def read_bytes(num):
            data = b""
            for i in range(num):
                data = data + self.socket.recv(1)
            if not len(data) == num:
                raise Exception("did not get {0} bytes".format(num))
            return data
        # check header message
        data = read_bytes(4)
        if data == b"PING":
            # connection check only
            self.__send_data(b"PONG")
            return False
        if data != b"FSNP":
            print(data)
            raise Exception("did not get expected FSNP header")
        # check protocol version
        data = self.socket.recv(1)
        #print(repr(data), repr(server_protocol_version))
        if data != server_protocol_version:
            print("protocol mismatch")
            self.send_error_message(ERROR_PROTOCOL_MISMATCH)
            return False
        # check net play password
        password = bytes_to_int(read_bytes(4))
        if password != game_password:
            print("wrong password")
            self.send_error_message(ERROR_WRONG_PASSWORD)
            return False
        # read emulator version
        self.emulator_version = read_bytes(8)
        # read player number and session key, session key is checked to
        # make sure another client cannot hijack this player slot
        self.session_key = bytes_to_int(b"\0" + read_bytes(3))
        self.player = byte_ord(self.socket.recv(1))
        self.tag = read_bytes(3)
        # get package sequence number
        self.resume_from_packet = bytes_to_int(read_bytes(4))

        error = game.add_client(self)
        if error:
            print(repr(error))
            self.send_error_message(error)
            return False

        message = create_ext_message(MESSAGE_SESSION_KEY, self.session_key)
        self.queue_message(message)
        data = (self.player << 8) | num_clients
        message = create_ext_message(MESSAGE_PLAYERS, data)
        self.queue_message(message)

        game.send_player_tags(self)
        self.send_queued_messages()
        print("initialize done for", self)
        return True

    def __thread_function(self):
        try:
            try:
                if not self.initialize_client():
                    #print("initialize failed for", self)
                    return
                self.receive_loop()
            except Exception:
                traceback.print_exc()
                game.stop = True
        finally:
            try:
                self.socket.close()
            except Exception:
                pass


    def receive_loop(self):
        data = b""
        count = 0
        while not game.stop:
            new_data = self.socket.recv(4 - count)
            if not new_data:
                # FIXME
                return
            data = data + new_data
            count = len(data)
            if count == 4:
                self.on_message(data)
                count = 0
                data = b""

    def send_ping(self):
        with self.lock:
            #print("ping?", self.ping_sent_at)
            if self.ping_sent_at == 0:
                self.temp_a += 1
                self.ping_sent_at = time.time()
                message = int_to_bytes(0x80000000 | (7 << 24))
                self.__send_data(message)
                assert self.ping_sent_at > 0

    def on_ping(self):
        # may not need lock here
        with self.lock:
            self.temp_b += 1
            if self.temp_a != self.temp_b:
                print(self.temp_a, self.temp_b, self.temp_c)
            assert self.ping_sent_at > 0
            t = time.time()
            new = (t - self.ping_sent_at) / 1.0
            #print(t, "-", self.ping_sent_at, "=", new)
            old = self.pings.popleft()
            self.pings.append(new)
            #print(self.pings)
            self.pings_sum = self.pings_sum - old + new
            self.pings_avg = self.pings_sum / len(self.pings)
            self.ping_sent_at = 0

    def on_message(self, m):
        message = bytes_to_int(m)
        #with game.lock:
        #    if not self.playing:
        #        print(self, "is no longer playing/valid, ignoring message")

        if message & 0x80000000:
            # ext message
            command = (message & 0x7f000000) >> 24
            data = message & 0x00ffffff;
            if command == MESSAGE_MEM_CHECK:
                self.memcheck[self.frame % 100] = (data, self.frame)
            elif command == MESSAGE_RND_CHECK:
                self.rndcheck[self.frame % 100] = (data, self.frame)
            elif command == MESSAGE_PING:
                #print("{0:x}".format(message))
                self.on_ping()
            elif command == MESSAGE_TEXT:
                print("received text command")
                remaining = data
                text = b""
                while not game.stop:
                    part = self.socket.recv(remaining)
                    count = len(part)
                    text += part
                    remaining = remaining - count
                    if remaining == 0:
                        game.add_text_message(self, text)
                        break

        elif message & (1 << 30):
            frame =  message & 0x3fffffff
            #print("received frame", frame)
            if frame != self.frame + 1:
                print("error, expected frame", self.frame + 1, "got", frame)
            #print("received frame", frame)
            self.frame = frame
            t = time.time()
            self.frame_times[self.frame % 100] = t
            game_t = game.frame_times[self.frame % 100]
            self.lag = t - game_t

        elif message & (1 << 29):
            game.add_input_event(self, message & 0x00ffffff)

        else:
            print("warning: unknown command received %x" % (message,))

    def __str__(self):
        return "<Client {0}:{1} {2}>".format(self.player, self.tag,
                self.address)

def create_session_key():
    return random.randint(0, 2**24 - 1)

class Game:

    def __init__(self, num_clients):
        self.started = False
        self.frame = 0
        self.time = 0
        self.clients = []
        self.num_clients = 0
        self.frame_times = [0.0 for x in range(100)]
        self.lock = threading.Lock()
        self.stop = False
        self.session_keys = [0 for x in range(MAX_PLAYERS)]
        self.emulator_version = b""
        self.verified_frame = -1

    def __start(self):
        if len(self.clients) != num_clients:
            printf("error - cannot start until all players have connected")
            return
        print("{0} clients connected, starting game".format(num_clients))
        self.started = True
        threading.Thread(target=self.__thread_function).start()

    def add_client(self, client):
        with self.lock:
            if client.player == 0xff:
                if client.resume_from_packet != 0:
                    return ERROR_CLIENT_ERROR
                if self.started:
                    return ERROR_GAME_ALREADY_STARTED
                client.player = len(self.clients)
                if client.player == 0:
                    self.emulator_version = client.emulator_version
                else:
                    if self.emulator_version != client.emulator_version:
                        return ERROR_EMULATOR_MISMATCH
                client.session_key = create_session_key()
                self.session_keys[client.player] = client.session_key
                self.clients.append(client)
                client.playing = True
                if not self.started:
                    if len(self.clients) == num_clients:
                        self.__start()
            else:
                if client.player >= len(self.clients):
                    return ERROR_PLAYER_NUMBER
                if self.session_keys[client.player] != client.session_key:
                    return ERROR_SESSION_KEY
                old_client = self.clients[client.player]
                # FIXME: must transfer settings for resuming to work
                self.clients[client.player] = client
                client.playing = True

                if client.resume_from_packet > 0:
                    # cannot resume yet...
                    print("cannot resume at packet", resume_from_packet)
                    return ERROR_CANNOT_RESUME
        return 0
        # FIXME: start the game..?

    def __thread_function(self):
        try:
            self.__game_loop()
        except Exception:
            traceback.print_exc()
            self.stop = True

    def __send_player_tags(self, send_to_client):
        for i, client in enumerate(game.clients):
            data = bytes_to_int(b"\0" + client.tag)
            message = create_ext_message(MESSAGE_PLAYER_TAG_0 + i, data)
            send_to_client.queue_message(message)

    def send_player_tags(self, client):
        with game.lock:
            self.__send_player_tags(client)

    def __game_loop(self):
        with self.lock:
            for client in self.clients:
                self.__send_player_tags(client)
        print("game loop running")
        self.time = time.time()
        while True:
            if self.stop:
                print("stopping game loop")
                # try to send error message to all players
                with self.lock:
                    for client in self.clients:
                        try:
                            client.send_error_message(ERROR_GAME_STOPPED)
                        except Exception:
                            traceback.print_exc()
                return
            self.__game_loop_iteration()

    def __game_loop_iteration(self):
        # FIXME: higher precision sleep?
        target_time = self.time + 0.02
        t2 = time.time()
        diff = target_time - t2
        sleep = diff - 0.001
        if sleep > 0.0:
            #print(sleep)
            time.sleep(sleep)
        self.time = target_time
        while time.time() < target_time:
            # busy-loop until target time
            pass

        with self.lock:
            if self.frame % 100 == 0:
                self.__send_status()
            self.frame += 1
            self.frame_times[self.frame % 100] = time.time()
            message = (1 << 30) | self.frame
            self.__send_to_clients(message, True)
            if self.frame % 10 == 0:
                for client in self.clients:
                    client.send_ping()
            if self.frame % 200 == 0:
                self.__print_status()

        self.__check_game()

    def __check_game(self):
        oldest_frame, frames = self.__check_frame_status()
        while oldest_frame > self.verified_frame:
            self.check_synchronization(self.verified_frame + 1)
            self.verified_frame += 1
        diff = self.frame - oldest_frame
        if diff <= max_drift:
            # clients have not drifted too far
            return
        first = True
        count = 0;
        while diff > 0 and not self.stop:
            if first:
                first = False
                print("---", self.frame, "acked", frames)
            elif count % 100 == 0:
                print("   ", self.frame, "acked", frames)
            time.sleep(0.02)
            oldest_frame, frames = self.__check_frame_status()
            diff = self.frame - oldest_frame
            count += 1
        self.time = time.time() - 0.02

    def __check_frame_status(self):
        oldest_frame = self.frame
        frames = []
        with self.lock:
            for client in self.clients:
                af = client.frame
                if af < oldest_frame:
                    oldest_frame = af
                frames.append(af)
        return oldest_frame, frames

    def __print_status(self):
        for i, client in enumerate(self.clients):
            print("{0} f {1:6d} p avg: {2:3d} {3:3d}".format(i,
                    client.frame, int(client.pings_avg * 1000),
                    int(client.lag * 1000)))

    def __send_status(self):
        for i, client in enumerate(self.clients):
            v = int(client.lag * 1000) & 0x0000ffff
            message = 0x80000000 | MESSAGE_PLAYER_LAG << 24 | i << 16 | v
            self.__send_to_clients(message)
            v = int(client.pings_avg * 1000) & 0x0000ffff
            message = 0x80000000 | MESSAGE_PLAYER_PING << 24 | i << 16 | v
            self.__send_to_clients(message)

    def add_input_event(self, client, input_event):
        if not self.started:
            # game was not started - ignoring input event
            print("game not started, ignoring input event {0:08x}".format(
                    input_event))
            return
        with self.lock:
            if not client.playing:
                print("client", client, "is no longer valid, dropping msg")
                return
            # for now, just broadcast out again to all clients
            message = (1 << 29) | input_event
            self.__send_to_clients(message)

    def add_text_message(self, from_client, text):
        print("add text message")
        with self.lock:
            for client in self.clients:
                #if from_client == client:
                #    continue
                message = 0x80000000 | MESSAGE_TEXT << 24 \
                        | from_client.player << 16 | len(text)
                message = int_to_bytes(message) + text
                print("send", repr(message), "to", client)
                client.queue_bytes(message)

    def send_to_clients(self, message, force_send=False):
        # using a lock here to ensure that send_to_clients can
        # be called from multiple threads, but all clients will
        # still get messages in expected order
        with self.lock:
            self.__send_to_clients(message, force_send)

    def __send_to_clients(self, message, force_send=False):
        for client in self.clients:
            #print("send", message, "to", client)
            client.queue_message(message)
            if force_send:
                client.send_queued_messages()

    def check_synchronization(self, check_frame):
        # FIXME: MOVE TO GAME CLASS
        # FIXME: ONLY RUN ONCE PER FRAME, not once per frame * clients
        #if self.frame == 0:
        #    return
        with game.lock:
            #print("received memcheck", data)
            #check_frame = self.frame - 1
            if check_frame < 0:
                return
            index = check_frame % 100
            mem_check_data = []
            rnd_check_data = []
            for client in game.clients:
                if client.frame <= check_frame:
                    # cannot check yet
                    return
                #print(client, client.frame, client.memcheck)
                mem_check_data.append((client, client.memcheck[index]))
                rnd_check_data.append((client, client.rndcheck[index]))
            check = mem_check_data[0][1]
            for client, data in mem_check_data:
                if check != data:
                    print("memcheck failed for frame", check_frame)
                    for c, d in mem_check_data:
                        print("* {0:08x}  f {1:05d} {2}".format(d[0], d[1], c))
                    for c in game.clients:
                        c.send_error_message(ERROR_MEMORY_DESYNC)
                        c.send_queued_messages()
                    raise Exception("mem check failed")
            check = rnd_check_data[0][1]
            for client, data in rnd_check_data:
                if check != data:
                    print("rndcheck failed for frame", check_frame)
                    for c, d in rnd_check_data:
                        print("* {0:08x}  f {1:05d} {2}".format(d[0], d[1], c))
                    for c in game.clients:
                        c.send_error_message(ERROR_RANDOM_DESYNC)
                        c.send_queued_messages()
                    raise Exception("rnd check failed")

address_map = {

}

def accept_client(server_socket):
    global last_keepalive_time

    server_socket.settimeout(1)
    client_socket, client_address = server_socket.accept()
    #client_socket.setsockopt(socket.SOL_SOCKET, socket.SO_SNDLOWAT, 4)
    client_socket.setsockopt(socket.SOL_TCP, socket.TCP_NODELAY, 1)
    client_socket.settimeout(None)
    client = Client(client_socket, client_address)
    #client.player = len(game.clients) + 1
    print("Client connected", client)
    last_keepalive_time = time.time()

    #game.add_client(client)
    #client.start()
    #if game.can_start():
    #    game.start()

#stop_accepting = False

def accept_thread(server_socket):
    while not game.stop:
        try:
            accept_client(server_socket)
        except socket.timeout:
            pass
        except Exception:
            traceback.print_exc()
            time.sleep(1.0)

def _run_server():
    global game
    global last_keepalive_time
    game = Game(num_clients)
    address = (host, port)
    server_socket = socket.socket()
    server_socket.bind(address)
    server_socket.listen(10)
    print("listening")
    sys.stdout.flush()
    if address[0] != "0.0.0.0":
        print("server listening on", address[0], "port", address[1])
    else:
        print("server listening on port", address[1])
    print("want", num_clients, "client(s)")
    if num_clients > MAX_PLAYERS:
        print("ERROR: max clients are", MAX_PLAYERS)
    threading.Thread(target=accept_thread, args=(server_socket,)).start()
    last_keepalive_time = time.time()
    while not game.started:
        time.sleep(0.1)
        if launch_timeout:
            t2 = time.time()
            if t2 - last_keepalive_time > launch_timeout:
                print("game not started yet, aborting (timeout)")
                game.stop = True
                return
                #sys.exit()
    print("game started")
    while not game.stop:
        time.sleep(0.1)

def run_server():
    try:
        _run_server()
    except Exception:
        traceback.print_exc()
    except KeyboardInterrupt:
        traceback.print_exc()
    game.stop = True

if __name__ == "__main__":
    run_server()
