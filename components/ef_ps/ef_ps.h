#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/canbus/canbus.h"

namespace esphome {
namespace ef_ps {

// Change PollingComponent to esphome::PollingComponent to be explicit
class EfPs : public esphome::PollingComponent, public esphome::canbus::CanbusListener {
 public:
  void setup() override;
  void update() override;
  void dump_config() override; // Added this declaration
  
  // This must match the signature in esphome/components/canbus/canbus.h
  void on_frame(const esphome::canbus::CanFrame &frame) override;

  void set_canbus_id(esphome::canbus::Canbus *canbus) { this->canbus_ = canbus; }

  // Setters for sensors (keep these from previous step)
  void set_battery_soc_sensor(sensor::Sensor *s) { this->battery_soc_sensor_ = s; }
  void set_battery_voltage_sensor(sensor::Sensor *s) { this->battery_voltage_sensor_ = s; }
  // ... (include other sensor setters here)

  // Data methods
  void set_battery_soc(float val);
  void set_battery_voltage(float val);

 protected:
  esphome::canbus::Canbus *canbus_;
  
  float battery_soc_{0};
  float battery_voltage_{0};

  sensor::Sensor *battery_soc_sensor_{nullptr};
  sensor::Sensor *battery_voltage_sensor_{nullptr};

  void send_can_heartbeat();
  void send_can_battery_info();
};

}  // namespace ef_ps
}  // namespace esphome
