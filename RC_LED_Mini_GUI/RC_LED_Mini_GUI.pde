/*   

	AAAAA

	RC-LED-Mini GUI 
      Version 0.95 
      + Code Cleanup 1
      Alex B.  
     (KillaMeerschwein) 
	 
	 neues feature alles gut
	 erweiterung fertig.

Master Löschen!
	 PiffPaff
	 
	 feature A
	   und weiter
	   usw
	 
	ultra sinnvolle funktion
*/


import processing.serial.*;
import controlP5.*;

String Version = "Version 0.95";

int MaxLEDAnzahl = 18;
int MaxLEDStates = 6;
int MaxLEDBrightness = 3;
int LEDModes = 10;

// BBBBBBBB

ControlP5 cp5;

color yellow_ = color(200, 200, 20), green_ = color(30, 120, 30), red_ = color(120, 30, 30), blue_ = color(50, 50, 100),
       grey_ = color(30, 30, 30),black_ = color(0, 0, 0),orange_ =color(200,128,0), white_ =color(255);


public class Mode {
          public String Name;                            // Name des Modes
          public byte ONBrightness = 100;                // Helligkeit in % (kann auch über 100% liegen) für den Zustand "AN"
          public byte OFFBrightness = 0;                 // Helligkeit in % (kann auch über 100% liegen) für den Zustand "AUS"
          public byte ONTime = 0;                        // Zeit des Zustands "AN"
          public byte OFFTime = 0;                       // Zeit des Zustands "AN"
          public byte Cycles = 0;                        // Anzahl an Wiederholungen
          public byte PauseBeforeSequence = 0;           // Pausenzeit vor einem Zyklus
          public byte PauseAfterSequence = 0;            // Pausenzeit nach einem Zyklus
          public byte PauseBetweenSequences = 0;         // Pausenzeit zwischen einem Zyklus
          
    }

/* -------- SERIAL ----------------------------- */
//PrintWriter output;
//BufferedReader reader;

int GUI_BaudRate = 19200; // Default.
String SerialPort;
int init_com;
Serial g_serial;



int commListMax;
int tabHeight = 30;
int tabWidth = 80;
int slbHeight = 20;
int slbWidth = 60;
int ObjectSpace = 10;
int ObjectSize[] = {100,20};

int WindowX = 380 , WindowY = 400;

Tab tSetting, tBrightness, tModes, tStates;

ScrollableList slbComPorts,slbPosSwitch,slbLED,slbRCChannel,
               slbLEDStates,
               slbLEDModes[] = new ScrollableList[MaxLEDAnzahl];

Button   btnWriteSettings, btnReadSettings, btnReadLED, btnWriteLED, btnSync, btnEEPROM, btnExit,
         btnConnect, btnDisconnect;

Group  gSettings, gModuleSettings, gLEDSettings, gLEDStates;

Slider  sLEDBrightness,
        sBrightnessON, sBrightnessOFF,
        sCycles;
        
Textfield tfModuleName, tfLEDModeName;

Toggle tState[] = new Toggle[6];;

Numberbox nbONTime, nbOFFTime, nbCycles, nbPauseBeforeTime, nbPauseAfterTime, nbPauseBetweenTime;

int DefaultSliderValue = 100;

       
void setup() {

  surface.setSize(WindowX,WindowY);
  
  noStroke();
  cp5 = new ControlP5(this);
  
  //surface.setLocation(420, 10);


    /* ------- FRAMERATE, Version ---------------- */
  cp5               .addFrameRate()
                    .setInterval(10)
                    .setPosition(0,height - 10);
  
  cp5               .addTextlabel("Version")
                    .setText(Version)
                    .setPosition(width -65,height - 10)
                    ; 
                    
  /* -------- CALLBACKLISTENER toFront, close, changeLEDStates ----------- */
             
   CallbackListener toFront = new CallbackListener() {
      public void controlEvent(CallbackEvent theEvent) {
          theEvent.getController().bringToFront();
          ((ScrollableList)theEvent.getController()).open();
      }
    };
  
    CallbackListener close = new CallbackListener() {
      public void controlEvent(CallbackEvent theEvent) {
          ((ScrollableList)theEvent.getController()).close();
      }
    };
    
    CallbackListener changeLEDStates = new CallbackListener() {
      public void controlEvent(CallbackEvent theEvent) {
          
                 if (slbPosSwitch.getValue() == 0){
                  tState[0].show();
                  tState[1].show();
                  tState[2].hide();
                  tState[3].hide();
                  tState[4].hide();
                  tState[5].hide(); 
                }
                else if (slbPosSwitch.getValue() == 1){
                  tState[0].show();
                  tState[1].show();
                  tState[2].show();
                  tState[3].hide();
                  tState[4].hide();
                  tState[5].hide();
                }
                else if (slbPosSwitch.getValue() == 2){
                  tState[0].show();
                  tState[1].show();
                  tState[2].show();
                  tState[3].show();
                  tState[4].show();
                  tState[5].show();
                }
         }
    };
    
 
   /* -------- Globale Buttons: Exit, Save EEPROM, Sync----------- */ 
        
    
   btnExit       = cp5      .addButton      ("Exit")
                            .setPosition    (ObjectSpace + ObjectSpace, height - ObjectSize[1] - ObjectSpace - ObjectSpace )
                            .setSize        (ObjectSize[0], ObjectSize[1])
                            .setLabel       ("Exit")
                            .setColorActive (green_)
                            ;
                            
    btnEEPROM       = cp5   .addButton      ("SaveEEPROM")
                            .setPosition    (ObjectSpace + ObjectSpace, btnExit.getPosition()[1] - ObjectSize[1] - ObjectSpace )
                            .setSize        (ObjectSize[0], ObjectSize[1])
                            .setLabel       ("Save EPROM")
                            .setColorActive (green_)
                            ;                            
  
    btnSync         = cp5   .addButton         ("Sync")
                            .setPosition    (ObjectSpace + ObjectSpace, btnEEPROM.getPosition()[1] - ObjectSize[1] - ObjectSpace )
                            .setSize        (ObjectSize[0], ObjectSize[1])
                            .setLabel       ("Sync LEDs")
                            .setColorActive (green_)
                            ;   
                            
                            
  /* ------------------ SETTINGS ------------------------------ */
  
  
        gSettings = cp5         .addGroup                  ("gSettings")
                                .setPosition               (ObjectSpace ,
                                                            ObjectSpace + ObjectSpace)
                                .setWidth                  (ObjectSpace + ObjectSize[0] + ObjectSpace)
                                .setBackgroundColor        (color(255,80))
                                .setBackgroundHeight       (ObjectSpace + ObjectSize[1] + ObjectSpace + ObjectSize[1] + ObjectSpace)
                                .setLabel                  ("Communication")
                                .disableCollapse           ()
                                ;
       
                     
        slbComPorts = cp5  .addScrollableList  ("COM Port")
                           .setPosition        (ObjectSpace, 
                                                ObjectSpace)
                           .setWidth           (ObjectSize[0])
                           .setBarHeight       (slbHeight)
                           .setItemHeight      (slbHeight)
                           .setGroup           ("gSettings")
                           .close()
                           .onEnter            (toFront)
                           .onLeave            (close)
                           ;
          
                          for(int i=0;i<Serial.list().length;i++) {
                              String pn = Serial.list()[i];
                              if (pn.length() >0 )     slbComPorts.addItem(pn,i); 
                              commListMax = i;
                          }
                          
        btnConnect = cp5    .addButton      ("ConnectButton")
                            .setPosition    (ObjectSpace,  slbComPorts.getPosition()[1] + slbComPorts.getHeight() + ObjectSpace )
                            .setSize        (ObjectSize[0], ObjectSize[1])
                            .setLabel       ("Connect")
                            .setColorActive (green_)
                            .setGroup       ("gSettings")
                            ;
                            
        btnDisconnect = cp5 .addButton      ("DisconnectButton")
                            .setPosition    (btnConnect.getPosition()[0],  btnConnect.getPosition()[1] )
                            .setSize        (ObjectSize[0], ObjectSize[1])
                            .setLabel       ("Disconnect")
                            .setColorActive (red_)
                            .setGroup       ("gSettings")
                            .hide()
                            ;       
     
                      
      gModuleSettings = cp5     .addGroup                  ("gModuleSettings")
                                .setPosition               (ObjectSpace ,
                                                            ObjectSpace + gSettings.getPosition()[1] + gSettings.getBackgroundHeight() + ObjectSpace)
                                .setWidth                  (ObjectSpace + ObjectSize[0] + ObjectSpace )
                                .setBackgroundColor        (color(255,80))
                                .setBackgroundHeight       (ObjectSpace + 5 * (ObjectSize[1] + ObjectSpace) + ObjectSpace)
                                .setLabel                  ("Module Settings")
                                .disableCollapse           ()
                                ;
                                
                 tfModuleName  =  cp5        .addTextfield         ("tfModuleName")
                                             .setPosition          (ObjectSpace, 
                                                                    ObjectSpace + ObjectSpace)
                                             .setAutoClear         (false)
                                             .setWidth             (ObjectSize[0])
                                             .setLabel             ("Module Name")
                                             .setGroup             ("gModuleSettings")
                                             ;           
                                
                                
                   slbPosSwitch = cp5   .addScrollableList    ("PosSwitchList")
                                        .setPosition          ( ObjectSpace, 
                                                               (int)tfModuleName.getPosition()[1] + tfModuleName.getHeight() + ObjectSpace)
                                        .setItemHeight        (slbHeight)
                                        .setBarHeight         (slbHeight)
                                        .setLabel             ("Switch Type")
                                        .setWidth             (ObjectSize[0])
                                        .setBackgroundColor   (color(255,80))
                                        .setColorActive       (green_)
                                        .setGroup             ("gModuleSettings")
                                        .setDefaultValue      (0.0)
                                        .close()
                                        .onEnter              (toFront)
                                        .onLeave              (close)
                                        .onChange             (changeLEDStates)
                                        .addItem              ("2 Position" ,2)
                                        .addItem              ("3 Position" ,3)
                                        .addItem              ("6 Position" ,6)
                                        ;
                                        
                   slbRCChannel = cp5   .addScrollableList    ("RCChannelList")
                                        .setPosition          ( ObjectSpace, 
                                                               (int)slbPosSwitch.getPosition()[1] + slbPosSwitch.getHeight()  + ObjectSpace)
                                        .setItemHeight        (slbHeight)
                                        .setBarHeight         (slbHeight)
                                        .setLabel             ("RC Channel")
                                        .setWidth             (ObjectSize[0])
                                        .setBackgroundColor   (color(255,80))
                                        .setColorActive       (green_)
                                        .setGroup             ("gModuleSettings")
                                        .setDefaultValue      (0.0)
                                        .close()
                                        .onEnter(toFront)
                                        .onLeave(close)
                                        .addItem              ("Channel 1" ,1)
                                        .addItem              ("Channel 2" ,2)
                                        ;
                                        
                cp5.getController("tfModuleName").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);
               
                btnWriteSettings = cp5    .addButton      ("WriteSetting")
                                          .setPosition    (ObjectSpace, 
                                                           gModuleSettings.getBackgroundHeight() - ObjectSize[1] - ObjectSpace )
                                          .setSize        (ObjectSize[0], ObjectSize[1])
                                          .setLabel       ("Write")
                                          .setGroup       ("gModuleSettings")
                                          .setColorActive (green_)
                                          .lock()
                                          ;
                            
                btnReadSettings = cp5     .addButton      ("ReadSetting")
                                          .setPosition    (ObjectSpace, 
                                                           btnWriteSettings.getPosition()[1] - ObjectSize[1] - ObjectSpace )
                                          .setSize        (ObjectSize[0], ObjectSize[1])
                                          .setLabel       ("Read")
                                          .setGroup       ("gModuleSettings")
                                          .setColorActive (green_)
                                          .lock()
                                          ;   
   
/* ------------------ LED SETTINGS ------------------------------ */

           gLEDSettings    =  cp5   .addGroup                  ("gLEDSettings")
                                    .setPosition               ((int)gSettings.getPosition()[0] + gSettings.getWidth() + ObjectSpace,
                                                                ObjectSpace + ObjectSpace)
                                    .setWidth                  (ObjectSpace + ObjectSize[0] + ObjectSpace + ObjectSize[0] + ObjectSpace)
                                    .setBackgroundColor        (color(255,80))
                                    .setBackgroundHeight       (WindowY - ObjectSize[1] - ObjectSpace)
                                    .setLabel                  ("LED Settings")
                                    .disableCollapse           ()
                                    ;
  
  
                 slbLED = cp5            .addScrollableList       ("slbLED")
                                         .setPosition             (ObjectSpace, 
                                                                   ObjectSpace)
                                          .setItemHeight          (slbHeight)
                                          .setBarHeight           (slbHeight)
                                          .setSize                (ObjectSize[0],150)
                                          .setLabel               ("LED Auswahl")
                                          .setBackgroundColor     (150)
                                          .onEnter                (toFront)
                                          .onLeave                (close)
                                          .setColorActive         (green_)
                                          .setGroup               ("gLEDSettings")
                                          .close()
                                          .setValue               (0.0)
                                          ;  
                                          for (int j = 1; j <= MaxLEDAnzahl ; j++) {
                                          slbLED.addItem("LED "+ j,j);
                                    }
                                    
                 tfLEDModeName  =  cp5       .addTextfield         ("tfLEDModeName")
                                             .setPosition          (ObjectSpace, 
                                                                    (int)slbLED.getPosition()[1] + slbLED.getHeight() + ObjectSpace + ObjectSpace)
                                             .setAutoClear         (false)
                                             .setWidth             (ObjectSize[0])
                                             .setCaptionLabel      ("Name")
                                             .setGroup             ("gLEDSettings")
                                             ;                   
                                             
                 sLEDBrightness = cp5        .addSlider            ("sBrightness")
                                             .setPosition          ((int)tfLEDModeName.getPosition()[0] + tfLEDModeName.getWidth() + ObjectSpace,
                                                                    (int)tfLEDModeName.getPosition()[1] )
                                             .setRange             (0,255)
                                             .setSize              (ObjectSize[0],ObjectSize[1])
                                             .setValue             (DefaultSliderValue)
                                             .setDecimalPrecision  (0)
                                             .setCaptionLabel      ("Main Brightness")
                                             .setGroup             ("gLEDSettings")
                                             ;
                                             
                                          
                sBrightnessON = cp5          .addSlider            ("sBrightnessON")
                                             .setPosition          (ObjectSpace,
                                                                    (int)tfLEDModeName.getPosition()[1] + tfLEDModeName.getHeight() + ObjectSpace + ObjectSpace)
                                             .setRange             (0,100)
                                             .setSize              (ObjectSize[0],ObjectSize[1])
                                             .setValue             (DefaultSliderValue)
                                             .setDecimalPrecision  (0)
                                             .setCaptionLabel      ("ON Brightness")
                                             .setGroup             ("gLEDSettings")
                                             ;                                            
                                             
               nbONTime =  cp5               .addNumberbox         ("nbONTime")
                                             .setPosition          ((int)sBrightnessON.getPosition()[0] + sBrightnessON.getWidth() + ObjectSpace,
                                                                    (int)sBrightnessON.getPosition()[1])
                                             .setSize              (ObjectSize[0],ObjectSize[1])
                                             .setRange             (0,10)
                                             .setMultiplier        (0.1)
                                             .setDecimalPrecision  (1)
                                             .setDirection         (Controller.HORIZONTAL)
                                             .setValue             (1)
                                             .setCaptionLabel      ("ON Time")
                                             .setGroup             ("gLEDSettings")
                                             ;
                                             
              sBrightnessOFF = cp5           .addSlider            ("sBrightnessOFF")
                                             .setPosition          (ObjectSpace,
                                                                    (int)sBrightnessON.getPosition()[1] + sBrightnessON.getHeight() + ObjectSpace + ObjectSpace)
                                             .setRange             (0,100)
                                             .setSize              (ObjectSize[0],ObjectSize[1])
                                             .setValue             (0)
                                             .setDecimalPrecision  (0)
                                             .setCaptionLabel      ("OFF Brightness")
                                             .setGroup             ("gLEDSettings")
                                             ;
                                             
             nbOFFTime =  cp5                .addNumberbox        ("nbOFFTime")
                                             .setPosition         ((int)sBrightnessOFF.getPosition()[0] + sBrightnessOFF.getWidth() + ObjectSpace,
                                                                   (int)sBrightnessOFF.getPosition()[1])
                                             .setSize             (ObjectSize[0],ObjectSize[1])
                                             .setRange            (0,10)
                                             .setMultiplier       (0.1) 
                                             .setDecimalPrecision (1)
                                             .setDirection        (Controller.HORIZONTAL)
                                             .setValue            (0)
                                             .setCaptionLabel     ("OFF Time")
                                             .setGroup            ("gLEDSettings")
                                             ;
                                             
             sCycles =  cp5                  .addSlider           ("sCycles")
                                             .setPosition         (ObjectSpace,
                                                                   (int)sBrightnessOFF.getPosition()[1] + sBrightnessOFF.getHeight() + ObjectSpace + ObjectSpace)
                                             .setSize             (ObjectSize[0],ObjectSize[1])
                                             .setRange            (0,10)  
                                             .setDecimalPrecision (0)                                            
                                             .setValue            (0)
                                             .setCaptionLabel     ("Cycles")
                                             .setGroup            ("gLEDSettings")
                                             ;   
                                             
             nbPauseBetweenTime =  cp5       .addNumberbox        ("nbPauseBetweenTime")
                                             .setPosition         ((int)sCycles.getPosition()[0] + sCycles.getWidth() + ObjectSpace,
                                                                   (int)sCycles.getPosition()[1])
                                             .setSize             (ObjectSize[0],ObjectSize[1])
                                             .setRange            (0,10)
                                             .setMultiplier       (0.1)
                                             .setDecimalPrecision (1)
                                             .setDirection        (Controller.HORIZONTAL)
                                             .setValue            (0)
                                             .setCaptionLabel     ("Cycle Time")
                                             .setGroup            ("gLEDSettings")
                                             ;
           
           nbPauseBeforeTime =  cp5          .addNumberbox        ("nbPauseBeforeTime")
                                             .setPosition         (ObjectSpace,
                                                                   (int)sCycles.getPosition()[1] + sCycles.getHeight() + ObjectSpace + ObjectSpace)
                                             .setSize             (ObjectSize[0],ObjectSize[1])
                                             .setRange            (0,10)
                                             .setMultiplier       (0.1) 
                                             .setDecimalPrecision (1)
                                             .setDirection        (Controller.HORIZONTAL)
                                             .setValue            (0)
                                             .setCaptionLabel     ("Before Cycle Time")
                                             .setGroup            ("gLEDSettings")
                                             ;
                                             
           nbPauseAfterTime =  cp5           .addNumberbox        ("nbPauseAfterTime")
                                             .setPosition         ((int)nbPauseBeforeTime.getPosition()[0] + nbPauseBeforeTime.getWidth() + ObjectSpace,
                                                                    (int)nbPauseBeforeTime.getPosition()[1])
                                             .setSize             (ObjectSize[0],ObjectSize[1])
                                             .setRange            (0,10)
                                             .setMultiplier       (0.1)
                                             .setDecimalPrecision (1)
                                             .setDirection        (Controller.HORIZONTAL)
                                             .setValue            (0)
                                             .setCaptionLabel     ("After Cycle Time")
                                             .setGroup            ("gLEDSettings")
                                             ;
         
                                             
           cp5.getController      ("tfLEDModeName").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);
           cp5.getController      ("sBrightness").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);
           cp5.getController      ("sBrightnessON").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);
           cp5.getController      ("sBrightnessOFF").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);
           cp5.getController      ("nbONTime").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);
           cp5.getController      ("nbOFFTime").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);
           cp5.getController      ("sCycles").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);
           cp5.getController      ("nbPauseBetweenTime").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);                     
           cp5.getController      ("nbPauseBeforeTime").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);          
           cp5.getController      ("nbPauseAfterTime").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);           
           
           
            gLEDStates    =  cp5   .addGroup                  ("gLEDStates")
                                    .setPosition               (ObjectSpace,
                                                               (int)nbPauseBeforeTime.getPosition()[1] + nbPauseBeforeTime.getHeight() + ObjectSpace + ObjectSpace)
                                    .setWidth                  ( gLEDSettings.getWidth() - ObjectSpace - ObjectSpace)
                                    .setBackgroundColor        (color(255,80))
                                    .setBackgroundHeight       ( ObjectSpace + ObjectSize[1] + ObjectSpace)
                                    .setLabel                  ("States")
                                    .setGroup                 ("gLEDSettings")
                                    .disableCollapse           ()
                                    ;
                                    
                           for (int g = 0; g < 6; g++){
                               tState[g] = cp5   .addToggle  ("State"+g)
                                                .setPosition            ( ObjectSpace + g * (ObjectSpace + 20),
                                                                          ObjectSpace)
                                                .setSize                (20,10)
                                                .setLabelVisible        (false)
                                                //.setMode(ControlP5.SWITCH)
                                                .setColorActive(green_)
                                                .setGroup("gLEDStates")
                                                .setVisible(false)
                                                ;
                                 }
                    
                                 
             btnReadLED = cp5         .addButton      ("ReadLED")
                                      .setPosition    ( ObjectSpace, 
                                                       gLEDSettings.getBackgroundHeight() - ObjectSize[1] - ObjectSpace )
                                      .setSize        (ObjectSize[0], ObjectSize[1])
                                      .setLabel       ("Read")
                                      .setColorActive (green_)
                                      .setGroup       ("gLEDSettings")
                                      .lock           ()
                                      ; 
             btnWriteLED = cp5        .addButton      ("WriteLED")
                                      .setPosition    (btnReadLED.getPosition()[0] + btnReadLED.getWidth() + ObjectSpace, 
                                                       btnReadLED.getPosition()[1] )
                                      .setSize        (ObjectSize[0], ObjectSize[1])
                                      .setLabel       ("Write")
                                      .setColorActive (green_)
                                      .setGroup       ("gLEDSettings")
                                      .lock           ()
                                      ;                    

 
/* ------------------ MESSAGE BOX ------------------------------ */

/*
      messageBox = cp5        .addGroup            ("messageBox")
                              .setPosition         (width / 2 - 100, height / 2 - 50)
                              .setBackgroundHeight (100)
                              .setWidth            (width/2)
                              .setBackgroundColor  (color(0,100))
                              .moveTo             ("global")      
                              .hideBar            ()
                              .hide               ()
                              ;
  

           messageBoxLabel = cp5    .addTextlabel("messageBoxLabel")
                                            .setPosition(20,20)
                                            .setValue(" ----- ")
                                            .moveTo(messageBox);
                                
                                
                          Button btnOK   = cp5    .addButton            ("OKButton")
                                                  .setPosition          (ObjectSpace, 
                                                                         messageBox.getBackgroundHeight()  - ObjectSize[1] - ObjectSpace)
                                                  .setSize              (ObjectSize[0], ObjectSize[1])
                                                  .setColorBackground   (color(40))
                                                  .setColorActive       (color(20))
                                                  .moveTo               (messageBox)
                                                  //.setGroup             (messageBox)
                                                  .setBroadcast         (false) 
                                                  .setValue             (1)
                                                  .setBroadcast         (true)
                                                  .setCaptionLabel      ("OK")                                                  
                                                  ;    

                         Button btnCancel = cp5  .addButton             ("CancelButton")
                                                  .setPosition          (messageBox.getWidth()  - ObjectSize[0] - ObjectSpace, 
                                                                         btnOK.getPosition()[1])
                                                  .setSize              (ObjectSize[0], ObjectSize[1])
                                                  .setColorBackground   (color(40))
                                                  .setColorActive       (color(20))
                                                  .moveTo               (messageBox)
                                                  //.setGroup             (messageBox)
                                                  .setBroadcast         (false) 
                                                  .setValue             (1)
                                                  .setBroadcast         (true)
                                                  .setCaptionLabel      ("Cancel")
                                                  ;
*/                      
}        
                                /* --------------------------------------------------- */
                                /* ------------------- END SETUP --------------------- */
                                /* --------------------------------------------------- */

void draw() {
  
  background(150);

}

  
 void ReadLED() {
   
     g_serial.write("L?");
     g_serial.write("," + (int)slbLED.getValue());
     g_serial.write("\n");
     
 }
 
 void WriteLED() {
   
     String LEDName;
    
     if (tfLEDModeName.getStringValue().length() > 8)
       LEDName = tfLEDModeName.getStringValue().substring(0, 8);
      else if (tfLEDModeName.getStringValue().length() == 0) 
       LEDName = "NULL";
     else LEDName = tfLEDModeName.getStringValue();
     
     g_serial.write("L!");
     g_serial.write("," + (int)slbLED.getValue());
     g_serial.write("," + LEDName);
     g_serial.write("," + (int)sLEDBrightness.getValue());
     g_serial.write("," + (int)sBrightnessON.getValue());
     g_serial.write("," + (int)sBrightnessOFF.getValue());
     g_serial.write("," + (int)(nbONTime.getValue()*10));
     g_serial.write("," + (int)(nbOFFTime.getValue()*10));
     g_serial.write("," + (int)sCycles.getValue());
     g_serial.write("," + (int)(nbPauseBetweenTime.getValue()*10));
     g_serial.write("," + (int)(nbPauseBeforeTime.getValue()*10));
     g_serial.write("," + (int)(nbPauseAfterTime.getValue()*10));
     
     for (int g = 0; g < 6; g++){
       g_serial.write("," + (int)(tState[g].getValue()));
     }
     
     g_serial.write("\n");
 
 }
  
void ReadSetting() {
  
     g_serial.write("S?");
     g_serial.write("\n");
     
 }
 
 void WriteSetting() {
   
     String ModuleName;
    
     if (tfModuleName.getStringValue().length() > 8)
       ModuleName = tfModuleName.getStringValue().substring(0, 8);
     else ModuleName = tfModuleName.getStringValue();
     
     g_serial.write("S!");
     g_serial.write("," + ModuleName);
     g_serial.write("," + (int)slbPosSwitch.getValue());
     g_serial.write("," + (int)slbRCChannel.getValue());
     g_serial.write("\n");
     
 }

/* --------------- BUTTON HANDLER ----------------------- */

void ConnectButton() {

      btnConnect.hide();
      SerialPort  = Serial.list()[int(slbComPorts.getValue())];
      g_serial = new Serial(this, SerialPort, GUI_BaudRate);                                                     
      println( SerialPort + "    " + GUI_BaudRate);
      btnDisconnect.show();
      btnReadLED.unlock();
      btnWriteLED.unlock();
      btnReadSettings.unlock();
      btnWriteSettings.unlock();
      g_serial.write("COM\n");         // Verbindungsaufbau
 
}

void DisconnectButton() {
  
      btnDisconnect.hide();
      g_serial.stop();
      btnConnect.show();
      btnReadLED.lock();
      btnWriteLED.lock();
      btnReadSettings.lock();
      btnWriteSettings.lock();
      
}

void SaveEEPROM() {
  
      g_serial.write("SAVE!");
      g_serial.write("\n");
  
}

void Sync() {
  
      g_serial.write("SYNC!");
      g_serial.write("\n");
  
}


void Exit() {

   //g_serial.stop();
  
   exit();
 
}

/* ---------------------- EVENT HANDLER ------------------------- */

void controlEvent(ControlEvent theEvent) {
    if(theEvent.isController()) {      
            /* ------- COM PORT Liste refresh --------- */
            if (slbComPorts.isOpen()) {             
                      slbComPorts.clear();
                      for(int i=0;i<Serial.list().length;i++) {
                                String pn = Serial.list()[i];
                                if (pn.length() >0 )     slbComPorts.addItem(pn,i); 
                                commListMax = i;
                      }
            }
    }
}

void Parser (String msg){
  
  String[] list = split(msg, ',');

  if (list[0].equals("?L")) {
     
     slbLED.setValue(int(list[1]));
     tfLEDModeName.setText(list[2]);
     sLEDBrightness.setValue(int(list[3]));
     sBrightnessON.setValue(int(list[4]));
     sBrightnessOFF.setValue(int(list[5]));
     nbONTime.setValue(float(list[6])/10);
     nbOFFTime.setValue(float(list[7])/10);
     sCycles.setValue(int(list[8]));
     nbPauseBetweenTime.setValue(float(list[9])/10);
     nbPauseBeforeTime.setValue(float(list[10])/10);
     nbPauseAfterTime.setValue(float(list[11])/10);
     
     for (int g = 0; g < 6; g++){
       tState[g].setValue(float(list[12+g]));
     }
   }
      
  if (list[0].equals("?S")) {
        tfModuleName.setText(list[1]);
        slbPosSwitch.setValue(int(list[2]));
        slbRCChannel.setValue(int(list[3]));
  }
      
  if (list[0].equals("CONNECTED!")) {
    // DO SOMETHING COOL
  }
 
}

void serialEvent(Serial p) {
  
  String msg = p.readStringUntil((int)('\n'));
  if (msg != null) {
      print("Empfangen: >");
      println(msg); println("<");
      Parser(msg);
  }
  
}

void keyPressed() {
    // MAYBE LATER
}