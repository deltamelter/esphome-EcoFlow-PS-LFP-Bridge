#include "ecoflow.h"
#include <stdio.h>

// Minimal stub implementations to allow local build/tests.
EcoflowConfig config = {};
BMS bms;
bool canHealth = false;

float BMS::get_cell_voltage(uint8_t idx) { (void)idx; return 3.7f; }
int BMS::get_0x12_full_charge_voltage() { return 4200; }
float BMS::get_balance_capacity() { return 0.0f; }

void streamDebug(const char *msg) { (void)msg; }
void streamCanLog(const char *msg) { (void)msg; }
double now_seconds() { return 0.0; }
uint32_t millis() { return 0; }
void taskYIELD() {}
int random(int a, int b) { return a; }

// empty implementations for web.h dependencies (if any)
