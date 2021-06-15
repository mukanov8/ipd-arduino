
#include <Arduino.h>
#include <U8g2lib.h>
#include <Servo.h>
#include <EEPROM.h>
#include <CuteBuzzerSounds.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
  This is a page buffer example.    
*/

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

const int buttonPin = 8; 
int buttonPinState = 0;

const int sensorPin = 6; 
int sensorPinState = 0;

const int servoPin = 3;
Servo dispenser;
const int dispenserInitPos = 0;
int dispenserPos = 270;
int dispenserState = 0;  // 0 = in, 1 = out
bool shouldDispense = false;

const int buzzerPin = 4;
int once = 1;
// S_OHOOH for when removing phone/alert

const int addr = 0;
long int habitTime = 1890;
long int dailyTime = 90;
long int lifeTime = 8100;

// enum State_enum {INIT, WAIT, INFO, START};
int state = 0;


void setup(void) {
  u8g2.begin();
  
  dispenser.attach(servoPin);
  dispenser.write(dispenserInitPos);  

  pinMode(buttonPin, INPUT);
  pinMode(sensorPin, INPUT);

  // EEPROM.write(addr, habitTime);
  cute.init(buzzerPin);
  once=1;

  Serial.begin(9600);
}

void loop(void) {
  // Serial.println("b");
  buttonState = digitalRead(buttonPin);
  sensorPinState = digitalRead(sensorPin);
  if(once) {
    cute.play(S_CONNECTION);
    once=0;
  }
  // Serial.println(startButtonState);
  // Serial.println(buttonState);
  delay(100);
  if(startButtonState) {
    // Serial.println('aaaa');
    cute.play(S_CONNECTION);
    shouldDispense=true;
    // shouldDispense=true;
  }
  if(buttonState) {
    // Serial.println('aaaa');
    cute.play(S_CONNECTION);
    // shouldDispense=true;
  }
  // delay(100);
  // for (dispenserPos; dispenserPos > 0; dispenserPos-=1){
  //       dispenser.write(dispenserPos);
  //       delay(20);
  // }
  // Serial.print("init");
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.firstPage();
    do {
      u8g2.setCursor(4, 20);
      u8g2.print(F("Hello, press"));
      u8g2.setCursor(4, 40);
      u8g2.print(F("the button"));
      u8g2.setCursor(4, 60);
      u8g2.print(F("to start"));
    } while (u8g2.nextPage());

  // startButtonState = digitalRead(startButtonPin);
  // if(once){
  //   cute.play(S_CONNECTION);
  //   once=0;
  // }
  // if(startButtonState) {
  //   cute.play(S_CONNECTION);
  //   // shouldDispense=true;
  // }
  // init_routine();
    
  if (shouldDispense){
    // already dispensed
    if(dispenserState){
      for (dispenserPos; dispenserPos > 0; dispenserPos-=1){
        dispenser.write(dispenserPos);
        delay(20);
      }
      dispenserState=0;
      Serial.println("retrieved");
      
    }
    // will be dispensed
    else{
      for (dispenserPos; dispenserPos <360; dispenserPos+=1){
        dispenser.write(dispenserPos);
        delay(20);
      }
      dispenserState=1;
      Serial.println("dispensed");
    }
    shouldDispense=false;
  }
  // for (dispenserPos = 140; dispenserPos >=80; dispenserPos-=1){
  //     dispenser.write(dispenserPos);
  //     delay(10);
  //   }
  // dispenser.write(dispenserInitPos);

  // state_machine_run();
  // delay(10);  
  
  // phone_proximity_routine();
  
  
  // if (startButtonState) {
  //   // Serial.print('geet');
  //   Serial.println('check');
  //   // tone(piezoPin, 200, 500);
  //   cute.play(S_CONNECTION);
    // delay(2000);
    // cute.play(S_DISCONNECTION);
    // delay(2000);
    // cute.play(S_BUTTON_PUSHED);
    // delay(2000);
    // cute.play(S_HAPPY);
    // delay(2000);
    // cute.play(S_SUPER_HAPPY);
    // delay(2000);
    // cute.play(S_MODE1);
    // delay(2000);
    // cute.play(S_MODE2);
    // delay(2000);
    // cute.play(S_MODE3);
    // turn LED on:

  // }
}

void init_routine() {
  if(startButtonState) {
    cute.play(S_BUTTON_PUSHED);
    state = 1;
    delay(500);
  }
  else {
    Serial.print("init");
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.firstPage();
    do {
      u8g2.setCursor(4, 20);
      u8g2.print(F("Hello, press"));
      u8g2.setCursor(4, 40);
      u8g2.print(F("the button"));
      u8g2.setCursor(4, 60);
      u8g2.print(F("to start"));
    } while (u8g2.nextPage());
    delay(1000);
  }
}

void info_total_routine() {
  if(startButtonState) {
    cute.play(S_BUTTON_PUSHED);
    state = 2;
    delay(500);
  }
  else{
    Serial.print("info");
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.firstPage();
    do {
      u8g2.setCursor(4, 20);
      u8g2.print(F("You have"));
      u8g2.setCursor(4, 40);
      u8g2.print(habitTime);    
      u8g2.setCursor(9, 40);
      u8g2.print(F("total"));
      u8g2.setCursor(4, 60);
      u8g2.print(F("hours left"));
    } while (u8g2.nextPage());
    delay(2000);
    u8g2.firstPage();
    do {
      u8g2.nextPage();
      u8g2.setCursor(4, 20);
      u8g2.print(F("You have"));
      u8g2.setCursor(4, 40);
      u8g2.print(dailyTime);   
      // u8g2.print(dailyTime);
      // u8g2.print(" hours");	 
      u8g2.setCursor(9, 40);
      u8g2.print(F("hours"));
      u8g2.setCursor(4, 60);
      u8g2.print(F("left today"));
    } while (u8g2.nextPage());
    delay(2000);
  }
}

// void info_daily_routine() {
//   u8g2.setFont(u8g2_font_ncenB14_tr);
//   u8g2.firstPage();
//   do {
//     u8g2.setCursor(4, 20);
//     u8g2.print(F("You have"));
//     u8g2.setCursor(4, 40);
//     u8g2.print(("%d hours", dailyTime));
//     u8g2.setCursor(4, 60);
//     u8g2.print(F("left today"));
//   } while (u8g2.nextPage());
//   delay(2000);
// }

void wait_routine() {
  u8g2.firstPage();
  do {
    u8g2.setCursor(4, 20);
    u8g2.print(F("Put your"));
    u8g2.setCursor(4, 40);
    u8g2.print(F("phone on"));
    u8g2.setCursor(4, 60);
    u8g2.print(F("the stand!"));
  } while (!is_phone_on());
  delay(1000);
  
}

bool is_phone_on() {
  sensorValue = analogRead(sensorPin); // read the value from the sensor
  delay(500);
  Serial.println(sensorValue); //prints the values coming from the sensor on the screen
  return sensorValue<20;
}

void servo_routine() {
    for (dispenserPos = 140; dispenserPos >=80; dispenserPos-=1){
      dispenser.write(dispenserPos);
      delay(10);
    }
    dispenser.write(dispenserInitPos);
    dispenser.detach();
}

void main_routine() {
  EEPROM.read(addr);
}

void state_machine_run() {
  buttonState = digitalRead(buttonPin);
  startButtonState = digitalRead(startButtonPin);
  if(startButtonState) {
    cute.play(S_BUTTON_PUSHED);
    // state = 2;
    delay(100);
    servo_routine();
  }
  dispenser.detach();
  // Serial.println(state);
  // Serial.println("check");
  
  // switch(state){
  //   case 0:
  //       init_routine();
  //       break;
  //   case 1:
  //       info_total_routine();
  //       break;
  //   case 2:
  //     Serial.print("wait");
  //     // wait_routine();
  //     // if(is_phone_on){
  //     //   cute.play(S_BUTTON_PUSHED);
  //     //   state = 3;
  //     //   // break;
  //     // }
  //     break;
  // }
}
