#include <usbh_midi.h>
#include <usbhub.h>
#include <SPI.h>

// Create USB host object
USB Usb;
// Create USB MIDI driver object
USBH_MIDI Midi(&Usb);

const int OCTAVE = 1;
const int SENSOR_PIN = 2;
const int LISTEN_PIN = 3;
const int S0 = 4;
const int S1 = 5;
const int S2 = 6;
const int S3 = 7;
const int NUM_CHANNELS = 12;

bool keyStates[NUM_CHANNELS] = {0};
unsigned long keyPressTimes[NUM_CHANNELS] = {0};

// helper: set mux channel (0–15)
void setMuxChannel(byte channel) {
  digitalWrite(S0, channel & 0x01);
  digitalWrite(S1, (channel >> 1) & 0x01);
  digitalWrite(S2, (channel >> 2) & 0x01);
  digitalWrite(S3, (channel >> 3) & 0x01);
}

void setup() {
  Serial.begin(115200);
  while (!Serial);  // wait for Serial Monitor
  if (Usb.Init() == -1) {
    Serial.println("USB Host Shield did not start.");
    while (1);  // halt
  }

  pinMode(SENSOR_PIN, OUTPUT);
  pinMode(LISTEN_PIN, INPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  Serial.println("USB MIDI Host started...");
}

void loop() {


  // for (int ch = 0; ch < NUM_CHANNELS; ch++) {
  digitalWrite(SENSOR_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SENSOR_PIN, LOW);
  // delayMicroseconds(100);
  // return;
  for (int ch = 1; ch < 2; ch++) {
    setMuxChannel(ch);
    delayMicroseconds(20);
    unsigned long begin = micros();
    // for (int timer=0; timer<50; ++timer)
    // {
    //   if ( digitalRead(LISTEN_PIN) ) break;
    //   delayMicroseconds(3);
    // }
    /*
    while (!digitalRead(LISTEN_PIN)) {
      if (micros()-begin > 200) break;
    }
    */
    bool keyPressed = digitalRead(LISTEN_PIN);

    if (keyPressed) {
      if (keyStates[ch] == false) {  // first press
        keyStates[ch] = true;
        keyPressTimes[ch] = micros();
        Serial.println("YOOO");
        // remove delay if you want non-blocking scanning
      }
    } else {
      keyStates[ch] = false;  // key released
    }

    /*
    if ((((unsigned long)micros()-begin)>36))
    {
      if (keyStates[ch] == false) {
        keyStates[ch] = true;
        keyPressTimes[ch] = micros();
        Serial.println("YOOO");
        delay(1000);
        // while(true);
      }
    }
    else
    {
      keyStates[ch] = false;
    }
    */
    // if (current != keyStates[ch]) {
    //   keyStates[ch] = current;

    //   if (current) {
    //     int end = micros();
    //     int out = end - begin;
    //     Serial.println(out);
    //     buf[2] = out;
    //     // Example: send MIDI Note On here
    //   }
    // }
  }

  Usb.Task();  // Poll USB for events

    // Check if a MIDI message has been received
  if (Midi) {
    uint8_t buf[3];
    uint16_t size;
    while ((size = Midi.RecvData(buf)) > 0) {
      unsigned long midiTime = micros();
      // Print out each byte (word by word)
      unsigned long elapsed = midiTime - keyPressTimes[(uint8_t)buf[1]-12*OCTAVE-12];
      Serial.println("elapsed");
      Serial.println(elapsed);
      Serial.println("buf0");
      Serial.println(buf[0]);
      Serial.println("buf1");
      Serial.println(buf[1]);
      Serial.println("buf2");
      Serial.println(buf[2]);
      Serial.println("miditime");
      Serial.println(midiTime);
      // Serial.write(buf[0]); // Note On, Channel 1
      // Serial.write(buf[1]);   // Middle C
      // Serial.write(buf[2]);  // Velocity
    }
  }

  
}