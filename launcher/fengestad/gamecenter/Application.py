from fengestad.base.Application import Application as BaseApplication


class Application(BaseApplication):

    def __init__(self, name):
        BaseApplication.__init__(self, name)
        #Application.set_instance(self)
