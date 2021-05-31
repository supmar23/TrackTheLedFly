// Mario Ciccioni

#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>

#define LEDRED 6

#define LED0 9
#define LED1 10
#define LED2 11
#define LED3 12

#define BUTTON0 2
#define BUTTON1 3
#define BUTTON2 4
#define BUTTON3 5

#define POT A0

volatile boolean played;
unsigned char leds[4];
unsigned char buttons[4];
int randomLed;
int spostamento;
int previous;
int Tmin;
const float p = 0.875;
const int moltiplier = 3;
volatile int score;
const float k = 1.2;
int level;
int brightness;
int fadeAmount;
int currIntensity;

// variables for delate buttons bouncing
unsigned long t = 0;
#define DEBOUNCE_DELAY 300

void setup() {
  played = false;
  randomLed = random(0, 4);
  score = 0;
  
  Serial.begin(9600);

  currIntensity = 0;
  fadeAmount = 5;
  pinMode(LEDRED, OUTPUT);

  leds[0] = LED0;
  leds[1] = LED1;
  leds[2] = LED2;
  leds[3] = LED3;
  for (int i = 0; i <= 3; i++) {
    pinMode(leds[i], OUTPUT);
  }

  buttons[0] = BUTTON0;
  buttons[1] = BUTTON1;
  buttons[2] = BUTTON2;
  buttons[3] = BUTTON3;
  for (int i = 0; i <= 3; i++) {
    pinMode(buttons[i], INPUT);
  }
  
  enableInterrupt(BUTTON0, buttonsInt, FALLING);
  enableInterrupt(BUTTON1, buttonsInt, FALLING);
  enableInterrupt(BUTTON2, buttonsInt, FALLING);
  enableInterrupt(BUTTON3, buttonsInt, FALLING);

  Serial.println("Welcome to Track the Led Fly game. Choose a level and Press Key T1 To Start after choosing a level");
}

void loop() {
  // the game is started
  if (played == true) {
    previous = randomLed;
    spostamento = random(0, 2);
    if (spostamento == 0) {
      randomLed++;
      if (randomLed > 3) {
        randomLed = 0;
      }
    } else {
      randomLed--;
      if (randomLed < 0) {
        randomLed = 3;
      }
    }
    digitalWrite(leds[randomLed], HIGH);
    Tmin = random(Tmin, Tmin*k);
    delay(Tmin);
    int ledStatus = digitalRead(leds[randomLed]);
    if (ledStatus == HIGH) {
      noInterrupts();
      played = false;
      interrupts();
      Serial.print("Game Over - Score: ");
      Serial.println(score);
      noInterrupts();
      score = 0;
      interrupts();
      digitalWrite(leds[randomLed], LOW);
      digitalWrite(LEDRED, HIGH);
      delay(2000); 
    }
    Tmin = Tmin * p; // new Tmin = 7/8*Tmin old
  }

  // initial state of the game, the red led pulse and is possible chose the level
  if (played == false) {
    analogWrite(LEDRED, currIntensity);
    currIntensity = currIntensity + fadeAmount;
    if (currIntensity == 0 || currIntensity == 255) {
      fadeAmount = -fadeAmount ;
    }
    delay(20);
    int potValue = analogRead(POT);
    level = map (potValue, 0, 1023, 1, 8);
  }
}

void buttonsInt() {
  if (played == true) {
    if (arduinoInterruptedPin == 2 && randomLed == 0) {
      digitalWrite(leds[randomLed], LOW);
      if((millis() - t) > DEBOUNCE_DELAY) {
        Serial.print("Tracking the fly in pos: ");
        Serial.println(randomLed);
        score++;
        t = millis();
      }
    } else if (arduinoInterruptedPin == 3 && randomLed == 1) {
      digitalWrite(leds[randomLed], LOW);
      if((millis() - t) > DEBOUNCE_DELAY) {
        Serial.print("Tracking the fly in pos: ");
        Serial.println(randomLed);
        score++;
        t = millis();
      }
    } else if (arduinoInterruptedPin == 4 && randomLed == 2) {
      digitalWrite(leds[randomLed], LOW);
      if((millis() - t) > DEBOUNCE_DELAY) {
        Serial.print("Tracking the fly in pos: ");
        Serial.println(randomLed);
        score++;
        t = millis();
      }
    } else if (arduinoInterruptedPin == 5 && randomLed == 3) {
      digitalWrite(leds[randomLed], LOW);
      if((millis() - t) > DEBOUNCE_DELAY) {
        Serial.print("Tracking the fly in pos: ");
        Serial.println(randomLed);
        score++;
        t = millis();
      }
    } else {
      played = false;
      Serial.print("Game Over - Score: ");
      Serial.println(score);
      score = 0;
      digitalWrite(leds[randomLed], LOW);
      digitalWrite(LEDRED, HIGH);
      delay(2000);
    }
  }

  if (played == false && arduinoInterruptedPin == 2) {
    played = true;
    digitalWrite(LEDRED, LOW);
    Tmin = ((1/level) + moltiplier) * 1000;
    Serial.print("The level you have chosen is: ");
    Serial.println(level);
    Serial.println("GO!");
  }
}
