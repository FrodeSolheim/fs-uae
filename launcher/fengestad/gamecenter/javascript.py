from fengestad.gamecenter.main import Main

"""
print(PyV8)
ctxt = PyV8.JSContext()          # create a context with an implicit global object
ctxt.enter()                     # enter the context (also support with statement)
ctxt.eval("1+2")                 # evalute the javascript expression
                                   # return a native python int
class Global(PyV8.JSClass):      # define a compatible javascript class
  def hello(self):               # define a method
    print "Hello World"    
ctxt2 = PyV8.JSContext(Global()) # create another context with the global object
ctxt2.enter()                    
ctxt2.eval("hello()")            # call the global object from javascript

import sys
sys.exit(0)
"""

# when allocation JSCOntext from another thread, there seems to be
# a bug in V8 which throws RangeError / maximum call stack, probably
# because V8 is allocated in stack space outside it's "assumed"
# normal location, thereby invoking a stack overflow protection
# code erroneously.
# Therefore, we run all javascript in the main thread
# Check this again with later versions of V8

class JavaScript(object):
    
    def __init__(self, script):
        self.script = script

    def configure(self, config):
        if self.script:
            return Main.call(self._configure, config)
    
    def _configure(self, config):
        import PyV8
        class JSConfig(PyV8.JSClass):
            def __setattr__(self, key, value):
                print("setattr", key, value)
                self.__dict__[key] = value
        class JSGlobal(PyV8.JSClass):
            config = JSConfig()
        jsglobal = JSGlobal()
        # copy config keys to JSConfig instance
        for key, value in config.__dict__.iteritems():
            if not key.startswith('_'):
                setattr(jsglobal.config, key, value)
        script = u'\n'.join([self.script,
                u'configure(config);'])
        print("script:", script)
        _ctx = PyV8.JSContext(jsglobal)
        print(" -- created -- jscontext")
        with _ctx:
            # check if the function exists
            try:
                _ctx.eval(u'\n'.join([self.script, 'configure;']).encode('UTF-8'))
            except Exception:
                return
            # exists, run configure
            print("running configure...")
            _ctx.eval(script.encode('UTF-8'))
        print("BEFORE", repr(config.__dict__))
        # copy config keys to JSConfig instance
        for key, value in jsglobal.config.__dict__.iteritems():
            print(key, value)
            if not key.startswith('_'):
                setattr(config, key, value)
        print("AFTER ", repr(config.__dict__))

    def rename_file(self, name):
        if self.script:
            return Main.call(self._rename_file, name)
        return name
    
    def _rename_file(self, name):
        import PyV8
        script = '\n'.join([
                self.script,
                'rename_file("' + name + '");'])
        #print("script:", script)
        _ctx = PyV8.JSContext()
        #print(" -- created -- jscontext")
        with _ctx:
            # check if the function exists
            try:
                _ctx.eval(u'\n'.join([self.script, 'rename_file;']).encode(
                        'UTF-8'))
            except Exception:
                return name
            # exists, execute and return result
            result = _ctx.eval(script.encode('UTF-8'))
        #print("result is", repr(result))
        result = result.decode('UTF-8')
        if '/' in result or '\\' in result:
            raise Exception('invalid rename')
        return result

