#include <EEPROM.h>

const byte memoryEEPROMstart = 0;

//Ultrasonic variables
const int trigPin = 9;
const int echoPin = 10;
long duration = 0;
int distance = 0;

//LDR variables
int LDRvalue = 0;
const byte LDRPin = A0;
//RGB variables
const int redPin = 2;
const int greenPin = 3;
const int bluePin = 4;

int manualRedValue = 0;
int manualGreenValue = 0;
int manualBlueValue = 0;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

char stringInput[5] = "";
byte noOfCharacter = 0;
int samplingRate = 0;


//Menu variables
int menuState = 0;  //0 - main ; 1 - sensor ; 2 - reset; 3 - status;  4 - rgb;
const int mainMenuId = 0;
const int sensorMenuId = 1;
const int resetMenuId = 2;
const int statusMenuId = 3;
const int rgbMenuId = 4;
const int samplingRateId = 5;
const int setRgbLightId = 6;

const int resetValue = 0;
const int maxThresholdSamp = 10;
const int minThresholdSamp = 1;

void setup() {

  Serial.begin(9600);
  printMainMenu();
  pinMode(LDRPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

const int size = 10;
char inputValue[size];
int index = 0;

void loop() {
  if (readInput()) {
    int option = atoi(inputValue);
    //Serial.println(option);
    switch (menuState) {
      case 0:
        mainMenu(option);
        break;
      case 1:
        sensorSettingsMenu(option);
        break;
      case 2:
        resetLoggerMenu(option);
        break;
      case 3:
        systemStatusMenu(option);
        break;
      case 4:
        rgbControlMenu(option);
        break;
    }
  }
}


bool readInput() {
  if (index == size - 1) {
    inputValue[index] = 0;
    index = 0;
    return true;
  }
  if (Serial.available() > 0) {
    char getInput = Serial.read();
    inputValue[index] = getInput;
    index++;
    if (getInput == '\n') {
      inputValue[index] = 0;
      index = 0;
      return true;
    }
  }
  return false;
}

void printMainMenu() {
  Serial.println();
  Serial.println("Main Menu :");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB Led Control");
}

void mainMenu(int option) {
  switch (option) {
    case 1:
      menuState = sensorMenuId;
      printSensorMenu();
      break;
    case 2:
      menuState = resetMenuId;
      prinResetMenu();
      break;
    case 3:
      menuState = statusMenuId;
      printStatusMenu();
      break;
    case 4:
      menuState = rgbMenuId;
      printRgbMenu();
      break;
  }
}


void printSensorMenu() {
  Serial.println();
  Serial.println("Sensor Settings Menu :");
  Serial.println("1. Sensors Sampling Interval");
  Serial.println("2. Ultrasonic Alert Threshold");
  Serial.println("3 LDR Alert Threshold");
  Serial.println("4.Back");
}

void sensorSettingsMenu(int option) {
  switch (option) {
    case 1:
      Serial.println("Choose a value for the sampling rate(from 1 to 10):");
      if (Serial.available() > 0) {
        int chooseRate = readInput();
        if (chooseRate <= maxThresholdSamp && chooseRate >= minThresholdSamp) {
          samplingRate = chooseRate;
          Serial.print("The sampling rate is:");
          Serial.println(samplingRate);
        } else
          Serial.println("The value it's not valid. Choose a valid one!");
      }
      menuState = sensorMenuId;
      printSensorMenu();
      break;
    case 4:
      menuState = mainMenuId;
      printMainMenu();
      break;
  }
}

void prinResetMenu() {
  Serial.println();
  Serial.println("Reset Logger Menu :");
  Serial.println("Are you sure?");
  Serial.println("1. Yes");
  Serial.println("2. No");
}

void resetLoggerMenu(int option) {
  switch (option) {
    case 1:
      for (int i = memoryEEPROMstart; i <= EEPROM.length(); i++) {
        EEPROM.put(i, resetValue);
      }
      Serial.println("Data was reset!");
    case 2:
      menuState = mainMenuId;
      printMainMenu();
      break;
  }
}

void printStatusMenu() {
  Serial.println();
  Serial.println("System Status Menu :");
  Serial.println("1. Current Sensor Readings");
  Serial.println("2. Current Sensor Settings");
  Serial.println("3. Display Logged Data");
  Serial.println("4. Back");
}

void systemStatusMenu(int option) {
  switch (option) {
    case 1:
      Serial.println("Choose red value :");
      if (!manualRedValue) {
        manualRedValue = option;
      }
      if (manualRedValue != 0) {
        Serial.println("Choose green value :");
        if (!manualGreenValue) {
          manualGreenValue = option;
        }
      }
      if (manualGreenValue != 0 && manualRedValue != 0) {
        Serial.println("Choose blue value :");
        if (!manualBlueValue) {
          manualBlueValue = option;
        }
      }
      digitalWrite(redPin, manualRedValue);
      digitalWrite(greenPin, manualGreenValue);
      digitalWrite(bluePin, manualBlueValue);
      menuState = rgbMenuId;
      if (manualRedValue != 0 && manualGreenValue != 0 && manualBlueValue != 0)
        printRgbMenu();
    case 4:
      menuState = mainMenuId;
      printMainMenu();
      break;
  }
}

void printRgbMenu() {
  Serial.println();
  Serial.println("RGB Control Menu :");
  Serial.println("1. Manual Color Control");
  Serial.println("2. LED: Toggle Automatic ON/OFF");
  Serial.println("3. Back");
}

void rgbControlMenu(int option) {
  switch (option) {
    case 1:
      menuState = setRgbLightId;
      break;
    case 3:
      menuState = mainMenuId;
      printMainMenu();
      break;
  }
}
