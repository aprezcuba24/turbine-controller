int INPUT_WATER_LEVEL = 3;
int INPUT_RESET = 7;

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

int OUTPUT_TURBINE = 8;

int currentLevel = -1;
bool inErrorState = false;

void setup() {
  Serial.begin(9600);

  //Indicators leds
  pinMode(OUTPUT_WATER_HIGH, OUTPUT);
  pinMode(OUTPUT_WATER_MEDIUN, OUTPUT);
  pinMode(OUTPUT_WATER_UNDER, OUTPUT);

  pinMode(INPUT_SECURITY, INPUT);
  pinMode(OUTPU_ERROR, OUTPUT);
  pinMode(OUTPUT_TURBINE, OUTPUT);
  pinMode(INPUT_RESET, INPUT);
}

void loop() {
  reset();
  if (hasError()) {
    return;
  }
  int level = analogRead(INPUT_WATER_LEVEL);
  if (currentLevel != level) {
    currentLevel = level;
    controlTurbine(currentLevel);
    Serial.println(currentLevel);
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

void controlTurbine(int level) {
  if (level <= WATER_UNDER) {
    digitalWrite(OUTPUT_TURBINE, HIGH);
  }
  if (level >= WATER_HIGH_LEVEL) {
    digitalWrite(OUTPUT_TURBINE, LOW);
  }
}

void turnOffLeds() {
  digitalWrite(OUTPUT_WATER_HIGH, LOW);
  digitalWrite(OUTPUT_WATER_MEDIUN, LOW);
  digitalWrite(OUTPUT_WATER_UNDER, LOW);
}

void reset() {
  if (digitalRead(INPUT_RESET) == HIGH) {
    currentLevel = -1;
    inErrorState = false;
    Serial.println("Reset");
  }
}

void ledFlashing(int pin, int interval) {
  digitalWrite(pin, HIGH);
  delay(interval);
  digitalWrite(pin, LOW);
  delay(interval);
}

bool hasError() {
  if (digitalRead(INPUT_SECURITY) == HIGH) {
    inErrorState = true;
  }
  if (inErrorState) {
    turnOffLeds();    
    ledFlashing(OUTPU_ERROR, 500);
  }
  Serial.println(inErrorState ? "Error" : "No Error");
  return inErrorState;
}
