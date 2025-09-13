# from kivy.app import App
# from kivy.uix.boxlayout import BoxLayout
# from kivy.lang import Builder

# class MyRoot(BoxLayout):
#     pass

# class MyApp(App):
#     def build(self):
#         return Builder.load_file("layout.kv")

#     def on_button_click(self):
#         print("Button was clicked!")

# if __name__ == "__main__":
#     MyApp().run()

import serial
from kivy.app import App
from kivy.uix.label import Label
from kivy.clock import Clock

class ArduinoApp(App):
    def build(self):
        # Adjust to your Arduino's port and baud rate
        self.arduino = serial.Serial(port="/dev/cu.usbmodem1401", baudrate=9600, timeout=1)
        self.label = Label(text="Waiting for Arduino...")

        # Check for messages every 0.1 seconds
        Clock.schedule_interval(self.read_from_arduino, 0.1)

        return self.label

    def read_from_arduino(self, dt):
        if self.arduino.in_waiting > 0:
            raw = self.arduino.readline()
            try:
                message = raw.decode("utf-8").strip()
            except UnicodeDecodeError:
                message = raw.decode("latin-1").strip()  # fallback
            print(f"Arduino says: {message}")
            self.label.text = f"Arduino: {message}"

if __name__ == "__main__":
    ArduinoApp().run()