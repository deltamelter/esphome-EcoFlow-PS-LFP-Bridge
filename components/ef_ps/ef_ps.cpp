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

  // PowerStream Heartbeat/Data logic goes here
  // Example: std::vector<uint8_t> data = { (uint8_t)this->battery_soc_, ... };
  // this->canbus_->send_data(0x621, true, data);
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
