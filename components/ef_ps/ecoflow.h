#pragma once

#include <stdint.h>
#include <stddef.h>
#include <Arduino.h>
#include "can.h"

// Minimal config struct used by the messages (only fields referenced here)
struct EcoflowConfig {
  int temp;
  int volt;
  int chgvolt;
  int soc;
  uint32_t chgruntime;
  uint32_t disruntime;
  uint8_t bmsChgUp;
  uint8_t bmsChgDn;
  char serialStr[16];

  // message enable flags
  bool message70;
  bool message0B;
  bool message4F;
  bool message68;
  bool message13;
  bool messageCB;
  bool message5C;
  bool message24;
  bool message8C;
  bool message3C;

  bool canTxEnabled;
  bool txlogging;
  bool rxlogging;
};

extern EcoflowConfig config;

// Minimal BMS interface used
struct BMS {
  float get_cell_voltage(uint8_t idx);
  int get_0x12_full_charge_voltage();
  float get_balance_capacity();
};
extern BMS bms;

extern volatile uint32_t can_rx_count;
extern volatile uint32_t can_rx_dropped;
extern volatile uint32_t can_decoded;

extern float inputWatt;
extern float outputWatt;
extern String canLog;

// Functions provided by this module
void ecoflowMessagesInit();
void sendCANMessage(uint8_t* header, uint8_t* payload, size_t headerSize, size_t payloadSize);
void processEcoFlowCAN(const twai_message_t &rx);
void canTxSequencerTick();
void canSequencer_onHeartbeatC4();

// Helpers (likely implemented elsewhere in project; declared to allow linkage in tests)
void streamDebug(const char *msg);
void streamCanLog(const char *msg);
double now_seconds();

extern bool canHealth;
