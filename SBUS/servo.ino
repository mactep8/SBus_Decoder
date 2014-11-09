volatile uint8_t curr_chA=0;
volatile uint8_t curr_chB=0;
uint8_t cntA = 0;
uint8_t cntB = 0;
volatile uint8_t State_chA=0;
volatile uint8_t State_chB=0;

void Timer1_init()
{
  TCCR1B   =   0;   //stop timer
  TCCR1A   =   0;
  TCNT1   =    0;   //setup
  TCCR1A   =   0;
  TCCR1B = 0<<CS12 | 1<<CS11 | 0<<CS10;
  TIMSK1 = (1<<OCIE1A) | (1<<OCIE1B);   // Разрешить прерывание по совпадению
  OCR1A = 0;
  OCR1B = 0;

  TCCR2B   =   0;   //stop timer
  TCCR2A   =   0;
  TCNT2   =    0;   //setup
  TCCR2A   =   0;
  TCCR2A = 1<<WGM21;
  TCCR2B = (1<<CS21)|(1<<CS22);                   // CLK/256 
  TIMSK2 = 1<<OCIE2A;
  OCR2A = 249;

  TCNT1 = 0;
  UpdateBankA();
  UpdateBankB();
}

ISR(TIMER2_COMPA_vect) {
  cntA++;
  if (cntA>OCR_BANK_A){
    // end of frame to bank A
    curr_chA = 0;
    if (PPM_A == 1)
    {
      // PPM generator
      CH1_off;
      State_chA=0;
      OCR1A = TCNT1 + 400;
    }
    else
    {
      // Servo
      if (ChannelsMapA[curr_chA] < 16) OCR1A = TCNT1 + (sBus.channels[ChannelsMapA[curr_chA]]+2000);
      else OCR1A = TCNT1 + sBus.Failsafe() * 2000 + 2000;
      //OCR1A = TCNT1 + Set_TimeA(curr_chA);
      ServoOn(curr_chA);
    }
    cntA = 0;
  }
  
  cntB++;
  if (cntB>OCR_BANK_B){
    curr_chB = 0;
    if (PPM_B == 1)
    {
      CH9_off;
      State_chB=0;
      OCR1B = TCNT1 + 400;
    }
    else
    {
      if (ChannelsMapB[curr_chB] < 16) OCR1B = TCNT1 + (sBus.channels[ChannelsMapB[curr_chB]]+2000);
      else OCR1B = TCNT1 + sBus.Failsafe() * 2000 + 2000;
      //OCR1B = TCNT1 + Set_TimeB(curr_chB);
      ServoOn(curr_chB+8);
    }
    cntB=0;
  }
}

ISR (TIMER1_COMPA_vect)
{
  if (PPM_A == 1) UpdatePPMA();
  else 
  UpdateBankA();
}

ISR (TIMER1_COMPB_vect)
{
  if (PPM_B == 1) UpdatePPMB();
  else 
  UpdateBankB();
}

void UpdateBankA()
{
  ServoOff(curr_chA);
  curr_chA++;
  if (curr_chA<ChannelsBankA) {
    if (ChannelsMapA[curr_chA] < 16) OCR1A = TCNT1 + (sBus.channels[ChannelsMapA[curr_chA]]+2000);
      else OCR1A = TCNT1 + sBus.Failsafe() * 2000 + 2000;
    //OCR1A = TCNT1 + Set_TimeA(curr_chA);
    ServoOn(curr_chA);
  }
}

void UpdateBankB()
{
  ServoOff(curr_chB+8);
  curr_chB++;
  if (curr_chB<ChannelsBankB) {
    if (ChannelsMapB[curr_chB] < 16) OCR1B = TCNT1 + (sBus.channels[ChannelsMapB[curr_chB]]+2000);
      else OCR1B = TCNT1 + sBus.Failsafe() * 2000 + 2000;
    //OCR1B = TCNT1 + Set_TimeB(curr_chB);
    ServoOn(curr_chB+8);
  }
}

void ServoOn(uint8_t ch)
{
  switch(ch)
  {
    case 0: CH0_on; break;
    case 1: CH1_on; break;
    case 2: CH2_on; break;
    case 3: CH3_on; break;
    case 4: CH4_on; break;
    case 5: CH5_on; break;
    case 6: CH6_on; break;
    case 7: CH7_on; break;
    case 8: CH8_on; break;
    case 9: CH9_on; break;
    case 10: CH10_on; break;
    case 11: CH11_on; break;
    case 12: CH12_on; break;
    case 13: CH13_on; break;
    case 14: CH14_on; break;
    case 15: CH15_on; break;
  }
}

void ServoOff(uint8_t ch)
{
  switch(ch)
  {
    case 0: CH0_off; break;
    case 1: CH1_off; break;
    case 2: CH2_off; break;
    case 3: CH3_off; break;
    case 4: CH4_off; break;
    case 5: CH5_off; break;
    case 6: CH6_off; break;
    case 7: CH7_off; break;
    case 8: CH8_off; break;
    case 9: CH9_off; break;
    case 10: CH10_off; break;
    case 11: CH11_off; break;
    case 12: CH12_off; break;
    case 13: CH13_off; break;
    case 14: CH14_off; break;
    case 15: CH15_off; break;
  }
}

void UpdatePPMA()
{
  if (State_chA==0)
  {
    CH1_on;
    State_chA=1;
    if (curr_chA<ChannelsBankA){
      if (ChannelsMapA[curr_chA] < 16) OCR1A = TCNT1 + (sBus.channels[ChannelsMapA[curr_chA]]+2000-400);
      else OCR1A = TCNT1 + sBus.Failsafe() * 2000 + 2000-400;
    }
      //OCR1A = TCNT1 + SET_TIME_PPM_A(curr_chA);
  }
  else
  {
    CH1_off;
    State_chA=0;
    OCR1A = TCNT1 + 400;
    curr_chA++;
  }
}

void UpdatePPMB()
{
  if (State_chB==0)
  {
    CH9_on;
    State_chB=1;
    if (curr_chB<ChannelsBankB){
      if (ChannelsMapB[curr_chB] < 16) OCR1B = TCNT1 + (sBus.channels[ChannelsMapB[curr_chB]]+2000-400);
      else OCR1B = TCNT1 + sBus.Failsafe() * 2000 + 2000-400;
    }
      //OCR1B = TCNT1 + SET_TIME_PPM_B(curr_chB);
  }
  else
  {
    CH9_off;
    State_chB=0;
    OCR1B = TCNT1 + 400;
    curr_chB++;
  }
}
/*
byte Set_TimeA(byte pos)
{
  if (ChannelsMapA[pos] < 16) return (sBus.channels[ChannelsMapA[pos]]+2000);
  else return sBus.Failsafe() * 2000 + 2000;
}

byte Set_TimeB(byte pos)
{
  if (ChannelsMapB[pos] < 16) return (sBus.channels[ChannelsMapB[pos]]+2000);
  else return sBus.Failsafe() * 2000 + 2000;
}
*/
/*
byte Set_Time_PPMA(byte pos)
{
  if (pos < 16) return (sBus.channels[ChannelsMapA[pos]]+2000-400);
  else return sBus.Failsafe() * 2000 + 2000-400;
}

byte Set_Time_PPMB(byte pos)
{
  if (pos < 16) return (sBus.channels[ChannelsMapB[pos]]+2000-400);
  else return sBus.Failsafe() * 2000 + 2000-400;
}*/

