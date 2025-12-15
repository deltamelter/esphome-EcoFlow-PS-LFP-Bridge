#pragma once

#include "esphome/core/component.h"
#include "esphome/components/canbus/canbus.h"
#include <vector>

namespace ef_ps {

class EfPsComponent : public esphome::PollingComponent {
 public:
  static EfPsComponent *instance;
  void set_canbus(esphome::canbus::Canbus *canbus) {
    this->canbus_ = canbus;
  }

  void setup() override;
  void loop() override;
  void update() override;
  void dump_config() override;
  void send_data(uint32_t id, const std::vector<uint8_t> &payload);

 protected:
  esphome::canbus::Canbus *canbus_{nullptr};

  static void on_can_frame(const esphome::canbus::CanFrame &frame);
};

}  // namespace ef_ps
