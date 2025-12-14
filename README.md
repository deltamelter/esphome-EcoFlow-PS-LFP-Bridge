# EcoFlow PowerStream ESPHome Component

A complete ESPHome external component that emulates an EcoFlow Delta 2 battery on the CAN bus, enabling the PowerStream inverter to work with any battery system.

## 📁 Project Structure

```
your-esphome-project/
├── ecoflow-powerstream.yaml          # Your device configuration
├── secrets.yaml                      # WiFi and API credentials
└── components/
    └── ecoflow_powerstream/
        ├── __init__.py               # Python component registration
        ├── ecoflow_powerstream.h     # C++ header file
        └── ecoflow_powerstream.cpp   # C++ implementation
```

## ✨ Features

### Core Functionality
- ✅ **Complete Delta 2 Emulation** - Sends all required CAN messages (0x351, 0x355, 0x356, 0x35A, 0x35E)
- ✅ **Configurable Parameters** - Adjust charge/discharge limits, voltages, and capacity
- ✅ **Home Assistant Integration** - Full auto-discovery with all entities
- ✅ **Real-time Monitoring** - Live voltage, current, SOC, temperature, and power
- ✅ **Status Indicators** - Charging, discharging, and connection status
- ✅ **Enable/Disable Switch** - Control CAN transmission on demand
- ✅ **Web Interface** - Configure and monitor via built-in web server

### External Component Architecture
- Clean separation between CAN protocol and battery data
- Easy integration with any BMS (JBD, Daly, JK-BMS, etc.)
- Public API for feeding real battery data
- Simulated mode for testing without hardware

## 🔧 Hardware Requirements

### Minimum Setup
- ESP32 development board
- CAN transceiver (SN65HVD230 recommended)
- Connection to PowerStream battery port (XT150)

### Recommended: LilyGO T-CAN485
- Built-in ESP32 + CAN transceiver
- Compact design
- Direct plug-and-play

## 📦 Installation

### 1. Create Project Structure

```bash
mkdir -p ~/esphome-ecoflow/components/ecoflow_powerstream
cd ~/esphome-ecoflow
```

### 2. Copy Component Files

Copy the following files from the artifacts:

**Component Files:**
- `components/ecoflow_powerstream/__init__.py`
- `components/ecoflow_powerstream/ecoflow_powerstream.h`
- `components/ecoflow_powerstream/ecoflow_powerstream.cpp`

**Configuration Files:**
- `ecoflow-powerstream.yaml` (example configuration)
- `secrets.yaml` (create with your credentials)

### 3. Create secrets.yaml

```yaml
wifi_ssid: "YourWiFiNetwork"
wifi_password: "YourWiFiPassword"
api_encryption_key: "your-32-char-encryption-key"
ota_password: "your-ota-password"
```

Generate encryption key:
```bash
esphome wizard ecoflow-powerstream.yaml
```

### 4. Compile and Flash

```bash
# First time - USB cable required
esphome run ecoflow-powerstream.yaml

# Subsequent updates - OTA
esphome run ecoflow-powerstream.yaml --device ecoflow-powerstream.local
```

## 🔌 Wiring

### PowerStream Battery Port (XT150)

```
PowerStream XT150 Connector (looking at face):
┌─────────────┐
│ 1       2   │  1,2: Wakeup (optional)
│             │
│ 3       4   │  3: CAN High, 4: CAN Low  
│             │
│ 5       6   │  5,6: Battery Enable (SHORT THESE!)
└─────────────┘
```

**CRITICAL:** Pins 5 & 6 MUST be shorted together to enable the battery port!

### ESP32 to CAN Transceiver (SN65HVD230)

```
ESP32          CAN Transceiver      PowerStream
──────────────────────────────────────────────────
GPIO5    →     TX                →   
GPIO4    ←     RX                ←   
3.3V     →     VCC               
GND      →     GND    
                CANH              →   Pin 3 (CAN High)
                CANL              →   Pin 4 (CAN Low)
```

### LilyGO T-CAN485 Specific

The example YAML includes the required GPIO configuration for the T-CAN485 board:
- GPIO16: CAN boost power enable
- GPIO23: CAN transceiver mode select

## ⚙️ Configuration

### Basic Configuration

```yaml
external_components:
  - source:
      type: local
      path: components
    components: [ecoflow_powerstream]

canbus:
  - platform: esp32_can
    id: ecoflow_can
    tx_pin: GPIO5
    rx_pin: GPIO4
    bit_rate: 500kbps

ecoflow_powerstream:
  id: ecoflow_bridge
  canbus_id: ecoflow_can
  update_interval: 1s
  
  voltage:
    name: "Battery Voltage"
  
  current:
    name: "Battery Current"
  
  soc:
    name: "Battery SOC"
  
  # ... additional sensors and controls
```

### Available Configuration Options

#### Sensors (Read-Only)
- `voltage` - Battery voltage (V)
- `current` - Battery current (A, negative = discharge)
- `soc` - State of charge (%)
- `temperature` - Battery temperature (°C)
- `power` - Calculated power (W)
- `remaining_capacity` - Remaining capacity (Ah)

#### Numbers (Configurable)
- `max_charge_current` - Maximum charge current limit (A)
- `max_discharge_current` - Maximum discharge current limit (A)
- `battery_capacity` - Total battery capacity (Ah)
- `charge_voltage` - Charge voltage limit (V)
- `discharge_voltage` - Discharge voltage limit (V)
- `full_capacity` - Full capacity (Ah)

#### Switch
- `enabled` - Enable/disable CAN transmission

#### Binary Sensors
- `charging` - Charging status
- `discharging` - Discharging status

#### Text Sensor
- `status` - Overall status text

## 🔗 Integration with Real BMS

### Method 1: Using Public API (Recommended)

The component provides public methods to set battery values:

```yaml
interval:
  - interval: 1s
    then:
      - lambda: |-
          // Get values from your BMS component
          float voltage = id(your_bms_voltage).state;
          float current = id(your_bms_current).state;
          float soc = id(your_bms_soc).state;
          float temp = id(your_bms_temp).state;
          
          // Feed to EcoFlow component
          id(ecoflow_bridge)->set_battery_voltage(voltage);
          id(ecoflow_bridge)->set_battery_current(current);
          id(ecoflow_bridge)->set_battery_soc(soc);
          id(ecoflow_bridge)->set_battery_temperature(temp);
```

### Method 2: Using Sensor Configuration

Connect BMS sensors directly in the component config:

```yaml
# Your BMS component
jbd_bms:
  id: my_bms
  uart_id: bms_uart
  voltage:
    id: bms_voltage
  current:
    id: bms_current
  # ... other sensors

# EcoFlow component uses BMS sensors
ecoflow_powerstream:
  id: ecoflow_bridge
  canbus_id: ecoflow_can
  voltage: bms_voltage      # Reference BMS sensor
  current: bms_current      # Reference BMS sensor
  # ... other sensors
```

### Example: JBD BMS Integration

```yaml
uart:
  - id: bms_uart
    tx_pin: GPIO17
    rx_pin: GPIO16
    baud_rate: 9600

external_components:
  - source:
      type: local
      path: components
    components: [jbd_bms, ecoflow_powerstream]

jbd_bms:
  id: my_bms
  uart_id: bms_uart
  voltage:
    name: "BMS Voltage"
    id: bms_voltage
  current:
    name: "BMS Current"
    id: bms_current
  soc:
    name: "BMS SOC"
    id: bms_soc
  temperature:
    name: "BMS Temperature"
    id: bms_temp

ecoflow_powerstream:
  id: ecoflow_bridge
  canbus_id: ecoflow_can
  voltage: bms_voltage
  current: bms_current
  soc: bms_soc
  temperature: bms_temp
  # ... configuration
```

## 🎮 Testing Without Hardware

The example configuration includes simulated battery controls for testing:

1. **Flash the device** with example configuration
2. **Access web interface** at `ecoflow-powerstream.local`
3. **Adjust values** using sliders:
   - Simulated Voltage (40-58V)
   - Simulated Current (-100 to +100A)
   - Simulated SOC (0-100%)
   - Simulated Temperature (-20 to 60°C)
4. **Monitor CAN transmission** in logs
5. **Verify PowerStream** recognizes the battery

## 🏠 Home Assistant Integration

All entities auto-discover in Home Assistant:

### Entities Created

**Sensors:**
- `sensor.ecoflow_powerstream_voltage`
- `sensor.ecoflow_powerstream_current`
- `sensor.ecoflow_powerstream_soc`
- `sensor.ecoflow_powerstream_temperature`
- `sensor.ecoflow_powerstream_power`
- `sensor.ecoflow_powerstream_remaining_capacity`

**Numbers:**
- `number.ecoflow_powerstream_max_charge_current`
- `number.ecoflow_powerstream_max_discharge_current`
- `number.ecoflow_powerstream_battery_capacity`
- `number.ecoflow_powerstream_charge_voltage_limit`
- `number.ecoflow_powerstream_discharge_voltage_limit`

**Switches:**
- `switch.ecoflow_powerstream_can_transmission`

**Binary Sensors:**
- `binary_sensor.ecoflow_powerstream_charging`
- `binary_sensor.ecoflow_powerstream_discharging`

**Text Sensors:**
- `sensor.ecoflow_powerstream_status`

### Example Automations

**Stop Charging at 90% SOC:**
```yaml
automation:
  - alias: "Stop Charging at 90%"
    trigger:
      platform: numeric_state
      entity_id: sensor.ecoflow_powerstream_soc
      above: 90
    action:
      - service: number.set_value
        target:
          entity_id: number.ecoflow_powerstream_max_charge_current
        data:
          value: 0
```

**Emergency Disconnect on High Temperature:**
```yaml
automation:
  - alias: "Emergency Temp Shutdown"
    trigger:
      platform: numeric_state
      entity_id: sensor.ecoflow_powerstream_temperature
      above: 45
    action:
      - service: switch.turn_off
        target:
          entity_id: switch.ecoflow_powerstream_can_transmission
      - service: notify.mobile_app
        data:
          message: "Battery overheating! Disconnected from PowerStream."
```

## 🐛 Troubleshooting

### PowerStream Shows "Abnormal Voltage"

1. ✅ Verify pins 5 & 6 are shorted on XT150 connector
2. ✅ Check CAN High/Low wiring (pins 3 & 4)
3. ✅ Confirm 120Ω termination resistor
4. ✅ Check CAN transceiver has power (3.3V)
5. ✅ Verify bit rate is 500kbps

### No CAN Communication

1. ✅ Check TX/RX pins match your wiring
2. ✅ For T-CAN485: Verify GPIO16 and GPIO23 are configured
3. ✅ Look for "CAN TX" messages in logs
4. ✅ Test with CAN bus analyzer if available

### PowerStream Not Recognizing Battery

1. ✅ Ensure all CAN messages are being sent (check logs)
2. ✅ Verify voltage is within range (40-58V)
3. ✅ Check that CAN transmission is enabled
4. ✅ Try restarting PowerStream after ESP32 is running
5. ✅ Monitor logs for any CAN errors

### View Detailed Logs

```bash
esphome logs ecoflow-powerstream.yaml
```

Look for:
```
[D][ecoflow_powerstream:xxx] CAN TX - V:51.20V I:10.50A SOC:75% T:25.0°C
[D][canbus:xxx] send standard id=0x351 ...
```

## 📡 CAN Protocol Reference

### Message IDs and Formats

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