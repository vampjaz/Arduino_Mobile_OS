// include the library code:
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>


// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#include "pitches.h"

// setup menu 
#define MENUITEMS 6   
const String menu_strings[MENUITEMS] = {
 "Slots","Random numbers","Uptime","Settings","Music","Ping)))" };
// for music menu
#define NUMSONGS 3
const int songlen[] = {32,0,48};
  
const String song_strings[NUMSONGS] = {
  "Starwars","Gandnam Style","Happy"};
const int songs[][48] = {
  {NOTE_C4,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_D4,NOTE_C5,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_D4,NOTE_C5,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_F4,NOTE_D4,
  NOTE_C4,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_D4,NOTE_C5,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_D4,NOTE_C5,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_F4,NOTE_D4},
  {},
  {
NOTE_D3, NOTE_FS3, NOTE_A3, NOTE_B3, NOTE_C4, NOTE_B3, NOTE_A3, NOTE_FS3,
NOTE_D3, NOTE_FS3, NOTE_A3, NOTE_B3, NOTE_C4, NOTE_B3, NOTE_A3, NOTE_FS3,
NOTE_G3, NOTE_B3, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_B3,
NOTE_D3, NOTE_FS3, NOTE_A3, NOTE_B3, NOTE_C4, NOTE_B3, NOTE_A3, NOTE_FS3,
NOTE_A3, NOTE_A3, NOTE_CS4, NOTE_E4, NOTE_G3, NOTE_G3, NOTE_B3, NOTE_D4,
NOTE_D4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_A3, NOTE_G3, NOTE_FS3, NOTE_E3}};

const int tonelen[][48] = {
  {2,2,8,8,8,2,2,8,8,8,2,2,8,8,8,1,2,2,8,8,8,2,2,8,8,8,2,2,8,8,8,1},
  {},
  {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}};


void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
}

void loop() {
  // menu
  boolean select; static byte item, selection;
  select = false;
  lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(item + 1);
    lcd.setCursor(0,1);
    lcd.print(menu_strings[item]);
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
    if (buttons & BUTTON_SELECT) {
      selection = item;
      select = true;
    }
    item = constrain(item,0,MENUITEMS);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(item + 1);
    lcd.setCursor(0,1);
    lcd.print(menu_strings[item]);
    delay(100);
  }
  }
  
  switch (selection) {
    case 0:
    
    break;
    case 1:
    lcd.clear();
    lcd.setCursor(0,0);
    while (!lcd.readButtons()) {
      lcd.print(analogRead(3)*analogRead(4));
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
    case 4:
    
   boolean selects; static byte items, selections;
  selects = false;
  lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(item + 1);
    lcd.setCursor(0,1);
    lcd.print(song_strings[items]);
    delay(100);
    
  while (!selects) {
    uint8_t buttons = lcd.readButtons();
    if (buttons) {
    if (buttons & BUTTON_UP) {
     items++; 
    }
    if (buttons & BUTTON_DOWN) {
      items--;
    }
    if (buttons & BUTTON_SELECT) {
      selections = items;
      selects = true;
    }
    item = constrain(items,0,NUMSONGS);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(items + 1);
    lcd.setCursor(0,1);
    lcd.print(song_strings[items]);
    delay(100);
  }
  }
  
  music(selections);
  
    break;
    case 5:
    
    break;
}
}

void wait() {
  while (!lcd.readButtons()) { } 
}

void music(byte song) {
  for (int thisNote = 0; thisNote < songlen[song]; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/songs[song][thisNote];
    tone(48, tonelen[song][thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}
