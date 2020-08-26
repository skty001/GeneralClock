typedef unsigned char UC;
typedef const unsigned char cUC;
class CTRLRTC4543SA{
  public:
    void Initialize(UC CE, UC WR, UC CLK, UC DATA);
    void writeData(UC year, UC month, UC day, UC week, UC hour, UC minute, UC second);
    void readData(UC &year, UC &month, UC &day, UC &week, UC &hour, UC &minute, UC &second);
  private:
    void writeBit(UC data, UC pos);
    UC readBit();
    UC cnvBCDtoBYTE(UC BCD);
    void initTime();

    // ピン番号
    UC CE, WR, CLK, DATA;
    
    // 時刻
    UC year   = 0x00;
    UC month  = 0x00;
    UC day    = 0x00;
    UC week   = 0x00;
    UC hour   = 0x00;
    UC minute = 0x00;
    UC second = 0x00;
  
    // リード・ライトbit数
    cUC DATA_N = 52;
    
    // シーケンス定数
    #define SEQ_SEC   0
    #define SEQ_MIN   1
    #define SEQ_HOUR  2
    #define SEQ_WEEK  3
    #define SEQ_DAY   4
    #define SEQ_MONTH 5
    #define SEQ_YEAR  6
    
    // 初期時刻
    cUC INI_SEC   = 0x30;
    cUC INI_MIN   = 0x10;
    cUC INI_HOUR  = 0x15;
    cUC INI_WEEK  = 0x01;
    cUC INI_DAY   = 0x20;
    cUC INI_MONTH = 0x10;
    cUC INI_YEAR  = 0x30;
};
