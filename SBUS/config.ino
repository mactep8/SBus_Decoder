#include <EEPROM.h>

byte GetConfigState()
{
  return digitalRead(A4);
}

byte buf[41];
byte buf_ptr = 0;

bool bufValid()
{
  return (buf[0]==0xaa) && (buf[40]==0x5d);
}

void CallConfig()
{
  byte b = Serial.read();
  if (b == 0xaa)
  {
    buf_ptr = 0;
  }
  if (buf_ptr < 41)
  {
    buf[buf_ptr] = b;
    buf_ptr++;
  }
  
  if (buf_ptr > 40 && bufValid())
    CallCommand();
}

/*
Packet:
0 - start byte 0xaa
1 - command (R - read / W - write)
2 - PPM Bank A
3 - Timer2 loops to Bank A
4 - Channels bank A
5:20 - channel map bank A
21 - ppm bank B
22 - Timer2 loops to Bank B
23 - Channels bank B
24:39 - channel map bank B
40 - check byte 0x5d
*/

void CallCommand()
{
  if (buf[1]=='R') CallReadCFG();
  if (buf[1]=='W') CallWriteCFG();
}

void CallReadCFG()
{
  // Bank A
  buf[2] = EEPROM.read(ADDR_PPM_BANK_A);
  buf[3] = EEPROM.read(ADDR_TIMER_LOOPS_A);
  buf[4] = EEPROM.read(ADDR_CHANNELS_A);
  for(int i=0;i<16;i++)
    buf[i+5] = EEPROM.read(ADDR_CHANNEL_MAP_A + i);
  // Bank B
  buf[21] = EEPROM.read(ADDR_PPM_BANK_B);
  buf[22] = EEPROM.read(ADDR_TIMER_LOOPS_B);
  buf[23] = EEPROM.read(ADDR_CHANNELS_B);
  for(int i=0;i<16;i++)
    buf[i+24] = EEPROM.read(ADDR_CHANNEL_MAP_B + i);
    
  for (int i=0;i<41;i++)
    Serial.write(buf[i]);
}

void CallWriteCFG()
{
  // Bank A
  EEPROM.write(ADDR_PPM_BANK_A, buf[2]);
  EEPROM.write(ADDR_TIMER_LOOPS_A, buf[3]);
  EEPROM.write(ADDR_CHANNELS_A, buf[4]);
  for(int i=0;i<16;i++)
    EEPROM.write(ADDR_CHANNEL_MAP_A + i, buf[i+5]);
  // Bank B
  EEPROM.write(ADDR_PPM_BANK_B, buf[21]);
  EEPROM.write(ADDR_TIMER_LOOPS_B, buf[22]);
  EEPROM.write(ADDR_CHANNELS_B, buf[23]);
  for(int i=0;i<16;i++)
    EEPROM.write(ADDR_CHANNEL_MAP_B + i, buf[i+24]);
}

void PrepareCFG()
{
  // Bank A
  PPM_A = EEPROM.read(ADDR_PPM_BANK_A);
  if (PPM_A>1) PPM_A = 0;
  OCR_BANK_A = EEPROM.read(ADDR_TIMER_LOOPS_A);
  if (OCR_BANK_A>10) OCR_BANK_A = 5;
  ChannelsBankA = EEPROM.read(ADDR_CHANNELS_A);
  if (ChannelsBankA>16) ChannelsBankA = 8;
  for(int i=0;i<16;i++)
  {
    ChannelsMapA[i] = EEPROM.read(ADDR_CHANNEL_MAP_A + i);
    if (ChannelsMapA[i]>16) ChannelsMapA[i] = i;
  }
  // Bank B
  PPM_B = EEPROM.read(ADDR_PPM_BANK_B);
  if (PPM_B>1) PPM_B = 0;
  OCR_BANK_B = EEPROM.read(ADDR_TIMER_LOOPS_B);
  if (OCR_BANK_B>10) OCR_BANK_B = 5;
  ChannelsBankB = EEPROM.read(ADDR_CHANNELS_B);
  if (ChannelsBankB>16) ChannelsBankB = 8;
  for(int i=0;i<16;i++)
  {
    ChannelsMapB[i] = EEPROM.read(ADDR_CHANNEL_MAP_B + i);
    if (ChannelsMapB[i]>16) 
    {
      if (i<8) ChannelsMapB[i] = i+8;
      else ChannelsMapB[i] = i;
    }
  }
}
