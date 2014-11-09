#include "config.h"
#include <FUTABA_SBUS.h>
FUTABA_SBUS sBus;
byte Mode;

void setup()
{
  pinMode( A4 , INPUT_PULLUP);
  Mode = GetConfigState();
  if (Mode==0)
  {
    //config mode
    Serial.begin(9600);
  }
  else
  {
    // normal mode
    //////// BANK A //////////
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
//////// BANK B //////////
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
    PrepareCFG();
    sBus.begin();
    Timer1_init();
  }
}
uint32_t lastTime = 0;
uint32_t time = 0;

void loop()
{
  if (Mode==0)
  {
    if (Serial.available())
      CallConfig();
  }
  else
  {
    sBus.FeedLine();
    if (sBus.toChannels == 1){
      sBus.UpdateChannels();
      sBus.toChannels = 0;
    }
  }
}
