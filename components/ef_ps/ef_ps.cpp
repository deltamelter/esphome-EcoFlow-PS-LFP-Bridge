#include "ef_ps.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ef_ps {

static const char *const TAG = "ef_ps";

void EfPs::setup() {
  // Register this class as a listener for CAN frames
  this->canbus_->add_listener(this);
  ESP_LOGCONFIG(TAG, "Setting up EcoFlow PS Bridge...");
}

void EfPs::update() {
  // This is called every 'update_interval'
  this->send_can_heartbeat();
  this->send_can_battery_info();
}

void EfPs::on_frame(const canbus::CanFrame &frame) {
  // Logic for handling incoming frames from PowerStream
  // You can implement specific logic here if the PS sends requests
}

void EfPs::send_can_heartbeat() {
    if (!this->canbus_) return;
    // Example heartbeat ID - adjust based on specific PowerStream protocol requirements
    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}; 
    this->canbus_->send_data(0x2FF, true, data); 
}

void EfPs::send_can_battery_info() {
    if (!this->canbus_) return;

    // This is where you package your battery_soc_ and battery_voltage_ 
    // into the specific EcoFlow CAN format.
    
    // Example (Simplified):
    // uint16_t v = (uint16_t)(this->battery_voltage_ * 100);
    // std::vector<uint8_t> data = { (uint8_t)this->battery_soc_, ... };
    // this->canbus_->send_data(0x??? , true, data);
}

void EfPs::dump_config() {
    ESP_LOGCONFIG(TAG, "EcoFlow PS Bridge:");
    LOG_UPDATE_INTERVAL(this);
}

}  // namespace ef_ps
}  // namespace esphome
