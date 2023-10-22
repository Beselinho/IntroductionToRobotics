const int minInputValue = 0;
const int maxInputValue = 1023;

const int minLedValue = 0;
const int maxLedValue = 255;

const int blueInputPin = A0;
const int greenInputPin = A1;
const int redInputPin = A2;

const int ledBluePin = 9;
const int ledGreenPin = 10;
const int ledRedPin = 11;

 
int blueInputValue = 0;
int greenInputValue = 0;
int redInputValue = 0;

int ledBlueValue = 0;
int ledGreenValue = 0;
int ledRedValue = 0;
 
void setup() {
  pinMode(blueInputPin, INPUT);
  pinMode(greenInputPin, INPUT);
  pinMode(redInputPin, INPUT);

  
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);

  
Serial.begin(9600);
}
 
void loop() {
  blueInputValue = analogRead(blueInputPin);
  greenInputValue = analogRead(greenInputPin);
  redInputValue = analogRead(redInputPin);
  
  ledBlueValue = map(blueInputValue, minInputValue, maxInputValue, minLedValue, maxLedValue);
  ledGreenValue = map(greenInputValue, minInputValue, maxInputValue, minLedValue, maxLedValue);
  ledRedValue = map(redInputValue, minInputValue, maxInputValue, minLedValue, maxLedValue);

 
  analogWrite(ledBluePin, ledBlueValue);
  analogWrite(ledGreenPin, ledGreenValue);
  analogWrite(ledRedPin, ledRedValue);

}
