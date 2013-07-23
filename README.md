Arduino_Mobile_OS
=================

An arduino sketch that can be used to help debug and test electronics projects.

There are 3 folders in the project: 

Arduino_MobileOS_1: 
This was the first version, which had some extra functionality but lacked the organization that i wanted in such an os. I almost entirely rewrote version 1.5 from this version.

Arduino_MobileOS_1.5: 
This is the stable working release. It has a menu subroutine and nested menus instead of one single menu, like version 1.

Arduino_MobileOS_2: 
This is a newer but unstable release. It currently does not compile due to a library error.

You will also need to download some libraries to make it work. All functionality depends on an LCD and a 5 way button pad. I used the Adafruit 16x2 I2C LCD shield (http://www.adafruit.com/products/772), but if you use another shield, you will have to modify your program a little. first, change the lcd declarations to engage the LCD as an object called lcd. then replace all instances of lcd.readButtons with a reference to a function that returns a byte containing button data in the form (in binary) 0,0,0,left,up,down,right,select with each being an on or off bit. You will also need to replace all instances of lcd.setBacklight to a function you write that sets the backlight on if the function is sent 0x01 or off is it is sent 0x00.

The button setup i used looks like this:
          up          select
     left      right
          down





This code is provided without warranty.
