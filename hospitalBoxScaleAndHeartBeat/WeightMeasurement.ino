#include "HX711.h"

#define LOADCELL_DOUT_PIN  4
#define LOADCELL_SCK_PIN   5


#define   NUMBER_OF_SHIFT_BIT   0
#define   NUMBER_OF_SAMPLES  (1 << NUMBER_OF_SHIFT_BIT)
#define   SCALE_ADC_VALUE    0
#define   ADC_VALUE_OF_MAX_KG    2078000
#define   ADC_VALUE_OF_0_KG       -82000
#define   KG714                 71400
#define   NUMBER_OF_INDIVIDUALS   10
#define   OFFSET_ERROR            2500

long hx711Value = 0;
long arrayOfHx711Value[NUMBER_OF_SAMPLES];
float arrayOfWeightValues[NUMBER_OF_INDIVIDUALS];
long currentHx711Value = 0;
long scaleOffset = ADC_VALUE_OF_0_KG;
uint8_t arrayIndex = 0;
uint8_t indexOfArrayOfWeightValue = 0;
float scaleFactor = 0.0;
float currentWeight = 0.0;
const uint8_t errorWeight = 1;

enum Scale_State {
  SCALE_INIT = 0,
  SET_OFFSET,
  WAIT_FOR_SIGNAL_STABLE,
  KG_CALCULATION,
  WAIT_FOR_NEXT_SCALE
};

enum Scale_State scaleState = SCALE_INIT;

void computeScaleFactor(void);
void setScaleOffset(void);
void computeScaleOffset(void);
//void updateHX711Value(void);
//long getAverageHx711Value(void);
//float kgCalculation(void);
//void scaleFSM(void);


HX711 scale;


void scale_init() {
  PRINT_DEBUG(Serial.println("Setting Scale"));
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  computeScaleOffset();
  computeScaleFactor();
}

boolean scale_check() {
  float returnValue = scale.get_units(4);
  scaleReadBuffer[scaleReadBufferIndex++] = returnValue;
  if (scaleReadBufferIndex >= SCALE_BUFFER_MAX)       scaleReadBufferIndex = 0;
  if (returnValue > 2 ) { //>1837xx
    return true;
  }
  return false;
}

float scale_read(uint8_t nbrReadTimes) {
  float sum;
  byte i;
  float returnValue = scale.get_units(1);//= scale.get_units(nbrReadTimes);
  PRINT_DEBUG(Serial.println(scale.read()));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)
  for ( i = 0; i < nbrReadTimes; i++) {
    returnValue = scale.get_units(1);
    scaleReadBuffer[scaleReadBufferIndex] = returnValue;
    scaleReadBufferIndex++;
    if (scaleReadBufferIndex >= SCALE_BUFFER_MAX) {
      scaleReadBufferIndex = 0;
    }
  }
  sum = 0;
  for ( i = 0; i < SCALE_BUFFER_MAX; i++) {
    sum += scaleReadBuffer[i] ;
  }
  thisNode.weight = sum / SCALE_BUFFER_MAX;
  return returnValue;
}



float checkWeightValues(void){
  uint8_t i;
  for(i = 0; i < NUMBER_OF_INDIVIDUALS; i++){
    if((currentWeight <= arrayOfWeightValues[i] + errorWeight) && 
    (currentWeight >= arrayOfWeightValues[i] - errorWeight)){
      return arrayOfWeightValues[i];
    }
  }
  indexOfArrayOfWeightValue = (indexOfArrayOfWeightValue + 1) % NUMBER_OF_INDIVIDUALS;
  arrayOfWeightValues[indexOfArrayOfWeightValue] = currentWeight;
  return currentWeight;
}




long getAverageHx711Value(void){
  uint8_t i;
  long sum = 0;
  for (i = 0; i < NUMBER_OF_SAMPLES; i ++){
    sum = sum + arrayOfHx711Value[i];
  }
  return sum >> NUMBER_OF_SHIFT_BIT;
}

void computeScaleFactor(void){
  scaleFactor = (float)(ADC_VALUE_OF_MAX_KG - ADC_VALUE_OF_0_KG)/KG714;
  Serial.print("scaleFactor = ");
  Serial.println(scaleFactor);
  
}
void setScaleOffset(void){
  scaleOffset = getAverageHx711Value();
  Serial.print("scaleOffset = ");
  Serial.println(scaleOffset);
}
void computeScaleOffset(void){
  uint8_t i;
  for(i = 0; i < 20; i ++){
    updateHX711Value();
    delay(50);
  }
  setScaleOffset();  
}
void updateHX711Value(void){
  if (scale.is_ready()) {
    currentHx711Value = scale.read();
    arrayOfHx711Value[arrayIndex] =  currentHx711Value >> SCALE_ADC_VALUE;
    arrayIndex = (arrayIndex + 1) % NUMBER_OF_SAMPLES;
  }
}




float kgCalculation(void){
//  Serial.println(getAverageHx711Value());
//  Serial.println(scaleOffset);
//  Serial.println(scaleFactor);
  currentWeight = (float)(getAverageHx711Value() - scaleOffset)/(scaleFactor * 1000);
  return currentWeight;
}

uint16_t scaleFSMCounter = 0;
void scaleFSM(void){
  scaleFSMCounter ++;
  switch(scaleState){
    case SCALE_INIT:
      if(scaleFSMCounter >= 50){
        scaleFSMCounter = 0;
        scaleState = SET_OFFSET;  
        Serial.println("SET_OFFSET ");
      } 
    break;
    case SET_OFFSET:
      kgCalculation();
//      Serial.print("SET_OFFSET kgFinal = ");
//      Serial.println(kgFinal);
      if(currentWeight > 3.0){
        scaleState = WAIT_FOR_SIGNAL_STABLE;
        Serial.println("WAIT_FOR_SIGNAL_STABLE");
        scaleFSMCounter = 0;
      } else {
        if(scaleFSMCounter > 20){
            if((currentHx711Value >  scaleOffset - OFFSET_ERROR) && 
            (currentHx711Value <  scaleOffset + OFFSET_ERROR)){
              setScaleOffset();
            }
          } 
          scaleFSMCounter = 0;
      }
      
    break;
    case WAIT_FOR_SIGNAL_STABLE:
      if(scaleFSMCounter >= 50){
        scaleState = KG_CALCULATION;
        
        Serial.println("KG_CALCULATION");
      }
    break;
    case KG_CALCULATION:
    Serial.print("scaleOffset = ");
    Serial.println(scaleOffset);
    Serial.print("kgCalculation = ");
    Serial.println(kgCalculation());
    Serial.print("checkWeightValues = ");
    Serial.println(checkWeightValues());
    scaleState = WAIT_FOR_NEXT_SCALE;
    Serial.println("WAIT_FOR_NEXT_SCALE");
    break;
    case WAIT_FOR_NEXT_SCALE:
//    Serial.print("kgFinal = ");
//      Serial.println(kgFinal);
    if(kgCalculation() < 5.0){
      scaleState = SET_OFFSET;
      Serial.println("SCALE_INIT");
    }
    break;
  }
  
}
