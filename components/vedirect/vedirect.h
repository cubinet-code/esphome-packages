#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace vedirect {

class VedirectComponent : public uart::UARTDevice, public Component {
  public:
    VedirectComponent() = default;
    void set_throttle(uint32_t throttle) { this->throttle_ = throttle; }
    void loop() override;
    
    // SENSORS
    void set_load_off_reason_text_sensor(text_sensor::TextSensor *load_off_reason_text_sensor) {
      load_off_reason_text_sensor_ = load_off_reason_text_sensor;
    }

    void dump_config() override;
    
  protected:

    void publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state);
    
    text_sensor::TextSensor *load_off_reason_text_sensor_{nullptr};

    bool publishing_{true};

    int state_{0};
    std::string label_;
    std::string value_;
    std::string hex_data_;
    
    uint32_t last_transmission_{0};
    uint32_t last_publish_{0};
    uint32_t throttle_{0};
};

}  // namespace vedirect
}  // namespace esphome
