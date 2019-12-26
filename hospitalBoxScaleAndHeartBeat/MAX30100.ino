//#include <Wire.h>
//#include "MAX30100_PulseOximeter.h"
//#include "MAX30100.h"
//
//#define REPORTING_PERIOD_MS     1000
//
//// Sampling is tightly related to the dynamic range of the ADC.
//// refer to the datasheet for further info
//#define SAMPLING_RATE                       MAX30100_SAMPRATE_100HZ
//
//// The LEDs currents must be set to a level that avoids clipping and maximises the
//// dynamic range
//#define IR_LED_CURRENT                      MAX30100_LED_CURR_50MA
//#define RED_LED_CURRENT                     MAX30100_LED_CURR_27_1MA
//
//// The pulse width of the LEDs driving determines the resolution of
//// the ADC (which is a Sigma-Delta).
//// set HIGHRES_MODE to true only when setting PULSE_WIDTH to MAX30100_SPC_PW_1600US_16BITS
//#define PULSE_WIDTH                         MAX30100_SPC_PW_1600US_16BITS
//#define HIGHRES_MODE                        true
//
//
//
//PulseOximeter pox;
//uint32_t lastTimeReadHBOX;
//MAX30100 sensorHBOX;
//
//void onBeatDetected() {
//  PRINT_DEBUG(Serial.println("Beat"));
//}
//
//void HB_OXY_init() {
//  if (!sensorHBOX.begin()) {
//    PRINT_DEBUG(Serial.println("FAILED"));
////    for (;;);
//  thisNode.nodeStatus = HEARTBEAT_ERROR;
//  } else {
//    PRINT_DEBUG(Serial.println("SUCCESS"));
//  }
//
//
//  // Set up the wanted parameters
//  sensorHBOX.setMode(MAX30100_MODE_SPO2_HR);
//  sensorHBOX.setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT);
//  sensorHBOX.setLedsPulseWidth(PULSE_WIDTH);
//  sensorHBOX.setSamplingRate(SAMPLING_RATE);
//  sensorHBOX.setHighresModeEnabled(HIGHRES_MODE);
//
//  //
//  if (!pox.begin()) {
//    PRINT_DEBUG(Serial.println("Init MAX30100 FAILED"));
//  }
//  else {
//    PRINT_DEBUG(Serial.println("Init MAX30100 SUCCESS"));
//  }
//  //  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
//
//  pox.setIRLedCurrent(MAX30100_LED_CURR_20_8MA);
//  //  pox.setOnBeatDetectedCallback(onBeatDetected);
//
//}
//
//boolean HB_OXY_check() {
//  uint16_t ir, red;
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//
//  sensorHBOX.getRawValues(&ir, &red); //need while here?
//
//  PRINT_DEBUG(Serial.println(ir));
//  if ( ir > max30100_base) {
//    return true;
//  }
//  return false;
//}
//
//void HB_OX_update_base() {
//  uint16_t ir, red;
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.update();
//  sensorHBOX.getRawValues(&ir, &red); //need while here?
//  max30100_base = ir * 3 ;
//  PRINT_DEBUG(Serial.print("Update HBOX Base:  "));
//  PRINT_DEBUG(Serial.println(max30100_base));
//}
//
//void HB_OXY_read() {
//  pox.update();
//  if (millis() - lastTimeReadHBOX > REPORTING_PERIOD_MS) {
//    thisNode.heartBeat = pox.getHeartRate();
//    thisNode.SpO2 = pox.getSpO2();
//    //    Serial.print("Heart rate:");
//    //    Serial.print(thisNode.heartBeat);
//    //    Serial.print("bpm / SpO2:");
//    //    Serial.print(thisNode.SpO2);
//    //    Serial.println("%");
//
//    lastTimeReadHBOX = millis();
//  }
//}
