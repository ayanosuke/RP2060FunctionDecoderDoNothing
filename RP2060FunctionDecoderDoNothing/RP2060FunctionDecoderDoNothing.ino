//--------------------------------------------------------------------------------
// DCC Function Decoder Do Nothing
// DCC信号を受信するだけで何もしないデコーダ
// [RP2060FunctionDecoderDoNothing.ino]
// Copyright (c) 2022 Ayanosuke(Maison de DCC) / Desktop Station
//
// http://maison-dcc.sblo.jp/ http://dcc.client.jp/ http://ayabu.blog.shinobi.jp/
// https://twitter.com/masashi_214
//
// DCC電子工作連合のメンバーです
// https://desktopstation.net/tmi/ https://desktopstation.net/bb/index.php
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//
// 2022/1/23 DCCINはGP2
//--------------------------------------------------------------------------------
//

#include <arduino.h>
#include "DccCV.h"
#include "NmraDcc.h"

#define DEBUG      //リリースのときはコメントアウトすること

//各種設定、宣言

//使用クラスの宣言
NmraDcc   Dcc;
DCC_MSG  Packet;

unsigned char cvtblePrev[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

struct CVPair {
  uint16_t  CV;
  uint8_t Value;
};

CVPair FactoryDefaultCVs [] = {
  {CV_MULTIFUNCTION_PRIMARY_ADDRESS, DECODER_ADDRESS}, // CV01
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, 0},               // CV09 The LSB is set CV 1 in the libraries .h file, which is the regular address location, so by setting the MSB to 0 we tell the library to use the same address as the primary address. 0 DECODER_ADDRESS
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB, 0},          // CV17 XX in the XXYY address
  {CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB, 0},          // CV18 YY in the XXYY address
  {CV_29_CONFIG, 128 },
  {CV_dummy,0},
};

void(* resetFunc) (void) = 0;  //declare reset function at address 0

uint8_t FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);

void notifyDccReset(uint8_t hardReset );



//------------------------------------------------------------------
// Arduino固有の関数 setup() :初期設定
//------------------------------------------------------------------
void setup()
{

  //GP25ピンを出力に設定　LED_BUILTIN 予約語でもいけます
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);

#if 0
  Serial.begin(115200);
  Serial.println("Hello,Smile Function Decoder");
#endif

#ifdef DEBUG
  Serial1.setRX(PIN_SERIAL1_RX);
  Serial1.setTX(PIN_SERIAL1_TX);
  Serial1.begin(115200);
  Serial1.println("Hello,Smile Function Decoder");
#endif

  //ファンクションの割り当てピン初期化

  Dccinit();

  //Reset task
  gPreviousL5 = millis();
}

//---------------------------------------------------------------------
// Arduino main loop
//---------------------------------------------------------------------
void loop() {
  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation

  Dcc.process();

  if ( (millis() - gPreviousL5) >= 10) {
    FunctionProcess();
    gPreviousL5 = millis();
  }
}


//---------------------------------------------------------------------
// FunctionProcess()
//---------------------------------------------------------------------
void FunctionProcess(){
// F0 受信時の処理
    if(gState_F0 == 0 && cvtblePrev[0] == ON){
      gpio_put(25, LOW);
      cvtblePrev[0] = OFF;    
    } else if(gState_F0 == 1 && cvtblePrev[0] == OFF){
      gpio_put(25, HIGH);
      cvtblePrev[0] = ON;      
    }

// F1 受信時の処理
    if(gState_F1 == 0 && cvtblePrev[1] == ON){
//     myDFPlayer.stop();
      cvtblePrev[1] = OFF;    
    } else if(gState_F1 == 1 && cvtblePrev[1] == OFF){
//      myDFPlayer.play(2);
      cvtblePrev[1] = ON;      
    }
    
// F2 受信時の処理 gState_F2 に 2が入る
    if(gState_F2 == 0 && cvtblePrev[2] == ON){
//      myDFPlayer.stop();
      cvtblePrev[2] = OFF;  
    } else if(gState_F2 == 1 && cvtblePrev[2]== OFF){
//      myDFPlayer.play(3);
      cvtblePrev[2] = ON;  
    }

// F3 受信時の処理
    if(gState_F3 == 0 && cvtblePrev[3] == ON){
//      myDFPlayer.stop();
      cvtblePrev[3] = OFF;  
    } else if(gState_F3 == 1 && cvtblePrev[3]== OFF){
//      myDFPlayer.play(4);
      cvtblePrev[3] = ON;  
    }
    
// F4 受信時の処理
    if(gState_F4 == 0 && cvtblePrev[4] == ON){
//      myDFPlayer.stop();
      cvtblePrev[4] = OFF;  
    } else if(gState_F4 == 1 && cvtblePrev[4]== OFF){
//      myDFPlayer.play(5);
      cvtblePrev[4] = ON;  
    }
    
// F5 受信時の処理
    if(gState_F5 == 0 && cvtblePrev[5] == ON){
//      myDFPlayer.stop();
      cvtblePrev[5] = OFF;  
    } else if(gState_F5 == 1 && cvtblePrev[5]== OFF){
//      myDFPlayer.play(6);
      cvtblePrev[5] = ON;  
    }
    
// F6 受信時の処理
    if(gState_F6 == 0 && cvtblePrev[6] == ON){
//      myDFPlayer.stop();
      cvtblePrev[6] = OFF;  
    } else if(gState_F6 == 1 && cvtblePrev[6]== OFF){
//      myDFPlayer.play(7);
      cvtblePrev[6] = ON;  
    }

// F7 受信時の処理
    if(gState_F7 == 0 && cvtblePrev[7] == ON){
//      myDFPlayer.stop();
      cvtblePrev[7] = OFF;  
    } else if(gState_F7 == 1 && cvtblePrev[7]== OFF){
//      myDFPlayer.play(8);
      cvtblePrev[7] = ON;  
    }

// F8 受信時の処理
    if(gState_F8 == 0 && cvtblePrev[8] == ON){
//      myDFPlayer.stop();
      cvtblePrev[8] = OFF;  
    } else if(gState_F8 == 1 && cvtblePrev[8]== OFF){
//      myDFPlayer.play(9);
      cvtblePrev[8] = ON;  
    }

// F9 受信時の処理
    if(gState_F9 == 0 && cvtblePrev[9] == ON){
//      myDFPlayer.stop();
      cvtblePrev[9] = OFF;  
    } else if(gState_F9 == 1 && cvtblePrev[9]== OFF){
//      myDFPlayer.play(10);
      cvtblePrev[9] = ON;  
    }

// F10 受信時の処理
    if(gState_F10 == 0 && cvtblePrev[10] == ON){
//      myDFPlayer.stop();
      cvtblePrev[10] = OFF;  
    } else if(gState_F10 == 1 && cvtblePrev[10]== OFF){
//      myDFPlayer.play(11);
      cvtblePrev[10] = ON;  
    }
}




//---------------------------------------------------------------------
//DCC速度信号の受信によるイベント
//extern void notifyDccSpeed( uint16_t Addr, uint8_t Speed, uint8_t ForwardDir, uint8_t MaxSpeed )
//---------------------------------------------------------------------
extern void notifyDccSpeed( uint16_t Addr, DCC_ADDR_TYPE AddrType, uint8_t Speed, DCC_DIRECTION Dir, DCC_SPEED_STEPS SpeedSteps )
{
  uint16_t aSpeedRef = 0;
  //速度値の正規化(255を100%とする処理)
  if( Speed >= 1 ){
    aSpeedRef = ((Speed - 1) * 255) / SpeedSteps;
  } else {
    //緊急停止信号受信時の処理 //Nagoden comment 2016/06/11
    #ifdef DEBUG
        Serial1.println("***** Emagency STOP **** ");
    #endif
    aSpeedRef = 0;
  }


#ifdef DEBUG
    // デバッグメッセージ
    Serial1.print("Speed - ADR: ");
    Serial1.print(Addr);
    Serial1.print(", AddrType: ");
    Serial1.print(AddrType);
    Serial1.print(", SPD: ");
    Serial1.print(Speed);
    Serial1.print(", DIR: ");
    Serial1.print(Dir);
    Serial1.print(", SpeedSteps: ");
    Serial1.print(SpeedSteps);
    Serial1.print(", aSpeedRef: ");
    Serial1.println(aSpeedRef);
#endif

}


//---------------------------------------------------------------------------
//ファンクション信号受信のイベント
//FN_0_4とFN_5_8は常時イベント発生（DCS50KはF8まで）
//FN_9_12以降はFUNCTIONボタンが押されたときにイベント発生
//前値と比較して変化あったら処理するような作り。
//---------------------------------------------------------------------------
//extern void notifyDccFunc( uint16_t Addr, FN_GROUP FuncGrp, uint8_t FuncState)
extern void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState)
{

#ifdef DEBUG1
    // デバッグメッセージ
    Serial1.println("notifyDccFunc()");
    Serial1.print("Addr:");
    Serial1.print(Addr);
    Serial1.print(", AddrType:");
    Serial1.print(AddrType);
    Serial1.print(", FuncGrp: ");
    Serial1.print(FuncGrp,HEX);
    Serial1.print(", FuncState: ");
    Serial1.println(FuncState,HEX);
#endif
  
  if( FuncGrp == FN_0_4){  // F0〜F4の解析
    gState_F0 = (FuncState & FN_BIT_00) ? 1 : 0;
    gState_F1 = (FuncState & FN_BIT_01) ? 1 : 0;
    gState_F2 = (FuncState & FN_BIT_02) ? 1 : 0;
    gState_F3 = (FuncState & FN_BIT_03) ? 1 : 0;
    gState_F4 = (FuncState & FN_BIT_04) ? 1 : 0;
  }

  if( FuncGrp == FN_5_8){  // F5〜F8の解析
    gState_F5 = (FuncState & FN_BIT_05) ? 1 : 0;
    gState_F6 = (FuncState & FN_BIT_06) ? 1 : 0;
    gState_F7 = (FuncState & FN_BIT_07) ? 1 : 0;
    gState_F8 = (FuncState & FN_BIT_08) ? 1 : 0;
  }
  
  if( FuncGrp == FN_9_12) { // F9〜F12の解析
    gState_F9 = (FuncState & FN_BIT_09) ? 1 : 0;
    gState_F10 = (FuncState & FN_BIT_10) ? 1 : 0;
    gState_F11 = (FuncState & FN_BIT_11) ? 1 : 0;
    gState_F12 = (FuncState & FN_BIT_12) ? 1 : 0;
  }

  if( FuncGrp == FN_13_20){   // F13〜F20の解析
    gState_F13 = (FuncState & FN_BIT_13) ? 1 : 0;
    gState_F14 = (FuncState & FN_BIT_14) ? 1 : 0;
    gState_F15 = (FuncState & FN_BIT_15) ? 1 : 0;
    gState_F16 = (FuncState & FN_BIT_16) ? 1 : 0;
    gState_F17 = (FuncState & FN_BIT_17) ? 1 : 0;
    gState_F18 = (FuncState & FN_BIT_18) ? 1 : 0;
    gState_F19 = (FuncState & FN_BIT_19) ? 1 : 0;
    gState_F20 = (FuncState & FN_BIT_20) ? 1 : 0;
  }
}




//------------------------------------------------------------------
// CVをデフォルトにリセット(Initialize cv value)
// Serial.println("CVs being reset to factory defaults");
//------------------------------------------------------------------
void resetCVToDefault()
{
  for (int j = 0; j < FactoryDefaultCVIndex; j++ ) {
    Dcc.setCV( FactoryDefaultCVs[j].CV, FactoryDefaultCVs[j].Value);
  }
};

//------------------------------------------------------------------
// CV8 によるリセットコマンド受信処理
//------------------------------------------------------------------
void notifyCVResetFactoryDefault()
{
  //When anything is writen to CV8 reset to defaults.

  resetCVToDefault();
  delay(1000);  //typical CV programming sends the same command multiple times - specially since we dont ACK. so ignore them by delaying
  resetFunc();
};

//------------------------------------------------------------------
// CV Ackの処理
// そこそこ電流を流さないといけない
//------------------------------------------------------------------
void notifyCVAck(void)
{
//サーボモータを動かすとギミックを壊しかねないのでコメントアウト
#ifdef DEBUG
  Serial.println("notifyCVAck");
#endif

#if 0 
  digitalWrite(O1,HIGH);
  digitalWrite(O2,HIGH);
  digitalWrite(O3,HIGH);
  digitalWrite(O4,HIGH);
  delay( 6 );
  digitalWrite(O4,LOW);
  digitalWrite(O3,LOW);
  digitalWrite(O2,LOW);
  digitalWrite(O1,LOW);
#endif
//MOTOR_Ack();
}

void MOTOR_Ack(void)
{
//  analogWrite(O4, 128);
//  delay( 6 );  
//  analogWrite(O4, 0);
}

//------------------------------------------------------------------
// DCC初期化処理）
//------------------------------------------------------------------
void Dccinit(void)
{

  //DCCの応答用負荷ピン
#if defined(DCCACKPIN)
  //Setup ACK Pin
  pinMode(DccAckPin, OUTPUT);
  digitalWrite(DccAckPin, 0);
#endif

  // Setup which External Interrupt, the Pin it's associated with that we're using, disable pullup.
  Dcc.pin(2, 2, 0);   // RS Pi Pico GP2をDCC信号入力端子に設定

  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init( MAN_ID_DIY, 1,   FLAGS_MY_ADDRESS_ONLY , 0 );

  //Init CVs
  gCV1_SAddr = Dcc.getCV( CV_MULTIFUNCTION_PRIMARY_ADDRESS ) ;
  gCVx_LAddr = (Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_MSB ) << 8) + Dcc.getCV( CV_MULTIFUNCTION_EXTENDED_ADDRESS_LSB );

#if !defined(DECODER_DONT_DEFAULT_CV_ON_POWERUP)
//  Serial1.print("1.gCV1 : ");
//  Serial1.println(gCV1_SAddr);
  if ( gCV1_SAddr == 0xFF ) {   //if eeprom has 0xFF then assume it needs to be programmed
    notifyCVResetFactoryDefault();
  } else {
    Serial1.println("CV Not Defaulting");
  }
#else
  Serial1.println("CV Defaulting Always On Powerup");
  notifyCVResetFactoryDefault();
#endif    
  //Init CVs
  //E2P-ROMからCV値を読み込む

}


//------------------------------------------------------------------
// CV値が変化した時の処理（特に何もしない）
//------------------------------------------------------------------
extern void     notifyCVChange( uint16_t CV, uint8_t Value) {
   //CVが変更されたときのメッセージ
#ifdef DEBUG
    Serial1.print("CV "); 
    Serial1.print(CV); 
    Serial1.print(" Changed to "); 
    Serial1.println(Value, DEC);
#endif
}


//------------------------------------------------------------------
// Resrt処理（特に何もしない）
//------------------------------------------------------------------
void notifyDccReset(uint8_t hardReset )
{
}
