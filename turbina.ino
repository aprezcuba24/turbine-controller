int INPUT_WATER_LEVEL = 3;

///Security
int INPUT_SECURITY = 10;
int OUTPU_ERROR = 9;

//Leds indicators
int OUTPUT_WATER_HIGH = 13;
int OUTPUT_WATER_MEDIUN = 12;
int OUTPUT_WATER_UNDER = 11;

int WATER_HIGH_LEVEL = 1023;
int WATER_MEDIUM = 818;
int WATER_UNDER = 306;

int currentLevel = -1;

void setup() {
  Serial.begin(9600);

  //Indicators leds
  pinMode(OUTPUT_WATER_HIGH, OUTPUT);
  pinMode(OUTPUT_WATER_MEDIUN, OUTPUT);
  pinMode(OUTPUT_WATER_UNDER, OUTPUT);

  pinMode(INPUT_SECURITY, INPUT);
}

void loop() {
  if (hasError()) {
    return;
  }
   int level = analogRead(INPUT_WATER_LEVEL);
  if (currentLevel != level) {
    currentLevel = level;
    //Serial.println(currentLevel);
    updateIndicatorsLeds(currentLevel);    
  }
}

void updateIndicatorsLeds(int currentLevel) {
  turnOffLeds();
  if (currentLevel <= WATER_UNDER) {
    digitalWrite(OUTPUT_WATER_UNDER, HIGH);
  }
  if (currentLevel > WATER_UNDER && currentLevel <= WATER_MEDIUM) {
    digitalWrite(OUTPUT_WATER_MEDIUN, HIGH);
  }
  if (currentLevel > WATER_MEDIUM) {
    digitalWrite(OUTPUT_WATER_HIGH, HIGH);
  }
}

void turnOffLeds() {
  digitalWrite(OUTPUT_WATER_HIGH, LOW);
  digitalWrite(OUTPUT_WATER_MEDIUN, LOW);
  digitalWrite(OUTPUT_WATER_UNDER, LOW);
}

bool hasError() {
  if (digitalRead(INPUT_SECURITY) == HIGH) {
    turnOffLeds();
    currentLevel = -1;
    Serial.println("Error");
    return true;
  }
  Serial.println("No Error");
  return false;
}
