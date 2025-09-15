import os
import fluidsynth
import time
import serial

SERIAL_PORT = '/dev/cu.usbmodem11301'
BAUD_RATE = 9600

def main():

    sf_path = os.path.join(os.path.dirname(__file__), "pianojak.sf2")

    fs = fluidsynth.Synth()
    fs.start()

    sfid = fs.sfload(sf_path)
    fs.program_select(0, sfid, 0, 0)
    
    #test code
    fs.noteon(0, 60, 100)
    fs.noteon(0, 67, 100)
    fs.noteon(0, 76, 100)

    time.sleep(1.0)

    fs.noteoff(0, 60)
    fs.noteoff(0, 67)
    fs.noteoff(0, 76)

    time.sleep(1.0)

    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout = 1) as ser:
        while True:
            line = ser.readline()

            if len(line) >= 3:
                onoff, note, velocity = line[0], line[1], line[2]
                print(onoff, note, velocity)
            
                if onoff == 1:
                    print(f"onjon velocity is {velocity}")
                    fs.noteon(0, note, velocity)
                elif onoff == 0:
                    print("offjak")
                    fs.noteoff(0, note)

    fs.delete()

if __name__ == "__main__":
    main()