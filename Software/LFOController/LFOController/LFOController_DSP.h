#ifndef DWG_DSP_H_
#define DWG_DSP_H_

//#define LOOKUP_TABLE_ENABLED

enum DSP_EWAVEFORMS { SINE, SQUARE, SAW, TRIANGLE, STEP, INVSTEP, PULSE, UNDEFINED } ;
DSP_EWAVEFORMS const DSP_Waveforms[] = { SINE, SQUARE, SAW, TRIANGLE, STEP, INVSTEP, PULSE, UNDEFINED };
int DSP_Waveform;

// sine wave lookup table and sample rate
const int DSP_SampleResolution = 256;  
float DSP_WaveformLUT[DSP_SampleResolution];

// counter variable used by all waveforms
int DSP_WaveIndex;

void DSP_Setup() {

  DSP_Waveform = SINE;
}

//////////////////////////////////////////////////////
//// DSP_CreateSineTable
//////////////////////////////////////////////////////
void DSP_CreateSineTable() {
  for(int _WaveIndex = 0; _WaveIndex < DSP_SampleResolution; _WaveIndex++) {
    
    
    //int multiplier=1000;
    //int offset=1000;
    
    DSP_WaveformLUT[_WaveIndex] = (cos(0.99*((2.0*PI)/DSP_SampleResolution)*_WaveIndex));
    
    //WaveformLUT[LUT_Index] = multiplier * (cos(((2.0*PI)/sampleResolution)*LUT_Index));
  }     
}

void DSP_CreateSquareTable() {
  ////////////////////////////////////////////////////////
  // generate square lookup table
  for(int _WaveIndex=0; _WaveIndex<DSP_SampleResolution; _WaveIndex++) {  
    int _max=2000;
    if      (_WaveIndex < (DSP_SampleResolution*0.25))       { DSP_WaveformLUT[_WaveIndex] = _max; }
    else if (_WaveIndex < (DSP_SampleResolution*0.5))        { DSP_WaveformLUT[_WaveIndex] = 0; }
    else if (_WaveIndex < (DSP_SampleResolution*0.75))       { DSP_WaveformLUT[_WaveIndex] = _max; }
    else if (_WaveIndex < (DSP_SampleResolution))            { DSP_WaveformLUT[_WaveIndex] = 0; } 
  }
}

void DSP_CreateSawTable() {
  /////////////////////////////////////////////////////////
  // generate saw lookup table
  for(int _WaveIndex=0; _WaveIndex<DSP_SampleResolution; _WaveIndex++) {  
    int _multipler=10;
    int _offset=1000;
    if      (_WaveIndex < (DSP_SampleResolution*0.5))        { DSP_WaveformLUT[_WaveIndex] = _WaveIndex *_multipler;  }
    else if (_WaveIndex < DSP_SampleResolution)            { DSP_WaveformLUT[_WaveIndex] = ((_WaveIndex *_multipler) - _offset); }
  }
}

void DSP_CreateTriangleTable() {
  ///////////////////////////////////////////////////////////
  // generate triangle lookup table
  for(int _WaveIndex=0; _WaveIndex<DSP_SampleResolution; _WaveIndex++) {
    int _multiplier=32;
    if      (_WaveIndex < (DSP_SampleResolution*0.25))       { DSP_WaveformLUT[_WaveIndex] = _WaveIndex * _multiplier; }
    else if (_WaveIndex < (DSP_SampleResolution*0.5))        { 
     
      int _exponent = ( _WaveIndex- (( DSP_SampleResolution*0.25 )));      // exponent count starts at zero from 1/4 through LORES
      DSP_WaveformLUT[_WaveIndex] = ((DSP_WaveIndex * _multiplier) - ((_multiplier*_exponent)*2 ) ) ; 
      
    }
    else if (_WaveIndex < (DSP_SampleResolution*0.75))       { DSP_WaveformLUT[_WaveIndex] = _WaveIndex * _multiplier; }
    else if (_WaveIndex < (DSP_SampleResolution))        { 
      
      int _exponent = ( _WaveIndex- (( DSP_SampleResolution*0.75 )));     // exponent count starts at zero from 3/4 through LORES
      DSP_WaveformLUT[_WaveIndex] = ((_WaveIndex * _multiplier) - ((_multiplier*_exponent)*2 ) ) ; 
      
    }
  }
}

void DSP_SwitchWaveform() {

  switch(DSP_Waveform) {

      case SINE:
        DSP_CreateSineTable();
        break;
      case SQUARE:
        DSP_CreateSquareTable();
        break;
      case SAW:
        DSP_CreateSquareTable();
        break;
      case TRIANGLE:
        DSP_CreateTriangleTable();
        break;
    }
}

//////////////////////////////////////////////////////
//// DSP_GetSampleData
//////////////////////////////////////////////////////


int DSP_GetLookupTableWaveData(DSP_EWAVEFORMS _Waveform, int _AmplitudeInput = 1000, int _OffsetInput = 1000) {

  
      switch(_Waveform) {
          case SINE:
            
            
            return _AmplitudeInput* DSP_WaveformLUT[DSP_WaveIndex] + _OffsetInput;
            
          case SQUARE:
            
            
            return _AmplitudeInput* DSP_WaveformLUT[DSP_WaveIndex] + _AmplitudeInput;
           
          case SAW:
            
            return _AmplitudeInput* DSP_WaveformLUT[DSP_WaveIndex];
            
          case TRIANGLE:
            
            return _AmplitudeInput* DSP_WaveformLUT[DSP_WaveIndex];
           
      }

}

int DSP_GetAlgorithmicWaveData(DSP_EWAVEFORMS _Waveform, int _AmplitudeInput = 500, int _OffsetInput = 1200) {
    
      int _triHalfPeriod=128;
      float _tri_temp=0;
      int _inv_temp=0;
      //// SINE
      ///////////////////////////////////
      switch(_Waveform) {
          case SINE:
            _AmplitudeInput = 725;
            _OffsetInput = 750;
            return (_AmplitudeInput) * DSP_WaveformLUT[DSP_WaveIndex] + _OffsetInput;
            break;
            
          case SQUARE:
            _AmplitudeInput = 750;
            _OffsetInput = 0;
            return (DSP_WaveIndex++ % 256) < 128 ? (_AmplitudeInput*2) : 0;
            break;
          
          case SAW:
            _AmplitudeInput = 22;
            _OffsetInput = 0;
            return _AmplitudeInput * (DSP_WaveIndex/4) + _OffsetInput;
            break;
          
          case TRIANGLE:  
            _AmplitudeInput = 11;
            _OffsetInput = 0;
            _tri_temp = (_triHalfPeriod - abs(DSP_WaveIndex % (2 * _triHalfPeriod) - _triHalfPeriod) );
            return (_AmplitudeInput) * _tri_temp + _OffsetInput;
            break;
          
          case STEP:
            _AmplitudeInput = 470;
            _OffsetInput = 0;
            
            return _AmplitudeInput * (abs((DSP_WaveIndex++ % 4) - 3)) + _OffsetInput ;
            break;
          
          case INVSTEP:
           _AmplitudeInput = 470;
            _OffsetInput = 0;
            _inv_temp = DSP_WaveIndex /4;
            return _AmplitudeInput * (abs((_inv_temp++ % 4) - 3)) + _OffsetInput ;
            break;

          case PULSE:
            _AmplitudeInput = 750;
            _OffsetInput = 0;
            return (DSP_WaveIndex++ % 200) > 128 ? (_AmplitudeInput*2) : 0;
            break;
      }
      
 
}





#endif 
