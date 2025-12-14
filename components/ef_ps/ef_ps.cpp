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
	if (!bus || !bus->canbus_) return;

	esphome::canbus::CanFrame frame;
	frame.can_id = id;
	frame.extended = true;
	frame.data_length_code = len;
	memcpy(frame.data, data, len);

	bus->canbus_->send(frame);
}

// ===== singleton pointer =====
EfPsComponent *EfPsComponent::instance = nullptr;

void EfPsComponent::setup() {
	ESP_LOGI(TAG, "Setting up EcoFlow PS CAN LFP Bridge");

	instance = this;

	ecoflowMessagesInit();

	this->canbus_->add_on_frame_callback(
		[](const esphome::canbus::CanFrame &frame) {
			twai_message_t rx{};
			rx.identifier = frame.can_id;
			rx.extd = frame.extended;
			rx.data_length_code = frame.data_length_code;
			memcpy(rx.data, frame.data, frame.data_length_code);

			processEcoFlowCAN(rx);
		}
	);
}

void EfPsComponent::loop() {
	canTxSequencerTick();
}

void EfPsComponent::dump_config() {
	ESP_LOGCONFIG(TAG, "EcoFlow PS CAN LFP Bridge");
}

}  // namespace ef_ps
