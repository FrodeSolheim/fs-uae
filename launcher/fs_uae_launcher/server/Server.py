import sys
import time
import atexit
import subprocess

class Server:

    def __init__(self, port, players, password):
        self.port = port
        self.players = players
        self.password = password
        self.proces = None

    def start(self):
        args = [sys.executable] + sys.argv[:]
        args.append("--server")
        args.append("--port={0}".format(self.port))
        args.append("--players={0}".format(self.players))
        if self.password:
            args.append("--password={0}".format(self.password))
        print("start gamepad config, args =", args)
        self.process = subprocess.Popen(args, close_fds=True)
        # a quick hack to give the server time to start, should instead
        # read stdout or something else
        time.sleep(2.0)
        atexit.register(self.kill)

    def kill(self):
        print("Server.kill")
        if self.process is not None:
            self.process.kill()
        self.process = None
