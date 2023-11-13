const int startButtonPin = 3;
const int resetButtonPin = 8;
const int lapButtonPin = 2;
//const int buttonPinArray[3] = { 8, 3, 2 };

const int latchPin = 11; 
const int clockPin = 10;  
const int dataPin = 12;   

const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

unsigned int lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int displayDigits[] = { segD1, segD2, segD3, segD4 };
const int displayCount = 4; 
const int encodingsNumber = 16;

bool wasResetPressed;

volatile bool wasButtonPressed;
volatile bool possiblePress = false;
volatile bool shouldStartCounter = false;

byte isButtonReleasedState = LOW;


// in this vector i know which button was pressed to do a specific action 
//                                   R  S  L
int buttonPressedStatesVector[3] = { 0, 0, 0 };

const int lap = 2;
const int start = 1;
const int reset = 0;

int displayMode = -1;
const int startMode = 0;
const int countingMode = 1;
const int pauseMode = 2;

unsigned long lastIncrement = 0;
unsigned long delayCount = 100;  // Delay between updates (milliseconds)
unsigned long number = 0;       // The number being displayed
unsigned int newNumber = 0;

int lapsSavedMatrix[4][4] = {
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0}
};

byte byteEncodings[encodingsNumber] = {
  //A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
  B11101110,  // A
  B00111110,  // b
  B10011100,  // C
  B01111010,  // d
  B10011110,  // E
  B10001110   // F
};


void setup() {
  // Initialize the pins connected to the shift register as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(startButtonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(lapButtonPin, INPUT_PULLUP);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], HIGH);
  }

  attachInterrupt(digitalPinToInterrupt(startButtonPin), startHandleInterrupt, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(lapButtonPin), lapHandleInterrupt, CHANGE);

  //Serial.begin(9600);
}

void loop() {

  if (buttonPressedStatesVector[start] == true)
    displayMode = countingMode;
  else
    displayMode = pauseMode;
  
  if (millis() - lastIncrement > delayCount) {
    if (displayMode == countingMode)
      number++;
    else
      number = number;
    readReset();
    if(displayMode == pauseMode && buttonPressedStatesVector[reset] == true){
      number = 0;
      buttonPressedStatesVector[reset] = false;
    }
    else if (displayMode == countingMode && buttonPressedStatesVector[reset] == true){
      buttonPressedStatesVector[reset] = false;
    }
    number = mapSecondIntoMinutes(number);
    if (number)
      number %= 10000;
    lastIncrement = millis();
  }
  //Serial.println(number);
  writeNumber(number);
}


/*void saveLaps(){
    int copyNumber = number;
    
}*/

void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[displayNumber], LOW);
}

void readReset(){
    if (digitalRead(resetButtonPin) != isButtonReleasedState){
    lastDebounceTime = millis();
    wasResetPressed = true;
  }
  else if (millis() - lastDebounceTime > debounceDelay && wasResetPressed){
    buttonPressedStatesVector[reset] = true;
    wasResetPressed = false;
  }
}

void writeNumber(int temporaryNumber) {
  int currentNumber = temporaryNumber;
  int displayDigit = 3;
  int lastDigit = 0;

  while (currentNumber != 0) {
    lastDigit = currentNumber % 10;
    activateDisplay(displayDigit);
    if (displayDigit == 2)
      writeReg(byteEncodings[lastDigit] + 1);
    else
      writeReg(byteEncodings[lastDigit]);
    delay(1);
    displayDigit--;
    currentNumber /= 10;
    writeReg(B00000000);
  }
}

//whenever number reaches on display 60.0 second which means a minute I transform it into a minute, doing the same until 9 min and 60 sec
int mapSecondIntoMinutes(int nr) {
  int value;
  switch (nr) {
    case 600:
      value = 1000;  
      break;
    case 1600:
      value = 2000;  
      break;
    case 2600:
      value = 3000;  
      break;
    case 3600:
      value = 4000;  
      break;
    case 4600:
      value = 5000;  
      break;
    case 5600:
      value = 6000;  
      break;
    case 6600:
      value = 7000;  
      break;
    case 7600:
      value = 8000;  
      break;
    case 8600:
      value = 9000;  
      break;
    case 9600:
      value = 0;
      break;
    default:
      value = nr;
      break;
  }
  return value;
}


void startHandleInterrupt() {
  if (digitalRead(startButtonPin) != isButtonReleasedState){
    Serial.println(digitalRead(startButtonPin));
    lastDebounceTime = millis();
    wasButtonPressed = true;
  }
  else if (millis() - lastDebounceTime > debounceDelay && wasButtonPressed){
    buttonPressedStatesVector[start] = !buttonPressedStatesVector[start];
    wasButtonPressed = false;
  }
  
}

/*void lapHandleInterrupt() {
  if (digitalRead(lapButtonPin) != isButtonReleasedState){
    lastDebounceTime = millis();
    wasButtonPressed = true;
  }
  else if (millis() - lastDebounceTime > debounceDelay && wasButtonPressed){
    buttonPressedStatesVector[lap] = !buttonPressedStatesVector[lap];
    wasButtonPressed = false;
  }
  
}*/
