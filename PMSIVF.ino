#include "global.h"
#include "lcd.h"
#include"heart.h"
#include"dht11.h"
DHT dht;



// Create the Player object
  void iot(String s);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);

  U1.begin(6, 7);

  pinMode(buzzer, OUTPUT);
  pinMode(emergency, INPUT_PULLUP);
  lcd.setCursor(0, 0);
  lcd.print("DRIP MONITORING");
  lcd.setCursor(5, 1);
  lcd.print("SETUP");
  delay(1000);
  lcd.clear();
  digitalWrite(buzzer, LOW);
}

void loop() {
  lcd.clear();

  dht.dht_read(2);
  TEMP = dht.temperature;
  emergency_state = digitalRead(emergency);
  heart();
  cm1 = U1.ultra();
  percent = (cm1 - 5) * 100 / 13;
  lcd.setCursor(1, 0);
  lcd.print("TEMP:" + String(TEMP));
  lcd.setCursor(10, 0);
  lcd.print("CM:" + String(cm1));
  lcd.setCursor(0, 1);
  lcd.print("PERCT:" + String(percent));
  if (TEMP > 39) {
    lcd.setCursor(10, 0);
    lcd.print("TEMPERATURE HIGH");
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
  }
  if (emergency_state == 0) {
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0, 1);
    lcd.print("EMERGENCY");
  }
  lcd.setCursor(10, 1);
  lcd.print("H:" + String(G_i));
  if (G_i != 0 && G_i > 100) {
    lcd.setCursor(0, 1);
    lcd.print("HEARTY RATE HIGH");
  }
  if (count >5  && count <8  && cm1 > cm2) {
    cm2 = cm1;
    count = 0;
  }
  else if (count < 5 && cm1 > cm2 && count > 2) {
    cm2 = cm1;
    //digitalWrite(buzzer, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("DRIP OVERFLOW");
    delay(1000);
    //digitalWrite(buzzer, LOW);
    Serial.println("overflow");
    count = 0;
  }
  else if (count > 10) {
    lcd.setCursor(0, 1);
    lcd.print("DRIP UNDERFLOW");
    delay(1000);
//    digitalWrite(buzzer, HIGH);
//    digitalWrite(buzzer, LOW);
    count = 0;
  }
  if (count == 1 || cm2 > cm1) {
    cm2 = cm1;
  }
  iot("*" + String(TEMP) + "#@" + String(G_i) + "#$" + String(percent) + "#");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(count);
  delay(500);
  count++;


if (cm1 >= 15) {
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(" DRIP ALERT!!");
  delay(5000);
}
}
  void iot(String s)
  {
    for (int i = 0; i < s.length(); i++)
    {
      Serial.write(s[i]);
    }
    delay(2000);
  }
