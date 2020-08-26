#include "CTRLRTC4543SA.h"
#include "arduino.h"

void CTRLRTC4543SA::Initialize(UC CE, UC WR, UC CLK, UC DATA){
  this->CE = CE;
  this->WR = WR;
  this->CLK = CLK;
  this->DATA = DATA;
  
  // 入出力初期化
  pinMode(this->CE, OUTPUT);
  pinMode(this->WR, OUTPUT);
  pinMode(this->CLK, OUTPUT);
  pinMode(this->DATA, OUTPUT);

  // 書き込み時刻をRTCに書き込み
  writeData(0, 0, 0, 0, 0, 0, 0);
}

// データライト(引数はBCD)
void CTRLRTC4543SA::writeData(UC year, UC month, UC day, UC week, UC hour, UC minute, UC second){      
  UC snd_year    = year; 
  UC snd_month   = month   & 0x1F;
  UC snd_day     = day     & 0x3F;
  UC snd_week    = week    & 0x07;
  UC snd_hour    = hour    & 0x3F;
  UC snd_minute  = minute  & 0x7F;
  UC snd_second  = second  & 0x7F;
  UC seq = SEQ_SEC;
  UC pos = 0;
  
  // Write出力初期化
  pinMode(DATA, OUTPUT);
  digitalWrite(CLK, LOW);
  digitalWrite(WR, HIGH); delay(1);
  digitalWrite(CE, HIGH); delay(1);

  // Write開始
  while(1){
    switch(seq){
      case SEQ_SEC:   writeBit(snd_second, pos);break;
      case SEQ_MIN:   writeBit(snd_minute, pos);break;
      case SEQ_HOUR:  writeBit(snd_hour, pos);  break;
      case SEQ_WEEK:  writeBit(snd_week, pos);  break;
      case SEQ_DAY:   writeBit(snd_day, pos);   break;
      case SEQ_MONTH: writeBit(snd_month, pos); break;
      case SEQ_YEAR:  writeBit(snd_year, pos);  break;
    }

    pos++;
    if((seq == SEQ_WEEK && pos == 4) || (pos == 8)){
      seq = seq + 1;
      pos = 0;
    }

    if(seq == SEQ_YEAR + 1) break;
  }

  digitalWrite(WR, LOW); delay(1);
  digitalWrite(CE, LOW); delay(1);
}
void CTRLRTC4543SA::readData(UC &year, UC &month, UC &day, UC &week, UC &hour, UC &minute, UC &second){
  UC seq = SEQ_SEC;
  UC pos = 0;
  UC rcv, calc;

  initTime();
  
  // Read出力初期化
  digitalWrite(CLK, LOW);
  digitalWrite(WR, LOW);  delay(1);
  digitalWrite(CE, HIGH); delay(1); 
  pinMode(DATA, INPUT);
  
  // Read開始
  while(1){
    rcv = readBit() << pos;
    switch(seq){
      case SEQ_SEC:   this->second += rcv;  break;
      case SEQ_MIN:   this->minute += rcv;  break;
      case SEQ_HOUR:  this->hour += rcv;    break;
      case SEQ_WEEK:  this->week += rcv;    break;
      case SEQ_DAY:   this->day += rcv;     break;
      case SEQ_MONTH: this->month += rcv;   break;
      case SEQ_YEAR:  this->year += rcv;    break;
    }
    pos++;
    if((seq == SEQ_WEEK && pos == 4) || (pos == 8)){
      seq = seq + 1;
      pos = 0;
    }

    if(seq == SEQ_YEAR + 1) break;
  }

  second  = cnvBCDtoBYTE(this->second);
  minute  = cnvBCDtoBYTE(this->minute);
  hour    = cnvBCDtoBYTE(this->hour);
  week    = cnvBCDtoBYTE(this->week);
  day     = cnvBCDtoBYTE(this->day);
  month   = cnvBCDtoBYTE(this->month);
  year    = cnvBCDtoBYTE(this->year);

  digitalWrite(WR, LOW); delay(1);
  digitalWrite(CE, LOW); delay(1);
  pinMode(DATA, OUTPUT);
}



// 以下privateメソッド

void CTRLRTC4543SA::writeBit(UC data, UC pos){
  if(data & (0x01 << pos))   digitalWrite(DATA, HIGH);
  else                       digitalWrite(DATA, LOW);
  delay(1);
  digitalWrite(CLK,HIGH); delay(1);
  digitalWrite(CLK,LOW);  delay(1);
}

UC CTRLRTC4543SA::readBit(){
  UC ret;
  digitalWrite(CLK, HIGH); delay(1);
  if(digitalRead(DATA) == HIGH) ret = 0x01;
  else                          ret = 0x00;
  digitalWrite(CLK, LOW);  delay(1);
  return ret;
}

UC CTRLRTC4543SA::cnvBCDtoBYTE(UC BCD){
  return ((((BCD & 0xF0) >> 4) * 10) + (BCD & 0x0F));
}

void CTRLRTC4543SA::initTime(){
  second = 0; minute = 0; hour = 0;
  week = 0; day = 0;  month = 0;  year = 0;
}
