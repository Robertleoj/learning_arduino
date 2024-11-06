#include <LiquidCrystal_I2C.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)

const int DELAY_TIME = 700;

// Initialize display
LiquidCrystal_I2C lcd(
    0x27, // I2C address
    16,
    2
);

void setup() {
  // Start I2C communication
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  // lcd.setCursor(0, 0);
  // lcd.print("Hello!");
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
