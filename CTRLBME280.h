class CTRLBME280{
  public:
    void Initialize();   // 初期化
    void getData(double &temp, double &hum, double &pre);  // BME280 → メイン
    
  private:  
    long signed int BME280_compensate_T_int32(long signed int abc_T);       // 
    long unsigned int BME280_compensate_P_int64(long unsigned int adc_P);
    long unsigned int BME280_compensate_H_int32(long signed int adc_H);
    void readCalibration();
    void writeRegister(unsigned char addr, unsigned char data);
    
    const unsigned char BME280_ADDRESS = 0x76;  // BME280アドレス SDO:LOW → 0x76
    const unsigned char ADDR_CONFIG = 0xF5;     // CONFIG_REGISTER
    const unsigned char ADDR_CTRL_MSG = 0xF4;   // CONTROL_REGISTER(TEMP, PRESSURE)
    const unsigned char ADDR_CTRL_HUM = 0xF2;   // CONTROL_REGISTER(HUMID)
    comBME280();    // BME280→Arduino
    
    // 温度調整パラメータ
    unsigned short dig_T1;
    signed short dig_T2, dig_T3;
    
    // 気圧調整パラメータ
    unsigned short dig_P1;
    signed short dig_P2, dig_P3, dig_P4, dig_P5;
    signed short dig_P6, dig_P7, dig_P8, dig_P9;
    
    // 湿度調整パラメータ
    unsigned char dig_H1, dig_H3;
    signed char dig_H6;
    signed short dig_H2, dig_H4, dig_H5;
    
    long signed int t_fine;   

    unsigned long int raw_temp, raw_hum, raw_press;       // 生のセンサ値
};
