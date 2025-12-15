#pragma once

#include <stdint.h>

// Minimal TWAI-like message struct for local builds (avoid name clash with ESP-IDF)
typedef struct {
  uint32_t identifier;
  bool extd;
  uint8_t data_length_code;
  uint8_t data[8];
} ef_twai_message_t;

// sendCANFrame is implemented in ef_ps.cpp
void sendCANFrame(uint32_t id, const uint8_t *data, uint8_t len);
