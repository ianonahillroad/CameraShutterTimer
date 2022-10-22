
// Arduino Shutter Timer Hardware Test - basic hardware exercising.
// This new version Oct 2022 by Ian Wallace (IG: @ian_onahillroad) 
// This program provides a basic hardware test to validate newly constructed or faulty hardware
// checks to verify correct component operation.  Operator must check monitor and LCD and validate
// that the test has not logged errors and reports conditions as observed on the hardware 
// the test exercises each component in turn.  Make sure components work as expected in first test before bothering
// with results from next.
// Program assumes all hardware is configured as per the original shutter speed tester design.
// If you have altered the pin usage or components constants must be updated.
//
// This source available from my github https://github.com/ianonahillroad/CameraShutterTimer

// START TEST WITH LASER OFF

const byte Version = 1;               // This application version

// Includes of Libraies
#include <EEPROM.h>
// SD Card - Micro SD Storage Board TF Card Memory Shield Module SPI For Arduino
#include <SD.h>
// LCD - With I2C 2004 20X4 Character LCD Module Display
#include <LiquidCrystal_I2C.h>
const int LCDWIDTH = 20;

LiquidCrystal_I2C lcd(0x27, LCDWIDTH, 4); // I2C address 0x27, 20 column and 4 rows

// Configuration Parameters
// There are no "incode" constants all configuration is available here and the code is more legible

//PINS
const byte LASERSENSORPIN = 2;          // Number of the digital pin to read the sensor interrups on
const byte LASERPOWERONPIN = 3;         // Number of the digital pin to read laser power ON interrups on
const byte SDCARD_MOSI_PIN = 11;        // SD Card digital pin MOSI
const byte SDCARD_MISO_PIN = 12;        // SD Card digital pin MISO
const byte SDCARD_CLK_PIN = 13;         // SD Card digital pin CLK
const byte SDCARD_CS_PIN = 4;           // SD Card digital pin CS

// CONFIG

const long ARDUINOSERIALSPEED = 115200;   // this is the speed in the Arduino Monitor window. It can be 9600 but if you autowind the camera the slow output window may not keep up. hence the suggested 115200 baud

// SHARED DATA Declarations
//=========================

// General Shared Variables persisting until RESET
// ===============================================
//

// Volatile declarations as accessed shared by interrupt routine
//==============================================================

// SETUP Routine
// =============

void setup() {      //This part of the program is run exactly once on boot

  Serial.begin(ARDUINOSERIALSPEED);                                                 //opens a serial connection.
  Serial.println(F("Shutter Timer HARDWARE TEST Programme Setting up.."));                          // Let the user know we are off.

  //CHECK FOR PRESENCE OF DATA CARD
  Serial.println(F("Test 1) Check SD CARD found on pins "));
  Serial.print(F("SD Card digital pin MOSI expected on pin "));
  Serial.println(SDCARD_MOSI_PIN);
  Serial.print(F("SD Card digital pin MISO expected on pin "));
  Serial.println(SDCARD_MISO_PIN);
  Serial.print(F("SD Card digital pin CLK expected on pin "));
  Serial.println(SDCARD_CLK_PIN);
  Serial.print(F("SD Card digital CS MOSI expected on pin "));
  Serial.println(SDCARD_CS_PIN);

  if (!SD.begin(SDCARD_CS_PIN)) {
    Serial.println(F("SD Card Initialised successfully"));
  } else {
    Serial.println(F("Attempt SD Card Write"));
    File fLogfile;

    fLogfile = SD.open("HWTest.txt", FILE_WRITE);
    if (!fLogfile)
    {
      Serial.println(F("HWTest.txt Open Failed!"));  // warn if we thought we failed to open it.
    } else {
      Serial.println(F("HWTest.txt Seems to have opened OK"));
      fLogfile.print(F("test data to file"));
      Serial.println(F("HWTest.txt Seems to have Written OK"));
      fLogfile.close();
      Serial.println(F("HWTest.txt Seems to have Closed OK"));
      Serial.println(F("Please remove the SD card and check for text file HWTest.txt at end of test in alternitive card reader hardware."));
    }
  }
  delay(2000);
  Serial.println();
  Serial.println();
  Serial.println(F("Test 2) Check LCD Display"));

  // Start LCD Display
  Serial.println(F("Start LCD Display"));
  //initialize the lcd
  lcd.begin();
  Serial.println(F("Started LCD Library request backlight"));
  lcd.backlight(); //open the backlight MAY have no effect depends on board
  lcd.clear();
  Serial.println(F("Cleared LCD"));
  delay(5000);
  lcd.print(F("LCD TEST"));
  delay(5000);
  lcd.clear();
  Serial.println(F("LCD Test pattern writes to all character positions should look like this"));
  Serial.println(F("*1234567890123456789"));
  Serial.println(F("01234567890123456789"));
  Serial.println(F("01234567890123456789"));
  Serial.println(F("0123456789012345678*"));

  lcd.print(F("*1234567890123456789"));
  lcd.print(F("01234567890123456789"));
  lcd.print(F("01234567890123456789"));
  lcd.print(F("0123456789012345678*"));
  delay(5000);
  lcd.clear();

  Serial.println(F("Test 3) Check Laser ON OFF and saftey switch"));

  Serial.print(F("switch expected on pin "));
  Serial.println(LASERPOWERONPIN);
  pinMode(LASERPOWERONPIN, INPUT);

  Serial.print(F("switch and Laser expected to set OFF "));
  Serial.print(F("you have 10 sec to check"));
  Serial.print(F("Change switch when prompted check switch and laser agree with detected message"));
  Serial.print(F("repeats 5 times"));
  lcd.clear();
  lcd.print (F("switch and Laser"));
  lcd.setCursor(0, 1);
  lcd.print (F("set OFF to start"));
  lcd.setCursor(0, 2);
  lcd.print (F("you have 10 sec"));
  delay(10000);

  for (int i = 0; i <= 5; i++) {

    if (digitalRead(LASERPOWERONPIN) == HIGH) {

      // Laser ON

      lcd.clear();
      Serial.println (i);
      lcd.print(i);
      Serial.println (F("Switch Detected as ON"));
      lcd.setCursor(0, 1);
      lcd.print(F("Switch is ON"));
      delay(5000);

    } else  {

      // Laser Off


      lcd.clear();
      Serial.println (i);
      lcd.print(i);
      Serial.println (F("Switch Detected as OFF"));
      lcd.setCursor(0, 1);
      lcd.print(F("Switch is OFF"));
      delay(5000);


    }
    Serial.println (F("pausing 10 sec for you to toggle switch"));
    lcd.setCursor(0, 2);
    lcd.print(F("pausing 10 sec"));
    Serial.println (F("Toggle Switch"));
    lcd.setCursor(0, 3);
    lcd.print(F("Toggle Switch"));;
    delay(10000);

  }
  lcd.clear();



  Serial.println(F("Test 4) Check Laser Light sensing"));
  lcd.print(F("Test 4 Laser sensor"));
  Serial.print(F("Change switch when prompted check switch and laser agree with detected message"));
  Serial.print(F("ALTERNATIVE Block unblock laser light confirm detection (rather than switch)"));
  Serial.print(F("repeats 5 times"));

  Serial.print(F("Senesor signal expected on pin "));
  Serial.println(LASERSENSORPIN);
  pinMode(LASERSENSORPIN, INPUT);

  Serial.print(F("switch and Laser expected to set OFF at start of test"));
  Serial.print(F("you have 10 sec to set correctly"));
  lcd.clear();
  lcd.print (F("Sensor test "));
  lcd.setCursor(0, 1);
  lcd.print (F("Start laser OFF"));
  lcd.setCursor(0, 2);
  lcd.print (F("10 sec to check"));
  delay(10000);

  for (int i = 0; i <= 5; i++) {

    if (digitalRead(LASERSENSORPIN) == HIGH) {

      // Laser ON

      lcd.clear();
      Serial.println (i);
      lcd.print(i);
      Serial.println (F("Detected Laser ON"));
      lcd.setCursor(0, 1);
      lcd.print(F("Detected Laser ON"));
      delay(5000);


    } else  {

      // Laser Off

      lcd.clear();
      Serial.println (i);
      lcd.print(i);
      Serial.println (F("Detected Laser OFF"));
      lcd.setCursor(0, 1);
      lcd.print(F("Detected Laser OFF"));
      delay(5000);

    }
    Serial.println (F("pausing 10 sec for switch"));
    lcd.setCursor(0, 2);
    lcd.print(F("pausing 10 sec"));
    Serial.println (F("Toggle Laser Light"));
    lcd.setCursor(0, 3);
    lcd.print(F("Toggle Laser Light"));

    delay(10000);
    
  }
  
  lcd.clear();
  Serial.println (F("TEST EXERCISES ENDED"));
  lcd.print(F("TEST ENDED"));

}

// MAIN Code Loop
// ==============


void loop() {};
