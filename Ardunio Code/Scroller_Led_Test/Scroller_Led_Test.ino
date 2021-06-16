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

int OnTimer=0;

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
  if (OnTimer<3) {
    digitalWrite(SoundLED, HIGH);  // digital Pin for Sound Button LED
    digitalWrite(StarChLED, HIGH); // digital Pin for Start / Check Button LED
    digitalWrite(ModeLED, HIGH);   // digital Pin for Mode Button LED
    digitalWrite(AnsGLED, HIGH);   // Answer Green LEDs
  } else if (OnTimer<6) {
    digitalWrite(AnsGLED, LOW);   // Answer Green LEDs
    digitalWrite(AnsBLED, HIGH);   // Answer Red LEDs
  } else if (OnTimer<9) {
    digitalWrite(AnsBLED, LOW);   // Answer Red LEDs
    digitalWrite(AnsRLED, HIGH);   // Answer Blue LEDs
  } else if (OnTimer>12) { // disconnect power after value reached
    digitalWrite(SysPower, LOW); // Removes signal that keeps the power on (Turns off device)
  }
}
