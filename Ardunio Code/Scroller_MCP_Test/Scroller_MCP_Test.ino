#include <pu2clr_mcp23008.h>  // Used for MCP I2C commands
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
#define SysPower 2   // Digital Pin for keeping the Ardunio on
#define EquLED 3     // Equation LEDs
  /* A5 wired for the SDA line of the I2C
     A4 wired for the SCL line of the I2C */
#define TimerPOT A1  // Pin for Mode (Auto Check Mode) Timer Adjustment
#define SysPowBUT A0 // Pin for Power Shutdown button
 
MCP mcp1;
MCP mcp2;
MCP mcp3;
MCP mcp4; 

bool TimerRunning = false;  // Toggles the Start Check LED flashing
bool ModeState = false;     // Toggles Auto Check Mode
char PressStage = 'O';      // Tracks Start / Check button state [O,S,C]
int TimerPass = 0;          // Counter value for seting the pulsing StarChLED and Audio
int ClockCounter = 0;       // Counter value for runing in Auto Check Mode
int TimerLength = 0;        // Adjustment time added to the minimum time when in auto mode
  /*Boot Melody*/
int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};// notes in the melody:
int noteDuration[] = {4, 8, 8, 4, 4, 4, 4, 4};// note durations: 4 = quarter note, 8 = eighth note, etc.:
  /*Incorrect Melody*/
int melody1[] = { NOTE_A7,0,NOTE_A7,0,NOTE_A7,0,NOTE_G2,NOTE_D2};
int noteDuration1[] = { 8, 16,8,16,8,16,8,3};
  /*Correct Melody*/
int melody2[] = { NOTE_A7,0,NOTE_A7,0,NOTE_A7,0,NOTE_E7};
int noteDuration2[] = { 8, 16,8,16,8,16,6};
  /*Shutdown Melody*/
int melody3[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int noteDuration3[] = {4, 8, 8, 4, 4, 4, 4, 4};
  /*Button Debounce*/
byte SysPowPRESS = B00000000;   // Debounce record for Power Activation Press
byte StarChPRESS = B00000000;   // Debounce record for Start Check Activation Press
byte ModePRESS = B00000000;     // Debounce record for Mode Activation Press
byte SoundPRESS = B00000000;    // Debounce record for Sound Activation Press
byte DebounceRange = B11111111; // debounce range for Press
  /*MCP data*/
uint8_t Roller1_2=0;
uint8_t Roller3_4=0;
uint8_t Roller5_6=0;
uint8_t Roller7_8=0;
uint8_t Front=B11110000;
uint8_t Back=B00001111;

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
    Serial.begin(115200);
    Serial.println("----------------------------------------"); 
    Serial.println("setup started...");
/* Boot music */
    //Boot(); // Plays boot Melody
/* LED Check */
    digitalWrite(AnsGLED, HIGH);   // Answer Green LEDs
    digitalWrite(AnsBLED, HIGH);   // Answer Red LEDs
    digitalWrite(AnsRLED, HIGH);   // Answer Blue LEDs
    digitalWrite(SoundLED, HIGH);  // digital Pin for Sound Button LED
    digitalWrite(StarChLED, HIGH); // digital Pin for Start / Check Button LED
    digitalWrite(ModeLED, HIGH);   // digital Pin for Mode Button LED
    digitalWrite(EquLED, HIGH);    // Equation LEDs
/* Setup MCPS */ 
    // 0x20=0B0100000 - 0x21=0B0100001 - 0x22=0B0100010 - 0x24=0B0100100
/* if something wrong with i2c Lights will hang */
    mcp1.setup(0x20,0B11111111);   // I2C address 0x20; All GPIO port/pin are configured as intput
    mcp1.setRegister(REG_GPPU, 0B11111111); // sets GPIO 0 to 7 with internal pull up resistors
    mcp1.setRegister(REG_IPOL, 0B11111111); // inverts GPIO 0 to 7
delay(50); // Short delay to see LEDs
    mcp2.setup(0x21,0B11111111);   // I2C address 0x21; All GPIO port/pin are configured as intput
    mcp2.setRegister(REG_GPPU, 0B11111111); // sets GPIO 0 to 7 with internal pull up resistors
    mcp2.setRegister(REG_IPOL, 0B11111111); // inverts GPIO 0 to 7
delay(50); // Short delay to see LEDs
    mcp3.setup(0x22,0B11111111);   // I2C address 0x22; All GPIO port/pin are configured as intput
    mcp3.setRegister(REG_GPPU, 0B11111111); // sets GPIO 0 to 7 with internal pull up resistors
    mcp3.setRegister(REG_IPOL, 0B11111111); // inverts GPIO 0 to 7
delay(50); // Short delay to see LEDs
    mcp4.setup(0x24,0B11111111);   // I2C address 0x24; All GPIO port/pin are configured as intput
    mcp4.setRegister(REG_GPPU, 0B11111111); // sets GPIO 0 to 7 with internal pull up resistors
    mcp4.setRegister(REG_IPOL, 0B11111111); // inverts GPIO 0 to 7
    
/* Turn off LEDs */
                   
    digitalWrite(AnsGLED, LOW);    // Answer Green LEDs
    digitalWrite(AnsBLED, LOW);    // Answer Red LEDs
    digitalWrite(AnsRLED, LOW);    // Answer Blue LEDs
    digitalWrite(SoundLED, LOW);   // digital Pin for Sound Button LED
    digitalWrite(StarChLED, LOW);  // digital Pin for Start / Check Button LED
    digitalWrite(ModeLED, LOW);    // digital Pin for Mode Button LED
}
    
void loop() {
/* Write current button states */ // Bitshift Left x = x << 1
  SysPowPRESS <<= 1;              // Bitshift Left for Power Activation Press
  StarChPRESS <<= 1;              // Bitshift Left for Start Check Activation Press
  ModePRESS <<= 1;                // Bitshift Left for Mode Activation Press
  SoundPRESS <<= 1;               // Bitshift Left for Sound Activation Press
  
/* Adds Current state to each record */
  SysPowPRESS |= (digitalRead(SysPowBUT)); // Adds current value (0/1) to the RHS of the bit
  StarChPRESS |= (digitalRead(StarChBUT)); // Adds current value (0/1) to the RHS of the bit
  ModePRESS |= (digitalRead(ModeBUT));     // Adds current value (0/1) to the RHS of the bit
  SoundPRESS |= (digitalRead(SoundBUT));   // Adds current value (0/1) to the RHS of the bit

/* Reads MCPs */
  Roller1_2 = mcp1.getGPIOS(); // Reads GPIO 0 to 7
  Roller3_4 = mcp2.getGPIOS(); // Reads GPIO 0 to 7
  Roller5_6 = mcp3.getGPIOS(); // Reads GPIO 0 to 7
  Roller7_8 = mcp4.getGPIOS(); // Reads GPIO 0 to 7

/* Prints Binary*/
  Serial.print(Roller1_2, BIN); // Prints Binary byte
  Serial.print("\t");
  Serial.print(Roller3_4, BIN); // Prints Binary byte
  Serial.print("\t");
  Serial.print(Roller5_6, BIN); // Prints Binary byte
  Serial.print("\t");
  Serial.print(Roller7_8, BIN); // Prints Binary byte
  Serial.print("\t");
  Serial.print((uint8_t)(Roller1_2&Front)>>4, BIN); // Prints first part of byte
  Serial.print("\t\t");
  Serial.print(Roller1_2&Back, BIN); // Prints second part of byte
  Serial.println(" ");
  
/*Clear values*/
  Roller1_2=0;
  Roller3_4=0;
  Roller5_6=0;
  Roller7_8=0;
  
/* Apply button Actions */
  if (SysPowPRESS>=DebounceRange){  // Detects if the 'Power' button has been pressed
    ShutDown();                     // Starts the Shutdown sequence (Turns off device)
  }
  if (StarChPRESS>=DebounceRange){ // Detects if the 'Start/Check' button has been pressed
    if (PressStage == 'S'){        // Checks if the button has already been pressed
      Check();                     // Checks the Answer
    } else if (PressStage == 'O'){ // Checks if this is the first press
      Start();                     // Starts the time for the answer to be inputed
    }
  }
  if ( ModePRESS>=DebounceRange){Mode();} // Toggles the Automatic Checking Mode
  if (SoundPRESS>=DebounceRange){Sound();} // Toggles Sound On and Off

/* Activate Program Sub modules */  
  if (PressStage=='S'){Thinking();}  // Run the Thinking module if the start button has been pressed 
  if (digitalRead(ModeLED)==HIGH&&PressStage=='S'){// Set the counter if the start and mode button has been pressed
    ClockCounter++;
  } else {
    ClockCounter=0;
  }
  if (ClockCounter>=(TimerLength+1500)){ // Check if counter is out of time
    ClockCounter=0;
    Check();
  }
  
/* Prints code to demonstrate debounce concept *//*
  Serial.print(digitalRead(StarChBUT),BIN); // Prints current Start Check value
  Serial.print('\t');
  Serial.print(StarChPRESS);                // Prints value of Start Check Bit
  Serial.print('\t');
  Serial.println(StarChPRESS,BIN);          // Prints last eigth Start Check values
  */
/* Prints code to show all relevant buttons and counters for testing */
  /*
  Serial.print(SysPowPRESS,BIN);    // Prints last eigth SysPow values
  Serial.print('\t');
  Serial.print(StarChPRESS,BIN);    // Prints last eigth Start Check values
  Serial.print('\t');
  Serial.print(ModePRESS,BIN);      // Prints last eigth Mode values
  Serial.print('\t');
  Serial.print(SoundPRESS,BIN);     // Prints last eigth Sount values
  Serial.print('\t');
  Serial.print(PressStage);         // Prints the current Start Check Stage
  Serial.print('\t');
  Serial.print(TimerPass);          // Prints the TimerPass Value
  Serial.print('\t');
  Serial.print(analogRead(TimerPOT)); // Prints the Pot Value
  Serial.print('\t');
  Serial.print(TimerLength);        // Prints the stored timerlength from Pot
  Serial.print('\t');
  Serial.println(ClockCounter);     // Prints the current ClockCounter (max=timerlength from Pot + fixed value)
  */
  
  delay(2);                         // System loop delay, increasing will increase button detent length 
}

/*Sound related functions*/
void Boot(){            // Plays Boot Melody
  for (int thisNote = 0; thisNote < (sizeof(melody)/sizeof(melody[0])); thisNote++) {
    tone(Speaker, melody[thisNote],(1000/noteDuration[thisNote])); // to calculate the note duration, take one second divided by the note duration.
    delay((1000/noteDuration[thisNote]*1.3)); // Pause is +30% of note
    noTone(Speaker);    // stop the tone playing:
  }
}

void Thinking(){ // Flashes Start LED and plays Thinking Pulse
  TimerPass += 1;                            // Increment Timer counter
  if (TimerPass  >= 140 & TimerPass < 160) { // between range active pluse sound
    if (digitalRead(SoundLED)== HIGH){       // Only play sound if sound is active
      tone(Speaker, NOTE_A7, (1000/4));      // Play sound
    }
  } 
  if (TimerPass>=160) {                      // Reset Timer when above this range
      noTone(Speaker);                       // stop the tone playing:
      digitalWrite(StarChLED, !digitalRead(StarChLED)); // inverts LED
      TimerPass=0;                           // Reset the Timer counter
  }
}

void Correct(){ // Answer is correct
  analogWrite(AnsGLED,255);                  // Set LED to colour for correct answer
  if (digitalRead(SoundLED)== HIGH){         // Only play sound if sound is active
    for (int thisNote = 0; thisNote < (sizeof(melody2)/sizeof(melody2[0])); thisNote++) {
      tone(Speaker, melody2[thisNote], (1000/noteDuration2[thisNote]));
      delay((1000/noteDuration2[thisNote]*1.3));
      noTone(Speaker);                       // stop the tone playing:
    }
  }
}

void InCorrect(){ // Answer is incorrect
  analogWrite(AnsRLED,255);                 // Set LED to colour for incorrect answer
  if (digitalRead(SoundLED)== HIGH){        // Only play sound if sound is active
    for (int thisNote = 0; thisNote < (sizeof(melody1)/sizeof(melody1[0])); thisNote++) {
      tone(Speaker, melody1[thisNote], (1000/noteDuration1[thisNote]));
      delay((1000/noteDuration1[thisNote]*1.3));
      noTone(Speaker);                      // stop the tone playing:
    }
  }
}

/*Button functions*/
void ShutDown(){
  digitalWrite(SysPower, LOW);     // Removes signal that keeps the power on (Turns off device)
  digitalWrite(AnsGLED, LOW);      // Turn off Green Answer LEDs
  digitalWrite(AnsBLED, LOW);      // Turn off Blue Answer LEDs
  digitalWrite(AnsRLED, LOW);      // Turn off Red Answer LEDs
  digitalWrite(SoundLED, LOW);     // Turn off Sound Button LED
  digitalWrite(StarChLED, LOW);    // Turn off Start / Check Button LED
  digitalWrite(ModeLED, LOW);      // Turn off Mode Button LED
  digitalWrite(EquLED, LOW);       // Turn off Equation LEDs
  /*Shutdown Melody*/
  for (int thisNote = 0; thisNote < (sizeof(melody3)/sizeof(melody3[0])); thisNote++) {
    tone(Speaker, melody3[thisNote],(1000/noteDuration3[thisNote])); // to calculate the note duration, take one second divided by the note duration.
    delay((1000/noteDuration3[thisNote]*1.3)); // Pause is +30% of note
    noTone(Speaker);               // stop the tone playing:
  }
  delay(100000);                   // As the capacitor has to discharge to turn off this locks all inputs and outputs from being triggerd
}
void Start(){
  PressStage = 'S';                // Stores that Start has been pressed
  digitalWrite(StarChLED, HIGH);   // Sets Start Check LED On
  delay(400);                      // Delay alows light to be seen
  StarChPRESS <<= 8;               // Bitshift Left (x8) for Start Check Activation Press
  TimerRunning = true;             // Activates Flasing Timer
}
void Check(){
  digitalWrite(StarChLED, HIGH);   // Forces on Start Check LED
  delay(400);                      // Delay alows light to be seen
  StarChPRESS <<= 8;               // Bitshift Left (x8) for Start Check Activation Press
  Correct();                       // Calls Correct Answer module
  InCorrect();                     // Calls Incorrect Answer module
  if (digitalRead(SoundLED)== LOW){delay(1400);} // Adds extra time to module when operating with no sound
  TimerRunning = false;            // Deactivates Flasing Timer
  TimerPass = 0;                   // Reset Flasing Timer
  PressStage = 'O';                // Clears that Start has been pressed
  digitalWrite(StarChLED, LOW);    // Turn off Start / Check Button LED
  analogWrite(AnsGLED,0);          // Turn off Green Answer LEDs
  analogWrite(AnsBLED,0);          // Turn off Blue Answer LEDs
  analogWrite(AnsRLED,0);          // Turn off Red Answer LEDs
}
void Mode(){
  digitalWrite(ModeLED, !digitalRead(ModeLED));         // Inverts Mode LED
  TimerLength=map(analogRead(TimerPOT),0,1023,0,3000);  // Reads Pot Value and converts it to a larger range
  delay(200);                      // Delay alows debounce after press
  ModePRESS <<= 8;                 // Bitshift Left (x8) for Mode Activation Press  
}
void Sound(){
  digitalWrite(SoundLED, !digitalRead(SoundLED)); // Inverts Sound LED
  delay(200);                     // Delay alows debounce after press
  SoundPRESS <<= 8;               // Bitshift Left (x8) for Sound Activation Press
}
