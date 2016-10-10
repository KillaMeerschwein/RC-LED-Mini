
/* ------------------------------- EEPROM -------------------------*/


void loadConfig() {

  if (EEPROM.read(CONFIG_START + sizeof(LEDSettings) - 2) == LEDSettings.version_of_program[2] &&
      EEPROM.read(CONFIG_START + sizeof(LEDSettings) - 3) == LEDSettings.version_of_program[1] &&
      EEPROM.read(CONFIG_START + sizeof(LEDSettings) - 4) == LEDSettings.version_of_program[0] ) { 
    
      for (unsigned int t=0; t<sizeof(LEDSettings); t++)
          *((char*)&LEDSettings + t) = EEPROM.read(CONFIG_START + t);
  }  
  else {
    saveConfig();
  }
}

void saveConfig() {
  for (unsigned int t=0; t<sizeof(LEDSettings); t++)
  { // writes to EEPROM
    EEPROM.write(CONFIG_START + t, *((char*)&LEDSettings + t));
    // and verifies the data
    if (EEPROM.read(CONFIG_START + t) != *((char*)&LEDSettings + t))
    {
      // error writing to EEPROM
    }
  }
}

void clearConfig (){
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

