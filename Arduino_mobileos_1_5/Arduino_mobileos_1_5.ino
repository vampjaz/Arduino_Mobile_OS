// include the library code:
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

#define IRpin_PIN PINK
#define IRpin 5
 
// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000
 
// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

int money;
const String menuitems[7] = {"back","Utilities","Games","Hardware","Biometrics","Settings","Help"};
const String utilmenuitems[6] = {"back","Random Numbers","Uptime","Device Info"};
const String gamesmenuitems[2] = {"back","Slots"};
const String hardwaremenuitems[5] = {"back","Serial Input","IR Decoding","Analog input","Datalogging"};
const String biomenuitems[3] = {"back","Pulse Oximeter","C.A.N.E. demo"};
const String settingsmenuitems[2] = {"back","Backlight"};
const String backlightmenuitems[3] = {"back","Off","On"}; const byte RGB = 3;
// uncomment this next line and comment out the previous one if using an RGB LCD shield
// const String backlightmenuitems[9] = {"back","Off","Red","Yellow","Green","Teal","Blue","Violet","White"); const byte RGB = 9;
const String serialmenuitems[14] = {"back","300","600","1200","2400","4800","9600","14400","19200","28800","38400","57600","115200","Custom"};
const long baudrates[13] = {300,600,1200,2400,4800,9600,14400,19200,28800,38400,57600,115200};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  money = 0;
  lcd.createChar(0, heart);
}

void loop() {
  // main menu
  byte selection = menu("Main",menuitems,7);
  byte hard,game,util,bio;
  switch (selection) {
    case 1:
      boolean uesc;
      uesc = false;
      while (!uesc) {
        util = menu("Utilities",utilmenuitems,4);
        switch (util) {
          case 0:
           uesc = true;
          break;
          case 1:
            // random numbers
            lcd.clear();
            lcd.setCursor(0,0);
            randomSeed(analogRead(0));
              while (!lcd.readButtons()) {
                lcd.print(random(10));
              } 
              delay(250);
            wait();
            break;
            case 2:
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(millis());
            lcd.setCursor(0,1);
            lcd.print(micros());
            wait();
            break;
            case 3:
            
            break;
      } }
    break;
    case 2:
      boolean gesc;
      gesc = false;
      while (!gesc) {
        game = menu("Games",gamesmenuitems,2);
        switch (game) {
          case 0:
           gesc = true;
          break;
          case 1:
                //slots
            randomSeed(analogRead(0));
            lcd.clear();
            int i;
            i = 0;
            lcd.setCursor(0,0);
            lcd.print("$");
            lcd.print(money);
            
            byte d1,d2,d3;
            while (!lcd.readButtons() && i<700) {
              d1 = random(10);
              d2 = random(10);
              d3 = random(10);
              lcd.setCursor(5,1);
              lcd.print(d1);
              lcd.print(" ");
              lcd.print(d2);
              lcd.print(" ");
              lcd.print(d3);
              delay(i);
            }
            if (d1 == d2 || d2 == d3 || d3 == d1) {
              money += 30;
            }
            if (d1 == d2 && d2 == d3) {
              money += 100;
            }
            lcd.setCursor(0,0);
            lcd.print("$");
            lcd.print(money);
            delay(500);
            wait();
          break;
        } }
    break;
    case 3:
      boolean hesc;
      hesc = false;
      while (!hesc) {
        hard = menu("Hardware",hardwaremenuitems,5);
        switch (hard) {
          case 0:
           hesc = true;
          break;
          case 1:
            serialio = true;
            long baudrate;
            byte mbaud;
            mbaud = menu("Baud Rate",serialmenuitems,14);
            if (mbaud == 0) {
              break;
            }
            if (mbaud == 14) {
              baudrate = number("Baud",300,115200,300);
            } else {
              baudrate = baudrates[mbaud - 1];
            }
            Serial.begin(baudrate);
            lcd.clear();
            lcd.setCursor(0,0);
            byte lvl,f;
            f = 0;
            lvl = 1;
            while (!lcd.readButtons()) {
              if (Serial.available()) { 
                char c = Serial.read();
                lcd.print(c);
                f++;
                if (f > 15) {
                  f = 0;
                  lcd.setCursor(0,lvl);
                  if (lvl == 0) {
                    lvl = 1;
                  } else {
                    lvl = 0;
                  } 
                }
              }            
            }
            Serial.end();
            serialio = false;
            wait();
          break;
          case 2:
            // we will store up to 64 pulse pairs (this is -a lot-)
            int pulses[64][2]; // pair is high and low pulse
            byte currentpulse;
            
            currentpulse = 0; // index for pulses we're storing
            int longpulse,shortpulse,offpulse;
            longpulse = 0;
            shortpulse = MAXPULSE;
            offpulse = 0;
            boolean iesc;
            iesc = false;
            
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Use Remote Now");
            
            pinMode(68,OUTPUT);
            pinMode(69,OUTPUT);
            digitalWrite(68,LOW);
            digitalWrite(69,HIGH);
            
            while (!lcd.readButtons() && !iesc) {
              int highpulse, lowpulse; // temporary storage timing
              highpulse =0;
              lowpulse = 0; // start out with no pulse length
              
            // while (digitalRead(IRpin)) { // this is too slow!
                while (IRpin_PIN & (1 << IRpin)) {
                 // pin is still HIGH
             
                 // count off another few microseconds
                 highpulse++;
                 delayMicroseconds(RESOLUTION);
             
                 // If the pulse is too long, we 'timed out' - either nothing
                 // was received or the code is finished, so print what
                 // we've grabbed so far, and then reset
                 if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
                   
                   currentpulse=0;
                   iesc = true;
                 }
              }
              // we didn't time out so lets stash the reading
              pulses[currentpulse][0] = highpulse;
              
              if (highpulse > longpulse) {
                longpulse = highpulse;
              }
              if (highpulse < shortpulse) {
                shortpulse = highpulse;
              }
              
              // same as above
              while (! (IRpin_PIN & _BV(IRpin))) {
                 // pin is still LOW
                 lowpulse++;
                 delayMicroseconds(RESOLUTION);
                 if ((lowpulse >= MAXPULSE) && (currentpulse != 0)) {
                   int midpulse,logicpulse;
                   byte cp,bytes,byteform[8];
                   cp = 0;
                   bytes = 0;
                   midpulse = (longpulse + shortpulse) / 2;
                   for (int pulse = 0; pulse < currentpulse; pulse+8) {
                     for (int a = 0; a < 7; a++) {
                       cp << 1;
                       if (pulses[0][pulse + a] > midpulse) {
                         cp++;
                       }
                     }
                     bytes++;
                     byteform[pulse / 8] = cp;
                   }
                   lcd.clear();
                   lcd.setCursor(0,0);
                   for (int a = 0; a < bytes; a++) {
                     lcd.print(byteform[a]);
                     lcd.print(" ");
                   }
                   lcd.setCursor(0,1);
                   lcd.print("H: ");
                   lcd.print(longpulse);
                   lcd.print("L: ");
                   lcd.print(shortpulse);
                   lcd.print("O: ");
                   lcd.print(offpulse);
                   wait;
                   iesc = true;
                 }
              
              pulses[currentpulse][1] = lowpulse;
              if (currentpulse == 1) {
                offpulse = lowpulse;
              }
              // we read one high-low pulse successfully, continue!
              currentpulse++;
            }
            }
          break;
          case 3:
            byte apin = number("Analog Pin",0,15,3);
            while(!lcd.readButtons()) {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(analogRead(apin));
              delay(20);
            }\
            
          break;
        }
      }
    break;
    case 4:
      boolean besc;
      besc = false;
      while (!besc) {
        bio = menu("Biometrics",biomenuitems,4);
        switch (bio) {
          case 0:
           besc = true;
          break;
          case 1:
            pinMode(67,OUTPUT);
            pinMode(69,OUTPUT);
            digitalWrite(67,LOW);
            digitalWrite(69,HIGH);
            double alpha;
            alpha = 0.75;
            double change;
            change = 0.0;
            double bpm;
            long lasttime, pulseinc;
            pulseinc = 0;
            lasttime = millis();
            
            while (!lcd.readButtons()) {
              static double oldvalue = 0.0;
              static double oldchange = 0.0;
              int rawvalue = analogRead(14);
              double value = alpha * oldvalue + (1 - alpha) * rawvalue;
              change = value - oldvalue;
              lcd.clear();
              if (change > 0.0 && oldchange < 0.0) {
                //lcd.setCursor(15,0);
                //lcd.write(0);
                pulseinc = millis() - lasttime;
                bpm = (1000 / pulseinc) * 60;
                lasttime = millis();
              } else {
                //lcd.setCursor(15,0);
                //lcd.print(" ");
              }
              oldchange = change;
              lcd.setCursor(0,0);
              lcd.print(pulseinc);
             // lcd.print(" BPM");
              delay(50);
            }
            
            digitalWrite(69,LOW);
          break;
          case 2:
          
          break;
        }
      }
    break;
    case 5:
      //settings
      boolean sesc;
      sesc = false;
      while (!sesc) {
        byte sel4 = menu("Settings",settingsmenuitems,2);
        switch (sel4) {
          case 0:
            sesc = true;
          break;
          case 1:
            lcd.setBacklight(menu("Backlight",backlightmenuitems,RGB) - 1);
        }
      } 
    break;
    case 6:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Arduino Mobile O");
      lcd.setCursor(0,1);
      lcd.print("S: Arduino-based");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("development plat");
      lcd.setCursor(0,1);
      lcd.print("form and testing");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("utility. Written");
      lcd.setCursor(0,1);
      lcd.print("using Adafruit");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("LCD shield by Ja");
      lcd.setCursor(0,1);
      lcd.print("ckson Servheen");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Instructions:");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("in a menu, press");
      lcd.setCursor(0,1);
      lcd.print("up and down to");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("scroll. Press le");
      lcd.setCursor(0,1);
      lcd.print("ft to go back");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("and select to en");
      lcd.setCursor(0,1);
      lcd.print("ter a submenu");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("or mini app. If");
      lcd.setCursor(0,1);
      lcd.print("prompted to");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("enter a number,");
      lcd.setCursor(0,1);
      lcd.print("use up and");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("down to scroll,");
      lcd.setCursor(0,1);
      lcd.print("and left and");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("right to jump a");
      lcd.setCursor(0,1);
      lcd.print("larger amount.");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("If an app is doi");
      lcd.setCursor(0,1);
      lcd.print("ng something,");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("press any button");
      lcd.setCursor(0,1);
      lcd.print("to quit.");
      wait();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enjoy! feedback");
      lcd.setCursor(0,1);
      lcd.print(" is appreciated");
      wait();
    break;
  }
}

void wait() {
  delay(150);
  while (!lcd.readButtons()) { } 
}

byte menu(String title,const String* strings,byte length) {
  boolean select, esc; static byte item, selection;
  select = false;
  esc = false;
  item = 1;
  lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(title);
    lcd.print(": ");
    lcd.print(item);
    lcd.setCursor(0,1);
    lcd.print(strings[item]);
    delay(100);
    
  while (!select) {
    uint8_t buttons = lcd.readButtons();
    if (buttons) {
    if (buttons & BUTTON_UP) {
      item--; 
    }
    if (buttons & BUTTON_DOWN) {
      item++;
    }
    if (buttons & BUTTON_LEFT) {
      esc = true;
    }  
    if (buttons & BUTTON_SELECT) {
      selection = item;
      select = true;
    }
  
    item = constrain(item,1,length - 1);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(title);
    lcd.print(": ");
    lcd.print(item);
    lcd.setCursor(0,1);
    lcd.print(strings[item]);
    if (esc) {
      select = true;
      selection = 0;
    }
    delay(100);
  
  }
  } 
  return(selection);
}

long number(String title, long low, long high, long jump) {
  boolean select; static long item, selection;
  select = false;
  item = low;
  lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(title);
    lcd.print(": Number");
    lcd.setCursor(0,1);
    lcd.print(item);
    delay(100);
    
  while (!select) {
    uint8_t buttons = lcd.readButtons();
    if (buttons) {
    if (buttons & BUTTON_UP) {
      item++; 
    }
    if (buttons & BUTTON_DOWN) {
      item--;
    }
    if (buttons & BUTTON_LEFT) {
      item -= jump;
    }
    if (buttons & BUTTON_RIGHT) {
      item += jump;
    }
    if (buttons & BUTTON_SELECT) {
      selection = item;
      select = true;
    } 
    item = constrain(item,low,high);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(title);
    lcd.print(": Number");
    lcd.setCursor(0,1);
    lcd.print(item);
    delay(100);
    }
  } 
  return(item);
}
