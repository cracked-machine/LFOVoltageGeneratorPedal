
// DWG auxiliary source
#include "LFOController_CLK.h"
#include "LFOController_SPI.h"




unsigned long DWG_LastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long DWG_DebounceDelay = 400;    // the debounce time; increase if the output flickers
volatile uint8_t DWG_Portdhistory = 0xFF; 

void setup() {
  //Serial.begin(9600);
  DSP_SwitchWaveform();
  
  PCI_Setup();
  //CLK_Setup();
  DSP_Setup();
  SPI_Setup();
  ADC_Setup();
  
}

void loop() {
  
   // reset SPI pins...?
    DDRB |= _BV(_SS);               // set SS pin to output before enabling SPI (req. for master)
    SPCR |=  _BV(SPE) | _BV(MSTR);  // enable spi as master
    //DDRB |= _BV(_SCK) | _BV(_MOSI); // set pins to output
 
   SPI_SendData();
  delayMicroseconds(ADC_FreqInput);
  //Serial.println(DSP_Waveform);
  PCI_SetMUX();
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////  ITERRUPT SERVICE ROUTINES
/////////////////////////////////////////////
/////////////////////////////////////////////

//ISR(TIMER1_COMPA_vect) {
  //CLK_ToggleBuiltInLed();
//}

// Interrupt service routine for the ADC completion
ISR(ADC_vect){
  
  
  ADCInput = ADCL;
  ADCInput += ADCH << 8;

  
   switch(ADMUX) {
    case 0x00:
      ADC_FreqInput = ADCInput;
     // ADMUX = 0x01;
    break;
    
    /*case 0x01:
      ADC_FreqInput = ADCInput;
      ADMUX = 0x00;
    break;
    */
    
    
  }
  ADCSRA |= _BV(ADSC);
 
}

ISR(PCINT2_vect) {  
  //if ((millis() - DWG_LastDebounceTime) > DWG_DebounceDelay) {
    //uint8_t _changedbits;
  
    //_changedbits = PIND ^ DWG_Portdhistory;
    //DWG_Portdhistory = PIND;
    
    //if(_changedbits & _BV(PCI_WaveIncr)) {
      //increment the waveform in the program sequence 
   //   DWG_LastDebounceTime = millis();
      
    if(PIND & _BV(PCI_WaveIncrPin)) {
      
      (DSP_Waveform == UNDEFINED) ? DSP_Waveform = SINE : DSP_Waveform++; 
      //PIND |= ~_BV(MUXOUTA0);
      
      PCI_SetMUX();
     
    }
    

    //DSP_SwitchWaveform();
 //}
}

