#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;

int IR1 = 2; // Entry IR sensor
int IR2 = 3; // Exit IR sensor

int Slot = 4; // Total number of parking Slots

int flag1 = 0;
int flag2 = 0;

// Define the IR sensor pins for each slot (using digital pins)    (SENSORS PROGRAM)
const int irSensor1 = 6; // Slot 1 (D6)
const int irSensor2 = 7; // Slot 2 (D7)
const int irSensor3 = 8; // Slot 3 (D8)
const int irSensor4 = 9; // Slot 4 (D9)

// Define the status of each parking slot
bool slot1Occupied = false;
bool slot2Occupied = false;
bool slot3Occupied = false;
bool slot4Occupied = false;

void setup() {
  Serial.begin(9600);
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Open the backlight

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  // Initialize the IR sensor pins as inputs
  pinMode(irSensor1, INPUT);
  pinMode(irSensor2, INPUT);
  pinMode(irSensor3, INPUT);
  pinMode(irSensor4, INPUT);

  myservo.attach(4);
  myservo.write(180); // Initial position (closed)

  lcd.setCursor(0, 0);
  lcd.print(" ARDUINO ");
  lcd.setCursor(0, 1);
  lcd.print(" PARKING SYSTEM ");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read the IR sensor values for each slot using digitalRead
  int sensor1Value = digitalRead(irSensor1);
  int sensor2Value = digitalRead(irSensor2);
  int sensor3Value = digitalRead(irSensor3);
  int sensor4Value = digitalRead(irSensor4);

  // Determine if each slot is occupied
  slot1Occupied = (sensor1Value == LOW);
  slot2Occupied = (sensor2Value == LOW);
  slot3Occupied = (sensor3Value == LOW);
  slot4Occupied = (sensor4Value == LOW);

  // Display the status of each slot             (LCD PROGRAM)
  lcd.setCursor(0, 0);
  lcd.print("S1:");
  lcd.print(slot1Occupied ? "Full”: "Empty");
  lcd.print(" S2:");
  lcd.print(slot2Occupied ? "Full”: "Empty");

  lcd.setCursor(0, 1);
  lcd.print("S3:");
  lcd.print(slot3Occupied ? "Full”: "Empty");
  lcd.print(" S4:");
  lcd.print(slot4Occupied ? "Full”: "Empty");

  // Entry and exit logic
  if (digitalRead(IR1) == LOW && flag1 == 0) {
    int freeSlots = 4 - (slot1Occupied + slot2Occupied + slot3Occupied + slot4Occupied);
    if (freeSlots > 0) {
      flag1 = 1;
      myservo.write(100); // Open the gate by going up
      Slot = Slot - 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" WELCOME! ");
      lcd.setCursor(0, 1);
      if (freeSlots == 4) {
        lcd.print("Go Straight ");
      } else {
        if (!slot4Occupied) {
          lcd.print("Go Straight ");
        } else if (!slot3Occupied) {
          lcd.print("Turn Slight Left");
        } else if (!slot2Occupied) {
          lcd.print("Turn Left ");
        } else if (!slot1Occupied) {
          lcd.print("Turn Full Left ");
        }
      }
      delay(3000);
      lcd.clear();
    } else {
      lcd.setCursor(0, 0);
      lcd.print(" SORRY :( ");
      lcd.setCursor(0, 1);
      lcd.print(" Parking Full ");
      delay(3000);
      lcd.clear();
    }
  }

  if (digitalRead(IR2) == LOW && flag2 == 0) {
    flag2 = 1;
    myservo.write(100); // Open the gate by going up
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" GOODBYE! ");
    delay(3000);
    Slot = Slot + 1;
    lcd.clear();
  }

  if ((flag1 == 1 || flag2 == 1)) {
    myservo.write(180); // Close the gate by going straight
    flag1 = 0;
    flag2 = 0;
  }

  delay(100);
}








