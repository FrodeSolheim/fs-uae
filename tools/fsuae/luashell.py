
import socket
import re

class LuaShellError(Exception):
    def __init__(self, msg):
        self.msg = msg

class LuaShell:
    """helper class to access FS-UAE via the lua shell"""
    prompt = '> '

    def __init__(self, host="localhost", port=6800):
        self._host = host
        self._port = port
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.settimeout(1)
        self._fsuae_ver = None
        self._lua_ver = None

    def connect(self):
        try:
            self._socket.connect((self._host, self._port))
            # read and parse header
            header = self._socket.recv(80)
            header = header.decode('utf-8').strip()
            m = re.search("FS-UAE (\S+) Lua (\S+)", header)
            if m is None:
                raise LuaShellError("Invalid Header: " + header)
            self._fsuae_ver = m.group(1)
            self._lua_ver = m.group(2)
            # read prompt
            prompt = self._socket.recv(80)
            prompt = prompt.decode('utf-8')
            if prompt != self.prompt:
                raise LuaShellError("Invalid Prompt: " + prompt)
        except IOError as e:
            raise LuaShellError("IO Error: "+str(e))
        except socket.timeout:
            raise LuaShellError("Timeout")

    def disconnect(self):
        try:
            self._socket.shutdown(socket.SHUT_RDWR)
        except IOError as e:
            pass
        except socket.timeout:
            pass
        self._socket = None
        self._fsuae_ver = None
        self._lua_ver = None

    def get_versions(self):
        return (self._fsuae_ver, self._lua_ver)

    def execute(self, cmd, num_res=-1):
        try:
            cmd += '\r\n'
            buf = cmd.encode('utf-8')
            self._socket.sendall(buf)
            # read result
            result = ""
            while True:
                line = self._socket.recv(1024)
                if line == b'':
                    raise LuaShellError("Connection lost")
                result += line.decode('utf-8')
                # prompt at end?
                lp = len(self.prompt)
                if len(result) >= lp:
                    end = result[-lp:]
                    if end == self.prompt:
                        result = result[:-lp]
                        break
            # split lines
            lines = result.split('\n')[:-1]
            lres = []
            for l in lines:
                lres.append(l.replace('\r',''))
            # got an error?
            if len(lres) == 1 and lres[0].startswith('ERROR:'):
                raise LuaShellError(lres[0])
            # check result
            if num_res >= 0:
                if len(lres) != num_res:
                    raise LuaShellError("Wrong number of result lines: "+num_res)
            # return result
            return lres
        except IOError as e:
            raise LuaShellError("IO Error: "+str(e))
        except socket.timeout:
            raise LuaShellError("Timeout")

# quick shell test
if __name__ == '__main__':
    sh = LuaShell()
    sh.connect()
    # eval command
    res = sh.execute('=3+2',1)
    print(res)
    # raise error
    res = sh.execute('=2+')
