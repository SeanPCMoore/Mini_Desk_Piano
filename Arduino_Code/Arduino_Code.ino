#include <FastLED.h>

#define LED_Count 144

CRGB myLeds[LED_Count];

// Defining button pins
const int on_off_button = 9;  // the number of the pushbutton pin
const int brightness_button = 7;    // the number of the LED pin
const int colour_button = 3;  

// The brightness of the LEDs
int brightness = 100;
bool on_off_toggle = true;

// Declare button states
int one_off_State = 0;
int brightness_State = 0;
int colour_State = 0;

unsigned long debounce_delay = 1000;    // the debounce time; increase if the output flickers

int key_lights[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t hue = 0;
uint8_t thisHue = beat8(10,255);  

// This is the colour the piano is set to, default is rainbow
int piano_display = 0;

void setup() {
  // Init serial
  Serial.begin(115200);
  Serial.setTimeout(50);
  
  // Delay till LEDs are turned on
  delay(3000);

  // Initialise LEDs
  FastLED.addLeds<WS2812B, 5, GRB>(myLeds, LED_Count);

  // Set the Initial Brightness
  FastLED.setBrightness(40);

  // initialize the pushbutton pins as an input:
  pinMode(on_off_button, INPUT);
  pinMode(brightness_button, INPUT);
  pinMode(colour_button, INPUT);

  // Set Initial Colour
  fill_rainbow(myLeds, LED_Count, hue, 7);
  FastLED.show();
}

void loop() {
  // Detect Button Presses
  // read the state of the pushbutton value:
  one_off_State = digitalRead(on_off_button);
  brightness_State = digitalRead(brightness_button);
  colour_State = digitalRead(colour_button);

  if (one_off_State == HIGH) {
    if (on_off_toggle == true){
      on_off_toggle = false;
      fill_solid(myLeds, 144, CRGB::Black);
      piano_display = 10;
    } else if (on_off_toggle == false){
      on_off_toggle = true;
      fill_rainbow(myLeds, LED_Count, hue, 7);
      piano_display = 0;
    }
    delay(debounce_delay);
  } 

  if (brightness_State == HIGH) {
    if (brightness < 100){
      brightness = brightness + 20;
    } else {
      brightness = 20;
    }
    FastLED.setBrightness(brightness);
    delay(debounce_delay);
  } 

  if (colour_State == HIGH) {
    // turn LED on:
    if (piano_display < 4){
      piano_display = piano_display+1;
    } else {
      piano_display = 0;
    }
    delay(debounce_delay);

    switch (piano_display) {
    case 0: // Default Rainbow
      Serial.println("Rainbow 1");
      fill_rainbow(myLeds, LED_Count, hue, 7);
      break;
    case 1: // Fill like actual Piano
      Serial.println("Copy Piano");
      fill_solid(myLeds, 8, CRGB::White); // Key 1
      fill_solid(myLeds + 8, 3, CRGB::Red ); // Key 2
      fill_solid(myLeds + 11, 8, CRGB::White); // Key 3
      fill_solid(myLeds + 19, 3, CRGB::Red ); // Key 4
      fill_solid(myLeds + 22, 8, CRGB::White); // Key 5
      fill_solid(myLeds + 30, 3, CRGB::Red); // Key 6
      fill_solid(myLeds + 33, 8, CRGB::White ); // Key 7
      fill_solid(myLeds + 41, 8, CRGB::White); // Key 8
      fill_solid(myLeds + 49, 3, CRGB::Red ); // Key 9
      fill_solid(myLeds + 52, 8, CRGB::White); // Key 10
      fill_solid(myLeds + 60, 3, CRGB::Red ); // Key 11
      fill_solid(myLeds + 63, 8, CRGB::White); // Key 12
      fill_solid(myLeds + 71, 8, CRGB::White); // Key 13
      fill_solid(myLeds + 79, 3, CRGB::Red ); // Key 14
      fill_solid(myLeds + 82, 8, CRGB::White); // Key 15
      fill_solid(myLeds + 90, 3, CRGB::Red ); // Key 16
      fill_solid(myLeds + 93, 8, CRGB::White); // Key 17
      fill_solid(myLeds + 101, 3, CRGB::Red); // Key 18
      fill_solid(myLeds + 104, 8, CRGB::White ); // Key 19
      fill_solid(myLeds + 112, 8, CRGB::White); // Key 20
      fill_solid(myLeds + 120, 3, CRGB::Red ); // Key 21
      fill_solid(myLeds + 123, 8, CRGB::White); // Key 22
      fill_solid(myLeds + 131, 3, CRGB::Red ); // Key 23
      fill_solid(myLeds + 134, 8, CRGB::White); // Key 24
      break;
    case 2:
      Serial.println("Rainbow 3");
      fill_rainbow_circular(myLeds, LED_Count, 7);
      break;
    case 3:
      Serial.println("All white");
      fill_solid(myLeds, 144, CRGB::White);
      break;
    case 4: // Wave Rainbow 
      fill_rainbow_circular(myLeds, 72, 7);
      fill_rainbow_circular(myLeds+72, 72, 7);
      break;
    case 5:
      Serial.println("Wave Rainbow");  
      fill_rainbow(myLeds, LED_Count, thisHue, 10); 
      break;
    case 10: // Piano Display Off
      fill_solid(myLeds, 144, CRGB::Black);
      piano_display = 10;
      break;
    default:
      // pass if default or out of range
      break;
    }
  }

  
  if (Serial.available() > 0){
    String string = Serial.readString();

    fill_rainbow_circular(myLeds, 72, 7);
    fill_rainbow_circular(myLeds+72, 72, 7);
    
    for (int i = 0;i<24;i++){
      switch (i) {
      case 0: // Default Rainbow
        if ((int)string[23] != 49){
          fill_solid(myLeds, 8, CRGB::White); // Key 1
        }
        break;
      case 1: // Default Rainbow
        if ((int)string[22] != 49){
          fill_solid(myLeds + 8, 3, CRGB::Red ); // Key 2
        }
        break;
      case 2: // Default Rainbow
        if ((int)string[21] != 49){
          fill_solid(myLeds + 11, 8, CRGB::White); // Key 3
        }
        break;
      case 3: // Default Rainbow
        if ((int)string[20] != 49){
          fill_solid(myLeds + 19, 3, CRGB::Red ); // Key 4
        }
        break;
      case 4: // Default Rainbow
        if ((int)string[19] != 49){
          fill_solid(myLeds + 22, 8, CRGB::White); // Key 5
        }
        break;
      case 5: // Default Rainbow
        if ((int)string[18] != 49){
          fill_solid(myLeds + 30, 3, CRGB::Red); // Key 6
        }
        break;
      case 6: // Default Rainbow
        if ((int)string[17] != 49){
          fill_solid(myLeds + 33, 8, CRGB::White ); // Key 7
        }
        break;
      case 7: // Default Rainbow
        if ((int)string[16] != 49){
          fill_solid(myLeds + 41, 8, CRGB::White); // Key 8
        }
        break;
      case 8: // Default Rainbow
        if ((int)string[15] != 49){
          fill_solid(myLeds + 49, 3, CRGB::Red ); // Key 9
        }
        break;
      case 9: // Default Rainbow
        if ((int)string[14] != 49){
          fill_solid(myLeds + 52, 8, CRGB::White); // Key 10
        }
        break;
      case 10: // Default Rainbow
        if ((int)string[13] != 49){
          fill_solid(myLeds + 60, 3, CRGB::Red ); // Key 11
        }
        break;
      case 11: // Default Rainbow
        if ((int)string[12] != 49){
          fill_solid(myLeds + 63, 8, CRGB::White); // Key 12
        }
        break;
      case 12: // Default Rainbow
        if ((int)string[11] != 49){
          fill_solid(myLeds + 71, 8, CRGB::White); // Key 13
        }
        break;
      case 13: // Default Rainbow
        if ((int)string[10] != 49){
          fill_solid(myLeds + 79, 3, CRGB::Red ); // Key 14
        }
        break;
      case 14: // Default Rainbow
        if ((int)string[9] != 49){
          fill_solid(myLeds + 82, 8, CRGB::White); // Key 15
        }
        break;
      case 15: // Default Rainbow
        if ((int)string[8] != 49){
          fill_solid(myLeds + 90, 3, CRGB::Red ); // Key 16
        }
        break;
      case 16: // Default Rainbow
        if ((int)string[7] != 49){
          fill_solid(myLeds + 93, 8, CRGB::White); // Key 17
        }
        break;
      case 17: // Default Rainbow
        if ((int)string[6] != 49){
          fill_solid(myLeds + 101, 3, CRGB::Red); // Key 18
        }
        break;
      case 18: // Default Rainbow
        if ((int)string[5] != 49){
          fill_solid(myLeds + 104, 8, CRGB::White ); // Key 19
        }
        break;
      case 19: // Default Rainbow
        if ((int)string[4] != 49){
          fill_solid(myLeds + 112, 8, CRGB::White); // Key 20
        }
        break;
      case 20: // Default Rainbow
        if ((int)string[3] != 49){
          fill_solid(myLeds + 120, 3, CRGB::Red ); // Key 21
        }
        break;
      case 21: // Default Rainbow
        if ((int)string[2] != 49){
          fill_solid(myLeds + 123, 8, CRGB::White); // Key 22
        }
        break;
      case 22: // Default Rainbow
        if ((int)string[1] != 49){
          fill_solid(myLeds + 131, 3, CRGB::Red ); // Key 23
        }
        break;
      case 23: // Default Rainbow
        if ((int)string[0] != 49){
          fill_solid(myLeds + 134, 8, CRGB::White); // Key 24
        }
        break;
      default:
        // pass if default or out of range
        break;
      }
    }
    
//    for (int i = 0;i<24;i++){
//      switch (i) {
//      case 0: // Default Rainbow
//        if ((int)string[23] == 49){
//          fill_solid(myLeds, 8, CRGB::Red); // Key 1
//          Serial.println("Case 1 - On");
//        } else {
//          Serial.println("Case 1 - Off");
//          fill_solid(myLeds, 8, CRGB::White); // Key 1
//        }
//        break;
//      case 1: // Default Rainbow
//        if ((int)string[22] == 49){
//          fill_solid(myLeds + 8, 3, CRGB(255,191,0)); // Key 2 - Amber
//        } else {
//          fill_solid(myLeds + 8, 3, CRGB::Red ); // Key 2
//        }
//        break;
//      case 2: // Default Rainbow
//        if ((int)string[21] == 49){
//          fill_solid(myLeds + 11, 8, CRGB(255,165,0)); // Key 3 - Orange
//        } else {
//          fill_solid(myLeds + 11, 8, CRGB::White); // Key 3
//        }
//        break;
//      case 3: // Default Rainbow
//        if ((int)string[20] == 49){
//          fill_solid(myLeds + 19, 3, CRGB(255,255,0) ); // Key 4 - Yellow
//        } else {
//          fill_solid(myLeds + 19, 3, CRGB::Red ); // Key 4
//        }
//        break;
//      case 4: // Default Rainbow
//        if ((int)string[19] == 49){
//          fill_solid(myLeds + 22, 8, CRGB(0,128,0)); // Key 5 - Green
//        } else {
//          fill_solid(myLeds + 22, 8, CRGB::White); // Key 5
//        }
//        break;
//      case 5: // Default Rainbow
//        if ((int)string[18] == 49){
//          fill_solid(myLeds + 30, 3, CRGB(0,168,107)); // Key 6 - Jade
//        } else {
//          fill_solid(myLeds + 30, 3, CRGB::Red); // Key 6
//        }
//        break;
//      case 6: // Default Rainbow
//        if ((int)string[17] == 49){
//          fill_solid(myLeds + 33, 8, CRGB(0,128,128) ); // Key 7 - Teal
//        } else {
//          fill_solid(myLeds + 33, 8, CRGB::White ); // Key 7
//        }
//        break;
//      case 7: // Default Rainbow
//        if ((int)string[16] == 49){
//          fill_solid(myLeds + 41, 8, CRGB(0,255,255)); // Key 8 - Cyan
//        } else {
//          fill_solid(myLeds + 41, 8, CRGB::White); // Key 8
//        }
//        break;
//      case 8: // Default Rainbow
//        if ((int)string[15] == 49){
//          fill_solid(myLeds + 49, 3, CRGB(0,0,255) ); // Key 9 - Blue
//        } else {
//          fill_solid(myLeds + 49, 3, CRGB::Red ); // Key 9
//        }
//        break;
//      case 9: // Default Rainbow
//        if ((int)string[14] == 49){
//          fill_solid(myLeds + 52, 8, CRGB(128,0,128)); // Key 10 - Purple
//        } else {
//          fill_solid(myLeds + 52, 8, CRGB::White); // Key 10
//        }
//        break;
//      case 10: // Default Rainbow
//        if ((int)string[13] == 49){
//          fill_solid(myLeds + 60, 3, CRGB(255,192,203) ); // Key 11 - Pink
//        } else {
//          fill_solid(myLeds + 60, 3, CRGB::Red ); // Key 11
//        }
//        break;
//      case 11: // Default Rainbow
//        if ((int)string[12] == 49){
//          fill_solid(myLeds + 63, 8, CRGB(255,0,255)); // Key 12 - Magenta
//        } else {
//          fill_solid(myLeds + 63, 8, CRGB::White); // Key 12
//        }
//        break;
//      case 12: // Default Rainbow
//        if ((int)string[11] == 49){
//          fill_solid(myLeds + 71, 8, CRGB::Red); // Key 13
//        } else {
//          fill_solid(myLeds + 71, 8, CRGB::White); // Key 13
//        }
//        break;
//      case 13: // Default Rainbow
//        if ((int)string[10] == 49){
//          fill_solid(myLeds + 79, 3, CRGB(255,191,0) ); // Key 14 - Amber
//        } else {
//          fill_solid(myLeds + 79, 3, CRGB::Red ); // Key 14
//        }
//        break;
//      case 14: // Default Rainbow
//        if ((int)string[9] == 49){
//          fill_solid(myLeds + 82, 8, CRGB::Orange); // Key 15
//        } else {
//          fill_solid(myLeds + 82, 8, CRGB::White); // Key 15
//        }
//        break;
//      case 15: // Default Rainbow
//        if ((int)string[8] == 49){
//          fill_solid(myLeds + 90, 3, CRGB::Yellow ); // Key 16
//        } else {
//          fill_solid(myLeds + 90, 3, CRGB::Red ); // Key 16
//        }
//        break;
//      case 16: // Default Rainbow
//        if ((int)string[7] == 49){
//          fill_solid(myLeds + 93, 8, CRGB::Green); // Key 17
//        } else {
//          fill_solid(myLeds + 93, 8, CRGB::White); // Key 17
//        }
//        break;
//      case 17: // Default Rainbow
//        if ((int)string[6] == 49){
//          fill_solid(myLeds + 101, 3, CRGB(0,168,107)); // Key 18 - Jade
//        } else {
//          fill_solid(myLeds + 101, 3, CRGB::Red); // Key 18
//        }
//        break;
//      case 18: // Default Rainbow
//        if ((int)string[5] == 49){
//          fill_solid(myLeds + 104, 8, CRGB::Teal ); // Key 19
//        } else {
//          fill_solid(myLeds + 104, 8, CRGB::White ); // Key 19
//        }
//        break;
//      case 19: // Default Rainbow
//        if ((int)string[4] == 49){
//          fill_solid(myLeds + 112, 8, CRGB::Cyan); // Key 20
//        } else {
//          fill_solid(myLeds + 112, 8, CRGB::White); // Key 20
//        }
//        break;
//      case 20: // Default Rainbow
//        if ((int)string[3] == 49){
//          fill_solid(myLeds + 120, 3, CRGB::Blue ); // Key 21
//        } else {
//          fill_solid(myLeds + 120, 3, CRGB::Red ); // Key 21
//        }
//        break;
//      case 21: // Default Rainbow
//        if ((int)string[2] == 49){
//          fill_solid(myLeds + 123, 8, CRGB::Purple); // Key 22
//        } else {
//          fill_solid(myLeds + 123, 8, CRGB::White); // Key 22
//        }
//        break;
//      case 22: // Default Rainbow
//        if ((int)string[1] == 49){
//          fill_solid(myLeds + 131, 3, CRGB::Pink ); // Key 23
//        } else {
//          fill_solid(myLeds + 131, 3, CRGB::Red ); // Key 23
//        }
//        break;
//      case 23: // Default Rainbow
//        if ((int)string[0] == 49){
//          fill_solid(myLeds + 134, 8, CRGB::Magenta); // Key 24
//        } else {
//          fill_solid(myLeds + 134, 8, CRGB::White); // Key 24
//        }
//        break;
//      default:
//        // pass if default or out of range
//        break;
//      }
//    }
   Serial.println(string);
  }
  
  //Serial.println(piano_display);
  FastLED.show();
}
