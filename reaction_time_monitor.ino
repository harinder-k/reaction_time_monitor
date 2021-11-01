#include <LiquidCrystal.h>

const int INPUT_BTN_PIN = 2;
const int RST_BTN_PIN = 3;
const int LED_PIN = 13;

const int REACTION_ARR_SIZE = 3;

const unsigned long MIN_LED_DELAY = 5000;
const unsigned long MAX_LED_DELAY = 10000;
const unsigned long LED_ON_DURATION = 3000;

unsigned long ledOnDelay = 0;

unsigned long reactionArr[REACTION_ARR_SIZE];
int reactionArrIndex = -1;

bool reactedThisCycle = false;
bool canReact = false;

// define LCD pins
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);

void setup() {
  lcd.begin(16, 2);
  lcd.print("1: Start/reset");
  lcd.setCursor(0, 1);
  lcd.print("2: React");
  
  Serial.begin(9600);
  Serial.println("Welcome!");
  Serial.println("Button 1 to start/reset");
  Serial.println("Button 2 to record reaction");

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
  canReact = false;

  if (reactionArrIndex < 0) {
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
  canReact = true;

  ledOnDelay = millis();
  delay(LED_ON_DURATION);

  digitalWrite(LED_PIN, LOW);

  if (reactionArrIndex == REACTION_ARR_SIZE - 1) {
    bool discardResult = false;
    unsigned long sumReactionTime = 0;
    unsigned long avgReactionTime = 0;
    
    for (int i = 0; i < REACTION_ARR_SIZE; i++) {
      if (reactionArr[i] == MAX_LED_DELAY - MIN_LED_DELAY) {
        Serial.println("Missing data point(s)");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Did not react");
        lcd.setCursor(0, 1);
        lcd.print("1 to reset");
        discardResult = true;
        break;
      } else {
        sumReactionTime += reactionArr[i];
      }
    }

    avgReactionTime = sumReactionTime / REACTION_ARR_SIZE;

    if (!discardResult) {
      Serial.print("Average reaction time: ");
      Serial.print(avgReactionTime);
      Serial.println(" ms");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Average reaction");
      lcd.setCursor(0, 1);
      lcd.print("time: ");
      lcd.print(avgReactionTime);
      lcd.print(" ms");
    }
  }

  reactionArrIndex = (reactionArrIndex < REACTION_ARR_SIZE - 1) ? reactionArrIndex + 1 : -1;
}

void recordReaction() {
  if (!reactedThisCycle && canReact) {
    reactedThisCycle = true;
    reactionArr[reactionArrIndex] = millis() - ledOnDelay;
    Serial.print(reactionArrIndex + 1);
    Serial.print(": Reaction time: ");
    Serial.print(reactionArr[reactionArrIndex]);
    Serial.println(" ms");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reaction time: ");
    lcd.setCursor(0, 1);
    lcd.print(reactionArr[reactionArrIndex]);
    lcd.print(" ms");
  } else if (!canReact) {
    Serial.println("False start!");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("False start");
    lcd.setCursor(0, 1);
    lcd.print("1 to reset");
  }
}

void reset() {
  if (reactionArrIndex != 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (reactionArrIndex < 0) {
      Serial.println("Starting...");
      lcd.print("Starting...");
    } else {
      Serial.println("Resetting...");
      lcd.print("Resetting...");
    }
    digitalWrite(LED_PIN, LOW);
    delay(1000);
    reactionArrIndex = 0;
  }
}
