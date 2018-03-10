#ifndef DWG_SPI_H_
#define DWG_SPI_H_

#include "SPI.h"

#include "LFOController_DSP.h"
#include "LFOController_ADC.h"
#include "LFOController_PCI.h"

//// SPI PIN Definitions
int _MOSI = 3;
int _SCK = 5;
int _SS = 2;

///////////////////////////////
// DAC/SPI specific settings - MCP4921 datasheet http://ww1.microchip.com/downloads/en/DeviceDoc/22248a.pdf
//
// config portion of MCP4921 command register, first 4 bits:
// note that MSB=1 sets ignore flag and LSB=0 sets shutdown

#define SPI_DACSHUTDOWN      0x00
#define SPI_DACUNBUFFX2      0x01
#define SPI_DACUNBUFFX1      0x03
#define SPI_DACBUFFRDX2      0x05
#define SPI_DACBUFFRDX1      0x07
#define SPI_DACIGNORE        0x08

///////////////////////////////////////////////////////
////    void setupSPI()
///////////////////////////////////////////////////////
void SPI_Setup() {
  
  unsigned char sreg;   // save status register and disable interrupts
  sreg = SREG;          //
  cli();                //

    DDRB |= _BV(_SS);               // set SS pin to output before enabling SPI (req. for master)
    
    SPCR |=  _BV(SPE) | _BV(MSTR);  // enable spi as master
    
    DDRB |= _BV(_SCK) | _BV(_MOSI); // set pins to output

  sei();                // enable interrupts and restore status register 
  SREG = sreg;          //
  
}


///////////////////////////////////////////////////////
////    void sendSPIData()
///////////////////////////////////////////////////////
void SPI_SendData() {
  
  PORTB = ~_BV(_SS);    // begin 16bit DAC packet. set DAC slave select LOW
  
  word _newPacket = SPI_DACUNBUFFX1;                           // newPacket = 000000000000DCMD
  
  //newPacket = (newPacket << 12) | WaveformLUT[LUT_Index];   // newPacket = DCMDWAVEFORMDATA 
  #ifdef LOOKUP_TABLE_ENABLED
    int _sample = DSP_GetLookupTableWaveData(DSP_Waveform);
    //Serial.println(DSP_WaveIndex);
  #else
    int _sample = DSP_GetAlgorithmicWaveData(DSP_Waveform);
    //Serial.println(_sample);
  #endif
  
  _newPacket = (_newPacket << 12) | _sample;
  SPDR = _newPacket >> 8;                                  // send DCMDWAVE
  while (!(SPSR & _BV(SPIF))) ; // wait for slave ack
  SPDR = _newPacket & 0xFF;                                // send FORMDATA
  while (!(SPSR & _BV(SPIF))) ; // wait for slave ack
 
  (DSP_WaveIndex < DSP_SampleResolution) ? DSP_WaveIndex++ : DSP_WaveIndex=0;   // increment the lookup table  
  // sampleResolution is only effective for SINE
  
  
  PORTB = _BV(_SS);     // 16bit DAC packet finished. set DAC slave select HIGH 
}

#endif
