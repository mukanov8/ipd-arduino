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

// #include <Wire.h>
// #include <TimeLib.h>
// #include <DS1307RTC.h>

#include <DTime.h>

/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
  This is a page buffer example.    
*/

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

const int buttonPin = 8; 
int buttonState = 0;

const int sensorPin = 6; 
int sensorState = 0;

const int servoPin = 3;
Servo dispenser;
const int dispenserInitPos = 0;
int dispenserPos = 270;
int dispenserState = 0;  // 0 = in, 1 = out
bool shouldDispense = false;

const int buzzerPin = 4;
bool powerOn = true;
bool playOnce = true;
bool alarmOnce = true;
int once = 1;
// S_OHOOH for when removing phone/alert
// S_OHOOH2
// S_CONNECTION
// S_SUPER_HAPPY
// S_JUMP button clicks
// S_CONFUSED

const int addr = 0;
long int habitTime = 60000; //90m *21days = 1890h, 2 m * 21.
uint64_t dailyTime = 30000; //120000; //90 minutes real, 2 minutes demo.
long int lifeTime = 8100;    //90m * 90days = 8100h, 2m*90.
int days = 2;

DTime dtime;
uint64_t cycle;

// enum State_enum {INIT, WAIT, INFO, START};
int state = 0;

// AlarmId id;

void setup(void) {
  u8g2.begin();
  
  dispenser.attach(servoPin);
  dispenser.write(dispenserInitPos);  

  pinMode(buttonPin, INPUT);
  pinMode(sensorPin, INPUT);

  // EEPROM.write(addr, habitTime);
  cute.init(buzzerPin);
  powerOn=true;

  // id = Alarm.timerRepeat(2, Repeats2);  
  // Alarm.disable();

  Serial.begin(9600);
  delay(100);
  // cycle = (uint64_t)millis() + 1000;

  // while (!Serial) ; // wait until Arduino Serial Monitor opens
  // setSyncProvider(RTC.get);   // the function to get the time from the RTC
  // if(timeStatus()!= timeSet) 
  //    Serial.println("Unable to sync with the RTC");
  // else
  //    Serial.println("RTC has set the system time");  
     
}

void loop(void) {
  // Serial.println("b");
  buttonState = digitalRead(buttonPin);
  sensorState = digitalRead(sensorPin);
  state_machine_run();
  delay(10);  

  // if(powerOn) {
  //   cute.play(S_OHOOH2);
  //   powerOn=false;
  // }

  // // Serial.println(startButtonState);
  // // Serial.println(buttonState);
  // delay(100);
  // if(buttonState) {
  //   // Serial.println('aaaa');
  //   cute.play(S_CONFUSED);
  //   shouldDispense=true;
  //   // shouldDispense=true;
  // }
  // if(sensorState) {
  //   // Serial.println('aaaa');
  //   cute.play(S_JUMP);
  //   // shouldDispense=true;
  // }
  // // delay(100);
  // // for (dispenserPos; dispenserPos > 0; dispenserPos-=1){
  // //       dispenser.write(dispenserPos);
  // //       delay(20);
  // // }
  // // Serial.print("init");
  //   u8g2.setFont(u8g2_font_ncenB14_tr);
  //   u8g2.firstPage();
  //   do {
  //     u8g2.setCursor(4, 20);
  //     u8g2.print(F("Hello, press"));
  //     u8g2.setCursor(4, 40);
  //     u8g2.print(F("the button"));
  //     u8g2.setCursor(4, 60);
  //     u8g2.print(F("to start"));
  //   } while (u8g2.nextPage());

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
    
  // if (shouldDispense){
  //   // already dispensed
  //   if(dispenserState){
  //     for (dispenserPos; dispenserPos > 0; dispenserPos-=1){
  //       dispenser.write(dispenserPos);
  //       delay(10);
  //     }
  //     dispenserState=0;
  //     Serial.println("retrieved");
      
  //   }
  //   // will be dispensed
  //   else{
  //     for (dispenserPos; dispenserPos <360; dispenserPos+=1){
  //       dispenser.write(dispenserPos);
  //       delay(10);
  //     }
  //     dispenserState=1;
  //     Serial.println("dispensed");
  //   }
  //   shouldDispense=false;
  // }
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

String secToMin(uint64_t t, bool toZero=false) {
  int displayMin = t/60000;
  if (toZero && displayMin < 10) {
    return "0" + String(displayMin);
  }
  return String(displayMin);
}
String formatSec(uint64_t t, bool toZero=false) {
  uint32_t displaySec = (t/1000) % 60;
  if (toZero && displaySec < 10) {
    return "0" + String(displaySec);
  }
  return String(displaySec);
}
uint64_t formatSecTot(uint64_t t) {
  return t/1000;
}

uint64_t minToSec(uint64_t t) {
  return t*60000;
}

void greet_start_msg() {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.firstPage();
  do {
    u8g2.setCursor(4, 20);
    u8g2.print(F("Hello! press"));
    u8g2.setCursor(4, 40);
    u8g2.print(F("the button"));
    u8g2.setCursor(4, 60);
    u8g2.print(F("to start --->"));
  } while (u8g2.nextPage());
}
void start_msg() {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.firstPage();
  do {
    u8g2.setCursor(4, 20);
    u8g2.print(F("Press the"));
    u8g2.setCursor(4, 40);
    u8g2.print(F("button --->"));
    u8g2.setCursor(4, 60);
    u8g2.print(F("to start!"));
  } while (u8g2.nextPage());
}
void continue_msg() {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.firstPage();
  do {
    u8g2.setCursor(4, 20);
    u8g2.print(F("Press the"));
    u8g2.setCursor(4, 40);
    u8g2.print(F("button --->"));
    u8g2.setCursor(4, 60);
    u8g2.print(F("to continue!"));
  } while (u8g2.nextPage());
}


void init_routine() {
  delay(300);
  if(buttonState) {
    cute.play(S_JUMP);
    state = 1;
    playOnce=true;
    delay(300);
  }
  else {
    greet_start_msg();
    delay(300);
  }
}

void info_daily_routine() {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.firstPage();
  do {
    u8g2.setCursor(4, 20);
    u8g2.print(F("You have"));
    u8g2.setCursor(4, 40);
    u8g2.print(formatSecTot(dailyTime));    
    u8g2.print(" seconds");
    u8g2.setCursor(4, 60);
    u8g2.print(F("left today"));
  } while (u8g2.nextPage());
}
void info_total_routine() {
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.firstPage();
  do {
    u8g2.setCursor(4, 20);
    u8g2.print(F("You have"));
    u8g2.setCursor(4, 40);
    u8g2.print(formatSecTot(habitTime));    
    u8g2.print(" total");
    u8g2.setCursor(4, 60);
    u8g2.print(F("seconds left"));
  } while (u8g2.nextPage());
}

void info_routine() {
  if(buttonState) {
    cute.play(S_JUMP);
    state = 2;
    playOnce=true;
    delay(300);
  }
  else{
    info_daily_routine();
    delay(900);
    info_total_routine();
    delay(900);
    continue_msg();
    delay(700);
  }
}

void wait_routine() {
  Serial.println('waiting');
  sensorState = digitalRead(sensorPin);
  if (sensorState) {    
    delay(100);
    if(playOnce) {
      u8g2.firstPage();
      do {
        u8g2.setCursor(4, 20);
        u8g2.print(F("Put your"));
        u8g2.setCursor(4, 40);
        u8g2.print(F("phone on"));
        u8g2.setCursor(4, 60);
        u8g2.print(F("the stand!"));
      } while (u8g2.nextPage());
      delay(500);
      cute.play(S_SUPER_HAPPY);
      playOnce=false;
    }
    start_msg();
    if(buttonState) {
      cute.play(S_JUMP);
      state = 3;
      playOnce=true;
      delay(100);
    }
  } else {
    playOnce=true;    
    u8g2.firstPage();
    do {
      u8g2.setCursor(4, 20);
      u8g2.print(F("Put your"));
      u8g2.setCursor(4, 40);
      u8g2.print(F("phone on"));
      u8g2.setCursor(4, 60);
      u8g2.print(F("the stand!"));
    } while (u8g2.nextPage());
  }
  delay(300);
}

void servo_routine() {
  if(buttonState) {
    cute.play(S_JUMP);
    shouldDispense=!shouldDispense;
    delay(300);
  }
  if (shouldDispense){
    dispenser.attach(servoPin);
    // already dispensed
    if(dispenserState){
      // for (dispenserPos; dispenserPos > 0; dispenserPos-=1){
      dispenser.write(dispenserInitPos);
      delay(100);
      // }
      dispenserState=0;
      Serial.println("retrieved");
      
    }
    // will be dispensed
    else{
      
      for (dispenserPos; dispenserPos <380; dispenserPos+=2){
        dispenser.write(dispenserPos);
        delay(10);
      }
      dispenserState=1;
      Serial.println("dispensed");
    }
    shouldDispense=false;
    dispenser.detach();
  }
  u8g2.firstPage();
  do {
    u8g2.setCursor(4, 20);
    u8g2.print(F("Put container"));
    u8g2.setCursor(4, 40);
    u8g2.print(F("back and press"));
    u8g2.setCursor(4, 60);
    u8g2.print(F("the button --->"));
  } while (u8g2.nextPage());
  delay(300);
}

String decimateSec(byte b) {
  return String(59-b);
  // return ((b < 10) ? "0" : "") + String(9-b);
}
String decimateMin(byte b, int mins) {
  return ((b < 10) ? "0" : "") + String(mins-b);
}

void alarm_routine() {
  cute.play(S_JUMP);
  days-=1;
  dailyTime = habitTime/days;
  state = 4;
  playOnce=true;
  delay(500);
}

bool phone_on() {
  sensorState = digitalRead(sensorPin);
  if (!sensorState) {    
    delay(1000); 
    return sensorState;
  }
}
void alert_phone() {
  sensorState = digitalRead(sensorPin);
  if(sensorState) return;
  else delay(3000);
  while(!sensorState){  
    delay(500); 
    sensorState = digitalRead(sensorPin);
    if(sensorState) break;
    // delay(100); 
    cute.play(S_OHOOH);
    cute.play(S_OHOOH2);
    u8g2.firstPage();
    do {
      u8g2.setCursor(4, 20);
      u8g2.print(F("Put your"));
      u8g2.setCursor(4, 40);
      u8g2.print(F("phone on"));
      u8g2.setCursor(4, 60);
      u8g2.print(F("the stand!"));
    } while (u8g2.nextPage());
  }
}
void main_routine() {
  if (once){
    // cycle = dailyTime;
    dailyTime = habitTime/days;
    cycle = (uint64_t)millis();
    once = 0;
  }
  if(buttonState) {
      cute.play(S_JUMP);
      while(true){
        u8g2.firstPage();
        do {
          u8g2.setCursor(4, 20);
          u8g2.print(F("Paused."));
          u8g2.setCursor(4, 40);
          u8g2.print(F("Press --->"));
          u8g2.setCursor(4, 60);
          u8g2.print(F("to resume!!"));
        } while (u8g2.nextPage());
        delay(300);
        buttonState = digitalRead(buttonPin);
        if(buttonState) {
          cute.play(S_JUMP);
          break;     
        }   
      }
      // state = 4;
      // playOnce=true;
      // delay(100);
    }
  alert_phone();
  if (dailyTime>1000 && habitTime>1000){
    if ((uint64_t)millis() - cycle >= 1000)
    {
      dailyTime-=1000;
      habitTime-=1000;
      cycle += 1000;
      // display(cycle);
      u8g2.firstPage();
      do {
        u8g2.setCursor(30, 40);
        u8g2.print(secToMin(dailyTime, true));
        u8g2.print(" : ");
        u8g2.print(formatSec(dailyTime, true));
      } while (u8g2.nextPage());      
      if (dailyTime<=1000) alarm_routine();
    }    
  }
  // else{
  //   alert_phone();
  // }
  // if ((uint64_t)millis() >=  cycle) {

  //   int dailyMinutes = dailyTime/60000;    
  //   if (dailyMinutes -dtime.minute < 0) {
  //     cute.play(S_JUMP);
  //     state = 4;
  //     playOnce=true;
  //     delay(100);
  //   }
  //   dailyTime -= 1000;
  //   cycle += 1000;
  //   dtime.tick();    

  //   u8g2.firstPage();
  //   do {
  //     u8g2.setCursor(30, 40);
  //     u8g2.print(decimateMin(dtime.minute,dailyMinutes));
  //     u8g2.print(" : ");
  //     u8g2.print(decimateSec(dtime.second));
  //   } while (u8g2.nextPage());    
  // }


//   EEPROM.read(addr);
}
// void clockDisplay(){
//   // digital clock display of the time
//   Serial.println(minute());
//   printDigits(minute());
//   printDigits(second());
//   Serial.println(second());
// }

// void printDigits(int digits){
//   // utility function for digital clock display: prints preceding colon and leading 0
//   Serial.print(":");
//   if(digits < 10)
//     Serial.print('0');
//   Serial.print(digits);
// }

void day_routine() {
  Serial.println(habitTime);

  if (habitTime<1000 || days<1) {
    state=5;
    return;
  }
  if(buttonState) {
    cute.play(S_JUMP);
    state=3;
    once=1;
    playOnce=true;
    delay(300);
    return;
  }
  u8g2.firstPage();
  do {
    u8g2.setCursor(3, 20);
    u8g2.print(F("Congrats, "));
    u8g2.setCursor(3, 40);
    u8g2.print(F("you have "));
    u8g2.setCursor(3, 60);
    u8g2.print(F("finished "));
  } while (u8g2.nextPage());
  delay(700);
  u8g2.firstPage();
  do {
    u8g2.setCursor(3, 20);
    u8g2.print(F("your "));
    u8g2.setCursor(3, 40);
    u8g2.print(F("daily "));
    u8g2.setCursor(3, 60);
    u8g2.print(F("goal"));
  } while (u8g2.nextPage());
  delay(1000);
  u8g2.firstPage();
  do {
    u8g2.setCursor(4, 20);
    u8g2.print("You have ");
    u8g2.print(days);
    u8g2.setCursor(4, 40);
    u8g2.print(F("more days "));
    u8g2.setCursor(4, 60);
    u8g2.print(F("to go!"));
  } while (u8g2.nextPage());
  delay(700);
  u8g2.firstPage();
  do {
    u8g2.setCursor(4, 20);
    u8g2.print(F("See you"));
    u8g2.setCursor(4, 40);
    u8g2.print(F("tomorrow!"));
  } while (u8g2.nextPage());
  delay(1000);
}

void goal_routine() {
  u8g2.firstPage();
  do {
    u8g2.setCursor(4, 20);
    u8g2.print(F("Congrats!"));
    u8g2.setCursor(4, 40);
    u8g2.print(F("you have "));
    u8g2.setCursor(4, 60);
    u8g2.print(F("finished"));
  } while (u8g2.nextPage());
  delay(700);
  u8g2.firstPage();
  do {
    u8g2.setCursor(4, 20);
    u8g2.print(F("your"));
    u8g2.setCursor(4, 40);
    u8g2.print(F("21 days "));
    u8g2.setCursor(4, 60);
    u8g2.print(F("goal"));
  } while (u8g2.nextPage());
  delay(1000);

  u8g2.firstPage();
  do {
    u8g2.setCursor(4, 20);
    u8g2.print(F("You will now"));
    u8g2.setCursor(4, 40);
    u8g2.print(F("get your "));
    u8g2.setCursor(4, 60);
    u8g2.print(F("reward"));
  } while (u8g2.nextPage());
  delay(700);
  u8g2.firstPage();
  continue_msg();
  delay(700);
  if(buttonState) {
    cute.play(S_JUMP);
    state=6;
    shouldDispense=true;
    delay(300);
  }
}


void state_machine_run() {

  switch(state) {
    case 0:
      init_routine();
      break;
    case 1:
      // if(playOnce) {
      //   cute.play(S_SUPER_HAPPY);
      //   playOnce=false;
      // }
      info_routine();
      break;
    case 2:
      // if(playOnce) {
      //   cute.play(S_SUPER_HAPPY);
      //   playOnce=false;
      // }
      wait_routine();
      break;
    case 3:
      // if(playOnce) {
      //   cute.play(S_SUPER_HAPPY);
      //   playOnce=false;
      // }
      main_routine();
      break;
    case 4:
    // if(playOnce) {
    //   cute.play(S_SUPER_HAPPY);
    //   playOnce=false;
    // }
      day_routine();
      break;
    case 5:
      goal_routine();
      break;
    case 6:
      servo_routine();
      break;
  }
}
