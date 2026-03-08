#include "ef_ps.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ef_ps {

static const char *const TAG = "ef_ps";

void EfPs::setup() {
  if (this->canbus_ != nullptr) {
    this->canbus_->add_listener(this);
  }
}

void EfPs::dump_config() {
  ESP_LOGCONFIG(TAG, "EcoFlow PS Bridge");
}

void EfPs::on_frame(const canbus::CanFrame &frame) {
  // This prints every message from the PowerStream to your ESPHome logs
  ESP_LOGI("ef_ps_rx", "Received ID: 0x%03X, Data: %02X %02X %02X %02X", 
           frame.can_id, frame.data[0], frame.data[1], frame.data[2], frame.data[3]);

  // Example: If PowerStream (0x600) asks for something, respond here
  if (frame.can_id == 0x600) {
     this->send_can_data();
  }
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
  std::vector<uint8_t> bat_msg(8, 0);
  uint16_t volt_scaled = (uint16_t)(this->battery_voltage_ * 100);
  bat_msg[0] = volt_scaled & 0xFF;
  bat_msg[1] = (volt_scaled >> 8) & 0xFF;
  bat_msg[4] = (uint8_t)this->battery_soc_;
  
  this->canbus_->send_data(0x621, true, bat_msg);

  // 0x2FF Heartbeat
  std::vector<uint8_t> hb = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  this->canbus_->send_data(0x2FF, true, hb);
}

}  // namespace ef_ps
}  // namespace esphome
