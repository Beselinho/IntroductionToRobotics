// declare all the joystick pins
const int pinSW = 2;  // digital pin connected to switch output
const int pinX = A0;  // A0 - analog pin connected to X output
const int pinY = A1;  // A1 - analog pin connected to Y output

// declare all the segments pins
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

const int segSize = 8;

int xValue = 0;
int yValue = 0;

bool joyMoved = false;

//minumum values at which the move from the joystick counts
const int minNeutralThreshold = 450;
const int maxNeutralThreshold = 550;
const int minValueThreshold = 150;
const int maxValueThreshold = 950;

int positionInMatrix = 0;
int currentLedPin = 0;


unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const int intervalBlink = 500;
const int intervalReset = 3000;

const int noOfLeds = 8;
const int directions = 4;  // up, down, left, right
int directionJoyMoved = 0;
bool defaultState = false;  //the starting state, where you are on dp and just dp blinks


unsigned long beforeButtonPress = 0;
bool wasPressed = 0;
int reading = 0;

volatile bool possiblePress = false;
volatile bool shouldStartCounter = false;

unsigned long lastDebounceTime = 0;
unsigned int debounceDelay = 50;

byte ledState = LOW;
byte state = HIGH;
byte swState = LOW;
byte lastSwState = HIGH;


int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

//for remembering which segments should remain high after you leave it
int brightSegments[segSize] = {
  0, 0, 0, 0, 0, 0, 0, 0
  //a, b, c, d, e, f, g, dp
};

//the matrix shows where you can go from a specific led, depending on the direction
//the columns are the directions ; the rows are the segments
//e.g. the second row shows where u can go from led B : can go UP to 0(A), can go DOWN to 6(G), can go LEFT to 5(F), and cant go RIGHT (-1 stands for impossible move)
int movesMatrix[noOfLeds][directions] = {
  // u  d  l  r
  { -1, 6, 5, 1 },   // a
  { 0, 6, 5, -1 },   // b
  { 6, 3, 4, 7 },    // c
  { 6, -1, 4, 2 },   // d
  { 6, 3, -1, 2 },   // e
  { 0, 6, -1, 1 },   // f
  { 0, 3, -1, -1 },  // g
  { -1, -1, 2, -1 }  // dp
};


//just makes the led blink :)
void makeItBlink(int ledPin) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= intervalBlink) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}

//functions that transform the positionInMatrix to the right segment pin
int convertIntoRightPin(int ledPin) {
  int value;
  switch (ledPin) {
    case 0:
      value = 12;  //A
      break;
    case 1:
      value = 10;  //B
      break;
    case 2:
      value = 9;  //C
      break;
    case 3:
      value = 8;  //D
      break;
    case 4:
      value = 7;  //E
      break;
    case 5:
      value = 6;  //F
      break;
    case 6:
      value = 5;  //G
      break;
    case 7:
      value = 4;  //DP
      break;
    default:
      value = -1;
      break;
  }
  return value;
}

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);

  currentLedPin = pinDP;
  positionInMatrix = 7;  // 7 = dp, you start from dp
  //Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(pinSW), handleInterrupt, FALLING);
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swState = digitalRead(pinSW);

  makeItBlink(currentLedPin);

  //after holding down the button for 3 second, defaultState becomes true and resets the 7 segment display
  if (defaultState == true) {
    for (int i = 0; i < segSize; i++) {
      digitalWrite(segments[i], LOW);
    }
    currentLedPin = pinDP;
    positionInMatrix = 7;
    defaultState = false;
  }

  if (possiblePress) {
    // Start the debounce counter if needed
    if (shouldStartCounter) {
      lastDebounceTime = millis();
      shouldStartCounter = false;
    }
    // If the debounce period has elapsed
    if ((millis() - lastDebounceTime) > debounceDelay) {
      // Read the current state of the button
      reading = digitalRead(pinSW);
      if (wasPressed == false) {
        beforeButtonPress = millis();
        wasPressed = true;
      }
      if (reading == 1) {
        if (millis() - beforeButtonPress >= intervalReset && wasPressed == true) {  //if it was a long press defaultState becomes true and will reset the 7 segment display(the if from the beginning of the loop)
          defaultState = true;
          wasPressed = false;
        } else {
          brightSegments[positionInMatrix] = 1;  //if it was a short press that means you want the led to remain high
          wasPressed = false;
        }
      }

      possiblePress = false;
    }
  }

  if (xValue < minValueThreshold && joyMoved == false) {  //verify in which direction the joystick moved
    directionJoyMoved = 0;
    if (movesMatrix[positionInMatrix][directionJoyMoved] != -1) {  //if the move is possible
      if (brightSegments[positionInMatrix] == 1)                   // i verify if the segment was supposed to remain high after leaving it
        digitalWrite(currentLedPin, HIGH);
      else
        digitalWrite(currentLedPin, LOW);
      positionInMatrix = movesMatrix[positionInMatrix][directionJoyMoved];  //positionInMatrix becomes the position of the led you are standing right now
      currentLedPin = convertIntoRightPin(positionInMatrix);                // and currentLedPin becomes the pin of the led you are standing right now
      joyMoved = true;
    }
  }

  //the logic is the same for the other ifs, just the direction changed

  if (xValue > maxValueThreshold && joyMoved == false) {
    directionJoyMoved = 1;
    if (movesMatrix[positionInMatrix][directionJoyMoved] != -1) {
      if (brightSegments[positionInMatrix] == 1)
        digitalWrite(currentLedPin, HIGH);
      else
        digitalWrite(currentLedPin, LOW);
      positionInMatrix = movesMatrix[positionInMatrix][directionJoyMoved];
      currentLedPin = convertIntoRightPin(positionInMatrix);
      joyMoved = true;
    }
  }

  if (yValue > maxValueThreshold && joyMoved == false) {
    directionJoyMoved = 2;
    if (movesMatrix[positionInMatrix][directionJoyMoved] != -1) {
      if (brightSegments[positionInMatrix] == 1)
        digitalWrite(currentLedPin, HIGH);
      else
        digitalWrite(currentLedPin, LOW);

      positionInMatrix = movesMatrix[positionInMatrix][directionJoyMoved];
      currentLedPin = convertIntoRightPin(positionInMatrix);
      joyMoved = true;
    }
  }
  if (yValue <= minValueThreshold && joyMoved == false) {
    directionJoyMoved = 3;
    if (movesMatrix[positionInMatrix][directionJoyMoved] != -1) {
      if (brightSegments[positionInMatrix] == 1)
        digitalWrite(currentLedPin, HIGH);
      else
        digitalWrite(currentLedPin, LOW);
      positionInMatrix = movesMatrix[positionInMatrix][directionJoyMoved];
      currentLedPin = convertIntoRightPin(positionInMatrix);
      joyMoved = true;
    }
  }

  //for the joystick to not go crazy, i want it to reset(return to the neutral positon) and only then I consider the direction in which it moves
  if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
    joyMoved = false;
}

void handleInterrupt() {
  possiblePress = true;
  shouldStartCounter = true;
}


//MOVE LIST
//UP => X < upValueThreshold  0
//DOWN => X > downValueThreshold 1
//LEFT => Y > leftValueThreshold 2
//RIGHT => Y < rightValueThreshold 3