/**
 * Holden Viva Steering Wheel Controls -> JVC Head Unit
 *
 * The Steering Wheel Controls are represented as a resistance
 * on a single wire, in respect to ground.
 *
 * Using one of the Arduino's 10 Bit ADC inputs, this signal is
 * sampled and matched to one of five possible buttons.
 * A simple voltage divider circuit was used to correctly distinguish each.
 *
 * The button is then translated to a digital high output that
 * serves to directly control the JVC Head Unit.
 *
 * Since the Volume Control on the JVC Head Unit uses a rotary encoder,
 * two digital outputs were required to emulate this function.
 *
 * As it's best to keep the original buttons on the JVC Head Unit functional,
 * the digital lines are always kept open as inputs and only changed to outputs
 * when an actual button push is registered. This allows the JVC Head Unit buttons
 * to function in conjunction with the Steering Wheel Control buttons. 
 *
 * The program also waits for the button to be released before sending the digital
 * output line low which allows the seek function to work as intended.
 *
 * There is a delay between rotary encoder signals that has been tweeked to be
 * acceptable to the end user when holding down the volume down or volume up control.
 *
 * @author Ronald Diaz <aussieronnied@gmail.com>
 *
 */
 
// Setup Analog input pin
int sensorPin = A0;
int sensorValue = 0;

// Setup Digital output pins
int powerPin = 9;
int modePin = 2;
int seekPin = 7;
int volAPin = 6;
int volBPin = 5;

/**
 * Modes available
 *
 * (this could be an enum)
 *
 * 0 = none
 * 1 = Vol Down
 * 2 = Vol Up
 * 3 = Seek
 * 4 = Mode
 * 5 = Pwr
 */
 
// Set the currentButton to none
int currentButton = 0;

/**
 * Setup
 */
void setup() {
  
  // Use Serial for debugging
  Serial.begin(57600);
  
  // Prepare all lines as Inputs
  pinMode(powerPin, INPUT);
  pinMode(modePin, INPUT);
  pinMode(seekPin, INPUT);
  pinMode(volAPin, OUTPUT);
  pinMode(volBPin, OUTPUT);
  
  // Prepare the Volume Rotary Encoder lines
  digitalWrite(volAPin, LOW);
  digitalWrite(volBPin, LOW);
}

/**
 * Main program loop
 */
void loop() {

  // Aquire the current button
  currentButton = getButton();
  
  //Serial.println(currentButton);
  
  // If it's not 0, then make that line high
  if(currentButton != 0) { 
   
    /**
     * Volume up
     */
    if(currentButton == 1) {
      Serial.println("Volume Down");
      volumeDown();
      delay(50);
    }
    /**
     * Volume down
     */
    if(currentButton == 2) {
      Serial.println("Volume Up");
      volumeUp();
      delay(50);
    }
    /**
     * Seek
     */
    if(currentButton == 3) {
      Serial.println("Seek: Down");
      pinMode(seekPin, OUTPUT);
      digitalWrite(seekPin, LOW);
      // Keep high while pushed
      while(getButton() == 3){
        delay(50);
      }
      Serial.println("Seek: Up");
      digitalWrite(seekPin, HIGH);
      pinMode(seekPin, INPUT);
    }
    /**
     * Mode
     */
    if(currentButton == 4) {
      Serial.println("Mode: Down");
      pinMode(modePin, OUTPUT);
      digitalWrite(modePin, LOW);
      // Keep high while pushed
      while(getButton() == 4){
        delay(50);
      }
      Serial.println("Mode: Up");
      digitalWrite(modePin, HIGH);
      pinMode(modePin, INPUT);
    }
    /**
     * Power
     */
    if(currentButton == 5) {
      Serial.println("Power: Down");
      pinMode(powerPin, OUTPUT);
      digitalWrite(powerPin, LOW);
      // Keep high while pushed
      while(getButton() == 5){
        delay(50);
      }
      Serial.println("Power: Up");
      digitalWrite(powerPin, HIGH);
      pinMode(powerPin, INPUT);
    }
  }

  // Main program delay  
  delay(100);
}

/**
 * Get the current state of the controls
 *
 * Uses ranges to allow a tolerance of
 * error in part of the ADC input signal
 *
 */
int getButton() {
 int s = analogRead(sensorPin);
 if(s < 244) {
  return 1;
 }
 if(s > 244 && s < 619){
  return 2;
 }
 if(s > 619 && s < 796){
  return 3;
 }
 if(s > 796 && s < 907){
   return 4;
 } 
 if(s > 907 && s < 988) {
   return 5;
 }
 return 0;
}

/**
 * Common method used when changing the volume
 */
void setupVolume() {
  pinMode(volAPin, OUTPUT);
  pinMode(volBPin, OUTPUT);
  digitalWrite(volAPin, LOW);
  digitalWrite(volBPin, LOW); 
}

/**
 * Common method used when changing the volume
 */
void resetVolume() {
  pinMode(volAPin, INPUT);
  pinMode(volBPin, INPUT);
}

/**
 * Perform Volume Down signals, emulating a rotary encoder
 */
void volumeDown() {
  
  setupVolume();
  
  // B up 1st...
  digitalWrite(volBPin, HIGH);
  delay(10);
  
  // A next...
  digitalWrite(volAPin, HIGH);
  delay(10);
  
  // B down
  digitalWrite(volBPin, LOW);
  delay(10);
  
  // A down
  digitalWrite(volAPin, LOW);
  delay(10);
  
  resetVolume();
}

/**
 * Perform Volume Up signals, emulating a rotary encoder
 */
void volumeUp() {
  
  setupVolume();
  
  // A up 1st...
  digitalWrite(volAPin, HIGH);
  delay(10);
  
  // B next...
  digitalWrite(volBPin, HIGH);
  delay(10);
  
  // A down
  digitalWrite(volAPin, LOW);
  delay(10);
  
  // B down
  digitalWrite(volBPin, LOW);
  delay(10);

  resetVolume();  
}

