import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import canbus
from esphome.const import CONF_ID, CONF_UPDATE_INTERVAL

DEPENDENCIES = ["canbus"]
AUTO_LOAD = ["canbus", "sensor"]

ef_ps_ns = cg.esphome_ns.namespace("ef_ps")

# FIX: Remove cg.Component from the list. CanbusTrigger handles it.
CanbusTrigger = canbus.canbus_ns.class_("CanbusTrigger")
EfPs = ef_ps_ns.class_("EfPs", CanbusTrigger)
CONF_CANBUS_ID = "canbus_id"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(EfPs),
        cv.Required(CONF_CANBUS_ID): cv.use_id(canbus.CanbusComponent),
        cv.Optional(CONF_UPDATE_INTERVAL, default="1s"): cv.update_interval,
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    can = await cg.get_variable(config[CONF_CANBUS_ID])
    # Matches the method in our simplified ef_ps.h
    cg.add(var.set_canbus_id(can))
    
    if CONF_UPDATE_INTERVAL in config:
        cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))
