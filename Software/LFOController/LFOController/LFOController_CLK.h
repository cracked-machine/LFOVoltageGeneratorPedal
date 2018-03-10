#ifndef DWG_CLK_H_
#define DWG_CLK_H_

///////////////////////////////////////////////////////
////    void setupTimer()
///////////////////////////////////////////////////////
void CLK_Setup() {
  unsigned char _sreg;   // save status register and disable interrupts
  _sreg = SREG;          //
  cli();                //
  
    TCCR1B = _BV(WGM12) | _BV(CS10);  // enable CTC mode, no prescaler, 
    TCCR1A = 0;                       // no compare output 
  
    // Timer1 interrupt upper limit
    // with prescaler set to x1
    //OCR1A = 32;        // ~205KHz
    //OCR1A = 64;     // ~120KHz
    //OCR1A = 256;    // ~32KHz
    OCR1A = 4096;   // ~2KHz
    //OCR1A = 65535;  // ~12Hz
    TIMSK1 = _BV(OCIE1A);       // enable Timer1 Compare Match A interrupt
    
  sei();            
  SREG = _sreg;
}

void CLK_ToggleBuiltInLed() {
  // don't forget to make pin13 output!
  if(digitalRead(13) == HIGH) {
    digitalWrite(13, LOW);
  }
  else {
    digitalWrite(13, HIGH);
  }
}


#endif
