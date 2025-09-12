from kivy.app import App
from kivy.uix.boxlayout import BoxLayout

class MyRoot(BoxLayout):
    pass

class MyApp(App):
    def build(self):
        return MyRoot()

    def on_button_click(self):
        print("Button was clicked!")

if __name__ == "__main__":
    MyApp().run()