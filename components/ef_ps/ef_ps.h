#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/canbus/canbus.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ef_ps {

// Inherit only from CanbusTrigger to avoid ambiguity errors
class EfPs : public canbus::CanbusTrigger {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  
  // Signature for CanbusTrigger
  void on_frame(const canbus::CanFrame &frame) override;

  void set_canbus_id(canbus::Canbus *canbus) { this->canbus_ = canbus; }

  void set_battery_soc_sensor(sensor::Sensor *s) { battery_soc_sensor_ = s; }
  void set_battery_voltage_sensor(sensor::Sensor *s) { battery_voltage_sensor_ = s; }

  void set_battery_soc(float val);
  void set_battery_voltage(float val);

 protected:
  canbus::Canbus *canbus_{nullptr};
  uint32_t last_transmission_{0};
  
  float battery_soc_{0};
  float battery_voltage_{0};

  sensor::Sensor *battery_soc_sensor_{nullptr};
  sensor::Sensor *battery_voltage_sensor_{nullptr};

  void send_can_data();
};

}  // namespace ef_ps
}  // namespace esphome
