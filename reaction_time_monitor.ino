const int INPUT_BTN_PIN = 2;
const int RST_BTN_PIN = 3;
const int LED_PIN = 13;

const int REACTION_ARR_SIZE = 5;

const unsigned long MIN_LED_DELAY = 5000;
const unsigned long MAX_LED_DELAY = 10000;
const unsigned long LED_ON_DURATION = 1000;

unsigned long ledOnDelay = 0;

unsigned long reactionArr[REACTION_ARR_SIZE];
int reactionArrIndex = -1;

bool reactedThisCycle = false;

void setup() {
  Serial.begin(9600);

  Serial.println("Welcome to the reaction time monitor!");
  Serial.println("Use button 1 to start or reset.");
  Serial.println("Use button 2 to react to the light turning on.");

  pinMode(LED_PIN, OUTPUT);
  pinMode(INPUT_BTN_PIN, INPUT_PULLUP);
  pinMode(RST_BTN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INPUT_BTN_PIN), recordReaction, RISING);
  attachInterrupt(digitalPinToInterrupt(RST_BTN_PIN), reset, RISING);

  // use noise on pin 0 as seed
  randomSeed(analogRead(0));
}

void loop() {
  reactedThisCycle = false;

  while (reactionArrIndex < 0) {
    delay(50);
  }
  
  ledOnDelay = random(MIN_LED_DELAY, MAX_LED_DELAY);
  delay(ledOnDelay);

  if (reactionArrIndex == 0) {  
    // reset reaction time array
    for (int i = 0; i < REACTION_ARR_SIZE; i++) {
      reactionArr[i] = MAX_LED_DELAY - MIN_LED_DELAY;
    }
  }
  
  digitalWrite(LED_PIN, HIGH);

  ledOnDelay = millis();

  delay(LED_ON_DURATION);

  digitalWrite(LED_PIN, LOW);

  if (reactionArrIndex == REACTION_ARR_SIZE - 1) {
    bool discardResult = false;
    unsigned long sumReactionTime = 0;
    unsigned long avgReactionTime = 0;
    
    for (int i = 0; i < REACTION_ARR_SIZE; i++) {
      if (reactionArr[i] == MAX_LED_DELAY - MIN_LED_DELAY) {
        Serial.println("Your average reaction time cannot be mesaured because you missed a reaction.");
        discardResult = true;
        break;
      } else {
        sumReactionTime += reactionArr[i];
      }
    }

    avgReactionTime = sumReactionTime / REACTION_ARR_SIZE;

    if (!discardResult) {
      Serial.print("Your average reaction time was ");
      Serial.print(avgReactionTime);
      Serial.println(" ms");
    }
  }
  
  reactionArrIndex = (reactionArrIndex + 1) % REACTION_ARR_SIZE;
}

void recordReaction() {
  if (!reactedThisCycle) {
    reactionArr[reactionArrIndex] = millis() - ledOnDelay;
    Serial.print(reactionArrIndex + 1);
    Serial.print(": Your reaction time was ");
    Serial.print(reactionArr[reactionArrIndex]);
    Serial.println(" ms");
  }

  reactedThisCycle = true;
}

void reset() {
  if (reactionArrIndex < 0) {
    Serial.println("Starting...");
  } else {
    Serial.println("Resetting...");
  }
  reactionArrIndex = 0;
}
