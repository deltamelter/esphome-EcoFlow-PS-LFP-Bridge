#include "ecoflow.h"
#include <stdio.h>

// Minimal stub implementations to allow local build/tests.
EcoflowConfig config = {};
BMS bms;
bool canHealth = false;

float BMS::get_cell_voltage(uint8_t idx) { (void)idx; return 3.7f; }
int BMS::get_0x12_full_charge_voltage() { return 4200; }
float BMS::get_balance_capacity() { return 0.0f; }

// Only provide these stubs when not building for Arduino/ESP platforms
#if !defined(ARDUINO) && !defined(ESP32) && !defined(ESP8266)
void streamDebug(const char *msg) { (void)msg; }
void streamCanLog(const char *msg) { (void)msg; }
double now_seconds() { return 0.0; }
uint32_t millis() { return 0; }
void taskYIELD() {}
int random(int a, int b) { return a; }
#else
#include "esphome/core/log.h"
void streamDebug(const char *msg) { ESP_LOGD("ef_ps", "%s", msg); }
void streamCanLog(const char *msg) { ESP_LOGD("ef_ps", "%s", msg); }
double now_seconds() {
#if defined(ESP32) || defined(ESP8266)
#include <esp_timer.h>
	return (double)esp_timer_get_time() / 1000000.0;
#else
	return (double)millis() / 1000.0;
#endif
}
#endif

// empty implementations for web.h dependencies (if any)
