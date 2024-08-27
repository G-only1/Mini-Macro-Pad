#include <Keypad.h>
//#include "Keyboard.h"
#include "HID-Project.h" //switched to HID-project so i can send raw keycodes and acces media keys
//https://githubutton1.com/NicoHood/HID
//https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
#include "Rotary.h" //for rotary encoder
#include "Button2.h" //for button on rotary encoder

//=====================================TODO=====================================
//move panic links array to better spot probably by key 8 pressed code
//add multi key support???
//add key combos?
//make holding a button switch between profiles and send a popup with msg command to tell what profile your on or use LEDs theres only 2 pins left so there could only be 2 profiles could use RGB LED meybe
//could use second rotary encoder to switch profiles? and pressing it could turn indicator LED on or off
//could use a RGB LED to get 3 colors instead of 2? nvm i killd the only rgb led i had
//use ctrl library to control LEDs for profiles possibly replace other libraries with it too
//==============================================================================

//setup pins for rotary encoder
#define ROTARY1_PIN1 10
#define ROTARY1_PIN2 16
#define BUTTON1_PIN 14 //setup pin rotary emcoder is connected to, connect other side of button to GND
//setup second rotary encoder pins
#define ROTARY2_PIN1 A0
#define ROTARY2_PIN2 A1
#define BUTTON2_PIN 15
#define CLICKS_PER_STEP 4 //depends on your rotary encoder
Rotary rotary1, rotary2; //idk what this does but rotary dosent work without it
Button2 button1, button2; //same with the one for rotary idk what this does but it breaks without this

const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
char keys[ROWS][COLS] = {
  { 1, 2, 3, 4 },
  { 5, 6, 7, 8 },
  { 9, 10, 11, 12 },
  { 13, 14, 15, 16 },
};

byte rowPins[ROWS] = { 6, 7, 8, 9 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 5, 4, 3, 2 };  //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//list of links for the panik button to use (key 8) //TODO move this to better spot
String panicLinks[7] = {"https://youtu.be/6RojLdt504c","https://youtu.be/AwtJzvZ51m0","https://youtu.be/Scw_dnXOJBw","https://www.youtube.com/watch?v=OpyLmIjZaxY","https://youtu.be/J1NxcgasTIU","https://youtu.be/7NA35gsOvnY","https://youtu.be/klsGsPY-Q9k"};
int panicLinksLength = sizeof (panicLinks) / sizeof (panicLinks[0]);

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  Consumer.begin();
  //begin rotary 1
  rotary1.begin(ROTARY1_PIN1, ROTARY1_PIN2, CLICKS_PER_STEP);
  rotary1.setChangedHandler(rotate1);
  rotary1.setLeftRotationHandler(rotateLeft1);
  rotary1.setRightRotationHandler(rotateRight1);
  //begin rotary button 1
  button1.begin(BUTTON1_PIN);
  button1.setTapHandler(clickButton1);
  button1.setLongClickHandler(holdButton1);
  //begin rotary 2
  rotary2.begin(ROTARY2_PIN1, ROTARY2_PIN2, CLICKS_PER_STEP);
  rotary2.setChangedHandler(rotate2);
  rotary2.setLeftRotationHandler(rotateLeft2);
  rotary2.setRightRotationHandler(rotateRight2);
  //begin button 2
  button2.begin(BUTTON2_PIN);
  button2.setTapHandler(clickButton2);
  button2.setLongClickHandler(holdButton2);
}

void loop() {
  switch()
  rotary1.loop();
  rotary2.loop();
  button1.loop();
  button2.loop();
  char key = keypad.getKey();
  if (key) {
    if (keypad.stateChanged = true) {
      //Serial.println("some key pressed");
      if (keypad.isPressed(1)) {
        Serial.println("key 1 pressed, opening task manager");
        Keyboard.press(KEY_LEFT_CTRL);
        //delay(50);
        Keyboard.press(KEY_LEFT_SHIFT);
        //delay(50);
        Keyboard.press(KEY_ESC);
        //delay(50);
        Keyboard.releaseAll();
      }
      else if (keypad.isPressed(2)) {
        Serial.println("key 2 pressed, opening powershell"); //print whats going on to serial monitor
        //opens windows run dialog then types into the box and presses enter
        Keyboard.press(KEY_LEFT_GUI); //hold left windows key
        delay(25);
        Keyboard.press(114); //hold r
        Keyboard.releaseAll(); //release all keys
        delay(200);
        Keyboard.println("powershell"); //type powershell and press enter
        Keyboard.write(224); //press enter
      }
      else if (keypad.isPressed(3)) {
        Serial.println("key 3 pressed, opening notepad");
        Keyboard.press(KEY_LEFT_GUI); //hold left windows key
        delay(25);
        Keyboard.press(114); //hold r
        Keyboard.releaseAll(); //release all keys
        delay(200);
        Keyboard.println("notepad"); //type something and press enterotary1. change this to change what the key opens
        Keyboard.write(224); //press enter
      }
      else if (keypad.isPressed(4)) {
        Serial.println("key 4 pressed, opening spotify");
        Keyboard.press(KEY_LEFT_GUI); //hold left windows key
        delay(25);
        Keyboard.press(114); //hold r
        Keyboard.releaseAll(); //release all keys
        delay(200);
        Keyboard.println("spotify"); //type something and press enterotary1. change this to change what the key opens
        Keyboard.write(224); //press enter
      }
      else if (keypad.isPressed(5)) {
        Serial.println("key 5 pressed, opening file explorer");
        Keyboard.press(KEY_LEFT_GUI); //hold left windows key
        delay(25);
        Keyboard.press(101); //hold e
        Keyboard.releaseAll(); //release all keys
      }
      else if (keypad.isPressed(6)) {
        Serial.println("key 6 pressed, opening firefox");
        Keyboard.press(KEY_LEFT_GUI); //hold left windows key
        delay(25);
        Keyboard.press(114); //hold r
        Keyboard.releaseAll(); //release all keys
        delay(200);
        Keyboard.println("firefox"); //type something and press enterotary1. change this to change what the key opens
        Keyboard.write(224); //press enter
      }
      else if (keypad.isPressed(7)) {
        Serial.println("key 7 pressed, minimizing all windows");
        Keyboard.press(KEY_LEFT_GUI); //hold left windows key
        delay(25);
        Keyboard.press(100); //hold d
        delay(25);
        Keyboard.releaseAll(); //release all keys
      }
      else if (keypad.isPressed(8)) {
        Serial.println("key 8 pressed, PANIK BUTON (minimize all windows pause all media then open firefox and play a youtube video)");
        //minimize all windows
        Keyboard.press(KEY_LEFT_GUI); //hold left windows key
        delay(25);
        Keyboard.press(100); //hold d
        delay(25);
        Keyboard.releaseAll(); //release all keys
        delay(25);
        //pause all media
        Consumer.write(0xB7); //stop all media
        //open firefox
        Keyboard.press(KEY_LEFT_GUI); //hold left windows key
        delay(25);
        Keyboard.press(114); //hold r
        Keyboard.releaseAll(); //release all keys
        delay(200);
        Keyboard.println("firefox"); //type something. change this to change what the key opens
        Keyboard.write(224); //press enter
        delay(250);
        //open new firefox tab
        Keyboard.press(KEY_LEFT_CTRL); //press and hold ctrl
        Keyboard.press(116); //press t
        Keyboard.releaseAll();
        //open run dialoug
        Keyboard.press(KEY_LEFT_GUI); //hold left windows key
        delay(25);
        Keyboard.press(114); //hold r
        Keyboard.releaseAll(); //release all keys
        delay(200);
        String link = panicLinks[random(panicLinksLength)]; //pick a random link from the array to open
        Serial.println("link opened: "+link);
        Keyboard.println(link); //types random link from array of safe video links to play and hits enter
        Keyboard.write(224); //press enter
      }
      else if (keypad.isPressed(9)) {
        Serial.println("key 9 pressed, pausing all media");
        Consumer.write(0xCD); //pause
      }
      else if (keypad.isPressed(10)) {
        Serial.println("key 10 pressed, volume up");
        Consumer.write(0xE9); //volume up
      }
      else if (keypad.isPressed(11)) {
        Serial.println("key 11 pressed, volume down");
        Consumer.write(0xEA); //volume down
      }
      else if (keypad.isPressed(12)) {
        Serial.println("key 12 pressed, muting audio");
        Consumer.write(0xE2); //mute
      }
      else if (keypad.isPressed(13)) {
        Serial.println("key 13 pressed, skipping song");
        Consumer.write(0x5B); //next media
      }
      else if (keypad.isPressed(14)) {
        Serial.println("key 14 pressed, bass down");
        Consumer.write(0x0153);
      }
      else if (keypad.isPressed(15)) {
        Serial.println("key 15 pressed, bass up");
        Consumer.write(0x0152);
      }
      else if (keypad.isPressed(16)) {
        Serial.println("key 16 pressed, opening calculator");
        Consumer.write(0x0192);
      }
    }
  }
}

//rotary1 handlers
void rotate1(Rotary) {

}

void rotateLeft1(Rotary) {
  Serial.println("Rotary1 Rotated left, volume down");
  Consumer.write(0xEA); //volume down
}

void rotateRight1(Rotary) {
  Serial.println("Rotary1 Rotated right, volume up");
  Consumer.write(0xE9); //volume up
}

void clickButton1(Button2) {
  Serial.println("Rotary1 Button1 pressed, mute toggled");
  Consumer.write(0xE2); //mute
}

void holdButton1(Button2) {

}

//rotary 2 handlers
void rotate2(Rotary) {

}

void rotateLeft2(Rotary) {
  Serial.println("Rotary2 Rotated left, volume down");
  Consumer.write(0xEA); //volume down
}

void rotateRight2(Rotary) {
  Serial.println("Rotary2 Rotated right, volume up");
  Consumer.write(0xE9); //volume up
}

void clickButton2(Button2) {
  Serial.println("Rotary2 Button2 pressed, mute toggled");
  Consumer.write(0xE2); //mute
}

void holdButton2(Button2) {

}
