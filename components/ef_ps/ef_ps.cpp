#include "ef_ps.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ef_ps {

static const char *const TAG = "ef_ps";

void EfPs::setup() {
  // Fix: The method is add_listener, not add_driver or add_trigger
  if (this->canbus_ != nullptr) {
    this->canbus_->add_listener(this);
  }
}

void EfPs::dump_config() {
  ESP_LOGCONFIG(TAG, "EcoFlow PS Bridge:");
  LOG_UPDATE_INTERVAL(this);
}

void EfPs::update() {
  if (this->canbus_ == nullptr) return;

  // PowerStream Protocol Logic
  std::vector<uint8_t> bat_msg(8, 0);
  
  uint16_t volt_scaled = (uint16_t)(this->battery_voltage_ * 100);
  bat_msg[0] = volt_scaled & 0xFF;
  bat_msg[1] = (volt_scaled >> 8) & 0xFF;
  
  int16_t curr_scaled = (int16_t)(this->battery_current_ * 100);
  bat_msg[2] = curr_scaled & 0xFF;
  bat_msg[3] = (curr_scaled >> 8) & 0xFF;
  
  bat_msg[4] = (uint8_t)this->battery_soc_;
  
  this->canbus_->send_data(0x621, true, bat_msg);

  std::vector<uint8_t> heartbeat = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  this->canbus_->send_data(0x2FF, true, heartbeat);
}

void EfPs::on_frame(const esphome::canbus::CanFrame &frame) {
  // Logic for incoming frames
}

void EfPs::set_battery_soc(float val) {
  this->battery_soc_ = val;
  if (this->battery_soc_sensor_ != nullptr) this->battery_soc_sensor_->publish_state(val);
}

void EfPs::set_battery_voltage(float val) {
  this->battery_voltage_ = val;
  if (this->battery_voltage_sensor_ != nullptr) this->battery_voltage_sensor_->publish_state(val);
}

void EfPs::set_battery_current(float val) {
  this->battery_current_ = val;
  if (this->battery_current_sensor_ != nullptr) this->battery_current_sensor_->publish_state(val);
}

}  // namespace ef_ps
}  // namespace esphome

