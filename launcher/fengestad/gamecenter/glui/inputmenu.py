from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

from fengestad.input.inputdevice import InputDevice
from .opengl import *
from .menu import Menu
from .font import Font
from .render import Render
from .state import State
from .items import GameCenterItem, HomeItem, MenuItem
from .input import InputHandler

class InputMenu(Menu):

    def __init__(self, item, controller):
        Menu.__init__(self)
        #self.top_menu_transition = 0.0
        self.items.append(item)
        self.top.left.append(GameCenterItem())
        self.top.left.append(HomeItem())
        self.top.left.append(MenuItem(item.title))

        self.controller = controller
        self.first_shown_at = 0

        # controller must now initialize input ports
        self.controller.init_input()
        #for input in self.controller.inputs:
        #    input["device_id"] = None

        # get all available input devices
        self.devices, info = InputDevice.get_input_devices("", 0, 100, 
                version=2)
        self.device_list_version = InputDevice.device_list_version
        self.device_data = {}
        #[{"index": 0} for x in self.devices]
        for device in self.devices:
            self.device_data[device.id] = {"port": 0, "device": device}
            self.check_device(self.device_data[device.id])
        # FIXME: Make InputHandler / InputDevice set variables
        # etc and self detect when to reinit?
        #InputHandler.reinit_joysticks()

        # calling activate to try to set the active input device
        # to the first input port, if possible
        #self.activate()
        self.set_defaults()

    def set_defaults(self):
        devices = []
        for device in self.devices:
            score = 0
            if device.id == InputHandler.last_device:
                score = -1
            devices.append([score, device])
        devices = [x[1] for x in sorted(devices)]
        for i, input in enumerate(self.controller.inputs):
            for device in devices:
                try:
                    device.configure(input.type)
                except Exception, e:
                    pass
                else:
                    input.device_id = device.id
                    devices.remove(device)
                    self.device_data[device.id]["ok"] = True
                    self.device_data[device.id]["port"] = i
                    break

    def go_left(self, count=1):
        #device_id = InputHandler.last_device
        #for i, device in enumerate(self.devices):
        #    if not device.name == device_name:
        #        continue
        try:
            device_data = self.device_data[InputHandler.last_device]
        except KeyError, e:
            print(repr(e))
            return
        if device_data["port"] > 0:
            device_data["port"] -= 1
            #device_data["port"] -= 1
            self.check_device(device_data)

    def go_right(self, count=1):
        try:
            device_data = self.device_data[InputHandler.last_device]
        except KeyError, e:
            print(repr(e))
            return
        if device_data["port"] < len(self.controller.inputs) - 1:
            device_data["port"] += 1
            #device_data["port"] += 1
            self.check_device(device_data)
        """
        device_name = InputHandler.last_device
        for i, device in enumerate(self.devices):
            if not device.id == device_name:
                continue
            index = self.device_column[i]
            if index < len(self.controller.inputs) - 1:
                index += 1
                self.device_column[i] = index
                self.check_device(device)
            break
        """

    def on_status(self, status):
        print("received status", status)

    def run_game(self):
        print("run_game, controller = ", id(self.controller))
        
        # FIXME:
        from .window import run_game
        #run_game({"controller": self.controller})
        run_game(self.controller, self.on_status)

        go_back_to_game_selection = True

        # FIXME:
        from .window import go_back
        if go_back_to_game_selection:
            go_back()
            go_back()
        else:           
            # recreate controller for game menu
            #State.history[-2].recreate_controller()
            go_back()


    def activate(self):
        if len(self.controller.inputs) == 0:
            return self.run_game()
        #print(self.device_data)
        #print("InputHandler.last_device", InputHandler.last_device)
        try:
            device_data = self.device_data[InputHandler.last_device]
        except KeyError, e:
            print(repr(e))
            return
        #device_data = self.device_data[InputHandler.last_device]
        if not device_data["ok"]:
            return
        port = device_data["port"]
        device = device_data["device"]
        #print("activate", port, device)
        for i in range(len(self.controller.inputs)):
            if i == port:
                if i == 0 and device.id == self.controller.inputs[i].device_id:
                    return self.run_game()   
                self.controller.inputs[i].device_id = device.id
            elif self.controller.inputs[i].device_id:
                # remove this device from other ports
                #if self.controller.inputs[i]["device"].id == device.id:
                if self.controller.inputs[i].device_id == device.id:
                    self.controller.inputs[i].device_id = None

    def check_device(self, device_data):
        device = device_data["device"]
        port = device_data["port"]
        try:
            device.configure(self.controller.inputs[port].type)
        except InputDevice.MissingPlatformSupportException, e:
            device_data["ok"] = False
        except Exception, e:
            print(repr(e))
            device_data["ok"] = False
        else:
            device_data["ok"] = True
        #print(device, index, self.controller.inputs[index]["type"], ok)

    def render(self):
        if self.first_shown_at == 0:
            self.first_shown_at = State.time    
        # FIXME:
        #from .gamemenu import render_wall, render_screen
        #glClear(GL_DEPTH_BUFFER_BIT)
        #render_wall()
        #render_screen()
        
        Render.hd_perspective()
        if len(self.controller.inputs) == 0:
            color = (1.0, 0.0, 0.0, 1.0)
            text = "No input configuration needed"
            tw, th = Render.text(text, Font.main_path_font,
                    200, 730, 400, color=color, halign=0)
            text = "Press enter or primary key to start game"
            tw, th = Render.text(text, Font.main_path_font,
                    200, 670, 400, color=color, halign=0)
            return
        
        if self.device_list_version != InputDevice.device_list_version:
            print(" -- device list version changed")
            self.devices, info = InputDevice.get_input_devices("", 0, 100, version=2)
            self.device_list_version = InputDevice.device_list_version
            #[{"index": 0} for x in self.devices]
            device_ids = set()
            for device in self.devices:
                device_ids.add(device.id)
                try:
                    self.device_data[device.id]["device"] = device
                except KeyError:
                    print(" -- add device info for", device.id)
                    self.device_data[device.id] = {"port": 0, "device": device}
                    self.check_device(self.device_data[device.id])
            for data_key in self.device_data.keys():
                if not data_key in device_ids:
                    print(" -- removing device_data for", data_key)
                    del self.device_data[data_key]
            for input in self.controller.inputs:
                if not input["device_id"] in device_ids:
                    print(" -- removing device from input", input.device_id)
                    input["device_id"] = None
        

        for port, input in enumerate(self.controller.inputs):
            center_x = 400 + 400 * port
            
            text = input.name.upper()
            color = (1.0, 0.0, 0.0, 1.0)
            tw, th = Render.text(text, Font.main_path_font,
                    center_x - 200, 760, 400, color=color, halign=0)
            text = input.description.upper()
            color = (1.0, 0.0, 0.0, 1.0)
            tw, th = Render.text(text, Font.main_path_font,
                    center_x - 200, 730, 400, color=color, halign=0)
            if input.device_id:
                device = self.device_data[input.device_id]["device"]
                color = (1.0, 0.5, 0.5, 1.0)
                text = device.name.upper()
                tw, th = Render.text(text, Font.main_path_font,
                        center_x - 200, 680, 400, color=color, halign=0)

            for j, device in enumerate(self.devices):
                device_data = self.device_data[device.id]
                #print(1, repr(device))
                if device_data["port"] != port:
                    continue
                text = device.name.upper()
                if device_data["ok"]:
                    color = (1.0, 1.0, 1.0, 1.0)
                else:
                    color = (0.5, 0.5, 0.5, 1.0)
                tw, th = Render.text(text, Font.main_path_font,
                        center_x - 200, 600 - j * 40, 400, color=color,
                        halign=0)
        
        fade = 1.0 - (State.time - self.first_shown_at) * 3.0
        if fade > 0.0:
            Render.dirty = True
            fs_emu_blending(True)
            fs_emu_texturing(False)
            glDisable(GL_DEPTH_TEST)
            Render.hd_perspective()
            glBegin(GL_QUADS)
            glColor4f(0.0, 0.0, 0.0, fade)
            glVertex2f(0, 0)
            glVertex2f(1920, 0)
            glVertex2f(1920, 1080)
            glVertex2f(0, 1080)            
            glEnd()
            glEnable(GL_DEPTH_TEST)
