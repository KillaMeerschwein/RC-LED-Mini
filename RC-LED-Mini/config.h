#ifndef CONFIG_H
#define CONFIG_H

/* ---------------------- Pin Belegung ------------------ */

#define WS2803PinCLK                          A5
#define WS2803PinDATA                         A4

#define RC_CHANNEL_1_PIN                      2
#define RC_CHANNEL_2_PIN                      3

#define RCChannelPressTime                  1000    // in ms
#define RCChannelToLongPressTime            5000

#define MaxLEDAnzahl                         18

volatile unsigned long lRC_CHANNEL_1_TIMER_START;
volatile int lRC_CHANNEL_1_INTERRUPT_TIME;
volatile int iRC_CHANNEL_1_PULSE;
volatile int iRC_CHANNEL_1_COUNT;

volatile unsigned long lRC_CHANNEL_2_TIMER_START;
volatile int lRC_CHANNEL_2_INTERRUPT_TIME;
volatile int iRC_CHANNEL_2_PULSE;
volatile int iRC_CHANNEL_2_COUNT;

int RC_CHANNEL_PULSE;


/* ---------- Serial ------------ */
char inputString[150];// = "";

char* pInputString = &inputString[0];
boolean stringComplete = false;
char sDelimiter[] = ",";
const int iNumberOfToken = 20;
char* pTokenArray[iNumberOfToken];
char** pTok = &pTokenArray[0];

boolean ResetSync = 0;

byte SetLEDArray[MaxLEDAnzahl] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
byte SetLEDState = 7;
byte SettingLEDArray[MaxLEDAnzahl];

/* ---------------------- RC Channel 1 and 2 ---------------------- */
/* -------------------- StateMaschine RC Channels---------------------*/
//#define Low             0
//#define Mid             1 
//#define High            2
//#define MidToHigh       3 
//#define HighToMid       4
//#define MidToLow        5
//#define LowToMid        6
//#define LongPress       7

//byte RCChannel1, RCChannel2;                // aktueller Zustand der Kanäle
//byte previousRCChannel1,previousRCChannel2; // vorheriger Zustand der Kanäle
//long RCChannelPressedTime = 0;              //  


/* ------------------ StateMachine Modes -----------------*/

#define Normal              0
#define Settings            1
#define SoftwareProgamming  2
#define Exit                7

byte OperationMode    =     0; 

// Versionsnummer der Speicherinhalts

#define CONFIG_VERSION "V94" 
#define CONFIG_START 1

typedef struct {
  char Name[8];                       // Name des Modes
  byte onBrightness;                  // Helligkeit in % für den Zustand "AN"
  byte offBrightness;                 // Helligkeit in % für den Zustand "AUS"
  byte onTime;                        // Zeit des Zustands "AN"
  byte offTime;                       // Zeit des Zustands "AN"
  byte Cycles;                        // Anzahl an Wiederholungen
  byte PauseBeforeSequence;
  byte PauseAfterSequence;
  byte PauseBetweenSequences;
} LightMode_t;

typedef struct  {
  char ModuleName[8];                        // Name des Moduls
  byte RCChannel;
  byte SwitchType;                                                                                                // UNUSED
  int SwitchPositions[7];                                                                                        // UNUSED
  byte MainBrightness[MaxLEDAnzahl];
  byte LEDModeStates[MaxLEDAnzahl];
  LightMode_t LightModes[MaxLEDAnzahl];
  char version_of_program[4]; 
} StoreStruct;

/* LEDNumber          - Anzahl der benutzen LEDs 
 * SwitchType         - Anzahl der Schaltpositionen
 * SwitchPositions    - Umschaltpositionen zwischen den Zuständen der Kanäle
 *  
 * MainBrightness      - Helligkeitsstufen der einzelnen LEDs
 * LEDModeStates      - Gibt an, ob die LEDs in dem aktuellen Schaltzustand aktiv oder inaktiv sind
 * LEDBlinkStates     - In welchem Zustand soll die LED blinken
 * LEDBlinkTime       - mit welcher Geschwindigkeit soll die LED blinken
 *  
 */


/* --------------------------- LEDSettigs - Vorbelegung des EEPROMS  --------------------- */

StoreStruct LEDSettings =             {   
              "Module",
                0,                                                                                           //  RCChannel
                2,                                                                                           //  SwitchType    0 = 2 Pos, 1 = 3 Pos, 2 = 6 Pos       
                   
              //      0         1         2          3          4           5            6     
              //  Min |         Low                  |                 High              |   Max                //  2 Position Switch                   Siehe *(2)     
              //  Min |         Low       |         Mid         |           High         |   Max                //  3 Position Switch                   Siehe *(3)
              //  Min |  Low 3  |  Low 2  |   Low 1  |  High 1  |   High 2  |   High 3   |   Max                //  6 Positionen Switch     // UNUSED   Siehe *(4)
              {     1000,     1100,     1300,      1500,      1700,       1900,        2000 },                  //  SwitchPositions         
              
              //                                *                       *                       *                       *                        
              //        1           2           3           4           5           6           7           8           9          10          11          12          13          14          15          16          17          18       // LED (Anschluss -= 1)
              {       100,        100,        100,        100,        100,        100,        100,        100,        100,        100,        100,        100,        100,        100,        100,        100,        100,        100    },    // MainBrightness
             
              {0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111, 0b10000111    },    // LEDModeStates   Siehe *(1)

              {
                {"LED 1", 100, 0, 0, 0, 0, 0, 0, 0 },
                {"LED 2", 100, 0, 0, 0, 0, 0, 0, 0 },
                {"LED 3", 100, 0, 0, 0, 0, 0, 0, 0 },
                {"LED 4", 100, 0, 0, 0, 0, 0, 0, 0 },
                {"LED 5", 100, 0, 0, 0, 0, 0, 0, 0 },
                {"LED 6", 100, 0, 0, 0, 0, 0, 0, 0 },
                {"LED 7", 100, 0, 0, 0, 0, 0, 0, 0 },
                {"LED 8", 100, 0, 0, 0, 0, 0, 0, 0 },
                {"LED 9", 100, 0, 0, 0, 0, 0, 0, 0 },
                {"NULL", 0, 0, 0, 0, 0, 0, 0, 0 },
                {"NULL", 0, 0, 0, 0, 0, 0, 0, 0 },
                {"NULL", 0, 0, 0, 0, 0, 0, 0, 0 },
                {"NULL", 0, 0, 0, 0, 0, 0, 0, 0 },
                {"NULL", 0, 0, 0, 0, 0, 0, 0, 0 },
                {"NULL", 0, 0, 0, 0, 0, 0, 0, 0 },
                {"NULL", 0, 0, 0, 0, 0, 0, 0, 0 },
                {"NULL", 0, 0, 0, 0, 0, 0, 0, 0 },
                {"NULL", 0, 0, 0, 0, 0, 0, 0, 0 },
              },
              
              CONFIG_VERSION
}; 

byte LightOutputPercent[MaxLEDAnzahl];

enum {MODE_SLEEP, MODE_INIT, MODE_PAUSE_BEFORE, MODE_SWITCH_ON, MODE_KEEP_ON, MODE_SWITCH_OFF, MODE_KEEP_OFF, MODE_PAUSE_AFTER, CATCH_TIME, MODE_PAUSE_BETWEEN, MODE_CYCLE_REPETITION, MODE_END};

/*  ------------ *(1) Bit Postion im Array LEDModeStates --------------- */

#define PosMid          0
#define PosLow          1
#define PosHigh         2
#define PosLow2         3                
#define PosHigh2        4                 
#define PosLow3         5                 
#define PosHigh3        6                
#define PosAlwaysOn     7

/*  ------------ *(2) Wertepostion im Array SwitchPositions für einen 2 Postions Switch --------------- */

#define Limit2SWMin       0

#define Limit2SWMid       3

#define Limit2SWMax       6

/*  ------------ *(3) Wertepostion im Array SwitchPositions für einen 3 Postions Switch --------------- */

#define Limit3SWMin       0

#define Limit3SWLow       2
#define Limit3SWHigh      4

#define Limit3SWMax       6

/*  ------------ *(4) Wertepostion im Array SwitchPositions für einen 6 Postions Switch --------------- */

#define Limit6SWMin       0

#define Limit6SWLow2      1
#define Limit6SWLow1      2
#define Limit6SWMid       3
#define Limit6SWHigh1     4
#define Limit6SWHigh2     5

#define Limit6SWMax       6

#endif
