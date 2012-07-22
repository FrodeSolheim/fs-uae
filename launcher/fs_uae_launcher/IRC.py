class IRC:
    irc = None
    irc_listeners = []
    #nick = ""
    my_nick = ""

    @classmethod
    def get(cls, key):
        return cls.config.setdefault(key, "")

    @classmethod
    def add_listener(cls, listener):
        cls.irc_listeners.append(listener)

    @classmethod
    def broadcast(cls, command, args):
        for listener in cls.irc_listeners:
            fname = "on_irc_" + command
            if hasattr(listener, fname):
                if getattr(listener, fname)(args):
                    return True
            #if listener.on_irc_message(command, args):
            #    return True
        return False

    @classmethod
    def nick(cls, spec):
        nick = spec.split("!")[0]
        nick = nick.strip("@+")
        return nick

    @classmethod
    def me(cls, spec):
        nick = spec.split("!")[0]
        return nick == cls.my_nick

    #def me(cls, who):
    #    return IRC.me(who)
    #    try:
    #        nick = who.split("!")[0]
    #    except ValueError:
    #        nick = who
    #    if nick == cls.nick:
    #        return True
    #    return False
