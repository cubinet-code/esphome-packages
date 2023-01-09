import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID, CONF_STATUS

from . import CONF_VEDIRECT_ID, VedirectComponent

TYPES = [
    CONF_STATUS,
]

DEPENDENCIES = ["vedirect"]

CODEOWNERS = ["@cubinet-code"]

CONF_LOAD_OFF_REASON = "load_off_reason"

TEXT_SENSORS = [
    CONF_LOAD_OFF_REASON,
]

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_VEDIRECT_ID): cv.use_id(VedirectComponent),
            cv.Optional(CONF_STATUS): text_sensor.text_sensor_schema(),
            cv.Optional(CONF_LOAD_OFF_REASON): text_sensor.TEXT_SENSOR_SCHEMA.extend(
                {cv.GenerateID(): cv.declare_id(text_sensor.TextSensor)}
            ),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)

async def setup_conf(config, key, hub):
    if key in config:
        conf = config[key]
        sens = await text_sensor.new_text_sensor(conf)
        cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))

async def to_code(config):
    hub = await cg.get_variable(config[CONF_VEDIRECT_ID])
    for key in TYPES:
        await setup_conf(config, key, hub)