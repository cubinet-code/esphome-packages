#include "vedirect.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome
{
  namespace vedirect
  {

    static const char *const TAG = "vedirect";

    void VedirectComponent::dump_config() {
      ESP_LOGCONFIG(TAG, "VE.Direct:");
    }

    void VedirectComponent::loop()
    {

      const uint32_t now = millis();
      if ((state_ > 0) && (now - last_transmission_ >= 200))
      {
        // last transmission too long ago. Reset RX index.
        ESP_LOGW(TAG, "Last transmission too long ago");
        state_ = 0;
      }

      if (!available())
        return;

      last_transmission_ = now;

      while (available())
      {
        uint8_t c;
        read_byte(&c);

        if (state_ == 0)
        {
          if (c == '\r' || c == '\n')
          {
            continue;
          }
          // Text Data
          label_.clear();
          value_.clear();
          // HEX Data
          hex_data_.clear();
          // Move to Receive
          state_ = 1;
        }

        if (state_ == 1)
        {

          // Start of a ve.direct hex frame
          if (c == ':')
          {
            state_ = 3;
            continue;
          }

          if (c == '\t')
          {
            state_ = 2;
          }
          else
          {
            label_.push_back(c);
          }

          continue;
        }
        if (state_ == 2)
        {
          if (label_ == "Checksum")
          {
            state_ = 0;
            // The checksum is used as end of frame indicator
            if (now - this->last_publish_ >= this->throttle_)
            {
              this->last_publish_ = now;
              this->publishing_ = true;
            }
            else
            {
              this->publishing_ = false;
            }
            continue;
          }
          if (c == '\r' || c == '\n')
          {
            if (this->publishing_)
            {
              // handle_value_();
            }
            state_ = 0;
          }
          else
          {
            value_.push_back(c);
          }
        }
        // ve.direct hex frame
        if (state_ == 3)
        {
          // End of HEX protocol line
          if (c == '\r' || c == '\n')
          {
            std::string register_, flag_, raw_;
            raw_ = hex_data_;
            // Extract Command
            long int cmd_ = strtol(hex_data_.substr(0, 1).c_str(), NULL, 16);
            hex_data_.erase(0, 1);
            // Extract Checksum
            long int checksum_ = strtol( hex_data_.substr(hex_data_.size() - 2).c_str(), NULL, 16);
            hex_data_.erase(hex_data_.length() - 2);

            if (cmd_ == 0x7 || cmd_ == 0x8 || cmd_ == 0xa)
            {
              // Extract Register
              register_ = hex_data_.substr(0, 4);
              register_ = register_.substr(2, 2) + register_.substr(0, 2);
              hex_data_.erase(0, 4);
              // Extract Flag
              flag_ = hex_data_.substr(0, 2);
              hex_data_.erase(0, 2);
              ESP_LOGE("victron", "CMD:%lx REG:%s FLAG:%s DATA:%s CS:%02lx RAW:%s", cmd_, register_.c_str(), flag_.c_str(), hex_data_.c_str(), checksum_, raw_.c_str());

              this->publish_state_(load_off_reason_text_sensor_, hex_data_);  // NOLINT(cert-err34-c)

            }
            else
            {
              ESP_LOGE("victron", "CMD:%lx DATA:%s CS:%02lx", cmd_, hex_data_.c_str(), checksum_);
            }
            state_ = 0;
          }
          else
          {
            hex_data_.push_back(c);
          }
        }
      }
    }

    void VedirectComponent::publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state) {
      if (text_sensor == nullptr)
        return;

      text_sensor->publish_state(state);
    }

  }
}
