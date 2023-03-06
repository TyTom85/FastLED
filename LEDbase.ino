/*
 * This is digitally-addressable WS2811 LED pixel string lights. 
 * 
 * This controls LEDs with a transition between two colors. Transition is initiated by button push but can be 
 * triggered by output of other props if necessary.
 * 
 * Developed by: Tyler Dahl | Escape Room San Antonio USA
 * Date:03-03-2023
 * Email: tyler@escaperoomsa.com
 * No rights retained for this project as it is a hack job of many others code. 
 * 
 * If you improve this code please let me know.
 * 
 * NOTE If you plan to integrate ethernet shields, pins 11,12,13 are used to communicate with the Network sheild and can not be used.
 */

/////////////////////////////////           DECLARE VARIABLES   /////////////////////////////

#include <FastLED.h>
#define UPDATES_PER_SECOND 100                          // set variable for how many updates occur during second
#define LED_PIN     31                                  // set the pin that will talk to the LEDs
#define NUM_LEDS    50                                  // set how many LEDs are connected together
#define BRIGHTNESS  60                                  // set default brightness for LEDs  
#define LED_TYPE    WS2811                              // tell arduino what type of LED processor is used
CRGB leds[NUM_LEDS];                                    // establish LEDs at length per variable defined as NUM_LEDS
const int Red_Btn = 11;                                 // change to prop inputs if calling LED effect from another prop
const int Blue_Btn = 12;                                // change to prop inputs if calling LED effect from another prop
bool Red_Btn_Status;                                    // change to prop inputs if calling LED effect from another prop
bool Blue_Btn_Status;                                   // change to prop inputs if calling LED effect from another prop
long lastDebounceTime = 0;                              // the last time the output pin was toggled
long debounceDelay = 100;                               // the debounce time; increase if the output flickers
CRGBPalette16 currentPalette( CRGB::Black);             // set palette so LEDs are off
CRGBPalette16 targetPalette(CRGB::Black);               // set palette so LEDs are off

void setup() {
  pinMode (Red_Btn, INPUT);                             // change to prop inputs if calling LED effect from another prop
  pinMode (Blue_Btn, INPUT);                            // change to prop inputs if calling LED effect from another prop
  Red_Btn_Status = false;                               // change to prop inputs if calling LED effect from another prop
  Blue_Btn_Status = false;                              // change to prop inputs if calling LED effect from another prop
  delay(1000 );                                         // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS);    // establish LED string of lights
  FastLED.setBrightness(  BRIGHTNESS );                  // set brightness per variable definition above 
  FastLED.setMaxPowerInVoltsAndMilliamps(5,1000);
  set_max_power_indicator_LED(13);
}

void loop()
{
  ChangeColor_by_Button();                                                      // function to change target palette per input
  uint8_t maxChanges =   48;                                                    //  - meaningful values are 1-48.  1=veeeeeeeery slow, 48=quickest
  nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);       //function changes the active palette slowly from current to evetually target

  static uint8_t startIndex = 0;
  startIndex = startIndex + 3;//1; /* motion speed */
  FillLEDsFromPaletteColors(startIndex);                                        // this is the function to actually display the color on the LEDs after palette change occurs
  FastLED.delay(10 / UPDATES_PER_SECOND);                                       // this controls how fast the transition is from one palette to the next
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness =150; //255
  CRGB color=ColorFromPalette( currentPalette, 1, brightness, LINEARBLEND);
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
   // colorIndex += 3;
  }
}

void ChangeColor_by_Button()                                                    // change to prop inputs if calling LED effect from another prop
{
    if ( (millis() - lastDebounceTime) > debounceDelay)
    { 
      if (Red_Btn_Status == false)
        {
          if (digitalRead(Red_Btn) == HIGH) 
            {
              Red_Btn_Status = true;
              Blue_Btn_Status = false;
              targetPalette =( CRGBPalette16(CRGB::Red));
              lastDebounceTime = millis();                      
            }
         }
      if (Blue_Btn_Status == false)
        {       
          if (digitalRead(Blue_Btn) == HIGH) 
            {
              Blue_Btn_Status = true;
              Red_Btn_Status = false;
              targetPalette = ( CRGBPalette16(CRGB::Blue));
              lastDebounceTime = millis();             
            }
        }
    }
}
