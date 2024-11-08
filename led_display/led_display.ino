#include <LiquidCrystal_I2C.h>

const int DELAY_TIME = 700;

LiquidCrystal_I2C lcd(
    0x27, // I2C address
    16,
    2
);

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
}

void loop() {
  lcd.clear();
  delay(DELAY_TIME);

  lcd.setCursor(0, 0);
  lcd.print("    Love you    ");
  lcd.setCursor(0, 1);
  lcd.print("   baby girl    ");

  delay(DELAY_TIME);
}
