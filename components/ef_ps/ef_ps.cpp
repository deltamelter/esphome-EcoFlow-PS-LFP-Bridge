#include "ef_ps.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ef_ps {

static const char *const TAG = "ef_ps";

void EfPs::setup() {
  if (this->canbus_ != nullptr) {
    // This registers our component to receive CAN frames
    this->canbus_->add_driver(this);
  }
}

void EfPs::dump_config() {
  ESP_LOGCONFIG(TAG, "EcoFlow PS Bridge");
}

void EfPs::on_frame(const ::esphome::canbus::CanFrame &frame) {
  // Logic to handle incoming data from PowerStream
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

  // 0x621 Battery Status Frame
  ::esphome::canbus::CanFrame bat_frame{};
  bat_frame.can_id = 0x621;
  bat_frame.use_extended_id = true;
  bat_frame.can_dlc = 8;
  
  uint16_t volt_scaled = (uint16_t)(this->battery_voltage_ * 100);
  bat_frame.data[0] = volt_scaled & 0xFF;
  bat_frame.data[1] = (volt_scaled >> 8) & 0xFF;
  bat_frame.data[4] = (uint8_t)this->battery_soc_;
  
  this->canbus_->send_frame(bat_frame);

  // 0x2FF Heartbeat Frame
  ::esphome::canbus::CanFrame hb_frame{};
  hb_frame.can_id = 0x2FF;
  hb_frame.use_extended_id = true;
  hb_frame.can_dlc = 8;
  hb_frame.data[0] = 0x01;
  
  this->canbus_->send_frame(hb_frame);
}

}  // namespace ef_ps
}  // namespace esphome
