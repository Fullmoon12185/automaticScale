//
//#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
//
//LiquidCrystal_I2C lcd(0x20, 16, 2);
//uint32_t lastTimePrintLCD;
//
//void LCD_init() {
//  lcd.init();
//  lcd.backlight();
//  lcd.setCursor(0, 0);
//  lcd.print("Hospital Box");
//  lcd.setCursor(0, 1);
//  lcd.print("NPNLab deving");
//}
//
//void LCD_print_init() {
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("Weight: ");
//  lcd.setCursor(13, 0);
//  lcd.print("kg");
//  lcd.setCursor(0, 1);
//  lcd.print("Hb: ");
//  lcd.setCursor(8, 1);
//  lcd.print("SpO2 :");
//}
//
//void lcd_print_weight(float weight) {
//  lcd.setCursor(8, 0);
//  lcd.print(thisNode.weight);
//}
//
//void lcd_print_hpox(uint8_t hb) {
//  lcd.setCursor(4, 1);
//  lcd.print(thisNode.heartBeat);
//  lcd.setCursor(12, 1);
//  lcd.print(thisNode.SpO2);
//}
//
//void LCD_print_all(float printValue) {
//  if (millis() - lastTimePrintLCD > 5000) {
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Weight: ");
//    lcd.setCursor(8, 0);
//    lcd.print(thisNode.weight);
//    lcd.setCursor(14,0);
//    lcd.print("kg");
//
//    lcd.setCursor(0, 1);
//    lcd.print("Hb: ");
//    lcd.setCursor(4, 1);
//    lcd.print(thisNode.heartBeat);
//
//    lcd.setCursor(8, 1);
//    lcd.print("SpO2 :");
//    lcd.setCursor(14, 1);
//    lcd.print(thisNode.SpO2);
//
//    lastTimePrintLCD =  millis();
//  }
//}
