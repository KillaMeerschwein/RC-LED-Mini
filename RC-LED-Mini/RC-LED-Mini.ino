/*
 * 
 * 
 *        RC-LED Mini
 *        Version 0.94
 *        
 *        0.94 + State Option
 *        0.93 + RC Channel Option
 * 
 * 
 */



#include <EEPROM.h>
#include <Servo.h>
#include <string.h>
#include "config.h"

/* ---------------------- SETUP --------------------*/

void setup() {
  Serial.begin(19200);  
  Serial.flush();
  pinMode(WS2803PinCLK, OUTPUT);
  pinMode(WS2803PinDATA, OUTPUT);
  digitalWrite(WS2803PinCLK, HIGH);
  digitalWrite(WS2803PinDATA, LOW);
  
  lRC_CHANNEL_1_TIMER_START = 0; 
  iRC_CHANNEL_2_COUNT = 0;
  attachInterrupt(digitalPinToInterrupt(RC_CHANNEL_1_PIN), __ISR_RC_CHANNEL_1, CHANGE);
  lRC_CHANNEL_2_TIMER_START = 0; 
  iRC_CHANNEL_2_COUNT = 0;
  attachInterrupt(digitalPinToInterrupt(RC_CHANNEL_2_PIN), __ISR_RC_CHANNEL_2, CHANGE);
  loadConfig();
  SetLEDState = PosAlwaysOn;

}
                     
/* ------------------------ MAIN LOOP ------------------ */

void loop () {

      delay(50);
      RCChannelCheck();
     // _Debug();
      
      if (OperationMode == Normal) {
         cyclicRunMode(50);
      }
      
     /* if (OperationMode == Settings) {
         SettingMenu();
      }*/
      
      if (stringComplete) {
         SerialParser();
      }
      setLEDs(SetLEDArray, SetLEDState);
}

/* ------------------------ LEDs ---------------------------------*/

void setLEDs(byte setLEDArray[18], byte SwitchState) {
   delayMicroseconds(500);            // Verzögerung zum Beginn einer neuen Übertragung
   for(byte c=0; c<= 17;c++) {
          for(int digit=7;digit >=0;digit--) {
              if(setLEDArray[c] & (1 << digit) && bitRead(LEDSettings.LEDModeStates[c],SwitchState)) {
                  digitalWrite(WS2803PinDATA, HIGH);
              } 
              else {
                  digitalWrite(WS2803PinDATA,LOW);
              }
              digitalWrite(WS2803PinCLK, HIGH);
              digitalWrite(WS2803PinCLK, LOW);
          }
   }
}

/* ------------------------------- ISR -------------------------*/

void __ISR_RC_CHANNEL_1() 
{
    lRC_CHANNEL_1_INTERRUPT_TIME = micros(); 
    if(digitalRead(RC_CHANNEL_1_PIN) == HIGH) 
    { 
        lRC_CHANNEL_1_TIMER_START = micros();
    } 
    else
    { 
        if(lRC_CHANNEL_1_TIMER_START != 0)
        { 
            iRC_CHANNEL_1_PULSE = ((volatile int)micros() - lRC_CHANNEL_1_TIMER_START);
            lRC_CHANNEL_1_TIMER_START = 0;
        }
    } 
} 

void __ISR_RC_CHANNEL_2() 
{
    lRC_CHANNEL_2_INTERRUPT_TIME = micros(); 
    if(digitalRead(RC_CHANNEL_2_PIN) == HIGH) 
    { 
        lRC_CHANNEL_2_TIMER_START = micros();
    }  
    else
    { 
        if(lRC_CHANNEL_2_TIMER_START != 0)
        { 
            iRC_CHANNEL_2_PULSE = ((volatile int)micros() - lRC_CHANNEL_2_TIMER_START);
            lRC_CHANNEL_2_TIMER_START = 0;
        }
    } 
} 

/* ------------------------------- DEBUG -------------------------------- */
void _Debug() 
{
/*
  Serial.println("Serial Test");

  if (OperationMode == Normal) {

    // ---------- Kanäle Ausgeben -----------------
    Serial.print("Ch1:     ");
    Serial.print(iRC_CHANNEL_1_PULSE);
    Serial.print("    Ch2:     ");
    Serial.print(iRC_CHANNEL_2_PULSE);
    Serial.print("    Main:     ");   
    Serial.println( RC_CHANNEL_PULSE );

     // ---------- Zähler Ausgeben -----------------
    Serial.print("Count Ch1:   ");
    Serial.print(iRC_CHANNEL_1_COUNT);
    Serial.print("    Counts Ch2:    ");
    Serial.println(iRC_CHANNEL_2_COUNT);
    Serial.print("    Flashflag ");
    Serial.println(FlashFlag);
   
    // ---------- Zustand der Kanäle Ausgeben -----------------
    Serial.print("Ch1 Low:  ");
    Serial.print(bitRead(RCChannel1, Low));
    Serial.print("    Ch1 Mid:  ");
    Serial.print(bitRead(RCChannel1, Mid));
    Serial.print("    Ch1 High:  ");
    Serial.println(bitRead(RCChannel1,High));

    Serial.print("Ch2 Low:  ");
    Serial.print(bitRead(RCChannel2, Low));
    Serial.print("    Ch2 Mid:  ");
    Serial.print(bitRead(RCChannel2, Mid));
    Serial.print("    Ch2 High:  ");
    Serial.println(bitRead(RCChannel2,High));




  // ---------- Zustandswechsel der Kanäle Ausgeben -----------------

    Serial.print("Ch1 Mid->Low:");
    Serial.print(bitRead(RCChannel1, MidToLow));
    Serial.print("    Ch1 Low->Mid:  ");
    Serial.print(bitRead(RCChannel1, LowToMid));
    Serial.print("    Ch1 High->Mid:  ");
    Serial.print(bitRead(RCChannel1, HighToMid));
    Serial.print("    Ch1 Mid->High:  ");
    Serial.println(bitRead(RCChannel1, MidToHigh));

    Serial.print("Ch2 Mid->Low:");
    Serial.print(bitRead(RCChannel2, MidToLow));
    Serial.print("    Ch2 Low->Mid:  ");
    Serial.print(bitRead(RCChannel2, LowToMid));
    Serial.print("    Ch2 High->Mid:  ");
    Serial.print(bitRead(RCChannel2, HighToMid));
    Serial.print("    Ch2 Mid->High:  ");
    Serial.println(bitRead(RCChannel2, MidToHigh));

    // ---------- Tastendruck Ausgeben -----------------
    Serial.print("Tastendruckdauer:  ");
    Serial.print(RCChannelPressedTime);
    Serial.print("        LongPress Ch1:  ");
    Serial.print(bitRead(RCChannel1, LongPress));
    Serial.print("        LongPress Ch2:  ");
    Serial.println(bitRead(RCChannel2, LongPress));
  }

  if (OperationMode == Settings) {
   
    for(byte c=0; c<= 17;c++) {
      Serial.print(SettingLEDArray[c]);
      Serial.print("      ");
      Serial.println(LEDSettings.MainBrightness[c]);
    }
   
    
  } 
 */   
}

