#pragma once

#include "esphome/core/component.h"
#include "esphome/components/canbus/canbus.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ef_ps {

// Only inherit from CanbusTrigger. It already includes 'Component'.
class EfPs : public esphome::canbus::CanbusTrigger {
 public:
  void setup() override;
  void dump_config() override;
  
  // This is the function called when a CAN frame matches (if configured) 
  // or via the listener logic.
  void on_frame(const esphome::canbus::CanFrame &frame) override;

  void set_canbus_id(esphome::canbus::Canbus *canbus) { this->canbus_ = canbus; }
  void set_update_interval(uint32_t interval) { this->update_interval_ = interval; }

  // Sensor Setters
  void set_battery_soc_sensor(sensor::Sensor *s) { battery_soc_sensor_ = s; }
  void set_battery_voltage_sensor(sensor::Sensor *s) { battery_voltage_sensor_ = s; }
  void set_battery_current_sensor(sensor::Sensor *s) { battery_current_sensor_ = s; }

  // Data Setters
  void set_battery_soc(float val);
  void set_battery_voltage(float val);
  void set_battery_current(float val);

 protected:
  esphome::canbus::Canbus *canbus_{nullptr};
  uint32_t update_interval_{1000};
  
  float battery_soc_{0};
  float battery_voltage_{0};
  float battery_current_{0};

  sensor::Sensor *battery_soc_sensor_{nullptr};
  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *battery_current_sensor_{nullptr};

  void send_can_data();
};

}  // namespace ef_ps
}  // namespace esphome
