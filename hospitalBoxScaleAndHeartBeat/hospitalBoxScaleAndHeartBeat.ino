
#define PRINT_DEBUG(x)    x
#define PRINT_DEBUG_GRAPH(x)    //x
#define PRINT_LCD(x)      x

#define FSM_MAIN_BEGIN        0
#define FSM_MAIN_READ_MAIN    1
#define FSM_MAIN_SCALE_CHECK  6
#define FSM_MAIN_SCALE        2
#define FSM_MAIN_CHECK_HBOX   7
#define FSM_MAIN_HBOX         5
#define FSM_MAIN_SHOW_SERIAL  3
#define FSM_MAIN_SHOW_LCD     4
#define FSM_MAIN_FINISH       8
#define FSM_MAIN_SCALE_PHASE1 9
#define FSM_MAIN_SCALE_PHASE2 10
#define FSM_MAIN_SCALE_PHASE3 11



#define SCALE_BUFFER_MAX  20
//////////
enum BoxStatus {
  NORMAL,
  SCALE_ERROR,
  HEARTBEAT_ERROR,
  SPO2_ERROR,
  LCD_ERROR,
  COMMUNICATION_ERROR
};

struct hospitalBoxNode {
  float weight;
  uint8_t heartBeat;
  uint8_t SpO2;
  BoxStatus nodeStatus;
};
//////
struct scaleStatus {
  uint32_t offset;
  uint32_t scale;
  float   threshold;
  uint32_t rawValue;
  float   weightValue;
};
scaleStatus thisScale;

union uweight {
  float fweight;
  byte  bweight[4] ;
};

uint8_t fsmMode;
uint32_t scaleTareValue ;
uint32_t countReadHBOX, countReadScale;
uint32_t lastTimeReadHBOXFSM, lastTimeReadScaleFSM;
//float bufferReadWeight[100] ;
uint32_t timeOutCheckScale;

uint32_t scaleReadBuffer[SCALE_BUFFER_MAX] ;
uint8_t scaleReadBufferIndex = 0 ;
hospitalBoxNode thisNode = {0, 0, 0, NORMAL};
uint16_t max30100_base;
byte weightBufferCompare[3] ;
uint32_t lastTimeSendToMain = 0;


void fsm(void){
  // put your main code here, to run repeatedly:
  //
  switch (fsmMode) {
    case FSM_MAIN_BEGIN:
      lastTimeReadHBOXFSM = millis();
      lastTimeReadScaleFSM = millis();
      fsmMode = FSM_MAIN_READ_MAIN ; // FSM_MAIN_SCALE_CHECK;//FSM_MAIN_HBOX ;//FSM_MAIN_SCALE;
      break;
    case FSM_MAIN_READ_MAIN:
      communication();
      fsmMode = FSM_MAIN_SCALE_CHECK;
      if (millis() - lastTimeSendToMain > 2000) {
        //        rs_485_send(thisNode.weight);
        union uweight tmpWeight;
        tmpWeight.fweight = thisNode.weight;

        rs485_send('*');
        rs485_send(tmpWeight.bweight[3]);
        rs485_send(tmpWeight.bweight[2]);
        rs485_send(tmpWeight.bweight[1]);
        rs485_send(tmpWeight.bweight[0]);
        rs485_send('#');
        lastTimeSendToMain = millis();
      }
      break;
    case FSM_MAIN_SCALE_CHECK:
      if (scale_check()) {
        fsmMode = FSM_MAIN_SCALE_PHASE1;
        PRINT_DEBUG(Serial.println("Check Scale OK"));
        timeOutCheckScale = millis();
      }
      else {
        PRINT_DEBUG(Serial.println("Check Scale NO"));
        //        fsmMode = FSM_MAIN_CHECK_HBOX;
        fsmMode = FSM_MAIN_BEGIN;
        //        thisNode.weight = 0;
        //        thisScale.weightValue = 0;
      }
      scale_read(1);
      break;
    case FSM_MAIN_SCALE_PHASE1:
      scale_read(1);
      weightBufferCompare[2] = byte(thisNode.weight);
//      lcd_print_weight(thisNode.weight);
      fsmMode = FSM_MAIN_SCALE_PHASE2;
      break;
    case FSM_MAIN_SCALE_PHASE2:
      scale_read(1);
      weightBufferCompare[1] = weightBufferCompare[2];
//      weightBufferCompare[2] = byte(thisNode.weight);
//      lcd_print_weight(thisNode.weight);
      fsmMode = FSM_MAIN_SCALE_PHASE3;
      break;
    case FSM_MAIN_SCALE_PHASE3:
      scale_read(1);
//      lcd_print_weight(thisNode.weight);
      weightBufferCompare[0] = weightBufferCompare[1];
      weightBufferCompare[1] = weightBufferCompare[2];
      weightBufferCompare[2] = byte(thisNode.weight);
      if (weightBufferCompare[0] == weightBufferCompare[1]) { // && weightBufferCompare[1] == weightBufferCompare[2]) {
        fsmMode = FSM_MAIN_SCALE;
      }
      else {
        fsmMode = FSM_MAIN_SCALE_CHECK;
      }
      if (millis() - timeOutCheckScale > 10000) {
        fsmMode = FSM_MAIN_CHECK_HBOX;
      }
      break;
    case FSM_MAIN_SCALE: //store to buffer then calculate kg
//      lcd_print_weight(thisNode.weight);
      fsmMode = FSM_MAIN_SHOW_LCD;  //FSM_MAIN_CHECK_HBOX;
      break;
    case FSM_MAIN_CHECK_HBOX:
      //      if (HB_OXY_check()) {
      ////        PRINT_DEBUG(Serial.println("Check HBOX OK"));
      //        fsmMode = FSM_MAIN_HBOX;
      //      }
      //      else {
      ////        PRINT_DEBUG(Serial.print("Check HBOX NO"));
      //        thisNode.heartBeat = 0;
      //        thisNode.SpO2 = 0;
      //        fsmMode = FSM_MAIN_SHOW_SERIAL;
      //        thisNode.weight = 0;
      //      }
      //      fsmMode = FSM_MAIN_HBOX;
      break;
    case FSM_MAIN_HBOX: //read atleast 1 pulse then calculate HB
//      HB_OXY_read();
      //      countReadHBOX++;
      if (millis() - lastTimeReadHBOXFSM > 3000) {
        //        countReadHBOX = 0;
        fsmMode = FSM_MAIN_SHOW_SERIAL;

      }
      break;
    case FSM_MAIN_SHOW_SERIAL:
      //      PRINT_DEBUG(Serial.println(thisNode.weight));
      //      PRINT_DEBUG(Serial.println(""));
      //      PRINT_DEBUG(Serial.print("Weight: "));
      //      PRINT_DEBUG(Serial.println(thisNode.weight));
      //      PRINT_DEBUG(Serial.print("HB: "));
      //      PRINT_DEBUG(Serial.println(thisNode.heartBeat));
      //      PRINT_DEBUG(Serial.print("SpO2: "));
      //      PRINT_DEBUG(Serial.println(thisNode.SpO2));
      //      PRINT_DEBUG(Serial.println(""));
      fsmMode = FSM_MAIN_SHOW_LCD;
      break;
    case FSM_MAIN_SHOW_LCD:
//      PRINT_LCD(LCD_print_all(thisNode.weight));
      fsmMode = FSM_MAIN_BEGIN;
      break;
    case FSM_MAIN_FINISH:
      fsmMode = FSM_MAIN_BEGIN;
      break;
    default:
      fsmMode = FSM_MAIN_BEGIN;
      break;
  }
//  PRINT_DEBUG_GRAPH(Serial.println(thisNode.weight));
}


void setup() {
  // put your setup code here, to run once:
  PRINT_DEBUG(Serial.begin(115200));
  PRINT_DEBUG_GRAPH(Serial.begin(115200));
//  PRINT_LCD(LCD_init());
  communication_init();
  scale_init();
//  HB_OXY_init();

  fsmMode = FSM_MAIN_BEGIN;
  countReadHBOX = 0;
  countReadScale = 0;
//  HB_OX_update_base();
//  HB_OXY_check();
//  LCD_print_init();
}

uint8_t counter = 0;
//float scaleValue;
void loop() {

  //HB_OXY_read();
  updateHX711Value();
  scaleFSM();
  delay(50);
//  counter = (counter + 1) % 10;
//  if(counter == 0){
//    Serial.print("Hx711Value");
//    Serial.println(getAverageHx711Value());
//    Serial.print("Kg:");
//    Serial.println(kgCalculation()/1000);
//  }
}
