#include "ef_ps.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ef_ps {

static const char *const TAG = "ef_ps";

void EfPs::setup() {
  if (this->canbus_ != nullptr) {
    // The compiler confirmed this is the correct method name
    this->canbus_->add_trigger(this);
  }
}

void EfPs::dump_config() {
  ESP_LOGCONFIG(TAG, "EcoFlow PS Bridge");
}

void EfPs::on_frame(const canbus::CanFrame &frame) {
  // Logic for incoming frames
}

void EfPs::set_battery_soc(float val) {
  this->battery_soc_ = val;
  if (this->battery_soc_sensor_ != nullptr) this->battery_soc_sensor_->publish_state(val);
  this->send_can_data(); 
}

void EfPs::set_battery_voltage(float val) {
  this->battery_voltage_ = val;
  if (this->battery_voltage_sensor_ != nullptr) this->battery_voltage_sensor_->publish_state(val);
}

void EfPs::send_can_data() {
  if (this->canbus_ == nullptr) return;

  // 0x621 Battery Status
  // send_data(id, use_extended, data_vector)
  uint16_t volt_scaled = (uint16_t)(this->battery_voltage_ * 100);
  std::vector<uint8_t> bat_data(8, 0);
  bat_data[0] = volt_scaled & 0xFF;
  bat_data[1] = (volt_scaled >> 8) & 0xFF;
  bat_data[4] = (uint8_t)this->battery_soc_;
  
  this->canbus_->send_data(0x621, true, bat_data);

  // 0x2FF Heartbeat
  std::vector<uint8_t> hb_data = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  this->canbus_->send_data(0x2FF, true, hb_data);
}

}  // namespace ef_ps
}  // namespace esphome
