#include "Pitches.h"

#define Speaker 13   // Digital Pin for speaker
#define SoundBUT 12  // Digital Pin for Sound Button Activation
#define AnsGLED 11   // Answer Green LEDs
#define AnsBLED 10   // Answer Blue LEDs
#define AnsRLED 9    // Answer Red LEDs
#define StarChBUT 8  // Digital Pin for Start Check Activation
#define ModeBUT 7    // Digital Pin for Mode (Auto Check Mode) Activation
#define SoundLED 6   // Digital Pin for Sound Button LED
#define StarChLED 5  // Digital Pin for Start / Check Button LED
#define ModeLED 4    // Digital Pin for Mode (Auto Check Mode) Button LED
#define SysPower 3   // Digital Pin for keeping the Ardunio on
#define EquLED 2     // Equation LEDs
  /* A5 wired for the SDA line of the I2C
     A4 wired for the SCL line of the I2C */
#define TimerPOT A1  // Pin for Mode (Auto Check Mode) Timer Adjustment
#define SysPowBUT A0 // Pin for Power Shutdown button

int OnTimer=0;
/*Boot Melody*/
int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};// notes in the melody:
int noteDuration[] = {4, 8, 8, 4, 4, 4, 4, 4};// note durations: 4 = quarter note, 8 = eighth note, etc.:
/*Incorrect Melody*/
int melody1[] = { NOTE_A7,0,NOTE_A7,0,NOTE_A7,0,NOTE_G2,NOTE_D2};
int noteDuration1[] = { 8, 16,8,16,8,16,8,3};
/*Correct Melody*/
int melody2[] = { NOTE_A7,0,NOTE_A7,0,NOTE_A7,0,NOTE_E7};
int noteDuration2[] = { 8, 16,8,16,8,16,6};

void setup() {
/* Latch Power */
    pinMode(SysPower, OUTPUT);    // Digital Pin for keeping the Ardunio on
    digitalWrite(SysPower, HIGH); // When low system turns off
/* Set Digital Pins */
    pinMode(SoundBUT, INPUT);     // Digital Pin for Sound Button Activation
    pinMode(AnsGLED, OUTPUT);     // Answer Green LEDs
    pinMode(AnsBLED, OUTPUT);     // Answer Red LEDs
    pinMode(AnsRLED, OUTPUT);     // Answer Blue LEDs
    pinMode(StarChBUT, INPUT);    // digital Pin for Start Check Activation
    pinMode(ModeBUT, INPUT);      // digital Pin for Mode Activation
    pinMode(SoundLED, OUTPUT);    // digital Pin for Sound Button LED
    pinMode(StarChLED, OUTPUT);   // digital Pin for Start / Check Button LED
    pinMode(ModeLED, OUTPUT);     // digital Pin for Mode Button LED
    pinMode(EquLED, OUTPUT);      // Equation LEDs
/* Serial, only used for fault finding and testing */    
    //Serial.begin(115200);
/* Boot music */
    Boot(); // Plays boot music
/* LED Check*/
    digitalWrite(AnsGLED, HIGH);   // Answer Green LEDs
    digitalWrite(AnsBLED, HIGH);   // Answer Red LEDs
    digitalWrite(AnsRLED, HIGH);   // Answer Blue LEDs
    digitalWrite(SoundLED, HIGH);  // digital Pin for Sound Button LED
    digitalWrite(StarChLED, HIGH); // digital Pin for Start / Check Button LED
    digitalWrite(ModeLED, HIGH);   // digital Pin for Mode Button LED
    digitalWrite(EquLED, HIGH);    // Equation LEDs (stay on)
    delay(5000);                   // Long delay to CHECK LEDs
    digitalWrite(AnsGLED, LOW);    // Answer Green LEDs
    digitalWrite(AnsBLED, LOW);    // Answer Red LEDs
    digitalWrite(AnsRLED, LOW);    // Answer Blue LEDs
    digitalWrite(SoundLED, LOW);   // digital Pin for Sound Button LED
    digitalWrite(StarChLED, LOW);  // digital Pin for Start / Check Button LED
    digitalWrite(ModeLED, LOW);    // digital Pin for Mode Button LED
}
    
void loop() {
  delay(1000);  // delay 1 second
  OnTimer+=1;   // Increment value
  if (OnTimer>5){ // disconnect power after value reached
    digitalWrite(SysPower, LOW); // Removes signal that keeps the power on (Turns off device)
  }
}

/*Sound functions*/
void Boot(){
  for (int thisNote = 0; thisNote < (sizeof(melody)/sizeof(melody[0])); thisNote++) {
    tone(Speaker, melody[thisNote],(1000/noteDuration[thisNote])); // to calculate the note duration, take one second divided by the note duration.
    delay((1000/noteDuration[thisNote]*1.3)); // Pause is +30% of note
    noTone(Speaker);    // stop the tone playing:
  }
}
void InCorrect(){ // Answer is incorrect
  for (int thisNote = 0; thisNote < (sizeof(melody1)/sizeof(melody1[0])); thisNote++) {
    tone(Speaker, melody1[thisNote], (1000/noteDuration1[thisNote]));
    delay((1000/noteDuration1[thisNote]*1.3));
    noTone(Speaker);// stop the tone playing:
  }
}
void Correct(){ // Answer is correct
  for (int thisNote = 0; thisNote < (sizeof(melody2)/sizeof(melody2[0])); thisNote++) {
    tone(Speaker, melody2[thisNote], (1000/noteDuration2[thisNote]));
    delay((1000/noteDuration2[thisNote]*1.3));
    noTone(Speaker);// stop the tone playing:
  }
}
void Thinking(){ // Plays Thinking Pulse
    tone(Speaker, NOTE_A7, (1000/4));
    delay((1000/4*1.3));
    noTone(Speaker);// stop the tone playing:
}
