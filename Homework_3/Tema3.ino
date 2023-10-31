const int firstFloorButtonPin = 2;
const int secondFloorButtonPin = 3;
const int thirdFloorButtonPin = 4;

const int firstFloorLedPin = 5;
const int secondFloorLedPin = 6;
const int thirdFloorLedPin = 7;


const int elvFloorsPinArray[3] = { 2, 3, 4 };
const int elvLightsPinArray[3] = { 5, 6, 7 };

const int elvSoundPin = 11;
const int elvStatusLedPin = 13;

const int arrivalSound = 1915;
const int travelSound = 150;
const int arrivalSoundDuration = 400;

unsigned int lastDebounceTime = 0;
unsigned long debounceDelay = 50;


unsigned long elvStatusLastBlink = 0;
unsigned long elvStatusTime = 500;

bool elvStatusLedState = HIGH;

byte reading = LOW;
byte lastReading = LOW;
byte isElvLeaving = false;

int buzzerTone = 1000;
int buttonState = -1;

int destinationFloor = -1;
int buttonPressed = -1;
int currentFloor = 0;

unsigned long travelTime = 3000;
unsigned long leavingMoment = 0;
unsigned long doorClosing = 1000;

bool currentLedState = HIGH;
byte elvSoundState = 0;
bool defaultState = true;
bool checkedBefore = 1;


void checkButtonPress(int buttonPin) {

  reading = digitalRead(buttonPin);
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        destinationFloor = buttonPressed
      ;
        lastDebounceTime = 0;
        reading = false;
        lastReading = false;
        buttonState = false;
        defaultState = false;
        //checkedBefore = 0;
      }
    }
  }
  lastReading = reading;
}


void setup() {
  pinMode(firstFloorButtonPin, INPUT_PULLUP);
  pinMode(secondFloorButtonPin, INPUT_PULLUP);
  pinMode(thirdFloorButtonPin, INPUT_PULLUP);

  pinMode(elvStatusLedPin, OUTPUT);

  pinMode(firstFloorLedPin, OUTPUT);
  pinMode(secondFloorLedPin, OUTPUT);
  pinMode(thirdFloorLedPin, OUTPUT);

  pinMode(elvSoundPin, OUTPUT);

  //defaultState
  digitalWrite(firstFloorLedPin, HIGH);
  digitalWrite(elvStatusLedPin, HIGH);
}
void loop() {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(elvFloorsPinArray[i]) == 0) {
      buttonPressed = i;
    }
  }
  checkButtonPress(elvFloorsPinArray[buttonPressed]);

  /* if(destinationFloor == currentFloor && checkedBefore == 0 && isElvLeaving == false){
    tone(elvSoundPin, arrivalSound, arrivalSoundDuration);
    checkedBefore = 1;
  }*/
  if (isElvLeaving == false && destinationFloor != -1 && destinationFloor != currentFloor) {
    leavingMoment = millis();
    isElvLeaving = true;
  }
  if (destinationFloor > currentFloor && isElvLeaving == true) {
    if ((millis() - leavingMoment) >= doorClosing)
      currentLedState = false;
    if ((millis() - leavingMoment) >= travelTime) {
      currentFloor++;
      currentLedState = true;
      if (destinationFloor - currentFloor == 1) {
        currentLedState = false;
        currentFloor++;
      }
      if (destinationFloor == currentFloor) {
        isElvLeaving = false;
        currentLedState = true;
        tone(elvSoundPin, arrivalSound, arrivalSoundDuration);
        elvStatusLedState = 1;
      }
    }
  }

  if (destinationFloor < currentFloor && isElvLeaving == true) {
    if ((millis() - leavingMoment) >= doorClosing)
      currentLedState = false;
    if ((millis() - leavingMoment) >= travelTime) {
      currentFloor--;
      currentLedState = true;
      if (currentFloor - destinationFloor == 1) {
        currentLedState = false;
        currentFloor--;
      }
      if (destinationFloor == currentFloor) {
        currentLedState = true;
        isElvLeaving = false;
        tone(elvSoundPin, arrivalSound, arrivalSoundDuration);
        elvStatusLedState = 1;
      }
    }
  }

  if (currentFloor != -1) {
    if (currentFloor != destinationFloor && defaultState == false) {
      tone(elvSoundPin, travelSound);
      if (millis() - elvStatusLastBlink > elvStatusTime) {
        elvStatusLedState = !elvStatusLedState;
        elvStatusLastBlink = millis();
      }
      digitalWrite(elvStatusLedPin, elvStatusLedState);
    }
    if (currentFloor == destinationFloor)
      digitalWrite(elvStatusLedPin, HIGH);
  }
  if (defaultState != true)
    digitalWrite(elvLightsPinArray[currentFloor], currentLedState);
}