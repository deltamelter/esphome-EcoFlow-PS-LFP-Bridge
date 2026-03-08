#pragma once

#include "esphome/core/component.h"
#include "esphome/components/canbus/canbus.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ef_ps {

// Explicitly use the full namespaces for the base classes
class EfPs : public esphome::PollingComponent, public esphome::canbus::CanbusListener {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  
  // This signature must be exact
  void on_frame(const esphome::canbus::CanFrame &frame) override;

  void set_canbus_id(esphome::canbus::Canbus *canbus) { this->canbus_ = canbus; }

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
  
  float battery_soc_{0};
  float battery_voltage_{0};
  float battery_current_{0};

  sensor::Sensor *battery_soc_sensor_{nullptr};
  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *battery_current_sensor_{nullptr};
};

}  // namespace ef_ps
}  // namespace esphome
