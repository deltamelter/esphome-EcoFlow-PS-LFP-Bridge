#include "ef_ps.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ef_ps {

static const char *const TAG = "ef_ps";

void EfPs::setup() {
  if (this->canbus_ != nullptr) {
    this->canbus_->add_driver(this);
  }
}

void EfPs::dump_config() {
  ESP_LOGCONFIG(TAG, "EcoFlow PS Bridge:");
  LOG_UPDATE_INTERVAL(this);
}

void EfPs::update() {
  if (this->canbus_ == nullptr) return;

  // 1. Send Battery Info Frame (ID: 0x621)
  // This tells the PowerStream the Voltage, Current, and SoC
  std::vector<uint8_t> bat_msg(8, 0);
  
  // Pack Voltage (Example: 51.2V -> 5120)
  uint16_t volt_scaled = (uint16_t)(this->battery_voltage_ * 100);
  bat_msg[0] = volt_scaled & 0xFF;
  bat_msg[1] = (volt_scaled >> 8) & 0xFF;
  
  // Pack Current (Example: 10.5A -> 1050)
  int16_t curr_scaled = (int16_t)(this->battery_current_ * 100);
  bat_msg[2] = curr_scaled & 0xFF;
  bat_msg[3] = (curr_scaled >> 8) & 0xFF;
  
  // Pack SoC (Percentage 0-100)
  bat_msg[4] = (uint8_t)this->battery_soc_;
  
  // Send 0x621 as an Extended Frame
  this->canbus_->send_data(0x621, true, bat_msg);

  // 2. Send Heartbeat / Presence Frame (ID: 0x2FF)
  // This tells the PowerStream "I am an EcoFlow Battery, stay connected"
  std::vector<uint8_t> heartbeat = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  this->canbus_->send_data(0x2FF, true, heartbeat);

  ESP_LOGD(TAG, "Sent CAN Data: V=%.2f, I=%.2f, SoC=%.0f", 
           this->battery_voltage_, this->battery_current_, this->battery_soc_);
}

void EfPs::on_frame(const canbus::CanFrame &frame) {
  // Handle frames received from PowerStream
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
