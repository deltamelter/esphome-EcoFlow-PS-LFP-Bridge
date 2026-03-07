#pragma once

#include "esphome/core/component.h"
#include "esphome/components/canbus/canbus.h"
#include "esphome/components/sensor/sensor.h" // Required for sensor support
#include "esphome/core/log.h"

namespace esphome {
namespace ef_ps {

class EfPs : public PollingComponent, public canbus::CanbusListener {
 public:
  void setup() override;
  void update() override;
  void on_frame(const canbus::CanFrame &frame) override;

  // --- Configuration Setters (used by sensor.py) ---
  void set_canbus_id(canbus::Canbus *canbus) { this->canbus_ = canbus; }
  
  void set_battery_soc_sensor(sensor::Sensor *s) { this->battery_soc_sensor_ = s; }
  void set_battery_voltage_sensor(sensor::Sensor *s) { this->battery_voltage_sensor_ = s; }
  void set_battery_current_sensor(sensor::Sensor *s) { this->battery_current_sensor_ = s; }
  void set_battery_power_sensor(sensor::Sensor *s) { this->battery_power_sensor_ = s; }
  void set_battery_temperature_sensor(sensor::Sensor *s) { this->battery_temperature_sensor_ = s; }
  void set_remaining_capacity_sensor(sensor::Sensor *s) { this->remaining_capacity_sensor_ = s; }
  void set_full_capacity_sensor(sensor::Sensor *s) { this->full_capacity_sensor_ = s; }
  void set_cycles_sensor(sensor::Sensor *s) { this->cycles_sensor_ = s; }

  // --- Data Input Methods (Called via Lambdas in YAML) ---
  void set_battery_soc(float val) { 
    this->battery_soc_ = val; 
    if (this->battery_soc_sensor_ != nullptr) this->battery_soc_sensor_->publish_state(val);
  }
  
  void set_battery_voltage(float val) {
    this->battery_voltage_ = val;
    if (this->battery_voltage_sensor_ != nullptr) this->battery_voltage_sensor_->publish_state(val);
  }

  void set_battery_current(float val) {
    this->battery_current_ = val;
    if (this->battery_current_sensor_ != nullptr) this->battery_current_sensor_->publish_state(val);
  }

  void set_battery_power(float val) {
    this->battery_power_ = val;
    if (this->battery_power_sensor_ != nullptr) this->battery_power_sensor_->publish_state(val);
  }

  void set_battery_temperature(float val) {
    this->battery_temperature_ = val;
    if (this->battery_temperature_sensor_ != nullptr) this->battery_temperature_sensor_->publish_state(val);
  }

  void set_remaining_capacity(float val) {
    this->remaining_capacity_ = val;
    if (this->remaining_capacity_sensor_ != nullptr) this->remaining_capacity_sensor_->publish_state(val);
  }

  void set_full_capacity(float val) {
    this->full_capacity_ = val;
    if (this->full_capacity_sensor_ != nullptr) this->full_capacity_sensor_->publish_state(val);
  }

  void set_cycles(float val) {
    this->cycles_ = val;
    if (this->cycles_sensor_ != nullptr) this->cycles_sensor_->publish_state(val);
  }

 protected:
  canbus::Canbus *canbus_;

  // Internal Data Storage
  float battery_soc_{0};
  float battery_voltage_{0};
  float battery_current_{0};
  float battery_power_{0};
  float battery_temperature_{0};
  float remaining_capacity_{0};
  float full_capacity_{0};
  float cycles_{0};

  // Sensor Pointers
  sensor::Sensor *battery_soc_sensor_{nullptr};
  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *battery_current_sensor_{nullptr};
  sensor::Sensor *battery_power_sensor_{nullptr};
  sensor::Sensor *battery_temperature_sensor_{nullptr};
  sensor::Sensor *remaining_capacity_sensor_{nullptr};
  sensor::Sensor *full_capacity_sensor_{nullptr};
  sensor::Sensor *cycles_sensor_{nullptr};

  // EcoFlow Protocol Helpers
  void send_can_heartbeat();
  void send_can_battery_info();
};

}  // namespace ef_ps
}  // namespace esphome
