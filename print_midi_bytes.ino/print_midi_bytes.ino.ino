#include <usbh_midi.h>
#include <usbhub.h>
#include <SPI.h>

// Create USB host object
USB Usb;
// Create USB MIDI driver object
USBH_MIDI Midi(&Usb);

void setup() {
  Serial.begin(115200);
  while (!Serial);  // wait for Serial Monitor
  if (Usb.Init() == -1) {
    Serial.println("USB Host Shield did not start.");
    while (1);  // halt
  }
  Serial.println("USB MIDI Host started...");
}

void loop() {
  Usb.Task();  // Poll USB for events

  // Check if a MIDI message has been received
  if (Midi) {
    uint8_t buf[3];
    uint16_t size;
    while ((size = Midi.RecvData(buf)) > 0) {
      // Print out each byte (word by word)
      Serial.print("Status: ");
      Serial.print(buf[0], HEX);
      Serial.print("  Data1: ");
      Serial.print(buf[1], HEX);
      Serial.print("  Data2: ");
      Serial.println(buf[2], HEX);
    }
  }
}