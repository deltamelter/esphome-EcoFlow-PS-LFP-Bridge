#include "ef_ps.h"
#include "esphome/core/log.h"

extern "C" {
	#include <string.h>
}

// ===== include your original headers =====
#include "ecoflow.h"
#include "can.h"

namespace ef_ps {

static const char *TAG = "ef_ps";

// Bridge ESPHome → your sendCANFrame()
void sendCANFrame(uint32_t id, const uint8_t *data, uint8_t len) {
    auto *bus = EfPsComponent::instance;
    if (!bus) return;

    std::vector<uint8_t> payload(data, data + len);
    bus->send_data(id, payload);
}

// ===== singleton pointer =====
EfPsComponent *EfPsComponent::instance = nullptr;

void EfPsComponent::setup() {
	ESP_LOGI(TAG, "Setting up EcoFlow PS CAN LFP Bridge");

	instance = this;

	ecoflowMessagesInit();

	this->canbus_->add_callback(
		[](uint32_t can_id, bool extended_id, bool rtr, const std::vector<uint8_t> &data) {
			(void)rtr;
			ef_twai_message_t rx{};
			rx.identifier = can_id;
			rx.extd = extended_id;
			rx.data_length_code = (uint8_t)std::min<size_t>(data.size(), 8);
			memcpy(rx.data, data.data(), rx.data_length_code);

			processEcoFlowCAN(rx);
		}
	);
}

void EfPsComponent::loop() {
	canTxSequencerTick();
}

void EfPsComponent::update() {
    canTxSequencerTick();
}

void EfPsComponent::send_data(uint32_t id, const std::vector<uint8_t> &payload) {
	if (!this->canbus_) return;
	this->canbus_->send_data(id, /*use_extended_id=*/true, payload);
}

void EfPsComponent::dump_config() {
	ESP_LOGCONFIG(TAG, "EcoFlow PS CAN LFP Bridge");
}

}  // namespace ef_ps

// Provide a global wrapper so legacy unqualified calls from `ecoflow.cpp`
// resolve to the namespaced implementation.
void sendCANFrame(uint32_t id, const uint8_t *data, uint8_t len) {
	ef_ps::sendCANFrame(id, data, len);
}
