#include <Servo.h>
#include <Keypad.h>

// -------------------- Servo --------------------
Servo door;

// -------------------- Ultrasonic --------------------
#define TRIG_PIN 7
#define ECHO_PIN 6

// -------------------- LEDs --------------------
#define GREEN_LED A0
#define RED_LED A1
#define BLUE_LED A2
#define YELLOW_LED A3

// -------------------- Buzzer --------------------
#define BUZZER A4

// -------------------- Keypad --------------------
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2,3,4,5};
byte colPins[COLS] = {8,10,11,12};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// -------------------- Password --------------------
String password = "9999";
String input = "";

int wrongAttempts = 0;

void setup()
{
  Serial.begin(9600);

  door.attach(1  + );
  door.write(0);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(BUZZER, LOW);

  Serial.println("==== SMART DOOR LOCK ====");
}

void loop()
{
  // -------- Measure Distance --------
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.0343 / 2;

  if(distance <= 15)
  {
    digitalWrite(BLUE_LED, HIGH);

    Serial.println("Person Detected");
    Serial.println("Enter Password:");

    input = "";

    while(input.length() < 4)
    {
      char key = keypad.getKey();

      if(key)
      {
        Serial.print("*");
        input += key;
      }
    }

    Serial.println();

    if(input == password)
    {
      Serial.println("Access Granted");

      digitalWrite(GREEN_LED, HIGH);

      door.write(180);

      delay(5000);

      door.write(0);

      digitalWrite(GREEN_LED, LOW);

      wrongAttempts = 0;

      Serial.println("Door Locked");
    }
    else
    {
      Serial.println("Wrong Password");

      digitalWrite(RED_LED, HIGH);

      delay(2000);

      digitalWrite(RED_LED, LOW);

      wrongAttempts++;

      if(wrongAttempts >= 3)
      {
        Serial.println("SYSTEM LOCKED");

        digitalWrite(YELLOW_LED, HIGH);

        for(int i=0;i<10;i++)
        {
          digitalWrite(BUZZER,HIGH);
          delay(250);
          digitalWrite(BUZZER,LOW);
          delay(250);
        }

        delay(30000);

        digitalWrite(YELLOW_LED, LOW);

        wrongAttempts = 0;

        Serial.println("System Reset");
      }
    }

    delay(1000);
  }
  else
  {
    digitalWrite(BLUE_LED, LOW);
  }
}
