int INPUT_WATER_LEVEL = 3;
int INPUT_RESET = 7;
int INPUT_STOP_START = 6;

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
int TIME_TURBINE_LIMIT = 2000;

int OUTPUT_TURBINE = 8;

int currentLevel = -1;
bool inErrorState = false;
unsigned long startTurbineTime = 0;

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);  // 1.1 volt

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
  startStopTurbine();
  checkTimeTurbine();
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

void startTurbine() {
  startTurbineTime = millis();
  digitalWrite(OUTPUT_TURBINE, HIGH);
}

void stopTurbine() {
  startTurbineTime = 0;
  digitalWrite(OUTPUT_TURBINE, LOW);
}

void checkTimeTurbine() {
  if (startTurbineTime == 0) {
    return;
  }
  unsigned long time = millis() - startTurbineTime;
  Serial.println(time);
  if (time > TIME_TURBINE_LIMIT) {
    stopTurbine();
  }
}

void startStopTurbine() {
  if (digitalRead(INPUT_STOP_START) == LOW) {
    return;
  }
  delay(500);
  if (startTurbineTime != 0) {
    stopTurbine();
  } else {
    startTurbine();
  }
}

void controlTurbine(int level) {
  if (level <= WATER_UNDER) {
    startTurbine();
  }
  if (level >= WATER_HIGH_LEVEL) {
    stopTurbine();
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
    stopTurbine();
    ledFlashing(OUTPU_ERROR, 500);
  }
  Serial.println(inErrorState ? "Error" : "No Error");
  return inErrorState;
}
