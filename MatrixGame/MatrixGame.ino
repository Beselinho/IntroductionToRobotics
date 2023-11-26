#include "LedControl.h"

const byte pinSW = 2; 
const int pinX = A0;   
const int pinY = A1;   
const int actionButtonPin = 7;

int xValue = 0;
int yValue = 0;
bool joyMoved = false;

//minumum values at which the move from the joystick counts
const int minNeutralThreshold = 350;
const int maxNeutralThreshold = 650;
const int minValueThreshold = 150;
const int maxValueThreshold = 950;


const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;

const int matrixSize = 8;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
byte matrixBrightness = 2;

//directions
int directionJoyMoved = -1;
const int up = 0;
const int down = 1;
const int left = 2;
const int right = 3;


int playerPositionRow = -1;
int playerPositionCol = -1;
int bombRow = -1;
int bombCol = -1;

unsigned long currentPlayerMillis = 0;
unsigned long previousPlayerMillis = 0;
const int intervalPlayerBlink = 400;

unsigned long currentBombMillis = 0;
unsigned long previousBombMillis = 0;
const int intervalBombBlink = 100;

unsigned long previousBombExplodeMillis = 0;
const int explodeInterval = 3000;

unsigned long trackCooldown = 0;
unsigned long lastTrackCooldown = 0;
unsigned long momentBombExplode = 0;
const int putBombCooldown = 3500;
const int explosionDissapear = 500;


bool wasActionPressed = 0;
byte isButtonReleasedState = HIGH;

bool bombHasExploded = false;
bool playerAlive = true;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

byte firstRow = 0;
byte firstCol = 0;


int matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};


void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);

  pinMode(pinSW, INPUT_PULLUP);
  pinMode(actionButtonPin, INPUT_PULLUP);

  randomSeed(analogRead(0));
  playerPositionRow = firstRow;
  playerPositionCol = firstCol;
  setupRandomTerrain();
  Serial.begin(9600);
}

void loop() {

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  //swState = digitalRead(pinSW);
  if (playerAlive) {
    playerMove();
    makePlayerBlink();
    readActionButton();
    makeBombBlink();
  }

  for (int row = firstRow; row < matrixSize; row++) {
    for (int col = firstCol; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

void makePlayerBlink() {
  currentPlayerMillis = millis();
  if (currentPlayerMillis - previousPlayerMillis >= intervalPlayerBlink) {
    previousPlayerMillis = currentPlayerMillis;
    matrix[playerPositionRow][playerPositionCol] = !matrix[playerPositionRow][playerPositionCol];
    lc.setLed(0, playerPositionRow, playerPositionCol, matrix[playerPositionRow][playerPositionCol]);
  }
}

void makeBombBlink() {
  //readActionButton();
  if (bombRow != -1 && bombCol != -1) {
    currentBombMillis = millis();
    if (currentBombMillis - previousBombMillis >= intervalBombBlink) {
      previousBombMillis = currentBombMillis;
      matrix[bombRow][bombCol] = !matrix[bombRow][bombCol];
      lc.setLed(0, bombRow, bombCol, matrix[bombRow][bombCol]);
    }

    //bomb explosion radius is 3x3 so the moment when the bomb explodes it flashes the spots affected
    if (currentBombMillis - previousBombExplodeMillis >= explodeInterval) {
      previousBombExplodeMillis = currentBombMillis;
      for (int i = bombRow - 1; i <= bombRow + 1; i++)
        for (int j = bombCol - 1; j <= bombCol + 1; j++) {
          matrix[i][j] = HIGH;
          lc.setLed(0, i, j, matrix[i][j]);
        }
      momentBombExplode = millis();
      bombHasExploded = true;
    }
    if (currentBombMillis - momentBombExplode >= explosionDissapear && bombHasExploded == true) {
      momentBombExplode = currentBombMillis;
      for (int i = bombRow - 1; i <= bombRow + 1; i++)
        for (int j = bombCol - 1; j <= bombCol + 1; j++) {
          matrix[i][j] = LOW;
          lc.setLed(0, i, j, matrix[i][j]); //if player is inside the bomb radius he dies, and bad ending screen appears, game over
          if (i == playerPositionRow && j == playerPositionCol) {
            playerAlive = false;
            badEnding();
          }
        }
      bombRow = -1;
      bombCol = -1;
      bombHasExploded = false;
    }
  }
}

void playerMove() {
  if (xValue < minValueThreshold && joyMoved == false) {  //verify in which direction the joystick moved
    directionJoyMoved = up;
    if (playerPositionRow - 1 >= firstRow && matrix[playerPositionRow - 1][playerPositionCol] != HIGH) {
      matrix[playerPositionRow][playerPositionCol] = LOW;
      playerPositionRow -= 1;
    }
    joyMoved = true;
  }
  if (xValue > maxValueThreshold && joyMoved == false) {
    directionJoyMoved = down;
    if (playerPositionRow + 1 <= matrixSize - 1 && matrix[playerPositionRow + 1][playerPositionCol] != HIGH) {
      matrix[playerPositionRow][playerPositionCol] = LOW;
      playerPositionRow += 1;
    }
    joyMoved = true;
  }
  if (yValue > maxValueThreshold && joyMoved == false) {
    directionJoyMoved = left;
    if (playerPositionCol - 1 >= firstCol && matrix[playerPositionRow][playerPositionCol - 1] != HIGH) {
      matrix[playerPositionRow][playerPositionCol] = LOW;
      playerPositionCol -= 1;
    }
    joyMoved = true;
  }
  if (yValue <= minValueThreshold && joyMoved == false) {
    directionJoyMoved = right;
    if (playerPositionCol + 1 <= matrixSize - 1 && matrix[playerPositionRow][playerPositionCol + 1] != HIGH) {
      matrix[playerPositionRow][playerPositionCol] = LOW;
      playerPositionCol += 1;
    }
    joyMoved = true;
  }
  if (yValue > minNeutralThreshold && yValue < maxNeutralThreshold && xValue > minNeutralThreshold && xValue < maxNeutralThreshold)
    joyMoved = false;
}


void readActionButton() {
  if (digitalRead(actionButtonPin) != isButtonReleasedState) {
    lastDebounceTime = millis();
    wasActionPressed = true;
  } else if (millis() - lastDebounceTime > debounceDelay && wasActionPressed) {
    wasActionPressed = false;
  }

  if (wasActionPressed) {
    trackCooldown = millis();
    bombRow = playerPositionRow;
    bombCol = playerPositionCol;
    previousBombExplodeMillis = millis();
    wasActionPressed = false;
  }
}

// made a function to generate random terrain with being covered with walls(50-75%)
void setupRandomTerrain() {
  int minPercentage = 50;
  int maxPercentage = 75;

  int minHighs = matrixSize * matrixSize * minPercentage / 100;
  int maxHighs = matrixSize * matrixSize * maxPercentage / 100;

  int noOfHighs = random(minHighs, maxHighs + 1);
  for (int index = 0; index < noOfHighs; ++index) {
    int randRow = random(matrixSize);
    int randCol = random(matrixSize);
    if (randRow - playerPositionRow > 1 || randCol - playerPositionCol > 1)
      matrix[randRow][randCol] = HIGH;
  }
}


//just tried a small ending
void badEnding(){
  delay(2);
  for(int i = firstRow; i < matrixSize; i++)
    for(int j = firstCol; j < matrixSize; j++)
      matrix[i][j] = LOW;
  for(int k = 2; k <= 5; k++){
    matrix[k][firstCol] = HIGH;
    matrix[k][matrixSize - 1] = HIGH;
    matrix[firstRow][k] = HIGH;
    matrix[matrixSize - 1][k] = HIGH;
    matrix[4][k] = HIGH;
  }

  //work in progress, numbers just to color them matrix to be a face
  matrix[1][1] = HIGH;
  matrix[1][6] = HIGH;
  matrix[6][1] = HIGH;
  matrix[6][6] = HIGH;
  matrix[2][2] = HIGH;
  matrix[2][5] = HIGH;


}
