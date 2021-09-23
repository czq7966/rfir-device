#ifndef IR_MCQUAY_H_
#define IR_MCQUAY_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

/// Native representation of a McQuay A/C message.
union McQuayProtocol{
  uint8_t remote_state[kMcQuayStateLength];  ///< The state in native IR code form
  struct {
    // Byte 0
    uint8_t Header          :8;       // 固定值：0b00010110（0x16）
    // Byte 1    
    uint8_t Mode            :4;       // 高4位=风速：0b1000=高风；0b0100=中风；0b0010=低风；0b0001=自动
    uint8_t Fan             :4;       // 低4位=模式：0b0000=除湿；0b0001=吹风；0b1001=制冷；其它值待续
    // Byte 2    
    uint8_t TimerMinute     :8;       // 高4位=分钟之十位的值，如36分钟的3，=0b0011// 低4位=分钟之个位的值，如36分钟的6，=0b0110
    // Byte 3
    uint8_t TimerHour       :8;       // 高4位=小时之十位的值，如14时的1，=0b0001  // 低4位=小时之个位的值，如14时的4，=0b0100
    // Byte 4
    uint8_t OpenTime        :7;       //定时开时间，具体说明待续
    uint8_t TimingOpen      :1;       //第8位="定时开"功能开关，1=开启"定时开"功能；0=关闭"定时开"功能
    // Byte 5
    uint8_t CloseTime       :7;       //定时关时间，具体说明待续
    uint8_t TimingClose     :1;       //第8位="定时关"功能开关，1=开启"定时关"功能；0=关闭"定时开"功能    
    // Byte 6
    uint8_t Temp            :8;       //高4位=温度之十位的值，如25度的2，=0b0010   //低4位=温度之个位的值，如25时的5，=0b0101
    // Byte 7
    uint8_t Swing           :1;       //摇摆开关，1=开；0=关
    uint8_t Sleep           :1;       //睡眠开关，1=开；0=关
    uint8_t unknown         :2;       //未知，保留
    uint8_t Sum             :4;       //高4位=校验值=前7个字节的高4位+低4位的和，取总和的低4位

  };
};

// Constants
const uint8_t kMcQuayHeader  = 0x16;

const uint8_t kMcQuayAuto = 0b0000;
const uint8_t kMcQuayDry  = 0b0001;
const uint8_t kMcQuayCool = 0b0010;
const uint8_t kMcQuayFan  = 0b0100;
const uint8_t kMcQuayHeat = 0b1000;

const uint8_t kMcQuayFanAuto = 0b0001;
const uint8_t kMcQuayFanMin  = 0b0010;
const uint8_t kMcQuayFanMed  = 0b0100;
const uint8_t kMcQuayFanMax  = 0b1000;

const uint8_t kMcQuaySwingOpen  = 0b1;
const uint8_t kMcQuaySwingClose  = 0b0;

const uint8_t kMcQuaySleepOpen  = 0b1;
const uint8_t kMcQuaySleepClose  = 0b0;

const uint8_t kMcQuayMinTempC = 16;  // Celsius
const uint8_t kMcQuayMaxTempC = 30;  // Celsius
const uint16_t kMcQuayTimerMax = 24 * 60;

// const uint8_t kGreeSwingLastPos    = 0b0000;
// const uint8_t kGreeSwingAuto       = 0b0001;
// const uint8_t kGreeSwingUp         = 0b0010;
// const uint8_t kGreeSwingMiddleUp   = 0b0011;
// const uint8_t kGreeSwingMiddle     = 0b0100;
// const uint8_t kGreeSwingMiddleDown = 0b0101;
// const uint8_t kGreeSwingDown       = 0b0110;
// const uint8_t kGreeSwingDownAuto   = 0b0111;
// const uint8_t kGreeSwingMiddleAuto = 0b1001;
// const uint8_t kGreeSwingUpAuto     = 0b1011;

// const uint8_t kGreeDisplayTempOff     = 0b00;  // 0
// const uint8_t kGreeDisplayTempSet     = 0b01;  // 1
// const uint8_t kGreeDisplayTempInside  = 0b10;  // 2
// const uint8_t kGreeDisplayTempOutside = 0b11;  // 3

// Legacy defines.
// #define MCQUAY_AUTO kGreeAuto
// #define MCQUAY_COOL kGreeCool
// #define MCQUAY_DRY kGreeDry
// #define MCQUAY_FAN kGreeFan
// #define MCQUAY_HEAT kGreeHeat
// #define MCQUAY_MIN_TEMP kGreeMinTempC
// #define MCQUAY_MAX_TEMP kGreeMaxTempC
// #define MCQUAY_FAN_MAX kGreeFanMax
// #define MCQUAY_SWING_LAST_POS kGreeSwingLastPos
// #define MCQUAY_SWING_AUTO kGreeSwingAuto
// #define MCQUAY_SWING_UP kGreeSwingUp
// #define MCQUAY_SWING_MIDDLE_UP kGreeSwingMiddleUp
// #define MCQUAY_SWING_MIDDLE kGreeSwingMiddle
// #define MCQUAY_SWING_MIDDLE_DOWN kGreeSwingMiddleDown
// #define MCQUAY_SWING_DOWN kGreeSwingDown
// #define MCQUAY_SWING_DOWN_AUTO kGreeSwingDownAuto
// #define MCQUAY_SWING_MIDDLE_AUTO kGreeSwingMiddleAuto
// #define MCQUAY_SWING_UP_AUTO kGreeSwingUpAuto

// Classes
/// Class for handling detailed McQuay A/C messages.
class IRMcQuayAC {
 public:
  explicit IRMcQuayAC(
      const uint16_t pin,
      const mcquay_ac_remote_model_t model = mcquay_ac_remote_model_t::G4AAP,
      const bool inverted = false, const bool use_modulation = true);
  void stateReset(void);
#if SEND_GREE
  void send(const uint16_t repeat = kMcQuayDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_GREE
  void begin(void);
  void setModel(const mcquay_ac_remote_model_t model);
  mcquay_ac_remote_model_t getModel(void) const;
  void setTemp(const uint8_t temp, const bool fahrenheit = false);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t new_mode);
  uint8_t getMode(void) const;
  void setSleep(const bool on);
  bool getSleep(void) const;
  void setSwing(const bool on);
  bool getSwing(void) const;  
  uint8_t getSwingVerticalPosition(void) const;
  uint8_t getTimerHour(void) const;
  void setTimerHour(const uint8_t hour);
  uint8_t getTimerMinute(void) const;
  void setTimerMinute(const uint8_t minute);  
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t swingv);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t pos);
  stdAc::state_t toCommon(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[]);
  static uint8_t calcBlockChecksum(
      const uint8_t* block, const uint16_t length = kMcQuayStateLength - 1);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kMcQuayStateLength);
  static uint8_t dec2hex(const uint8_t dec);
  static uint8_t hex2dec(const uint8_t hex);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  McQuayProtocol _;
  mcquay_ac_remote_model_t _model;
  void checksum(const uint16_t length = kMcQuayStateLength);
  void fixup(void);
  void setTimerEnabled(const bool on);
  bool getTimerEnabled(void) const;
};

#endif  // IR_MCQUAY_H_
