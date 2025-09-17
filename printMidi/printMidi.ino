#include <UHS2-MIDI.h>


USB Usb;
UHS2MIDI_CREATE_DEFAULT_INSTANCE(&Usb);

uint8_t sensorPin = 8;

// -----------------------------------------------------------------------------

// This function will be automatically called when a NoteOn is received.
// It must be a void-returning function with the correct parameters,
// see documentation here:
// https://github.com/FortySevenEffects/arduino_midi_library/wiki/Using-Callbacks

long t0 = 0;


void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  // Do whatever you want when a note is pressed.

  // Try to keep your callbacks short (no delays ect)
  // otherwise it would slow down the loop() and have a bad impact
  // on real-time performance.
  byte myData[] = {1, pitch, velocity};
  Serial.write(myData, 3);
  Serial.println();
  // Serial.print(pitch);
  // Serial.print(", ");
  // Serial.print(2048/(millis()-t0));
  // Serial.print(", ");
  // Serial.println(velocity);
  // t0 = 0;
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  byte myData[] = {0, pitch, 0};
  Serial.write(myData, 3);
  Serial.println();
  // Do something when the note is released.
  // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
}

// -----------------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);
  // Connect the handleNoteOn function to the library,
  // so it is called upon reception of a NoteOn.
  pinMode(sensorPin, INPUT);

  MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

  // Do the same for NoteOffs
  MIDI.setHandleNoteOff(handleNoteOff);

  // Initiate MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);

  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  Serial.println("inited");
  delay( 200 );
}

// 127 = 15
// 1 = 250

// 1/256 to 1/16
// (1/x-1/256)*16
// 16/x-1/16

void loop()
{
  // Serial.println(digitalRead(12));
  // delay(100);
  // return;
  
  Usb.Task();
  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read();

  bool keyDown = !digitalRead(sensorPin);
  if (t0 == 0 && keyDown) t0 = millis();
  else if (!keyDown && t0 > 0) t0 = 0;

  // There is no need to check if there are messages incoming
  // if they are bound to a Callback function.
  // The attached method will be called automatically
  // when the corresponding message has been received.
}