#pragma once

#include "esphome/core/component.h"
#include "esphome/components/canbus/canbus.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ef_ps {

class EfPs : public PollingComponent, public canbus::CanbusListener {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  void on_frame(const canbus::CanFrame &frame) override;

  void set_canbus_id(canbus::Canbus *canbus) { this->canbus_ = canbus; }

  // Sensor Setters
  void set_battery_soc_sensor(sensor::Sensor *s) { battery_soc_sensor_ = s; }
  void set_battery_voltage_sensor(sensor::Sensor *s) { battery_voltage_sensor_ = s; }
  void set_battery_current_sensor(sensor::Sensor *s) { battery_current_sensor_ = s; }

  // Data Setters (Lambdas)
  void set_battery_soc(float val);
  void set_battery_voltage(float val);
  void set_battery_current(float val);

 protected:
  canbus::Canbus *canbus_{nullptr};
  
  float battery_soc_{0};
  float battery_voltage_{0};
  float battery_current_{0};

  sensor::Sensor *battery_soc_sensor_{nullptr};
  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *battery_current_sensor_{nullptr};

  void send_can_data(uint32_t id, const std::vector<uint8_t> &data);
};

}  // namespace ef_ps
}  // namespace esphome
