import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import uart
from esphome.const import CONF_ID, CONF_THROTTLE

AUTO_LOAD = ["binary_sensor", "sensor", "text_sensor"]

DEPENDENCIES = ["uart"]

CODEOWNERS = ["@cubinet-code"]

MULTI_CONF = True

CONF_VEDIRECT_ID = "vedirect_id"

vedirect_ns = cg.esphome_ns.namespace("vedirect")
VedirectComponent = vedirect_ns.class_("VedirectComponent", uart.UARTDevice, cg.Component)

CONFIG_SCHEMA = (
    cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(VedirectComponent),
        cv.Optional(CONF_THROTTLE, default="1s"): cv.positive_time_period_milliseconds,
    }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    cg.add(var.set_throttle(config[CONF_THROTTLE]))
