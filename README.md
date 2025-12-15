ecoflow_powerstream:
# EcoFlow PowerStream Bridge (ESPHome component)

This repository contains an ESPHome external component that implements an EcoFlow Delta2-style CAN message stream and a small CAN-to-EcoFlow bridge component (`ef_ps`). It's intended to let an ESP32 present a battery to a PowerStream inverter over CAN by sending the messages PowerStream expects.

**Quick highlights:**
- Component: `components/ef_ps` — ESPHome Python registration + C++ implementation
- Example device config: `ecoflow-powerstream.yaml`
- Local stubs for development: `components/ef_ps/stubs.cpp`

**Note:** This repo contains experimental code and stubs used to validate builds locally. Replace stubs with real integrations (BMS, web, etc.) for production.

**Contents**
- **Component code:** [components/ef_ps](components/ef_ps)
  - `__init__.py` — ESPHome schema & `to_code()` registration
  - `ef_ps.h` / `ef_ps.cpp` — Core C++ component, CAN bridge, runtime hooks
  - `ecoflow.h` / `ecoflow.cpp` — EcoFlow message framing, CRC, message sequencer and handlers
  - `can.h` — Minimal CAN helper types used locally
  - `stubs.cpp` — Local stub implementations so `esphome config` can validate without full dependencies
- **Examples:** `ecoflow-powerstream.yaml` and `examples/ecoflow-test.yaml` — Example top-level configs used for validation and quick testing
- **Wiring notes:** `WIRING.md` — Wiring diagrams and safety tips (see `docs/weact-wiring.svg` for WeAct diagram)
- **Secrets for local testing:** `secrets.yaml` (not committed with real secrets)

**Usage**

1) Add this repo as an `external_components` source in your ESPHome YAML, or copy the `components/ef_ps` dir into your project.

Example using git source (recommended):

**WeAct (ESP32-C3) example:** see `examples/ecoflow-weact.yaml` for a WeAct-specific minimal example. The example uses `GPIO5`/`GPIO4` as suggested defaults for `tx_pin`/`rx_pin`; replace `board` with your WeAct board/variant and change pins if needed.

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/deltamelter/esphome-EcoFlow-PS-LFP-Bridge
      ref: main
    components: [ef_ps]

# PowerStream expects 1Mbps CAN. If your board supports it set `bit_rate: 1Mbps`.
canbus:
  - platform: esp32_can
    id: ecoflow_can
    tx_pin: GPIO5
    rx_pin: GPIO4
    bit_rate: 1Mbps
# Note: many ESP32 boards/ESPHome drivers do not support 1Mbps — use `500kbps` for compatibility where necessary.

ef_ps:
  id: ecoflow_bridge
  canbus_id: ecoflow_can
  update_interval: 1s
```

2) Validate the configuration locally before flashing:

```bash
esphome config ecoflow-powerstream.yaml
```

3) Build/flash with `esphome run` as usual.

Development notes
- The component currently exposes an internal `EfPsComponent` C++ class; `ef_ps` registers itself with ESPHome and hooks into the `CanbusComponent` to receive/send frames.
- `components/ef_ps/ecoflow.cpp` contains the message encoders/decoders and a transmit sequencer used to keep PowerStream happy.
- `components/ef_ps/stubs.cpp` provides simple, local-only implementations so the component can be validated with `esphome config` and basic builds.

Testing and validation
- Use `esphome config <your-yaml>` to validate schema and local components.
- `esphome logs <your-yaml>` is useful to observe CAN frames and the EcoFlow message logs.
- CI: This repository includes a GitHub Actions workflow that validates example YAMLs and renders wiring SVGs to PNG thumbnails (uploaded as build artifacts).

Contributing
- Add issues/PRs for bugs or improvements. If you add real BMS integrations, replace the stubs and include tests or example YAML.

License
- See the `LICENSE` file in the repository for license details.

--
If you'd like, I can also add a short example snippet to `ecoflow-powerstream.yaml` or expand `README.md` with wiring diagrams and a minimal test script. Would you like that added?

Uses ID ranges 0x10003001, 0x10103001, 0x10203001 for transmission
Receives on 0x10014001, 0x10114001, 0x10214001
XOR encryption on payloads with dynamic keys
CRC16 checksums on entire messages
Complex multi-frame protocol with headers and payloads
Message sequencer that sends messages in a specific order/timing

Key Message Types:

0xC4 - Heartbeat from PowerStream (contains serial number)
0x3C - Battery status reply
0x13 - Detailed battery info (cell voltages, temps, etc.)
0x4F - Power/SOC status
0x68 - Extended battery info
0x70 - Serial number broadcast
0x0B - Voltage messages (multiple variants)
0xCB - BMS limit acknowledgments
0x5C, 0x8C, 0x24 - Various other messages
0xDE - Triggers for firmware version responses



## 🔒 Safety Warnings

⚠️ **CRITICAL SAFETY INFORMATION**

1. **Experimental project** - use at your own risk
2. **Monitor continuously** during initial testing
3. **Start with low limits** - 20-30A initially
4. **Never leave unattended** until proven stable
5. **Fire safety equipment** - keep extinguisher nearby
6. **Proper wire gauge** - match to current ratings
7. **Fuses/breakers** - protect all connections
8. **Insurance implications** - this modifies EcoFlow equipment

### Recommended Testing Procedure

1. Test CAN-only (no battery connected)
2. Verify PowerStream recognition
3. Connect via current-limited supply
4. Monitor for 24 hours at low power
5. Gradually increase limits
6. Check cell balance weekly

## 📚 References

- [EcoFlow CAN Reverse Engineering](https://github.com/bulldog5046/EcoFlow-CanBus-Reverse-Engineering)
- [Original PlatformIO Bridge](https://github.com/RGarrett93/EcoFlow-PS-LFP-Bridge)
- [ESPHome CAN Bus Docs](https://esphome.io/components/canbus.html)

## 🤝 Contributing

Improvements and bug reports welcome! This is a community project.

## 📄 License

Provided as-is for educational purposes. Not affiliated with or endorsed by EcoFlow.

---

**Disclaimer:** Modifying EcoFlow equipment may void warranties and violate local codes. You assume all risks for using this information.