# pocket hang drum

This is a digital musical instrument inspired by the sounds of the Hang drum.

[![Watch the video](https://img.youtube.com/vi/T-4I31jZFDY90/maxresdefault.jpg)](https://youtu.be/T-4I31jZFDY90)

Did you ever feel that the original Hang Drum is a really cool instrument but way too expensive? Not digital enough? Not hackable? And way too big and heavy? Then this digital pocket version of the Hang Drum is for you.

# features

* 12 touch pads tuned to a pentatonic (?) scale playing hang drum like sounds. Touches are sensed using capacitive touch.
* 8 note polyphony
* 1/4" mono output jack
* USB power
* A joystick. It works like a 4 way switch: Up / Down transposes the pads in semitone steps, left / right changes the release time.

# components

* Wooden box
* Teensy 3.2 (should work with other Teensy Versions as well)
* Audio Shield for Teensy 3.2
* Arduino Thumb Joystick. I bought the cheapest joystick I and this turned out to be a bad idea...
* Adafruit MPR121 capacitive touch sensor
* I used stripboard, headerpins / female header pins and wires to connect it all together
* Conductive foil tape. I use copper tape.
* Audio output jack, i use a mono 1/4" jack.

# building instructions

The components cost around 40 Euros. If you have basic soldering knowledge and used an arduino / teensy before then this should be an easy build. 

## electronics

1. Solder audio shield to teensy using header pins
2. Solder female header pins to stripboard for teensy and MPR121
3. Cut the traces between the two rows of header pins. Verify this with a multimeter in continuity mode, I fried an Teensy + Audio shield (~30 Euros, ouch) because a strip was not properly cut.
4. Use wires to connect the four pins of MPR121 with the respective teensy pins (+5V, GND, SDA, SDL).
5. Use 4 long wires to connect the four pins of the Joystick to the respective teensy pins (+5V, GND, A0, A1).
6. Solder 12 long wires to the 12 touch sensor inputs of the MPR121.
7. The teensy audio shield has two solder pads for audio out left and gnd. Connect these to the audio jack: Audio Out Left to the jacks tip, GND to the jacks sleeve.

## enclosure

1. Drill 12 holes in the top of the box at the centers of the later locations of the touch pads.
2. Drill a big hole for the joystick, probably more holes for screws to attach the joystick to the top of the box.
3. Drill a hole for USB cable, another one for the audio jack.
4. Put the wires through from the MP3121 sensor inputs through the holes. The lowest note is input 0, the highest note input 11.
5. Cut a hole for the joystick in the tape. Stick the joystick through and attach it. I could take of the hat of the joystick which makes the next step easier...
6. Then strip the wires about one centimeter each. But the tape strips on so that each tape strip is connected to its wire. I put rows of tape and cut off what I did not need with a cutter knife. It is important that the pads do not touch each other.
7. Stick the audio jack through its hole and tighten it on the other side with a screw.
8. Last step, when everything else works: Drill a hole in the strip board and attach the strip board to the box using a screw.

## software

1. Teensy IDE with Teensyduino
2. Install library "Adafruit MPR121"
3. Connect teensy via USB and upload software. If you set the serial console of the Arduino IDE to 9600 baud then you should see messages like "5 touched", "5 relased", ... when you touch the pads.

# if I built a second version then I would...

* either ommit the joystick or use a better joystick. The one I used is too noisy so I could not use it for pitch bend, tremolo or anything performance oriented. It was already installed to the box when I found out so I added the transpose and release functionality. 
* play around more with the proximity mode of the MPR121. Hopefully this can add touch sensitivity and velocity sensitivity.
* add a voltage regulator, external power connector (probably compatible to guitar pedals) and 9V battery pack
* add switches,  on the side of the instrument to add more soundshaping capabilities: Wave form selector, ADSR, scale selector, looper, volume control, FX, ...
* Use an Teensy like 3.6 with onboard DAC. Then I would add trigger and cv outs to connect it my modular synthesizer.
* Add software to turn it into a MIDI controller. The hardware is there, this just needs to be implemented in the software.
* Add a proper neutrik USB plug instead of just a whole.
* Sometimes nodes get stuck.

# license

This project is released under GNU General Public License v3.
