/* ------------------------------- RC Eingänge -----------------------------*/


void RCChannelCheck(){

 /* ------------ RC Channel -------------------*/
  
    if ( LEDSettings.RCChannel == 0)    RC_CHANNEL_PULSE = iRC_CHANNEL_1_PULSE;
    else                                RC_CHANNEL_PULSE = iRC_CHANNEL_2_PULSE;
   
    // SetLEDState = PosAlwaysOn;

    if (LEDSettings.SwitchType == 2) {
              if (RC_CHANNEL_PULSE > LEDSettings.SwitchPositions[Limit6SWMin] && RC_CHANNEL_PULSE < LEDSettings.SwitchPositions[Limit6SWLow2]) {
                SetLEDState = PosLow3;
              }
              if (RC_CHANNEL_PULSE > LEDSettings.SwitchPositions[Limit6SWLow2] && RC_CHANNEL_PULSE < LEDSettings.SwitchPositions[Limit6SWLow1]) {
                SetLEDState = PosLow2;
              }
              if (RC_CHANNEL_PULSE > LEDSettings.SwitchPositions[Limit6SWLow1] && RC_CHANNEL_PULSE < LEDSettings.SwitchPositions[Limit6SWMid]) {
                SetLEDState = PosLow;
              }
              if (RC_CHANNEL_PULSE > LEDSettings.SwitchPositions[Limit6SWMid] && RC_CHANNEL_PULSE < LEDSettings.SwitchPositions[Limit6SWHigh1]) {
                SetLEDState = PosHigh;
              }
              if (RC_CHANNEL_PULSE > LEDSettings.SwitchPositions[Limit6SWHigh1] && RC_CHANNEL_PULSE < LEDSettings.SwitchPositions[Limit6SWHigh2]) {
                SetLEDState = PosHigh2;
              }
              if (RC_CHANNEL_PULSE > LEDSettings.SwitchPositions[Limit6SWHigh2] && RC_CHANNEL_PULSE < LEDSettings.SwitchPositions[Limit6SWMax]) {
                SetLEDState = PosHigh3;
              }
    }

    else if (LEDSettings.SwitchType == 1) {
              if (RC_CHANNEL_PULSE > LEDSettings.SwitchPositions[Limit3SWMin] && RC_CHANNEL_PULSE < LEDSettings.SwitchPositions[Limit3SWLow]) {
                SetLEDState = PosLow;
              }
              if (RC_CHANNEL_PULSE > LEDSettings.SwitchPositions[Limit3SWLow] && RC_CHANNEL_PULSE < LEDSettings.SwitchPositions[Limit3SWHigh]) {
                SetLEDState = PosMid;
              }
              if (RC_CHANNEL_PULSE > LEDSettings.SwitchPositions[Limit3SWHigh] && RC_CHANNEL_PULSE < LEDSettings.SwitchPositions[Limit3SWMax]) {
                SetLEDState = PosHigh;
              }
    }

    else if (LEDSettings.SwitchType == 0) {
              if (RC_CHANNEL_PULSE > LEDSettings.SwitchPositions[Limit2SWMin] && RC_CHANNEL_PULSE < LEDSettings.SwitchPositions[Limit2SWMid]) {
                SetLEDState = PosLow;
              }
              if (RC_CHANNEL_PULSE > LEDSettings.SwitchPositions[Limit2SWMid] && RC_CHANNEL_PULSE < LEDSettings.SwitchPositions[Limit2SWMax]) {
                SetLEDState = PosHigh;
              }
    }
    else SetLEDState = PosAlwaysOn;


}
