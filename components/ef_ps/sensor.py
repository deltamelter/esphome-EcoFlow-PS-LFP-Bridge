import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_BATTERY_LEVEL,
    CONF_ID,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_WATT,
    UNIT_CELSIUS,
    UNIT_AMPERE_HOURS,
)
from . import ef_ps_ns, EfPs  # Imports the namespace and Hub class from __init__.py

# Define the keys that match your YAML
CONF_EF_PS_ID = "ef_ps_id"
CONF_BATTERY_SOC = "battery_soc"
CONF_BATTERY_VOLTAGE = "battery_voltage"
CONF_BATTERY_CURRENT = "battery_current"
CONF_BATTERY_POWER = "battery_power"
CONF_BATTERY_TEMPERATURE = "battery_temperature"
CONF_REMAINING_CAPACITY = "remaining_capacity"
CONF_FULL_CAPACITY = "full_capacity"
CONF_CYCLES = "cycles"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_EF_PS_ID): cv.use_id(EfPs),
    # Battery SOC (%)
    cv.Optional(CONF_BATTERY_SOC): sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_BATTERY,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    # Voltage (V)
    cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    # Current (A)
    cv.Optional(CONF_BATTERY_CURRENT): sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_CURRENT,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    # Power (W)
    cv.Optional(CONF_BATTERY_POWER): sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_POWER,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    # Temperature (°C)
    cv.Optional(CONF_BATTERY_TEMPERATURE): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    # Capacity (Ah)
    cv.Optional(CONF_REMAINING_CAPACITY): sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE_HOURS,
        accuracy_decimals=2,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Optional(CONF_FULL_CAPACITY): sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE_HOURS,
        accuracy_decimals=2,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    # Cycles
    cv.Optional(CONF_CYCLES): sensor.sensor_schema(
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    # Get the parent Hub (EfPs)
    hub = await cg.get_variable(config[CONF_EF_PS_ID])

    # Loop through the sensors and register them in the C++ Hub class
    for key in [
        CONF_BATTERY_SOC, CONF_BATTERY_VOLTAGE, CONF_BATTERY_CURRENT,
        CONF_BATTERY_POWER, CONF_BATTERY_TEMPERATURE, 
        CONF_REMAINING_CAPACITY, CONF_FULL_CAPACITY, CONF_CYCLES
    ]:
        if key in config:
            sens = await sensor.new_sensor(config[key])
            # This calls 'set_battery_soc_sensor(sens)' in the C++ class
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))
