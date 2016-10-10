
/* ------------------------------- SERIAL -------------------------*/ 
void SerialParser(void)
{

    int Ergebnisse = Parser()-1;
    
    if (String(pTokenArray[0]) == "COM") {
                 Serial.print("CONNECTED!");          
      }

    if (String(pTokenArray[0]) == "S!") {
          Serial.print("!S");
          strcpy(&LEDSettings.ModuleName[0], pTokenArray[1]);
          LEDSettings.SwitchType = byte(String(pTokenArray[2]).toInt());
          LEDSettings.RCChannel = byte(String(pTokenArray[3]).toInt());
    }

    if (String(pTokenArray[0]) == "S?") {
         Serial.print( "?S," );
         Serial.print( LEDSettings.ModuleName ); Serial.print( "," );
         Serial.print( LEDSettings.SwitchType);  Serial.print( "," );
         Serial.print( LEDSettings.RCChannel);   Serial.print( "," );
    } 

    
    if (String(pTokenArray[0]) == "L!") {
          int LEDNumber = String(pTokenArray[1]).toInt();
          strcpy(&LEDSettings.LightModes[LEDNumber].Name[0], pTokenArray[2]);
          LEDSettings.MainBrightness[LEDNumber]                   = (byte)String(pTokenArray[3]).toInt();
          LEDSettings.LightModes[LEDNumber].onBrightness          = (byte)String(pTokenArray[4]).toInt();
          LEDSettings.LightModes[LEDNumber].offBrightness         = (byte)String(pTokenArray[5]).toInt();
          LEDSettings.LightModes[LEDNumber].onTime                = (byte)String(pTokenArray[6]).toInt();
          LEDSettings.LightModes[LEDNumber].offTime               = (byte)String(pTokenArray[7]).toInt();
          LEDSettings.LightModes[LEDNumber].Cycles                = (byte)String(pTokenArray[8]).toInt();
          LEDSettings.LightModes[LEDNumber].PauseBetweenSequences = (byte)String(pTokenArray[9]).toInt();
          LEDSettings.LightModes[LEDNumber].PauseBeforeSequence   = (byte)String(pTokenArray[10]).toInt();
          LEDSettings.LightModes[LEDNumber].PauseAfterSequence    = (byte)String(pTokenArray[11]).toInt();

          for (int i = 0 ; i < 6 ; i++) {
              bitWrite(LEDSettings.LEDModeStates[LEDNumber],i,(byte)String(pTokenArray[12+i]).toInt());
         }
         Serial.print("!L");
         
        }

    if (String(pTokenArray[0]) == "L?") {
         int LEDNumber = String(pTokenArray[1]).toInt();
         Serial.print("?L,");
         Serial.print( LEDNumber);                                              Serial.print( "," );
         Serial.print( LEDSettings.LightModes[LEDNumber].Name);                 Serial.print( "," );
         Serial.print( LEDSettings.MainBrightness[LEDNumber]);                  Serial.print( "," );
         Serial.print( LEDSettings.LightModes[LEDNumber].onBrightness);         Serial.print( "," );
         Serial.print( LEDSettings.LightModes[LEDNumber].offBrightness);        Serial.print( "," );
         Serial.print( LEDSettings.LightModes[LEDNumber].onTime);               Serial.print( "," );
         Serial.print( LEDSettings.LightModes[LEDNumber].offTime );             Serial.print( "," );
         Serial.print( LEDSettings.LightModes[LEDNumber].Cycles  );             Serial.print( "," );
         Serial.print( LEDSettings.LightModes[LEDNumber].PauseBetweenSequences);Serial.print( "," );
         Serial.print( LEDSettings.LightModes[LEDNumber].PauseBeforeSequence);  Serial.print( "," );
         Serial.print( LEDSettings.LightModes[LEDNumber].PauseAfterSequence);   Serial.print( "," );
         for (int i = 0 ; i < 6 ; i++) {
              Serial.print( bitRead(LEDSettings.LEDModeStates[LEDNumber],i));  Serial.print( "," );
         }  
         
    } 

    if (String(pTokenArray[0]) == "SAVE!") {
      saveConfig();
      Serial.print("!SAVE");
    }

    if (String(pTokenArray[0]) == "SYNC!") {
      ResetSync = 1;
      Serial.print("!SYNC");
    }
    
     if (String(pTokenArray[0]) == "CLEAR!") {
      clearConfig();
      Serial.print("!CLEAR");
    }

    Serial.print("\n");                        // ABSCHLUSS 
    pInputString = &inputString[0];
    stringComplete = false;
}

byte Parser( void )
{
  memset(pTok, 0, iNumberOfToken*sizeof(char *));         // Token Puffer Nullen
  char *pResult = strtok(&inputString[0],sDelimiter);     // erster Durchlauf
  byte i = 0;                      // Loopvariable
  while ((pResult != NULL) && (i < iNumberOfToken))       // weitere Durchlaeufe, bis Ende der Zeichenketten
  {
    *(pTok+i) = pResult;                                  // Ergebnis speichern
    i++;
    pResult=strtok(NULL,sDelimiter);                      // naechster Token
  }
  return i;                       // gibt die Anzahl der Argumente zurueck
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    //Serial.print(inChar);
    if (inChar == '\n') {
      inChar = 0;
      stringComplete = true;
    }
    *pInputString++ = inChar;
  }
}

