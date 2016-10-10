

// Ruft zyklisch die Routine runMode() auf
void cyclicRunMode(long msec)
{
  static unsigned long prevTimeRunMode = millis();
  if ((millis() - prevTimeRunMode) > msec)
  {
    prevTimeRunMode = millis();
    LEDStateMachine();
  }
}

unsigned int checkModeActive(unsigned int i)
{
  unsigned int state;
  /* 
   * diese Funktion soll prüfen, ob der Mode gewechselt wurde
   * und Einfluss auf die Statemachine nehmen START und STOPP von Modis
  */
  return state;
}

void LEDStateMachine(void)
{
  static unsigned int state[MaxLEDAnzahl] = {0};
  static unsigned int cycleCounter[MaxLEDAnzahl];
  static unsigned long prevTime[MaxLEDAnzahl];

  unsigned long newTime = millis();

  if (ResetSync) {
     for (byte i = 0; i < MaxLEDAnzahl; i++) state[i] = MODE_INIT;
     ResetSync = 0;
  }
  
  for (byte i = 0; i < MaxLEDAnzahl; i++) {
    switch (state[i])
    {
      case MODE_SLEEP:                  //
                                        LightOutputPercent[i] = LEDSettings.LightModes[i].offBrightness;  // oder null
                                        state[i] = MODE_INIT;
                                        break;
                                        
      case MODE_INIT:                   // Init
                                        
                                        cycleCounter[i] = 0;
                                        prevTime[i] = newTime;
                                        if (LEDSettings.LightModes[i].PauseBeforeSequence > 0)
                                        {
                                          state[i] = MODE_PAUSE_BEFORE;
                                        }
                                        else
                                        {
                                          state[i] = MODE_SWITCH_ON;   
                                        }
                                        break;

      case MODE_PAUSE_BEFORE:           //
                                        
                                        if ((newTime -  prevTime[i]) > LEDSettings.LightModes[i].PauseBeforeSequence*100)
                                        {
                                          state[i] = MODE_SWITCH_ON;
                                        }
                                        else
                                          break;  // wenn Zeit erreicht - weiter ohne break!

      case MODE_SWITCH_ON:              //
                                        
                                        LightOutputPercent[i] = LEDSettings.LightModes[i].onBrightness;    
                                        prevTime[i] = newTime;
                                        if (LEDSettings.LightModes[i].onTime > 0)
                                          state[i] = MODE_KEEP_ON;
                                        break;

      case MODE_KEEP_ON:                //
                                        
                                        if ((newTime -  prevTime[i]) > LEDSettings.LightModes[i].onTime*100)
                                        {
                                          state[i] = MODE_SWITCH_OFF;
                                        }
                                        else
                                          break;  // wenn Zeit erreicht - weiter ohne break!

      case MODE_SWITCH_OFF:             //
                                        
                                        LightOutputPercent[i] = LEDSettings.LightModes[i].offBrightness;    
                                        prevTime[i] = newTime;
                                        state[i] = MODE_KEEP_OFF;
                                        break;

      case MODE_KEEP_OFF:               //
                                        
                                        if ((newTime -  prevTime[i]) > LEDSettings.LightModes[i].offTime*100)
                                        {
                                          cycleCounter[i] += 1;   // Zyklus zaehlen
                                          prevTime[i] = newTime;
                                          
                                          // Fallunterscheidung: Naechster Zyklus oder naechste Sequenz
                                          if (cycleCounter[i] == LEDSettings.LightModes[i].Cycles)
                                          {
                                            cycleCounter[i] = 0;
                                            // weiter ohne break zu MODE_PAUSE_AFTER
                                          }

                                          else
                                          {
                                            // Wiederholung des Zyklus  
                                            state[i] = MODE_SWITCH_ON;   
                                            break;
                                          }
                                          
                                          
                                        }
                                        else
                                          break;  // wenn oben Zeit erreicht ist - weiter ohne break!
                                        
                                          
      case MODE_PAUSE_AFTER:          //
                                      
                                      if (LEDSettings.LightModes[i].PauseAfterSequence > 0)         // Uebergang mit Pause nach der Sequenz
                                      {
                                        state[i] = MODE_PAUSE_AFTER;
                                        if ((newTime -  prevTime[i]) > LEDSettings.LightModes[i].PauseAfterSequence*100)
                                        {
                                          prevTime[i] = newTime;
                                          // weiter ohne break zu MODE_PAUSE_BETWEEN
                                        }
                                        else
                                          break;
                                      }           

      case MODE_PAUSE_BETWEEN:        //
                                      
                                      if (LEDSettings.LightModes[i].PauseBetweenSequences > 0)      // Pause zwischen zwei Sequenzen
                                      {
                                        state[i] = MODE_PAUSE_BETWEEN;
                                        if ((newTime -  prevTime[i]) > LEDSettings.LightModes[i].PauseBetweenSequences*100)
                                        {
                                          prevTime[i] = newTime;
                                          // weiter ohne break zu MODE_CYCLE_REPETITION
                                        }
                                        else
                                          break;
                                      }
                                      
      case MODE_CYCLE_REPETITION:     // Wiederholung des Zyklus  
                                     
                                      if (LEDSettings.LightModes[i].PauseBeforeSequence > 0)
                                      {
                                        state[i] = MODE_PAUSE_BEFORE;
                                      }
                                      else
                                      {
                                        state[i] = MODE_SWITCH_ON;   
                                      }
                                      break;                                   
      
      case MODE_END:                  // Modus wurde extern beendet
                                      
                                      LightOutputPercent[i] = LEDSettings.LightModes[i].offBrightness;  // oder null
                                      break;
                      
    }

    //state[i] = checkModeActive(i);
  }

  GenerateOutputValues();
}

void GenerateOutputValues(void)
{
  for (byte i = 0; i < MaxLEDAnzahl; i++)
  {
    SetLEDArray[i] = LightOutputPercent[i] * LEDSettings.MainBrightness[i] / 100 ;
  }
}

