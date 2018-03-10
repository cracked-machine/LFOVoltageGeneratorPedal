#ifndef DWG_ADC_H_
#define DWG_ADC_H_

int ADCInput;
int ADC_FreqInput = 0;      // ADC0
int ADC_AmplitudeInput = 0;      // ADC1
int ADC_OffsetInput = 0;      // ADC2

///////////////////////////////////////////////////////
////    void setupADC()
///////////////////////////////////////////////////////

void ADC_Setup() {
  
  
  ADMUX = 0;                          // AREF used(allows full pot range), ADC0 input enabled
  ADCSRA |= _BV(ADEN);                // Enable ADC
  ADCSRA |= _BV(ADATE);               // Enable ADC Auto-Trigger
  ADCSRA |= _BV(ADIE);                // Enable ADC ISR
  ADCSRA |= _BV(ADPS0);               // Set prescaler to /2
  
  ADCSRB = 0;                         // free running mode
  ADCSRA |= _BV(ADSC);                // Enable ADC conversions (must be done after ADEN)
}

#endif
