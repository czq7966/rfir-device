
#include "ir_McQuay.h"
#include <algorithm>
#include <cstring>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"
#include "ir_Kelvinator.h"

// Constants
const uint16_t kMcQuayHdrMark = 4600;
const uint16_t kMcQuayHdrSpace = 2600;
const uint16_t kMcQuayOneMark = 360;
const uint16_t kMcQuayOneSpace = 950;
const uint16_t kMcQuayZeroMark = 360;
const uint16_t kMcQuayZeroSpace = 400;
const uint16_t kMcQuayFooter1Mark = 360;
const uint16_t kMcQuayFooter1Space = 20500;
const uint16_t kMcQuayFooter2Mark = 4600;
const uint16_t kMcQuayFooter2Space = 20000;

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addModelToString;
using irutils::addFanToString;
using irutils::addTempToString;
using irutils::minsToString;

#if SEND_MCQUAY
/// Send a McQuay Heat Pump formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendMcQuay(const uint8_t data[], const uint16_t nbytes,
                      const uint16_t repeat) {
  if (nbytes < kMcQuayStateLength)
    return;  // Not enough bytes to send a proper message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Block #1
    sendGeneric(kMcQuayHdrMark, kMcQuayHdrSpace, kMcQuayOneMark, kMcQuayOneSpace,
                kMcQuayZeroMark, kMcQuayZeroSpace, kMcQuayFooter1Mark, kMcQuayFooter1Space,  // No Footer.
                data, 8, 38, false, 0, 50);
    mark(kMcQuayFooter2Mark);
    space(kMcQuayFooter2Space);                
  }
}

/// Send a McQuay Heat Pump formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendMcQuay(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  if (nbits != kMcQuayBits)
    return;  // Wrong nr. of bits to send a proper message.
  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(kMcQuayHdrMark);
    space(kMcQuayHdrSpace);

    // Data
    for (int16_t i = 8; i <= nbits; i += 8) {
      sendData(kMcQuayOneMark, kMcQuayOneSpace, kMcQuayZeroMark, kMcQuayZeroSpace,
               (data >> (nbits - i)) & 0xFF, 8, false);

    }
    // Footer
    mark(kMcQuayFooter1Mark);
    space(kMcQuayFooter1Space);   
    mark(kMcQuayFooter2Mark);
    space(kMcQuayFooter2Space);       
  }
}
#endif  // SEND_MCQUAY

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] model The enum of the model to be emulated.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRMcQuayAC::IRMcQuayAC(const uint16_t pin, const mcquay_ac_remote_model_t model,
                   const bool inverted, const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  stateReset();
  setModel(model);
}

/// Reset the internal state to a fixed known good state.
void IRMcQuayAC::stateReset(void) {
  // This resets to a known-good state to Power Off, Fan Auto, Mode Auto, 25C.
  std::memset(_.remote_state, 0, sizeof _.remote_state);
  _.Header = kMcQuayHeader;
  _.Temp = 0x26;  
  _.Mode = kMcQuayCool;  
  _.Fan = kMcQuayFanAuto;
}

/// Fix up the internal state so it is correct.
/// @note Internal use only.
void IRMcQuayAC::fixup(void) {
  checksum();  // Calculate the checksums
}

/// Set up hardware to be able to send a message.
void IRMcQuayAC::begin(void) { _irsend.begin(); }

#if SEND_MCQUAY
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMcQuayAC::send(const uint16_t repeat) {
  _irsend.sendMcQuay(getRaw(), kMcQuayStateLength, repeat);
}
#endif  // SEND_MCQUAY

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRMcQuayAC::getRaw(void) {
  fixup();  // Ensure correct settings before sending.
  return _.remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRMcQuayAC::setRaw(const uint8_t new_code[]) {
  std::memcpy(_.remote_state, new_code, kMcQuayStateLength);
  // We can only detect the difference between models when the power is on.
  _model = mcquay_ac_remote_model_t::G4AAP;
}

/// Calculate and set the checksum values for the internal state.
/// @param[in] length The size/length of the state array to fix the checksum of.
void IRMcQuayAC::checksum(const uint16_t length) {
  // Gree uses the same checksum alg. as Kelvinator's block checksum.
  _.Sum = IRMcQuayAC::calcBlockChecksum(_.remote_state, length);
}

uint8_t IRMcQuayAC::calcBlockChecksum(
      const uint8_t* block, const uint16_t length) {
  uint8_t sum = 0;
  // Sum the upper half and lower half of this block.
  for (uint8_t i = 0; i < length - 1; i++, block++) {
    sum += (*block & 0b1111) + (*block >> 4);
  }

  return sum & 0b1111;        
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRMcQuayAC::validChecksum(const uint8_t state[], const uint16_t length) {
  // Top 4 bits of the last byte in the state is the state's checksum.
  return GETBITS8(state[length - 1], kHighNibble, kNibbleSize) ==
      IRMcQuayAC::calcBlockChecksum(state, length);
}

uint8_t IRMcQuayAC::dec2hex(const uint8_t dec) {
  return  ((dec / 10) << 4) + ((dec % 10) & 0xFFFF);
}

uint8_t IRMcQuayAC::hex2dec(const uint8_t hex) {
  return  (hex >> 4) * 10 + hex & 0xFFFF;
}

/// Set the model of the A/C to emulate.
/// @param[in] model The enum of the appropriate model.
void IRMcQuayAC::setModel(const mcquay_ac_remote_model_t model) {
    _model = mcquay_ac_remote_model_t::G4AAP;
}

/// Get/Detect the model of the A/C.
/// @return The enum of the compatible model.
mcquay_ac_remote_model_t IRMcQuayAC::getModel(void) const { return _model; }

/// Set the temp. in degrees
/// @param[in] temp Desired temperature in Degrees.
/// @param[in] fahrenheit Use units of Fahrenheit and set that as units used.
///   false is Celsius (Default), true is Fahrenheit.
/// @note The unit actually works in Celsius with a special optional
///   "extra degree" when sending Fahrenheit.
void IRMcQuayAC::setTemp(const uint8_t temp, const bool fahrenheit) {
  uint8_t safecelsius = temp;
  safecelsius = std::max(kMcQuayMinTempC, safecelsius);
  safecelsius = std::min(kMcQuayMaxTempC, safecelsius);

  _.Temp = dec2hex(safecelsius);
}

/// Get the set temperature
/// @return The temperature in degrees in the current units (C/F) set.
uint8_t IRMcQuayAC::getTemp(void) const {
  return hex2dec(_.Temp);
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting. 0 is auto, 1-3 is the speed.
void IRMcQuayAC::setFan(const uint8_t speed) {
  uint8_t fan = std::min(kMcQuayFanMax, speed);  // Bounds check
  _.Fan = fan;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRMcQuayAC::getFan(void) const {
  return _.Fan;
}

/// Set the operating mode of the A/C.
/// @param[in] new_mode The desired operating mode.
void IRMcQuayAC::setMode(const uint8_t new_mode) {
  uint8_t mode = new_mode;
  _.Mode = mode;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMcQuayAC::getMode(void) const {
  return _.Mode;
}



/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMcQuayAC::setSleep(const bool on) {
  _.Sleep = on;
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMcQuayAC::getSleep(void) const {
  return _.Sleep;
}


void IRMcQuayAC::setSwing(const bool on) {
  _.Swing = on;
}


bool IRMcQuayAC::getSwing(void) const {
  return _.Swing;
}


uint8_t IRMcQuayAC::getTimerHour(void) const {
  return hex2dec(_.TimerHour);
}

void IRMcQuayAC::setTimerHour(const uint8_t hour) {
  _.TimerHour = dec2hex(hour);
}


uint8_t IRMcQuayAC::getTimerMinute(void) const {
  return hex2dec(_.TimerMinute);
}

void IRMcQuayAC::setTimerMinute(const uint8_t minute) {
  _.TimerHour = dec2hex(minute);
}




/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMcQuayAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kMcQuayCool;
    case stdAc::opmode_t::kHeat: return kMcQuayHeat;
    case stdAc::opmode_t::kDry:  return kMcQuayDry;
    case stdAc::opmode_t::kFan:  return kMcQuayFan;
    default:                     return kMcQuayAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMcQuayAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kMcQuayFanMin;
    case stdAc::fanspeed_t::kLow:
    case stdAc::fanspeed_t::kMedium: return kMcQuayFanMax - 1;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kMcQuayFanMax;
    default:                         return kMcQuayFanAuto;
  }
}


/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRMcQuayAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kMcQuayCool: return stdAc::opmode_t::kCool;
    case kMcQuayHeat: return stdAc::opmode_t::kHeat;
    case kMcQuayDry: return stdAc::opmode_t::kDry;
    case kMcQuayFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRMcQuayAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kMcQuayFanMax: return stdAc::fanspeed_t::kMax;
    case kMcQuayFanMax - 1: return stdAc::fanspeed_t::kMedium;
    case kMcQuayFanMin: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}


/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRMcQuayAC::toCommon(void) {
  stdAc::state_t result;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRMcQuayAC::toString(void) {
  String result = "";
  result.reserve(220);  // Reserve some heap for the string to reduce fragging.
  result += addModelToString(decode_type_t::MCQUAY, _model, false);
  // result += addBoolToString(_.Power, kPowerStr);
  result += addModeToString(_.Mode, kMcQuayAuto, kMcQuayCool, kMcQuayHeat,
                            kMcQuayDry, kMcQuayFan);
  result += addTempToString(getTemp(), true);
  result += addFanToString(_.Fan, kMcQuayFanMax, kMcQuayFanMin, kMcQuayFanAuto,
                           kMcQuayFanAuto, kMcQuayFanMed);
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addBoolToString(_.Swing, kSwingStr);
  result += ')';
  result += ')';
  return result;
}

#if DECODE_GREE
/// Decode the supplied Gree HVAC message.
/// Status: STABLE / Working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeMcQuay(decode_results* results, uint16_t offset,
                        const uint16_t nbits, bool const strict) {
  if (results->rawlen <=
      2 * (nbits) + (kHeader + kFooter + 1) - 1 + offset)
    return false;  // Can't possibly be a valid Gree message.
  if (strict && nbits != kMcQuayBits)
    return false;  // Not strictly a Gree message.

  // There are two blocks back-to-back in a full Gree IR message
  // sequence.

  uint16_t used;
  // Header + Data Block  (64 bits) + Footer
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, nbits ,
                      kMcQuayHdrMark, kMcQuayHdrSpace,
                      kMcQuayOneMark, kMcQuayOneSpace,
                      kMcQuayZeroMark, kMcQuayZeroSpace,
                      kMcQuayFooter1Mark, kMcQuayFooter1Space, false,
                      _tolerance, kMarkExcess, false);
  if (used == 0) return false;
  offset += used;


  if (strict) {
    // Verify the message's checksum is correct.
    if (!IRMcQuayAC::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = GREE;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_GREE
