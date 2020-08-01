/**
 * Pocket Hang Drum
 * 
 * by Juan Karacho
 * see https://github.com/juan-karacho/pocket-hang-drum
 */
 #include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform5; //xy=136,593
AudioSynthWaveform       waveform6; //xy=145,652
AudioSynthWaveform       waveform7; //xy=146,729
AudioSynthWaveform       waveform2; //xy=151,325
AudioSynthWaveform       waveform4; //xy=152,467
AudioSynthWaveform       waveform1;      //xy=154,266
AudioSynthWaveform       waveform8; //xy=155,788
AudioSynthWaveform       waveform3; //xy=160,384
AudioEffectEnvelope      envelope5; //xy=331,594
AudioEffectEnvelope      envelope4; //xy=339,468
AudioEffectEnvelope      envelope6; //xy=340,653
AudioEffectEnvelope      envelope7; //xy=341,730
AudioEffectEnvelope      envelope2; //xy=346,326
AudioEffectEnvelope      envelope1;      //xy=349,267
AudioEffectEnvelope      envelope8; //xy=350,789
AudioEffectEnvelope      envelope3; //xy=355,385
AudioMixer4              mixer2;         //xy=556,339
AudioMixer4              mixer3;         //xy=575,668
AudioMixer4              mixer1;         //xy=875,661
AudioOutputI2S           i2s1;           //xy=1112,654
AudioConnection          patchCord1(waveform5, envelope5);
AudioConnection          patchCord2(waveform6, envelope6);
AudioConnection          patchCord3(waveform7, envelope7);
AudioConnection          patchCord4(waveform2, envelope2);
AudioConnection          patchCord5(waveform4, envelope4);
AudioConnection          patchCord6(waveform1, envelope1);
AudioConnection          patchCord7(waveform8, envelope8);
AudioConnection          patchCord8(waveform3, envelope3);
AudioConnection          patchCord9(envelope5, 0, mixer3, 0);
AudioConnection          patchCord10(envelope4, 0, mixer2, 3);
AudioConnection          patchCord11(envelope6, 0, mixer3, 1);
AudioConnection          patchCord12(envelope7, 0, mixer3, 2);
AudioConnection          patchCord13(envelope2, 0, mixer2, 1);
AudioConnection          patchCord14(envelope1, 0, mixer2, 0);
AudioConnection          patchCord15(envelope8, 0, mixer3, 3);
AudioConnection          patchCord16(envelope3, 0, mixer2, 2);
AudioConnection          patchCord17(mixer2, 0, mixer1, 0);
AudioConnection          patchCord18(mixer3, 0, mixer1, 1);
AudioConnection          patchCord19(mixer1, 0, i2s1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=1119.3333740234375,875
// GUItool: end automatically generated code



// mpr121 initalization

#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;


// synthesizer initializations

float notes[64]; // stores all notes from a2 - a6

const int N_VOICES = 8;
int voiceMap[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int next_voice = 0;

AudioSynthWaveform *waves[12] = {
  &waveform1, &waveform2, &waveform3, &waveform4, &waveform5, &waveform6,
  &waveform7, &waveform8
};

AudioEffectEnvelope *envs[12] = {
  &envelope1, &envelope2, &envelope3, &envelope4, &envelope5, &envelope6,
  &envelope7, &envelope8
};

// scales
int chromatic_scale[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
int pentatonic_scale[12] = {0, 3, 5, 7, 9, 12, 15, 17, 19, 21, 24, 27};

int scale[12] = {0, 3, 5, 7, 9, 12, 15, 17, 19, 21, 24, 27};
int transposeOctave = 1;

// joystick initialization
const int JOYSTICK_X_PIN = A1;
const int JOYSTICK_Y_PIN = A0;
int joystickValueX = 0;
int joystickValueY = 0;

const int JOYSTICK_X_MIN = 378;
const int JOYSTICK_X_MAX = 1023;
const int JOYSTICK_Y_MIN = 524;
const int JOYSTICK_Y_MAX = 1023;

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    delay(10);
  }

  Serial.println("Starting...");

  //  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  // inputs
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);

  // audio init
  AudioMemory(10);

  mixer1.gain(0, 1.0);
  mixer1.gain(1, 1.0);
  mixer1.gain(2, 1.0);
  mixer1.gain(3, 1.0);

  mixer2.gain(0, 1.0);
  mixer2.gain(1, 1.0);
  mixer2.gain(2, 1.0);
  mixer2.gain(3, 1.0);

  mixer3.gain(0, 1.0);
  mixer3.gain(1, 1.0);
  mixer3.gain(2, 1.0);
  mixer3.gain(3, 1.0);

  Serial.println("1");

  // frequencies of // equal tempered scale from a2 - a6
  int i = 0;
  notes[0] = 110; notes[++i] = 116.54; notes[++i] = 123.47; notes[++i] = 130.81; notes[++i] = 138.59; notes[++i] = 146.83; notes[++i] = 155.56; notes[++i] = 164.81; notes[++i] = 174.61; notes[++i] = 185; notes[++i] = 196; notes[++i] = 207.65; notes[++i] = 220; notes[++i] = 233.08; notes[++i] = 246.94; notes[++i] = 261.63; notes[++i] = 277.18; notes[++i] = 293.66; notes[++i] = 311.13; notes[++i] = 329.63; notes[++i] = 349.23; notes[++i] = 369.99; notes[++i] = 392; notes[++i] = 415.3; notes[++i] = 440; notes[++i] = 466.16; notes[++i] = 493.88; notes[++i] = 523.25; notes[++i] = 554.37; notes[++i] = 587.33; notes[++i] = 622.25; notes[++i] = 659.25; notes[++i] = 698.46; notes[++i] = 739.99; notes[++i] = 783.99; notes[++i] = 830.61; notes[++i] = 880; notes[++i] = 932.33; notes[++i] = 987.77; notes[++i] = 1046.5; notes[++i] = 1108.73; notes[++i] = 1174.66; notes[++i] = 1244.51; notes[++i] = 1318.51; notes[++i] = 1396.91; notes[++i] = 1479.98; notes[++i] = 1567.98; notes[++i] = 1661.22; notes[++i] = 1760;

  Serial.println("1");
  for ( int i = 0; i < N_VOICES; i++) {
    envs[i]->attack(9.2);
    envs[i]->hold(0.0);
    envs[i]->delay(0.0);
    envs[i]->decay(80);
    envs[i]->sustain(0.4);
    envs[i]->release(700);

    waves[i]->amplitude(1.0);
    waves[i]->begin(WAVEFORM_TRIANGLE);
  }

  sgtl5000_1.enable();
  sgtl5000_1.volume(1.0); 

  Serial.println("done");
}

float lastJoystickX = 0;
float lastJoystickY = 0;

float readJoystickX() {
  int joyX = analogRead(JOYSTICK_X_PIN);
  joyX -= JOYSTICK_X_MIN;
  float joyXF = (float)joyX / (JOYSTICK_X_MAX - JOYSTICK_X_MIN);
  joyXF -= 0.5;
  joyXF *= -2;
  if (joyXF > 1.0) joyXF = 1.0;
  else if (joyXF < -1.0) joyXF = -1.0;
  return joyXF;
}

float readJoystickY() {
  int joyX = analogRead(JOYSTICK_Y_PIN);
  joyX -= JOYSTICK_Y_MIN;
  float joyXF = (float)joyX / (JOYSTICK_Y_MAX - JOYSTICK_Y_MIN);
  joyXF -= 0.5;
  joyXF *= 2;
  if (joyXF > 1.0) joyXF = 1.0;
  else if (joyXF < -1.0) joyXF = -1.0;
  return joyXF;
}

int joyButtonUp=LOW;
int joyButtonDown=LOW;
int joyButtonLeft=LOW;
int joyButtonRight=LOW;
int release=700;

boolean detectJoyButtonUp(){  
  float joystick=readJoystickY();
  if(joyButtonUp==LOW && joystick>0.8){
    joyButtonUp=HIGH;
    return true;
  }
  if(joyButtonUp==HIGH && joystick<0.8){
    joyButtonUp=LOW;
  }
  return false;
}

boolean detectJoyButtonDown(){  
  float joystick=readJoystickY();
  if(joyButtonDown==LOW && joystick<-0.8){
    joyButtonDown=HIGH;
    return true;
  }
  if(joyButtonDown==HIGH && joystick>-0.8){
    joyButtonDown=LOW;
  }
  return false;
}


boolean detectJoyButtonRight(){  
  float joystick=readJoystickX();
  if(joyButtonRight==LOW && joystick>0.8){
    joyButtonRight=HIGH;
    return true;
  }
  if(joyButtonRight==HIGH && joystick<0.8){
    joyButtonRight=LOW;
  }
  return false;
}

boolean detectJoyButtonLeft(){  
  float joystick=readJoystickX();
  if(joyButtonLeft==LOW && joystick<-0.8){
    joyButtonLeft=HIGH;
    return true;
  }
  if(joyButtonLeft==HIGH && joystick>-0.8){
    joyButtonLeft=LOW;
  }
  return false;
}

int transpose=12;
int releaseTime=700;

void loop() {

  if(detectJoyButtonUp()){
    transpose++;
    if(transpose>52){
      transpose=52;
    }
  }
  if(detectJoyButtonDown()){
    transpose--;
    if(transpose<0){
      transpose=0;
    }
  }
  boolean changeRelease=false;
  if(detectJoyButtonLeft()){
    releaseTime-=100;
    if(releaseTime<100){
      releaseTime=100;
    }
    changeRelease=true;
  }
  if(detectJoyButtonRight()){
    releaseTime+=100;
    if(releaseTime>2000){
      releaseTime=2000;
    }
    changeRelease=true;
  }
  if(changeRelease){
    for ( int i = 0; i < N_VOICES; i++) {
      envs[i]->release(releaseTime);
    }
  }
  
  // Get the currently touched pads and play note
  currtouched = cap.touched();

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      //int note = i;
      int note = scale[i] + transpose;
      float freq = notes[note];
      float r=((float)random(100)/50.0)-1.0;
      float randomChange=(freq*r/100.0)*0.5;
      freq+=randomChange;
      voiceMap[i] = next_voice;
      waves[next_voice]->frequency(freq);
      envs[next_voice]->noteOff();
      envs[next_voice]->noteOn();
      next_voice++;

      if (next_voice % N_VOICES == 0) {
        next_voice = 0;
      }

      //setFrequencies(0);
      Serial.print(i); Serial.println(" touched");
      //      envs[note]->noteOff();
      //      envs[note]->noteOn();

    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      int voice = voiceMap[i];
      envs[voice]->noteOff();
      Serial.print(i); Serial.println(" released");
      //envs[note]->noteOff();
    }
  }

  // reset touch state
  lasttouched = currtouched;

}
