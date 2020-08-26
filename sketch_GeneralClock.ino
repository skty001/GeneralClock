#include "CTRLBME280.h"
#include "CTRLRTC4543SA.h"
#include <LiquidCrystal.h>

// 温湿圧計
CTRLBME280 ctrlbme280;
double temp, hum, pre;

// RTC
CTRLRTC4543SA ctrlrtc4543sa;
unsigned char year, month, day, week, hour, mnt, sec;

// LCD
LiquidCrystal liquidcrystal(12,11,10,9,8,7,6);
byte dig[8] = {
  0b11000,
  0b11000,
  0b00011,
  0b00100,
  0b01000,
  0b00100,
  0b00011
};


void setup() {  
  Serial.begin(115200);                   // シリアル:初期化
  ctrlbme280.Initialize();                // 温湿圧計:初期化
  ctrlrtc4543sa.Initialize(2, 3, 4, 5);   // RTC:初期化
  ctrlrtc4543sa.writeData(0x20,0x08,0x12,0x03,0x15,0x25,0x00); // RTC:初期時刻書き込み
  liquidcrystal.begin(2,16);              // LCD:初期化
  liquidcrystal.clear();                  // LCD:クリア(カーソル1行目1ｶﾗﾑに移動);
  liquidcrystal.print("General Clock");   // LCD:スタートメッセージ
  liquidcrystal.setCursor(0, 1);          // LCD:カーソル2行目1ｶﾗﾑに移動
  liquidcrystal.print("Start-up");        // LCD:スタートメッセージ
  liquidcrystal.createChar(0,dig);
  delay(5000);
}

void loop() {
  String dispClock = "";
  String dispEnv = "";
  //liquidcrystal.clear();
  char t_year[3], t_month[3], t_day[3], t_hour[3], t_mnt[3], t_sec[3];
  char t_temp[4], t_hum[3], t_pre[5];
  
  // 時刻の取得
  ctrlrtc4543sa.readData(year, month, day, week, hour, mnt, sec);
  sprintf(t_year, "%02d", year);   // 年2桁
  sprintf(t_month,"%02d", month);  // 月2桁
  sprintf(t_day,  "%02d", day);    // 日2桁
  sprintf(t_hour, "%02d", hour); // 時2桁
  sprintf(t_mnt,  "%02d", mnt);  // 分2桁
  sprintf(t_sec,  "%02d", sec);  // 秒2桁
  dispClock = String(t_month) + "/" + String(t_day) + " " + 
              String(t_hour) + ":" + String(t_mnt) + "." + String(t_sec);
  liquidcrystal.setCursor(0,0);
  liquidcrystal.print(dispClock);

  // 温度・湿度・圧力の取得
  ctrlbme280.getData(temp, hum, pre);
  sprintf(t_temp, "%3d",  (unsigned char)temp);
  sprintf(t_hum, "%2d",   (unsigned char)hum);
  sprintf(t_pre, "%4d",   (unsigned short)pre);
  dispEnv = String(t_temp) + "  " + String(t_hum) + "% " + String(t_pre) + "hPa";
  liquidcrystal.setCursor(0,1);
  liquidcrystal.print(dispEnv);
  liquidcrystal.setCursor(3,1);
  liquidcrystal.write((byte)0);
}
