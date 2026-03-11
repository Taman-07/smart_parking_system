#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ===== LCD Setup =====
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 if your LCD has a different I2C address

// ===== Servo Setup =====
Servo myservo;

// ===== IR Sensor Pins =====
#define ir_enter 2
#define ir_back  4
#define ir_car1 5
#define ir_car2 6
#define ir_car3 7
#define ir_car4 8

// ===== Variables =====
int S1=0, S2=0, S3=0, S4=0;
int flag_enter=0, flag_exit=0;
int slot = 4;  // total number of slots

// ===== Setup =====
void setup() {
  Serial.begin(9600);

  // Set pin modes
  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_car4, INPUT);
  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);

  // Servo setup
  myservo.attach(3);    
  myservo.write(90);    // gate closed initially

  // LCD setup
  lcd.init();           
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("  Smart Parking ");
  lcd.setCursor(0,1);
  lcd.print("     System     ");
  delay(2000);
  lcd.clear();

  // Initialize slot count
  Read_Sensor();
  int total = S1 + S2 + S3 + S4;
  slot = slot - total;
}

// ===== Main Loop =====
void loop() {
  Read_Sensor();

  // Display slots available
  lcd.setCursor(0,0);
  lcd.print("Slots Avail: ");
  lcd.print(slot);
  lcd.print("   "); // overwrite old numbers

  // Display S1 and S2
  lcd.setCursor(0,1);
  lcd.print("S1:");
  lcd.print(S1 ? "Full " : "Empty");
  lcd.print("S2:");
  lcd.print(S2 ? "Full" : "Empty");

  delay(1000);

  // Display S3 and S4
  lcd.setCursor(0,1);
  lcd.print("S3:");
  lcd.print(S3 ? "Full " : "Empty");
  lcd.print("S4:");
  lcd.print(S4 ? "Full" : "Empty");

  delay(1000);

  // ===== Entry Gate Logic =====
  if(digitalRead(ir_enter) == 0 && flag_enter == 0) {
    if(slot > 0) {
      flag_enter = 1;
      myservo.write(0);  // open gate
      slot--;
      lcd.setCursor(0,0);
      lcd.print("Gate Opening   ");
      delay(1500);
      myservo.write(90); // close gate
    } else {
      lcd.setCursor(0,0);
      lcd.print(" Parking Full! ");
      delay(1500);
    }
  }

  // ===== Exit Gate Logic =====
  if(digitalRead(ir_back) == 0 && flag_exit == 0) {
    flag_exit = 1;
    myservo.write(0);  // open gate
    slot++;
    lcd.setCursor(0,0);
    lcd.print("Gate Opening   ");
    delay(1500);
    myservo.write(90); // close gate
  }

  // Reset flags after delay
  if(flag_enter == 1 || flag_exit == 1) {
    flag_enter = 0;
    flag_exit = 0;
  }

  delay(200);
}

// ===== Read IR Sensors =====
void Read_Sensor() {
  S1 = (digitalRead(ir_car1) == 0) ? 1 : 0;
  S2 = (digitalRead(ir_car2) == 0) ? 1 : 0;
  S3 = (digitalRead(ir_car3) == 0) ? 1 : 0;
  S4 = (digitalRead(ir_car4) == 0) ? 1 : 0;
}