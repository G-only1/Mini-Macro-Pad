#include <Keypad.h>
//#include "Keyboard.h"
#include "HID-Project.h" //switched to HID-project so i can send raw keycodes and acces media keys
//https://githubutton1.com/NicoHood/HID
//https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
#include "Rotary.h" //for rotary encoder
#include "Button2.h" //for button on rotary encoder
//#include "CtrlLed.h" //for controling LEDs

//=====================================TODO=====================================
//move panic links array to better spot probably by key 8 pressed code
//add multi key support???
//add key combos?
//make pressing profile select rotary encoder turn LEDs off and switch to sending popups to tell profile selected
//fix when the profile select rotary encoder goes into the negitive and all LEDs are off
//add persistance to LED on or off status with eeprom library https://docs.arduino.cc/learn/built-in-libraries/eeprom/
//==============================================================================

//setup pins for rotary encoder
#define ROTARY1_PIN1 A0
#define ROTARY1_PIN2 A1
#define BUTTON1_PIN 1 //setup pin rotary emcoder is connected to, connect other side of button to GND
//setup second rotary encoder pins
#define ROTARY2_PIN1 A2
#define ROTARY2_PIN2 A3
#define BUTTON2_PIN 0
#define CLICKS_PER_STEP 4 //depends on your rotary encoder
Rotary rotary1, rotary2; //idk what this does but rotary dosent work without it
Button2 button1, button2; //same with the one for rotary idk what this does but it breaks without this

//State LED pins
int S1 = 15;
int S2 = 14;
int S3 = 16;
int S4 = 10;
const int numStates = 4;
const int States[numStates] = {S1, S2, S3, S4};
int currentState = 0;

bool LEDsOn = true; //to keep track of LED status

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
  //set all state LEDs to off
  for (int i = 0; i < numStates; i++){
    pinMode(States[i], OUTPUT);
    digitalWrite(States[i], LOW);
  }

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

  StartAnimation();
  digitalWrite(States[currentState], HIGH);
}

void StartAnimation(){
  int waitTime = 250;
  digitalWrite(S1, HIGH);
  delay(waitTime);
  digitalWrite(S2, HIGH);
  delay(waitTime);
  digitalWrite(S3, HIGH);
  delay(waitTime);
  digitalWrite(S4, HIGH);
  delay(waitTime);
  digitalWrite(S1, LOW);
  delay(waitTime);
  digitalWrite(S2, LOW);
  delay(waitTime);
  digitalWrite(S3, LOW);
  delay(waitTime);
  digitalWrite(S4, LOW);
  delay(waitTime);
  return;
}

void ChangeStateUp(){
  digitalWrite(States[currentState], LOW);
  currentState++;
  if (currentState >= numStates){
    currentState = 0;
  }
  digitalWrite(States[currentState], HIGH);
  //Serial.print("State Changed. Current State: "); Serial.println(currentState);
  delay(100);
  return;
}

void ChangeStateDown(){
  digitalWrite(States[currentState], LOW);
  currentState--;
  if (currentState <= -1){
    currentState = numStates - 1;
  }
  digitalWrite(States[currentState], HIGH);
  //Serial.print("State Changed. Current State: "); Serial.println(currentState);
  delay(100);
  return;
}

void toggleLEDs() {
  if(LEDsOn == true) {
      //set all state LEDs to off
    for (int i = 0; i < numStates; i++){
    pinMode(States[i], OUTPUT);
    digitalWrite(States[i], LOW);
    }
    LEDsOn = false;
    return;
  } else if (LEDsOn == false) {
      //set all state LEDs to on
    for (int i = 0; i < numStates; i++){
    pinMode(States[i], OUTPUT);
    digitalWrite(States[i], HIGH);
    }
    LEDsOn = true;
    return;
  }
}

void Layout1(char button){
  switch(button){
    case 1://
      Serial.println(F("key 1 pressed, opening task manager"));
      Keyboard.press(KEY_LEFT_CTRL);
      //delay(50);
      Keyboard.press(KEY_LEFT_SHIFT);
      //delay(50);
      Keyboard.press(KEY_ESC);
      //delay(50);
      Keyboard.releaseAll();
      break;
    case 2://
    Serial.println(F("key 2 pressed, opening powershell")); //print whats going on to serial monitor
      //opens windows run dialog then types into the box and presses enter
      Keyboard.press(KEY_LEFT_GUI); //hold left windows key
      delay(25);
      Keyboard.press(114); //hold r
      Keyboard.releaseAll(); //release all keys
      delay(200);
      Keyboard.println("powershell"); //type powershell and press enter
      Keyboard.write(224); //press enter
      break;
    case 3://
      Serial.println(F("key 3 pressed, opening notepad"));
      Keyboard.press(KEY_LEFT_GUI); //hold left windows key
      delay(25);
      Keyboard.press(114); //hold r
      Keyboard.releaseAll(); //release all keys
      delay(200);
      Keyboard.println("notepad"); //type something and press enterotary1. change this to change what the key opens
      Keyboard.write(224); //press enter
      break;
    case 4://
      Serial.println(F("key 4 pressed, opening spotify"));
      Keyboard.press(KEY_LEFT_GUI); //hold left windows key
      delay(25);
      Keyboard.press(114); //hold r
      Keyboard.releaseAll(); //release all keys
      delay(200);
      Keyboard.println("spotify"); //type something and press enterotary1. change this to change what the key opens
      Keyboard.write(224); //press enter
      break;
    case 5://
      Serial.println(F("key 5 pressed, opening file explorer"));
      Keyboard.press(KEY_LEFT_GUI); //hold left windows key
      delay(25);
      Keyboard.press(101); //hold e
      Keyboard.releaseAll(); //release all keys
      break;
    case 6://
    Serial.println(F("key 6 pressed, opening firefox"));
      Keyboard.press(KEY_LEFT_GUI); //hold left windows key
      delay(25);
      Keyboard.press(114); //hold r
      Keyboard.releaseAll(); //release all keys
      delay(200);
      Keyboard.println("firefox"); //type something and press enterotary1. change this to change what the key opens
      Keyboard.write(224); //press enter
      break;
    case 7://
      Serial.println(F("key 7 pressed, minimizing all windows"));
      Keyboard.press(KEY_LEFT_GUI); //hold left windows key
      delay(25);
      Keyboard.press(100); //hold d
      delay(25);
      Keyboard.releaseAll(); //release all keys
      break;
    case 8://
    Serial.println(F("key 8 pressed, PANIK BUTON (minimize all windows pause all media then open firefox and play a youtube video)"));
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
      break;
    case 9://
      Serial.println(F("key 9 pressed, pausing all media"));
      Consumer.write(0xCD); //pause
      break;
    case 10://
      Serial.println(F("key 10 pressed, volume up"));
      Consumer.write(0xE9); //volume up
      break;
    case 11://
      Serial.println(F("key 11 pressed, volume down"));
      Consumer.write(0xEA); //volume down
      break;
    case 12://
      Serial.println(F("key 12 pressed, muting audio"));
      Consumer.write(0xE2); //mute
      break;
    case 13://
      Serial.println(F("key 13 pressed, skipping song"));
      Consumer.write(0x5B); //next media
      break;
    case 14://
      Serial.println(F("key 14 pressed, bass down"));
      Consumer.write(0x0153);
      break;
    case 15://
      Serial.println(F("key 15 pressed, bass up"));
      Consumer.write(0x0152);
      break;
    case 16://
      Serial.println(F("key 16 pressed, opening calculator"));
      Consumer.write(0x0192);
      break;
  };
}

void Layout2(char button){
  switch(button){
    case 1://
      Serial.println(F("Layout 2, key 1 pressed"));
      break;
    case 2://
      Serial.println(F("Layout 2, key 2 pressed"));
      break;
    case 3://
      Serial.println(F("Layout 2, key 3 pressed"));
      break;
    case 4://
      Serial.println(F("Layout 2, key 4 pressed"));
      break;
    case 5://
      Serial.println(F("Layout 2, key 5 pressed"));
      break;
    case 6://Return
      Serial.println(F("Layout 2, key 6 pressed"));
      break;
    case 7://Escape
      Serial.println(F("Layout 2, key 7 pressed"));
      break;
    case 8://
      Serial.println(F("Layout 2, key 8 pressed"));
      break;
    case 9://
      Serial.println(F("Layout 2, key 9 pressed"));
      break;
    case 10://
      Serial.println(F("Layout 2, key 10 pressed"));
      break;
    case 11://
      Serial.println(F("Layout 2, key 11 pressed"));
      break;
    case 12://
      Serial.println(F("Layout 2, key 12 pressed"));
      break;
    case 13://
      Serial.println(F("Layout 2, key 13 pressed"));
      break;
    case 14://
      Serial.println(F("Layout 2, key 14 pressed"));
      break;
    case 15://
      Serial.println(F("Layout 2, key 15 pressed"));
      break;
    case 16://
      Serial.println(F("Layout 2, key 16 pressed"));
      break;
  };
}

void Layout3(char button){
  switch(button){
    case 1://
      Serial.println(F("Layout 3, key 1 pressed"));
      break;
    case 2://
      Serial.println(F("Layout 3, key 2 pressed"));
      break;
    case 3://
      Serial.println(F("Layout 3, key 3 pressed"));
      break;
    case 4://
      Serial.println(F("Layout 3, key 4 pressed"));
      break;
    case 5://
      Serial.println(F("Layout 3, key 5 pressed"));
      break;
    case 6://Return
      Serial.println(F("Layout 3, key 6 pressed"));
      break;
    case 7://Escape
      Serial.println(F("Layout 3, key 7 pressed"));
      break;
    case 8://
      Serial.println(F("Layout 3, key 8 pressed"));
      break;
    case 9://
      Serial.println(F("Layout 3, key 9 pressed"));
      break;
    case 10://
      Serial.println(F("Layout 3, key 10 pressed"));
      break;
    case 11://
      Serial.println(F("Layout 3, key 11 pressed"));
      break;
    case 12://
      Serial.println(F("Layout 3, key 12 pressed"));
      break;
    case 13://
      Serial.println(F("Layout 3, key 13 pressed"));
      break;
    case 14://
      Serial.println(F("Layout 3, key 14 pressed"));
      break;
    case 15://
      Serial.println(F("Layout 3, key 15 pressed"));
      break;
    case 16://
      Serial.println(F("Layout 3, key 16 pressed"));
      break;
  };
}

void Layout4(char button){
  switch(button){
    case 1://
      Serial.println(F("Layout 4, key 1 pressed"));
      break;
    case 2://
      Serial.println(F("Layout 4, key 2 pressed"));
      break;
    case 3://
      Serial.println(F("Layout 4, key 3 pressed"));
      break;
    case 4://
      Serial.println(F("Layout 4, key 4 pressed"));
      break;
    case 5://
      Serial.println(F("Layout 4, key 5 pressed"));
      break;
    case 6://Return
      Serial.println(F("Layout 4, key 6 pressed"));
      break;
    case 7://Escape
      Serial.println(F("Layout 4, key 7 pressed"));
      break;
    case 8://
      Serial.println(F("Layout 4, key 8 pressed"));
      break;
    case 9://
      Serial.println(F("Layout 4, key 9 pressed"));
      break;
    case 10://
      Serial.println(F("Layout 4, key 10 pressed"));
      break;
    case 11://
      Serial.println(F("Layout 4, key 11 pressed"));
      break;
    case 12://
      Serial.println(F("Layout 4, key 12 pressed"));
      break;
    case 13://
      Serial.println(F("Layout 4, key 13 pressed"));
      break;
    case 14://
      Serial.println(F("Layout 4, key 14 pressed"));
      break;
    case 15://
      Serial.println(F("Layout 4, key 15 pressed"));
      break;
    case 16://
      Serial.println(F("Layout 4, key 16 pressed"));
      break;
  };
}

void loop() {
  rotary1.loop();
  rotary2.loop();
  button1.loop();
  button2.loop();
  char key = keypad.getKey();
  if(key) {
    switch(key){
      case '*':
        // ChangeState();
        break;
      case '-':
        Keyboard.press(KEY_RIGHT_CTRL);
        Keyboard.press('s');
        delay(10);
        Keyboard.releaseAll();
        break;
      default:
        switch(currentState){
          case 0:
            Layout1(key);
            break;
          case 1:
            Layout2(key);
            break;
          case 2:
            Layout3(key);
            break;
          case 3: 
            Layout4(key);
            break;
        }
    }
  }
}

//rotary1 handlers
void rotate1(Rotary) {

}

void rotateLeft1(Rotary) {
  Serial.println(F("Rotary1 Rotated left, volume down"));
  Consumer.write(0xEA); //volume down
}

void rotateRight1(Rotary) {
  Serial.println(F("Rotary1 Rotated right, volume up"));
  Consumer.write(0xE9); //volume up
}

void clickButton1(Button2) {
  Serial.println(F("Rotary1 Button1 pressed, mute toggled"));
  Consumer.write(0xE2); //mute
}

void holdButton1(Button2) {

}

//rotary 2 handlers
void rotate2(Rotary) {

}

void rotateLeft2(Rotary) {
  Serial.print(F("Rotary2 Rotated left, State down. Current State: ")); Serial.println(currentState);
  ChangeStateDown();
}

void rotateRight2(Rotary) {
  Serial.print(F("Rotary2 Rotated right, State up. Current State: ")); Serial.println(currentState);
  ChangeStateUp();
}

void clickButton2(Button2) {
  Serial.println(F("Rotary2 Button2 pressed, LEDs toggled"));
  toggleLEDs();
}

void holdButton2(Button2) {

}
