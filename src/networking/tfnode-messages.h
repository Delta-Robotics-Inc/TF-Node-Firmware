/*
 *  This file is generated with Embedded Proto, PLEASE DO NOT EDIT!
 *  source: tfnode-messages.proto
 */

// This file is generated. Please do not edit!
#ifndef TFNODE-MESSAGES_H
#define TFNODE-MESSAGES_H

#include <cstdint>
#include <MessageInterface.h>
#include <WireFormatter.h>
#include <Fields.h>
#include <MessageSizeCalculator.h>
#include <ReadBufferSection.h>
#include <RepeatedFieldFixedSize.h>
#include <FieldStringBytes.h>
#include <Errors.h>
#include <Defines.h>
#include <limits>

// Include external proto definitions

namespace tfnode {

enum class FunctionCode : uint32_t
{
  FUNCTION_RESET = 0,
  FUNCTION_ENABLE = 1,
  FUNCTION_DISABLE = 2,
  FUNCTION_SET_MODE = 3,
  FUNCTION_SET_SETPOINT = 4,
  FUNCTION_CONFIGURE = 5,
  FUNCTION_STATUS = 6,
  FUNCTION_SILENCE_NODE = 7
};

enum class ResponseCode : uint32_t
{
  RESPONSE_SUCCESS = 0,
  RESPONSE_FAILED = 1,
  RESPONSE_INVALID_DEVICE = 2,
  RESPONSE_UNSUPPORTED_COMMAND = 3,
  RESPONSE_ERROR = 4
};

enum class Device : uint32_t
{
  DEVICE_ALL = 0,
  DEVICE_NODE = 1,
  DEVICE_PORTALL = 2,
  DEVICE_PORT1 = 3,
  DEVICE_PORT2 = 4
};

enum class DeviceStatusMode : uint32_t
{
  STATUS_NONE = 0,
  STATUS_COMPACT = 1,
  STATUS_DUMP = 2,
  STATUS_DUMP_READABLE = 3
};

enum class SMAControlMode : uint32_t
{
  MODE_PERCENT = 0,
  MODE_AMPS = 1,
  MODE_VOLTS = 2,
  MODE_OHMS = 3,
  MODE_TRAIN = 4,
  MODE_CNT
};

class DisableCommand final: public ::EmbeddedProto::MessageInterface
{
  public:
    DisableCommand() = default;
    DisableCommand(const DisableCommand& rhs )
    {
      set_device(rhs.get_device());
    }

    DisableCommand(const DisableCommand&& rhs ) noexcept
    {
      set_device(rhs.get_device());
    }

    ~DisableCommand() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      DEVICE = 1
    };

    DisableCommand& operator=(const DisableCommand& rhs)
    {
      set_device(rhs.get_device());
      return *this;
    }

    DisableCommand& operator=(const DisableCommand&& rhs) noexcept
    {
      set_device(rhs.get_device());
      return *this;
    }

    static constexpr char const* DEVICE_NAME = "device";
    inline void clear_device() { device_.clear(); }
    inline void set_device(const Device& value) { device_ = value; }
    inline void set_device(const Device&& value) { device_ = value; }
    inline const Device& get_device() const { return device_.get(); }
    inline Device device() const { return device_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((static_cast<Device>(0) != device_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = device_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEVICE), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::DEVICE:
            return_value = device_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_device();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::DEVICE:
          name = DEVICE_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = device_.to_string(left_chars, indent_level + 2, DEVICE_NAME, true);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::enumeration<Device> device_ = static_cast<Device>(0);

};

class EnableCommand final: public ::EmbeddedProto::MessageInterface
{
  public:
    EnableCommand() = default;
    EnableCommand(const EnableCommand& rhs )
    {
      set_device(rhs.get_device());
    }

    EnableCommand(const EnableCommand&& rhs ) noexcept
    {
      set_device(rhs.get_device());
    }

    ~EnableCommand() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      DEVICE = 1
    };

    EnableCommand& operator=(const EnableCommand& rhs)
    {
      set_device(rhs.get_device());
      return *this;
    }

    EnableCommand& operator=(const EnableCommand&& rhs) noexcept
    {
      set_device(rhs.get_device());
      return *this;
    }

    static constexpr char const* DEVICE_NAME = "device";
    inline void clear_device() { device_.clear(); }
    inline void set_device(const Device& value) { device_ = value; }
    inline void set_device(const Device&& value) { device_ = value; }
    inline const Device& get_device() const { return device_.get(); }
    inline Device device() const { return device_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((static_cast<Device>(0) != device_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = device_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEVICE), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::DEVICE:
            return_value = device_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_device();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::DEVICE:
          name = DEVICE_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = device_.to_string(left_chars, indent_level + 2, DEVICE_NAME, true);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::enumeration<Device> device_ = static_cast<Device>(0);

};

class GeneralResponse final: public ::EmbeddedProto::MessageInterface
{
  public:
    GeneralResponse() = default;
    GeneralResponse(const GeneralResponse& rhs )
    {
      set_device(rhs.get_device());
      set_received_cmd(rhs.get_received_cmd());
      set_response_code(rhs.get_response_code());
    }

    GeneralResponse(const GeneralResponse&& rhs ) noexcept
    {
      set_device(rhs.get_device());
      set_received_cmd(rhs.get_received_cmd());
      set_response_code(rhs.get_response_code());
    }

    ~GeneralResponse() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      DEVICE = 1,
      RECEIVED_CMD = 2,
      RESPONSE_CODE = 3
    };

    GeneralResponse& operator=(const GeneralResponse& rhs)
    {
      set_device(rhs.get_device());
      set_received_cmd(rhs.get_received_cmd());
      set_response_code(rhs.get_response_code());
      return *this;
    }

    GeneralResponse& operator=(const GeneralResponse&& rhs) noexcept
    {
      set_device(rhs.get_device());
      set_received_cmd(rhs.get_received_cmd());
      set_response_code(rhs.get_response_code());
      return *this;
    }

    static constexpr char const* DEVICE_NAME = "device";
    inline void clear_device() { device_.clear(); }
    inline void set_device(const Device& value) { device_ = value; }
    inline void set_device(const Device&& value) { device_ = value; }
    inline const Device& get_device() const { return device_.get(); }
    inline Device device() const { return device_.get(); }

    static constexpr char const* RECEIVED_CMD_NAME = "received_cmd";
    inline void clear_received_cmd() { received_cmd_.clear(); }
    inline void set_received_cmd(const FunctionCode& value) { received_cmd_ = value; }
    inline void set_received_cmd(const FunctionCode&& value) { received_cmd_ = value; }
    inline const FunctionCode& get_received_cmd() const { return received_cmd_.get(); }
    inline FunctionCode received_cmd() const { return received_cmd_.get(); }

    static constexpr char const* RESPONSE_CODE_NAME = "response_code";
    inline void clear_response_code() { response_code_.clear(); }
    inline void set_response_code(const ResponseCode& value) { response_code_ = value; }
    inline void set_response_code(const ResponseCode&& value) { response_code_ = value; }
    inline const ResponseCode& get_response_code() const { return response_code_.get(); }
    inline ResponseCode response_code() const { return response_code_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((static_cast<Device>(0) != device_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = device_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEVICE), buffer, false);
      }

      if((static_cast<FunctionCode>(0) != received_cmd_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = received_cmd_.serialize_with_id(static_cast<uint32_t>(FieldNumber::RECEIVED_CMD), buffer, false);
      }

      if((static_cast<ResponseCode>(0) != response_code_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = response_code_.serialize_with_id(static_cast<uint32_t>(FieldNumber::RESPONSE_CODE), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::DEVICE:
            return_value = device_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::RECEIVED_CMD:
            return_value = received_cmd_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::RESPONSE_CODE:
            return_value = response_code_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_device();
      clear_received_cmd();
      clear_response_code();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::DEVICE:
          name = DEVICE_NAME;
          break;
        case FieldNumber::RECEIVED_CMD:
          name = RECEIVED_CMD_NAME;
          break;
        case FieldNumber::RESPONSE_CODE:
          name = RESPONSE_CODE_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = device_.to_string(left_chars, indent_level + 2, DEVICE_NAME, true);
      left_chars = received_cmd_.to_string(left_chars, indent_level + 2, RECEIVED_CMD_NAME, false);
      left_chars = response_code_.to_string(left_chars, indent_level + 2, RESPONSE_CODE_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::enumeration<Device> device_ = static_cast<Device>(0);
      EmbeddedProto::enumeration<FunctionCode> received_cmd_ = static_cast<FunctionCode>(0);
      EmbeddedProto::enumeration<ResponseCode> response_code_ = static_cast<ResponseCode>(0);

};

class NodeSettings final: public ::EmbeddedProto::MessageInterface
{
  public:
    NodeSettings() = default;
    NodeSettings(const NodeSettings& rhs )
    {
      set_can_id(rhs.get_can_id());
    }

    NodeSettings(const NodeSettings&& rhs ) noexcept
    {
      set_can_id(rhs.get_can_id());
    }

    ~NodeSettings() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      CAN_ID = 1
    };

    NodeSettings& operator=(const NodeSettings& rhs)
    {
      set_can_id(rhs.get_can_id());
      return *this;
    }

    NodeSettings& operator=(const NodeSettings&& rhs) noexcept
    {
      set_can_id(rhs.get_can_id());
      return *this;
    }

    static constexpr char const* CAN_ID_NAME = "can_id";
    inline void clear_can_id() { can_id_.clear(); }
    inline void set_can_id(const uint32_t& value) { can_id_ = value; }
    inline void set_can_id(const uint32_t&& value) { can_id_ = value; }
    inline uint32_t& mutable_can_id() { return can_id_.get(); }
    inline const uint32_t& get_can_id() const { return can_id_.get(); }
    inline uint32_t can_id() const { return can_id_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((0U != can_id_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = can_id_.serialize_with_id(static_cast<uint32_t>(FieldNumber::CAN_ID), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::CAN_ID:
            return_value = can_id_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_can_id();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::CAN_ID:
          name = CAN_ID_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = can_id_.to_string(left_chars, indent_level + 2, CAN_ID_NAME, true);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::uint32 can_id_ = 0U;

};

class NodeStatusCompact final: public ::EmbeddedProto::MessageInterface
{
  public:
    NodeStatusCompact() = default;
    NodeStatusCompact(const NodeStatusCompact& rhs )
    {
      set_uptime(rhs.get_uptime());
      set_error_code(rhs.get_error_code());
      set_v_supply(rhs.get_v_supply());
      set_pot_val(rhs.get_pot_val());
    }

    NodeStatusCompact(const NodeStatusCompact&& rhs ) noexcept
    {
      set_uptime(rhs.get_uptime());
      set_error_code(rhs.get_error_code());
      set_v_supply(rhs.get_v_supply());
      set_pot_val(rhs.get_pot_val());
    }

    ~NodeStatusCompact() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      UPTIME = 2,
      ERROR_CODE = 3,
      V_SUPPLY = 4,
      POT_VAL = 5
    };

    NodeStatusCompact& operator=(const NodeStatusCompact& rhs)
    {
      set_uptime(rhs.get_uptime());
      set_error_code(rhs.get_error_code());
      set_v_supply(rhs.get_v_supply());
      set_pot_val(rhs.get_pot_val());
      return *this;
    }

    NodeStatusCompact& operator=(const NodeStatusCompact&& rhs) noexcept
    {
      set_uptime(rhs.get_uptime());
      set_error_code(rhs.get_error_code());
      set_v_supply(rhs.get_v_supply());
      set_pot_val(rhs.get_pot_val());
      return *this;
    }

    static constexpr char const* UPTIME_NAME = "uptime";
    inline void clear_uptime() { uptime_.clear(); }
    inline void set_uptime(const uint32_t& value) { uptime_ = value; }
    inline void set_uptime(const uint32_t&& value) { uptime_ = value; }
    inline uint32_t& mutable_uptime() { return uptime_.get(); }
    inline const uint32_t& get_uptime() const { return uptime_.get(); }
    inline uint32_t uptime() const { return uptime_.get(); }

    static constexpr char const* ERROR_CODE_NAME = "error_code";
    inline void clear_error_code() { error_code_.clear(); }
    inline void set_error_code(const uint32_t& value) { error_code_ = value; }
    inline void set_error_code(const uint32_t&& value) { error_code_ = value; }
    inline uint32_t& mutable_error_code() { return error_code_.get(); }
    inline const uint32_t& get_error_code() const { return error_code_.get(); }
    inline uint32_t error_code() const { return error_code_.get(); }

    static constexpr char const* V_SUPPLY_NAME = "v_supply";
    inline void clear_v_supply() { v_supply_.clear(); }
    inline void set_v_supply(const float& value) { v_supply_ = value; }
    inline void set_v_supply(const float&& value) { v_supply_ = value; }
    inline float& mutable_v_supply() { return v_supply_.get(); }
    inline const float& get_v_supply() const { return v_supply_.get(); }
    inline float v_supply() const { return v_supply_.get(); }

    static constexpr char const* POT_VAL_NAME = "pot_val";
    inline void clear_pot_val() { pot_val_.clear(); }
    inline void set_pot_val(const float& value) { pot_val_ = value; }
    inline void set_pot_val(const float&& value) { pot_val_ = value; }
    inline float& mutable_pot_val() { return pot_val_.get(); }
    inline const float& get_pot_val() const { return pot_val_.get(); }
    inline float pot_val() const { return pot_val_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((0U != uptime_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = uptime_.serialize_with_id(static_cast<uint32_t>(FieldNumber::UPTIME), buffer, false);
      }

      if((0U != error_code_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = error_code_.serialize_with_id(static_cast<uint32_t>(FieldNumber::ERROR_CODE), buffer, false);
      }

      if((0.0 != v_supply_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = v_supply_.serialize_with_id(static_cast<uint32_t>(FieldNumber::V_SUPPLY), buffer, false);
      }

      if((0.0 != pot_val_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = pot_val_.serialize_with_id(static_cast<uint32_t>(FieldNumber::POT_VAL), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::UPTIME:
            return_value = uptime_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::ERROR_CODE:
            return_value = error_code_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::V_SUPPLY:
            return_value = v_supply_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::POT_VAL:
            return_value = pot_val_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_uptime();
      clear_error_code();
      clear_v_supply();
      clear_pot_val();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::UPTIME:
          name = UPTIME_NAME;
          break;
        case FieldNumber::ERROR_CODE:
          name = ERROR_CODE_NAME;
          break;
        case FieldNumber::V_SUPPLY:
          name = V_SUPPLY_NAME;
          break;
        case FieldNumber::POT_VAL:
          name = POT_VAL_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = uptime_.to_string(left_chars, indent_level + 2, UPTIME_NAME, true);
      left_chars = error_code_.to_string(left_chars, indent_level + 2, ERROR_CODE_NAME, false);
      left_chars = v_supply_.to_string(left_chars, indent_level + 2, V_SUPPLY_NAME, false);
      left_chars = pot_val_.to_string(left_chars, indent_level + 2, POT_VAL_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::uint32 uptime_ = 0U;
      EmbeddedProto::uint32 error_code_ = 0U;
      EmbeddedProto::floatfixed v_supply_ = 0.0;
      EmbeddedProto::floatfixed pot_val_ = 0.0;

};

class ResetCommand final: public ::EmbeddedProto::MessageInterface
{
  public:
    ResetCommand() = default;
    ResetCommand(const ResetCommand& rhs )
    {
      set_device(rhs.get_device());
    }

    ResetCommand(const ResetCommand&& rhs ) noexcept
    {
      set_device(rhs.get_device());
    }

    ~ResetCommand() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      DEVICE = 1
    };

    ResetCommand& operator=(const ResetCommand& rhs)
    {
      set_device(rhs.get_device());
      return *this;
    }

    ResetCommand& operator=(const ResetCommand&& rhs) noexcept
    {
      set_device(rhs.get_device());
      return *this;
    }

    static constexpr char const* DEVICE_NAME = "device";
    inline void clear_device() { device_.clear(); }
    inline void set_device(const Device& value) { device_ = value; }
    inline void set_device(const Device&& value) { device_ = value; }
    inline const Device& get_device() const { return device_.get(); }
    inline Device device() const { return device_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((static_cast<Device>(0) != device_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = device_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEVICE), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::DEVICE:
            return_value = device_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_device();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::DEVICE:
          name = DEVICE_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = device_.to_string(left_chars, indent_level + 2, DEVICE_NAME, true);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::enumeration<Device> device_ = static_cast<Device>(0);

};

class SMAControllerSettings final: public ::EmbeddedProto::MessageInterface
{
  public:
    SMAControllerSettings() = default;
    SMAControllerSettings(const SMAControllerSettings& rhs )
    {
      set_default_mode(rhs.get_default_mode());
      set_default_setpoint(rhs.get_default_setpoint());
      set_rctrl_kp(rhs.get_rctrl_kp());
      set_rctrl_ki(rhs.get_rctrl_ki());
      set_rctrl_kd(rhs.get_rctrl_kd());
    }

    SMAControllerSettings(const SMAControllerSettings&& rhs ) noexcept
    {
      set_default_mode(rhs.get_default_mode());
      set_default_setpoint(rhs.get_default_setpoint());
      set_rctrl_kp(rhs.get_rctrl_kp());
      set_rctrl_ki(rhs.get_rctrl_ki());
      set_rctrl_kd(rhs.get_rctrl_kd());
    }

    ~SMAControllerSettings() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      DEFAULT_MODE = 1,
      DEFAULT_SETPOINT = 2,
      RCTRL_KP = 3,
      RCTRL_KI = 4,
      RCTRL_KD = 5
    };

    SMAControllerSettings& operator=(const SMAControllerSettings& rhs)
    {
      set_default_mode(rhs.get_default_mode());
      set_default_setpoint(rhs.get_default_setpoint());
      set_rctrl_kp(rhs.get_rctrl_kp());
      set_rctrl_ki(rhs.get_rctrl_ki());
      set_rctrl_kd(rhs.get_rctrl_kd());
      return *this;
    }

    SMAControllerSettings& operator=(const SMAControllerSettings&& rhs) noexcept
    {
      set_default_mode(rhs.get_default_mode());
      set_default_setpoint(rhs.get_default_setpoint());
      set_rctrl_kp(rhs.get_rctrl_kp());
      set_rctrl_ki(rhs.get_rctrl_ki());
      set_rctrl_kd(rhs.get_rctrl_kd());
      return *this;
    }

    static constexpr char const* DEFAULT_MODE_NAME = "default_mode";
    inline void clear_default_mode() { default_mode_.clear(); }
    inline void set_default_mode(const SMAControlMode& value) { default_mode_ = value; }
    inline void set_default_mode(const SMAControlMode&& value) { default_mode_ = value; }
    inline const SMAControlMode& get_default_mode() const { return default_mode_.get(); }
    inline SMAControlMode default_mode() const { return default_mode_.get(); }

    static constexpr char const* DEFAULT_SETPOINT_NAME = "default_setpoint";
    inline void clear_default_setpoint() { default_setpoint_.clear(); }
    inline void set_default_setpoint(const float& value) { default_setpoint_ = value; }
    inline void set_default_setpoint(const float&& value) { default_setpoint_ = value; }
    inline float& mutable_default_setpoint() { return default_setpoint_.get(); }
    inline const float& get_default_setpoint() const { return default_setpoint_.get(); }
    inline float default_setpoint() const { return default_setpoint_.get(); }

    static constexpr char const* RCTRL_KP_NAME = "rctrl_kp";
    inline void clear_rctrl_kp() { rctrl_kp_.clear(); }
    inline void set_rctrl_kp(const float& value) { rctrl_kp_ = value; }
    inline void set_rctrl_kp(const float&& value) { rctrl_kp_ = value; }
    inline float& mutable_rctrl_kp() { return rctrl_kp_.get(); }
    inline const float& get_rctrl_kp() const { return rctrl_kp_.get(); }
    inline float rctrl_kp() const { return rctrl_kp_.get(); }

    static constexpr char const* RCTRL_KI_NAME = "rctrl_ki";
    inline void clear_rctrl_ki() { rctrl_ki_.clear(); }
    inline void set_rctrl_ki(const float& value) { rctrl_ki_ = value; }
    inline void set_rctrl_ki(const float&& value) { rctrl_ki_ = value; }
    inline float& mutable_rctrl_ki() { return rctrl_ki_.get(); }
    inline const float& get_rctrl_ki() const { return rctrl_ki_.get(); }
    inline float rctrl_ki() const { return rctrl_ki_.get(); }

    static constexpr char const* RCTRL_KD_NAME = "rctrl_kd";
    inline void clear_rctrl_kd() { rctrl_kd_.clear(); }
    inline void set_rctrl_kd(const float& value) { rctrl_kd_ = value; }
    inline void set_rctrl_kd(const float&& value) { rctrl_kd_ = value; }
    inline float& mutable_rctrl_kd() { return rctrl_kd_.get(); }
    inline const float& get_rctrl_kd() const { return rctrl_kd_.get(); }
    inline float rctrl_kd() const { return rctrl_kd_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((static_cast<SMAControlMode>(0) != default_mode_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = default_mode_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEFAULT_MODE), buffer, false);
      }

      if((0.0 != default_setpoint_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = default_setpoint_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEFAULT_SETPOINT), buffer, false);
      }

      if((0.0 != rctrl_kp_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = rctrl_kp_.serialize_with_id(static_cast<uint32_t>(FieldNumber::RCTRL_KP), buffer, false);
      }

      if((0.0 != rctrl_ki_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = rctrl_ki_.serialize_with_id(static_cast<uint32_t>(FieldNumber::RCTRL_KI), buffer, false);
      }

      if((0.0 != rctrl_kd_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = rctrl_kd_.serialize_with_id(static_cast<uint32_t>(FieldNumber::RCTRL_KD), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::DEFAULT_MODE:
            return_value = default_mode_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::DEFAULT_SETPOINT:
            return_value = default_setpoint_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::RCTRL_KP:
            return_value = rctrl_kp_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::RCTRL_KI:
            return_value = rctrl_ki_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::RCTRL_KD:
            return_value = rctrl_kd_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_default_mode();
      clear_default_setpoint();
      clear_rctrl_kp();
      clear_rctrl_ki();
      clear_rctrl_kd();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::DEFAULT_MODE:
          name = DEFAULT_MODE_NAME;
          break;
        case FieldNumber::DEFAULT_SETPOINT:
          name = DEFAULT_SETPOINT_NAME;
          break;
        case FieldNumber::RCTRL_KP:
          name = RCTRL_KP_NAME;
          break;
        case FieldNumber::RCTRL_KI:
          name = RCTRL_KI_NAME;
          break;
        case FieldNumber::RCTRL_KD:
          name = RCTRL_KD_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = default_mode_.to_string(left_chars, indent_level + 2, DEFAULT_MODE_NAME, true);
      left_chars = default_setpoint_.to_string(left_chars, indent_level + 2, DEFAULT_SETPOINT_NAME, false);
      left_chars = rctrl_kp_.to_string(left_chars, indent_level + 2, RCTRL_KP_NAME, false);
      left_chars = rctrl_ki_.to_string(left_chars, indent_level + 2, RCTRL_KI_NAME, false);
      left_chars = rctrl_kd_.to_string(left_chars, indent_level + 2, RCTRL_KD_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::enumeration<SMAControlMode> default_mode_ = static_cast<SMAControlMode>(0);
      EmbeddedProto::floatfixed default_setpoint_ = 0.0;
      EmbeddedProto::floatfixed rctrl_kp_ = 0.0;
      EmbeddedProto::floatfixed rctrl_ki_ = 0.0;
      EmbeddedProto::floatfixed rctrl_kd_ = 0.0;

};

class SMAStatusCompact final: public ::EmbeddedProto::MessageInterface
{
  public:
    SMAStatusCompact() = default;
    SMAStatusCompact(const SMAStatusCompact& rhs )
    {
      set_device_port(rhs.get_device_port());
      set_enabled(rhs.get_enabled());
      set_mode(rhs.get_mode());
      set_setpoint(rhs.get_setpoint());
      set_output_pwm(rhs.get_output_pwm());
      set_load_amps(rhs.get_load_amps());
      set_load_vdrop(rhs.get_load_vdrop());
      set_load_mohms(rhs.get_load_mohms());
    }

    SMAStatusCompact(const SMAStatusCompact&& rhs ) noexcept
    {
      set_device_port(rhs.get_device_port());
      set_enabled(rhs.get_enabled());
      set_mode(rhs.get_mode());
      set_setpoint(rhs.get_setpoint());
      set_output_pwm(rhs.get_output_pwm());
      set_load_amps(rhs.get_load_amps());
      set_load_vdrop(rhs.get_load_vdrop());
      set_load_mohms(rhs.get_load_mohms());
    }

    ~SMAStatusCompact() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      DEVICE_PORT = 1,
      ENABLED = 2,
      MODE = 3,
      SETPOINT = 4,
      OUTPUT_PWM = 5,
      LOAD_AMPS = 6,
      LOAD_VDROP = 7,
      LOAD_MOHMS = 8
    };

    SMAStatusCompact& operator=(const SMAStatusCompact& rhs)
    {
      set_device_port(rhs.get_device_port());
      set_enabled(rhs.get_enabled());
      set_mode(rhs.get_mode());
      set_setpoint(rhs.get_setpoint());
      set_output_pwm(rhs.get_output_pwm());
      set_load_amps(rhs.get_load_amps());
      set_load_vdrop(rhs.get_load_vdrop());
      set_load_mohms(rhs.get_load_mohms());
      return *this;
    }

    SMAStatusCompact& operator=(const SMAStatusCompact&& rhs) noexcept
    {
      set_device_port(rhs.get_device_port());
      set_enabled(rhs.get_enabled());
      set_mode(rhs.get_mode());
      set_setpoint(rhs.get_setpoint());
      set_output_pwm(rhs.get_output_pwm());
      set_load_amps(rhs.get_load_amps());
      set_load_vdrop(rhs.get_load_vdrop());
      set_load_mohms(rhs.get_load_mohms());
      return *this;
    }

    static constexpr char const* DEVICE_PORT_NAME = "device_port";
    inline void clear_device_port() { device_port_.clear(); }
    inline void set_device_port(const Device& value) { device_port_ = value; }
    inline void set_device_port(const Device&& value) { device_port_ = value; }
    inline const Device& get_device_port() const { return device_port_.get(); }
    inline Device device_port() const { return device_port_.get(); }

    static constexpr char const* ENABLED_NAME = "enabled";
    inline void clear_enabled() { enabled_.clear(); }
    inline void set_enabled(const bool& value) { enabled_ = value; }
    inline void set_enabled(const bool&& value) { enabled_ = value; }
    inline bool& mutable_enabled() { return enabled_.get(); }
    inline const bool& get_enabled() const { return enabled_.get(); }
    inline bool enabled() const { return enabled_.get(); }

    static constexpr char const* MODE_NAME = "mode";
    inline void clear_mode() { mode_.clear(); }
    inline void set_mode(const SMAControlMode& value) { mode_ = value; }
    inline void set_mode(const SMAControlMode&& value) { mode_ = value; }
    inline const SMAControlMode& get_mode() const { return mode_.get(); }
    inline SMAControlMode mode() const { return mode_.get(); }

    static constexpr char const* SETPOINT_NAME = "setpoint";
    inline void clear_setpoint() { setpoint_.clear(); }
    inline void set_setpoint(const float& value) { setpoint_ = value; }
    inline void set_setpoint(const float&& value) { setpoint_ = value; }
    inline float& mutable_setpoint() { return setpoint_.get(); }
    inline const float& get_setpoint() const { return setpoint_.get(); }
    inline float setpoint() const { return setpoint_.get(); }

    static constexpr char const* OUTPUT_PWM_NAME = "output_pwm";
    inline void clear_output_pwm() { output_pwm_.clear(); }
    inline void set_output_pwm(const float& value) { output_pwm_ = value; }
    inline void set_output_pwm(const float&& value) { output_pwm_ = value; }
    inline float& mutable_output_pwm() { return output_pwm_.get(); }
    inline const float& get_output_pwm() const { return output_pwm_.get(); }
    inline float output_pwm() const { return output_pwm_.get(); }

    static constexpr char const* LOAD_AMPS_NAME = "load_amps";
    inline void clear_load_amps() { load_amps_.clear(); }
    inline void set_load_amps(const float& value) { load_amps_ = value; }
    inline void set_load_amps(const float&& value) { load_amps_ = value; }
    inline float& mutable_load_amps() { return load_amps_.get(); }
    inline const float& get_load_amps() const { return load_amps_.get(); }
    inline float load_amps() const { return load_amps_.get(); }

    static constexpr char const* LOAD_VDROP_NAME = "load_vdrop";
    inline void clear_load_vdrop() { load_vdrop_.clear(); }
    inline void set_load_vdrop(const float& value) { load_vdrop_ = value; }
    inline void set_load_vdrop(const float&& value) { load_vdrop_ = value; }
    inline float& mutable_load_vdrop() { return load_vdrop_.get(); }
    inline const float& get_load_vdrop() const { return load_vdrop_.get(); }
    inline float load_vdrop() const { return load_vdrop_.get(); }

    static constexpr char const* LOAD_MOHMS_NAME = "load_mohms";
    inline void clear_load_mohms() { load_mohms_.clear(); }
    inline void set_load_mohms(const float& value) { load_mohms_ = value; }
    inline void set_load_mohms(const float&& value) { load_mohms_ = value; }
    inline float& mutable_load_mohms() { return load_mohms_.get(); }
    inline const float& get_load_mohms() const { return load_mohms_.get(); }
    inline float load_mohms() const { return load_mohms_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((static_cast<Device>(0) != device_port_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = device_port_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEVICE_PORT), buffer, false);
      }

      if((false != enabled_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = enabled_.serialize_with_id(static_cast<uint32_t>(FieldNumber::ENABLED), buffer, false);
      }

      if((static_cast<SMAControlMode>(0) != mode_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = mode_.serialize_with_id(static_cast<uint32_t>(FieldNumber::MODE), buffer, false);
      }

      if((0.0 != setpoint_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = setpoint_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SETPOINT), buffer, false);
      }

      if((0.0 != output_pwm_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = output_pwm_.serialize_with_id(static_cast<uint32_t>(FieldNumber::OUTPUT_PWM), buffer, false);
      }

      if((0.0 != load_amps_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = load_amps_.serialize_with_id(static_cast<uint32_t>(FieldNumber::LOAD_AMPS), buffer, false);
      }

      if((0.0 != load_vdrop_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = load_vdrop_.serialize_with_id(static_cast<uint32_t>(FieldNumber::LOAD_VDROP), buffer, false);
      }

      if((0.0 != load_mohms_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = load_mohms_.serialize_with_id(static_cast<uint32_t>(FieldNumber::LOAD_MOHMS), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::DEVICE_PORT:
            return_value = device_port_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::ENABLED:
            return_value = enabled_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::MODE:
            return_value = mode_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::SETPOINT:
            return_value = setpoint_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::OUTPUT_PWM:
            return_value = output_pwm_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::LOAD_AMPS:
            return_value = load_amps_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::LOAD_VDROP:
            return_value = load_vdrop_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::LOAD_MOHMS:
            return_value = load_mohms_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_device_port();
      clear_enabled();
      clear_mode();
      clear_setpoint();
      clear_output_pwm();
      clear_load_amps();
      clear_load_vdrop();
      clear_load_mohms();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::DEVICE_PORT:
          name = DEVICE_PORT_NAME;
          break;
        case FieldNumber::ENABLED:
          name = ENABLED_NAME;
          break;
        case FieldNumber::MODE:
          name = MODE_NAME;
          break;
        case FieldNumber::SETPOINT:
          name = SETPOINT_NAME;
          break;
        case FieldNumber::OUTPUT_PWM:
          name = OUTPUT_PWM_NAME;
          break;
        case FieldNumber::LOAD_AMPS:
          name = LOAD_AMPS_NAME;
          break;
        case FieldNumber::LOAD_VDROP:
          name = LOAD_VDROP_NAME;
          break;
        case FieldNumber::LOAD_MOHMS:
          name = LOAD_MOHMS_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = device_port_.to_string(left_chars, indent_level + 2, DEVICE_PORT_NAME, true);
      left_chars = enabled_.to_string(left_chars, indent_level + 2, ENABLED_NAME, false);
      left_chars = mode_.to_string(left_chars, indent_level + 2, MODE_NAME, false);
      left_chars = setpoint_.to_string(left_chars, indent_level + 2, SETPOINT_NAME, false);
      left_chars = output_pwm_.to_string(left_chars, indent_level + 2, OUTPUT_PWM_NAME, false);
      left_chars = load_amps_.to_string(left_chars, indent_level + 2, LOAD_AMPS_NAME, false);
      left_chars = load_vdrop_.to_string(left_chars, indent_level + 2, LOAD_VDROP_NAME, false);
      left_chars = load_mohms_.to_string(left_chars, indent_level + 2, LOAD_MOHMS_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::enumeration<Device> device_port_ = static_cast<Device>(0);
      EmbeddedProto::boolean enabled_ = false;
      EmbeddedProto::enumeration<SMAControlMode> mode_ = static_cast<SMAControlMode>(0);
      EmbeddedProto::floatfixed setpoint_ = 0.0;
      EmbeddedProto::floatfixed output_pwm_ = 0.0;
      EmbeddedProto::floatfixed load_amps_ = 0.0;
      EmbeddedProto::floatfixed load_vdrop_ = 0.0;
      EmbeddedProto::floatfixed load_mohms_ = 0.0;

};

class SetModeCommand final: public ::EmbeddedProto::MessageInterface
{
  public:
    SetModeCommand() = default;
    SetModeCommand(const SetModeCommand& rhs )
    {
      set_device(rhs.get_device());
      set_mode(rhs.get_mode());
    }

    SetModeCommand(const SetModeCommand&& rhs ) noexcept
    {
      set_device(rhs.get_device());
      set_mode(rhs.get_mode());
    }

    ~SetModeCommand() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      DEVICE = 1,
      MODE = 2
    };

    SetModeCommand& operator=(const SetModeCommand& rhs)
    {
      set_device(rhs.get_device());
      set_mode(rhs.get_mode());
      return *this;
    }

    SetModeCommand& operator=(const SetModeCommand&& rhs) noexcept
    {
      set_device(rhs.get_device());
      set_mode(rhs.get_mode());
      return *this;
    }

    static constexpr char const* DEVICE_NAME = "device";
    inline void clear_device() { device_.clear(); }
    inline void set_device(const Device& value) { device_ = value; }
    inline void set_device(const Device&& value) { device_ = value; }
    inline const Device& get_device() const { return device_.get(); }
    inline Device device() const { return device_.get(); }

    static constexpr char const* MODE_NAME = "mode";
    inline void clear_mode() { mode_.clear(); }
    inline void set_mode(const SMAControlMode& value) { mode_ = value; }
    inline void set_mode(const SMAControlMode&& value) { mode_ = value; }
    inline const SMAControlMode& get_mode() const { return mode_.get(); }
    inline SMAControlMode mode() const { return mode_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((static_cast<Device>(0) != device_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = device_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEVICE), buffer, false);
      }

      if((static_cast<SMAControlMode>(0) != mode_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = mode_.serialize_with_id(static_cast<uint32_t>(FieldNumber::MODE), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::DEVICE:
            return_value = device_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::MODE:
            return_value = mode_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_device();
      clear_mode();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::DEVICE:
          name = DEVICE_NAME;
          break;
        case FieldNumber::MODE:
          name = MODE_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = device_.to_string(left_chars, indent_level + 2, DEVICE_NAME, true);
      left_chars = mode_.to_string(left_chars, indent_level + 2, MODE_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::enumeration<Device> device_ = static_cast<Device>(0);
      EmbeddedProto::enumeration<SMAControlMode> mode_ = static_cast<SMAControlMode>(0);

};

class SetSetpointCommand final: public ::EmbeddedProto::MessageInterface
{
  public:
    SetSetpointCommand() = default;
    SetSetpointCommand(const SetSetpointCommand& rhs )
    {
      set_device(rhs.get_device());
      set_mode(rhs.get_mode());
      set_setpoint(rhs.get_setpoint());
    }

    SetSetpointCommand(const SetSetpointCommand&& rhs ) noexcept
    {
      set_device(rhs.get_device());
      set_mode(rhs.get_mode());
      set_setpoint(rhs.get_setpoint());
    }

    ~SetSetpointCommand() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      DEVICE = 1,
      MODE = 2,
      SETPOINT = 3
    };

    SetSetpointCommand& operator=(const SetSetpointCommand& rhs)
    {
      set_device(rhs.get_device());
      set_mode(rhs.get_mode());
      set_setpoint(rhs.get_setpoint());
      return *this;
    }

    SetSetpointCommand& operator=(const SetSetpointCommand&& rhs) noexcept
    {
      set_device(rhs.get_device());
      set_mode(rhs.get_mode());
      set_setpoint(rhs.get_setpoint());
      return *this;
    }

    static constexpr char const* DEVICE_NAME = "device";
    inline void clear_device() { device_.clear(); }
    inline void set_device(const Device& value) { device_ = value; }
    inline void set_device(const Device&& value) { device_ = value; }
    inline const Device& get_device() const { return device_.get(); }
    inline Device device() const { return device_.get(); }

    static constexpr char const* MODE_NAME = "mode";
    inline void clear_mode() { mode_.clear(); }
    inline void set_mode(const SMAControlMode& value) { mode_ = value; }
    inline void set_mode(const SMAControlMode&& value) { mode_ = value; }
    inline const SMAControlMode& get_mode() const { return mode_.get(); }
    inline SMAControlMode mode() const { return mode_.get(); }

    static constexpr char const* SETPOINT_NAME = "setpoint";
    inline void clear_setpoint() { setpoint_.clear(); }
    inline void set_setpoint(const float& value) { setpoint_ = value; }
    inline void set_setpoint(const float&& value) { setpoint_ = value; }
    inline float& mutable_setpoint() { return setpoint_.get(); }
    inline const float& get_setpoint() const { return setpoint_.get(); }
    inline float setpoint() const { return setpoint_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((static_cast<Device>(0) != device_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = device_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEVICE), buffer, false);
      }

      if((static_cast<SMAControlMode>(0) != mode_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = mode_.serialize_with_id(static_cast<uint32_t>(FieldNumber::MODE), buffer, false);
      }

      if((0.0 != setpoint_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = setpoint_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SETPOINT), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::DEVICE:
            return_value = device_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::MODE:
            return_value = mode_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::SETPOINT:
            return_value = setpoint_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_device();
      clear_mode();
      clear_setpoint();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::DEVICE:
          name = DEVICE_NAME;
          break;
        case FieldNumber::MODE:
          name = MODE_NAME;
          break;
        case FieldNumber::SETPOINT:
          name = SETPOINT_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = device_.to_string(left_chars, indent_level + 2, DEVICE_NAME, true);
      left_chars = mode_.to_string(left_chars, indent_level + 2, MODE_NAME, false);
      left_chars = setpoint_.to_string(left_chars, indent_level + 2, SETPOINT_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::enumeration<Device> device_ = static_cast<Device>(0);
      EmbeddedProto::enumeration<SMAControlMode> mode_ = static_cast<SMAControlMode>(0);
      EmbeddedProto::floatfixed setpoint_ = 0.0;

};

class SilenceNodeCommand final: public ::EmbeddedProto::MessageInterface
{
  public:
    SilenceNodeCommand() = default;
    SilenceNodeCommand(const SilenceNodeCommand& rhs )
    {
      set_silence(rhs.get_silence());
    }

    SilenceNodeCommand(const SilenceNodeCommand&& rhs ) noexcept
    {
      set_silence(rhs.get_silence());
    }

    ~SilenceNodeCommand() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      SILENCE = 1
    };

    SilenceNodeCommand& operator=(const SilenceNodeCommand& rhs)
    {
      set_silence(rhs.get_silence());
      return *this;
    }

    SilenceNodeCommand& operator=(const SilenceNodeCommand&& rhs) noexcept
    {
      set_silence(rhs.get_silence());
      return *this;
    }

    static constexpr char const* SILENCE_NAME = "silence";
    inline void clear_silence() { silence_.clear(); }
    inline void set_silence(const bool& value) { silence_ = value; }
    inline void set_silence(const bool&& value) { silence_ = value; }
    inline bool& mutable_silence() { return silence_.get(); }
    inline const bool& get_silence() const { return silence_.get(); }
    inline bool silence() const { return silence_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((false != silence_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = silence_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SILENCE), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::SILENCE:
            return_value = silence_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_silence();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::SILENCE:
          name = SILENCE_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = silence_.to_string(left_chars, indent_level + 2, SILENCE_NAME, true);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::boolean silence_ = false;

};

class StatusCommand final: public ::EmbeddedProto::MessageInterface
{
  public:
    StatusCommand() = default;
    StatusCommand(const StatusCommand& rhs )
    {
      set_device(rhs.get_device());
      set_mode(rhs.get_mode());
    }

    StatusCommand(const StatusCommand&& rhs ) noexcept
    {
      set_device(rhs.get_device());
      set_mode(rhs.get_mode());
    }

    ~StatusCommand() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      DEVICE = 1,
      MODE = 2
    };

    StatusCommand& operator=(const StatusCommand& rhs)
    {
      set_device(rhs.get_device());
      set_mode(rhs.get_mode());
      return *this;
    }

    StatusCommand& operator=(const StatusCommand&& rhs) noexcept
    {
      set_device(rhs.get_device());
      set_mode(rhs.get_mode());
      return *this;
    }

    static constexpr char const* DEVICE_NAME = "device";
    inline void clear_device() { device_.clear(); }
    inline void set_device(const Device& value) { device_ = value; }
    inline void set_device(const Device&& value) { device_ = value; }
    inline const Device& get_device() const { return device_.get(); }
    inline Device device() const { return device_.get(); }

    static constexpr char const* MODE_NAME = "mode";
    inline void clear_mode() { mode_.clear(); }
    inline void set_mode(const DeviceStatusMode& value) { mode_ = value; }
    inline void set_mode(const DeviceStatusMode&& value) { mode_ = value; }
    inline const DeviceStatusMode& get_mode() const { return mode_.get(); }
    inline DeviceStatusMode mode() const { return mode_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((static_cast<Device>(0) != device_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = device_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEVICE), buffer, false);
      }

      if((static_cast<DeviceStatusMode>(0) != mode_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = mode_.serialize_with_id(static_cast<uint32_t>(FieldNumber::MODE), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::DEVICE:
            return_value = device_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::MODE:
            return_value = mode_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_device();
      clear_mode();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::DEVICE:
          name = DEVICE_NAME;
          break;
        case FieldNumber::MODE:
          name = MODE_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = device_.to_string(left_chars, indent_level + 2, DEVICE_NAME, true);
      left_chars = mode_.to_string(left_chars, indent_level + 2, MODE_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::enumeration<Device> device_ = static_cast<Device>(0);
      EmbeddedProto::enumeration<DeviceStatusMode> mode_ = static_cast<DeviceStatusMode>(0);

};

class ConfigureSettingsCommand final: public ::EmbeddedProto::MessageInterface
{
  public:
    ConfigureSettingsCommand() = default;
    ConfigureSettingsCommand(const ConfigureSettingsCommand& rhs )
    {
      set_device(rhs.get_device());
      if(rhs.get_which_device_config() != which_device_config_)
      {
        // First delete the old object in the oneof.
        clear_device_config();
      }

      switch(rhs.get_which_device_config())
      {
        case FieldNumber::NODE_CONFIG:
          set_node_config(rhs.get_node_config());
          break;

        case FieldNumber::SMA_CONTROLLER_CONFIG:
          set_sma_controller_config(rhs.get_sma_controller_config());
          break;

        default:
          break;
      }

    }

    ConfigureSettingsCommand(const ConfigureSettingsCommand&& rhs ) noexcept
    {
      set_device(rhs.get_device());
      if(rhs.get_which_device_config() != which_device_config_)
      {
        // First delete the old object in the oneof.
        clear_device_config();
      }

      switch(rhs.get_which_device_config())
      {
        case FieldNumber::NODE_CONFIG:
          set_node_config(rhs.get_node_config());
          break;

        case FieldNumber::SMA_CONTROLLER_CONFIG:
          set_sma_controller_config(rhs.get_sma_controller_config());
          break;

        default:
          break;
      }

    }

    ~ConfigureSettingsCommand() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      DEVICE = 1,
      NODE_CONFIG = 2,
      SMA_CONTROLLER_CONFIG = 3
    };

    ConfigureSettingsCommand& operator=(const ConfigureSettingsCommand& rhs)
    {
      set_device(rhs.get_device());
      if(rhs.get_which_device_config() != which_device_config_)
      {
        // First delete the old object in the oneof.
        clear_device_config();
      }

      switch(rhs.get_which_device_config())
      {
        case FieldNumber::NODE_CONFIG:
          set_node_config(rhs.get_node_config());
          break;

        case FieldNumber::SMA_CONTROLLER_CONFIG:
          set_sma_controller_config(rhs.get_sma_controller_config());
          break;

        default:
          break;
      }

      return *this;
    }

    ConfigureSettingsCommand& operator=(const ConfigureSettingsCommand&& rhs) noexcept
    {
      set_device(rhs.get_device());
      if(rhs.get_which_device_config() != which_device_config_)
      {
        // First delete the old object in the oneof.
        clear_device_config();
      }

      switch(rhs.get_which_device_config())
      {
        case FieldNumber::NODE_CONFIG:
          set_node_config(rhs.get_node_config());
          break;

        case FieldNumber::SMA_CONTROLLER_CONFIG:
          set_sma_controller_config(rhs.get_sma_controller_config());
          break;

        default:
          break;
      }

      return *this;
    }

    static constexpr char const* DEVICE_NAME = "device";
    inline void clear_device() { device_.clear(); }
    inline void set_device(const Device& value) { device_ = value; }
    inline void set_device(const Device&& value) { device_ = value; }
    inline const Device& get_device() const { return device_.get(); }
    inline Device device() const { return device_.get(); }

    FieldNumber get_which_device_config() const { return which_device_config_; }

    static constexpr char const* NODE_CONFIG_NAME = "node_config";
    inline bool has_node_config() const
    {
      return FieldNumber::NODE_CONFIG == which_device_config_;
    }
    inline void clear_node_config()
    {
      if(FieldNumber::NODE_CONFIG == which_device_config_)
      {
        which_device_config_ = FieldNumber::NOT_SET;
        device_config_.node_config_.~NodeSettings();
      }
    }
    inline void set_node_config(const NodeSettings& value)
    {
      if(FieldNumber::NODE_CONFIG != which_device_config_)
      {
        init_device_config(FieldNumber::NODE_CONFIG);
      }
      device_config_.node_config_ = value;
    }
    inline void set_node_config(const NodeSettings&& value)
    {
      if(FieldNumber::NODE_CONFIG != which_device_config_)
      {
        init_device_config(FieldNumber::NODE_CONFIG);
      }
      device_config_.node_config_ = value;
    }
    inline NodeSettings& mutable_node_config()
    {
      if(FieldNumber::NODE_CONFIG != which_device_config_)
      {
        init_device_config(FieldNumber::NODE_CONFIG);
      }
      return device_config_.node_config_;
    }
    inline const NodeSettings& get_node_config() const { return device_config_.node_config_; }
    inline const NodeSettings& node_config() const { return device_config_.node_config_; }

    static constexpr char const* SMA_CONTROLLER_CONFIG_NAME = "sma_controller_config";
    inline bool has_sma_controller_config() const
    {
      return FieldNumber::SMA_CONTROLLER_CONFIG == which_device_config_;
    }
    inline void clear_sma_controller_config()
    {
      if(FieldNumber::SMA_CONTROLLER_CONFIG == which_device_config_)
      {
        which_device_config_ = FieldNumber::NOT_SET;
        device_config_.sma_controller_config_.~SMAControllerSettings();
      }
    }
    inline void set_sma_controller_config(const SMAControllerSettings& value)
    {
      if(FieldNumber::SMA_CONTROLLER_CONFIG != which_device_config_)
      {
        init_device_config(FieldNumber::SMA_CONTROLLER_CONFIG);
      }
      device_config_.sma_controller_config_ = value;
    }
    inline void set_sma_controller_config(const SMAControllerSettings&& value)
    {
      if(FieldNumber::SMA_CONTROLLER_CONFIG != which_device_config_)
      {
        init_device_config(FieldNumber::SMA_CONTROLLER_CONFIG);
      }
      device_config_.sma_controller_config_ = value;
    }
    inline SMAControllerSettings& mutable_sma_controller_config()
    {
      if(FieldNumber::SMA_CONTROLLER_CONFIG != which_device_config_)
      {
        init_device_config(FieldNumber::SMA_CONTROLLER_CONFIG);
      }
      return device_config_.sma_controller_config_;
    }
    inline const SMAControllerSettings& get_sma_controller_config() const { return device_config_.sma_controller_config_; }
    inline const SMAControllerSettings& sma_controller_config() const { return device_config_.sma_controller_config_; }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((static_cast<Device>(0) != device_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = device_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEVICE), buffer, false);
      }

      switch(which_device_config_)
      {
        case FieldNumber::NODE_CONFIG:
          if(has_node_config() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = device_config_.node_config_.serialize_with_id(static_cast<uint32_t>(FieldNumber::NODE_CONFIG), buffer, true);
          }
          break;

        case FieldNumber::SMA_CONTROLLER_CONFIG:
          if(has_sma_controller_config() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = device_config_.sma_controller_config_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SMA_CONTROLLER_CONFIG), buffer, true);
          }
          break;

        default:
          break;
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::DEVICE:
            return_value = device_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NODE_CONFIG:
          case FieldNumber::SMA_CONTROLLER_CONFIG:
            return_value = deserialize_device_config(id_tag, buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_device();
      clear_device_config();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::DEVICE:
          name = DEVICE_NAME;
          break;
        case FieldNumber::NODE_CONFIG:
          name = NODE_CONFIG_NAME;
          break;
        case FieldNumber::SMA_CONTROLLER_CONFIG:
          name = SMA_CONTROLLER_CONFIG_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = device_.to_string(left_chars, indent_level + 2, DEVICE_NAME, true);
      left_chars = to_string_device_config(left_chars, indent_level + 2, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::enumeration<Device> device_ = static_cast<Device>(0);

      FieldNumber which_device_config_ = FieldNumber::NOT_SET;
      union device_config
      {
        device_config() {}
        ~device_config() {}
        NodeSettings node_config_;
        SMAControllerSettings sma_controller_config_;
      };
      device_config device_config_;

      void init_device_config(const FieldNumber field_id)
      {
        if(FieldNumber::NOT_SET != which_device_config_)
        {
          // First delete the old object in the oneof.
          clear_device_config();
        }

        // C++11 unions only support nontrivial members when you explicitly call the placement new statement.
        switch(field_id)
        {
          case FieldNumber::NODE_CONFIG:
            new(&device_config_.node_config_) NodeSettings;
            break;

          case FieldNumber::SMA_CONTROLLER_CONFIG:
            new(&device_config_.sma_controller_config_) SMAControllerSettings;
            break;

          default:
            break;
         }

         which_device_config_ = field_id;
      }

      void clear_device_config()
      {
        switch(which_device_config_)
        {
          case FieldNumber::NODE_CONFIG:
            ::EmbeddedProto::destroy_at(&device_config_.node_config_);
            break;
          case FieldNumber::SMA_CONTROLLER_CONFIG:
            ::EmbeddedProto::destroy_at(&device_config_.sma_controller_config_);
            break;
          default:
            break;
        }
        which_device_config_ = FieldNumber::NOT_SET;
      }

      ::EmbeddedProto::Error deserialize_device_config(const FieldNumber field_id, 
                                    ::EmbeddedProto::ReadBufferInterface& buffer,
                                    const ::EmbeddedProto::WireFormatter::WireType wire_type)
      {
        ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
        
        if(field_id != which_device_config_)
        {
          init_device_config(field_id);
        }

        switch(which_device_config_)
        {
          case FieldNumber::NODE_CONFIG:
            return_value = device_config_.node_config_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::SMA_CONTROLLER_CONFIG:
            return_value = device_config_.sma_controller_config_.deserialize_check_type(buffer, wire_type);
            break;
          default:
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS != return_value)
        {
          clear_device_config();
        }
        return return_value;
      }

#ifdef MSG_TO_STRING 
      ::EmbeddedProto::string_view to_string_device_config(::EmbeddedProto::string_view& str, const uint32_t indent_level, const bool first_field) const
      {
        ::EmbeddedProto::string_view left_chars = str;

        switch(which_device_config_)
        {
          case FieldNumber::NODE_CONFIG:
            left_chars = device_config_.node_config_.to_string(left_chars, indent_level, NODE_CONFIG_NAME, first_field);
            break;
          case FieldNumber::SMA_CONTROLLER_CONFIG:
            left_chars = device_config_.sma_controller_config_.to_string(left_chars, indent_level, SMA_CONTROLLER_CONFIG_NAME, first_field);
            break;
          default:
            break;
        }

        return left_chars;
      }

#endif // End of MSG_TO_STRING
};

template<
    uint32_t NodeStatusDump_firmware_version_LENGTH, 
    uint32_t NodeStatusDump_board_version_LENGTH
>
class NodeStatusDump final: public ::EmbeddedProto::MessageInterface
{
  public:
    NodeStatusDump() = default;
    NodeStatusDump(const NodeStatusDump& rhs )
    {
      set_compact_status(rhs.get_compact_status());
      set_loaded_settings(rhs.get_loaded_settings());
      set_firmware_version(rhs.get_firmware_version());
      set_board_version(rhs.get_board_version());
      set_muscle_cnt(rhs.get_muscle_cnt());
      set_log_interval_ms(rhs.get_log_interval_ms());
      set_vrd_scalar(rhs.get_vrd_scalar());
      set_vrd_offset(rhs.get_vrd_offset());
      set_max_current(rhs.get_max_current());
      set_min_v_supply(rhs.get_min_v_supply());
    }

    NodeStatusDump(const NodeStatusDump&& rhs ) noexcept
    {
      set_compact_status(rhs.get_compact_status());
      set_loaded_settings(rhs.get_loaded_settings());
      set_firmware_version(rhs.get_firmware_version());
      set_board_version(rhs.get_board_version());
      set_muscle_cnt(rhs.get_muscle_cnt());
      set_log_interval_ms(rhs.get_log_interval_ms());
      set_vrd_scalar(rhs.get_vrd_scalar());
      set_vrd_offset(rhs.get_vrd_offset());
      set_max_current(rhs.get_max_current());
      set_min_v_supply(rhs.get_min_v_supply());
    }

    ~NodeStatusDump() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      COMPACT_STATUS = 1,
      LOADED_SETTINGS = 2,
      FIRMWARE_VERSION = 3,
      BOARD_VERSION = 4,
      MUSCLE_CNT = 5,
      LOG_INTERVAL_MS = 6,
      VRD_SCALAR = 7,
      VRD_OFFSET = 8,
      MAX_CURRENT = 9,
      MIN_V_SUPPLY = 10
    };

    NodeStatusDump& operator=(const NodeStatusDump& rhs)
    {
      set_compact_status(rhs.get_compact_status());
      set_loaded_settings(rhs.get_loaded_settings());
      set_firmware_version(rhs.get_firmware_version());
      set_board_version(rhs.get_board_version());
      set_muscle_cnt(rhs.get_muscle_cnt());
      set_log_interval_ms(rhs.get_log_interval_ms());
      set_vrd_scalar(rhs.get_vrd_scalar());
      set_vrd_offset(rhs.get_vrd_offset());
      set_max_current(rhs.get_max_current());
      set_min_v_supply(rhs.get_min_v_supply());
      return *this;
    }

    NodeStatusDump& operator=(const NodeStatusDump&& rhs) noexcept
    {
      set_compact_status(rhs.get_compact_status());
      set_loaded_settings(rhs.get_loaded_settings());
      set_firmware_version(rhs.get_firmware_version());
      set_board_version(rhs.get_board_version());
      set_muscle_cnt(rhs.get_muscle_cnt());
      set_log_interval_ms(rhs.get_log_interval_ms());
      set_vrd_scalar(rhs.get_vrd_scalar());
      set_vrd_offset(rhs.get_vrd_offset());
      set_max_current(rhs.get_max_current());
      set_min_v_supply(rhs.get_min_v_supply());
      return *this;
    }

    static constexpr char const* COMPACT_STATUS_NAME = "compact_status";
    inline void clear_compact_status() { compact_status_.clear(); }
    inline void set_compact_status(const NodeStatusCompact& value) { compact_status_ = value; }
    inline void set_compact_status(const NodeStatusCompact&& value) { compact_status_ = value; }
    inline NodeStatusCompact& mutable_compact_status() { return compact_status_; }
    inline const NodeStatusCompact& get_compact_status() const { return compact_status_; }
    inline const NodeStatusCompact& compact_status() const { return compact_status_; }

    static constexpr char const* LOADED_SETTINGS_NAME = "loaded_settings";
    inline void clear_loaded_settings() { loaded_settings_.clear(); }
    inline void set_loaded_settings(const NodeSettings& value) { loaded_settings_ = value; }
    inline void set_loaded_settings(const NodeSettings&& value) { loaded_settings_ = value; }
    inline NodeSettings& mutable_loaded_settings() { return loaded_settings_; }
    inline const NodeSettings& get_loaded_settings() const { return loaded_settings_; }
    inline const NodeSettings& loaded_settings() const { return loaded_settings_; }

    static constexpr char const* FIRMWARE_VERSION_NAME = "firmware_version";
    inline void clear_firmware_version() { firmware_version_.clear(); }
    inline ::EmbeddedProto::FieldString<NodeStatusDump_firmware_version_LENGTH>& mutable_firmware_version() { return firmware_version_; }
    inline void set_firmware_version(const ::EmbeddedProto::FieldString<NodeStatusDump_firmware_version_LENGTH>& rhs) { firmware_version_.set(rhs); }
    inline const ::EmbeddedProto::FieldString<NodeStatusDump_firmware_version_LENGTH>& get_firmware_version() const { return firmware_version_; }
    inline const char* firmware_version() const { return firmware_version_.get_const(); }

    static constexpr char const* BOARD_VERSION_NAME = "board_version";
    inline void clear_board_version() { board_version_.clear(); }
    inline ::EmbeddedProto::FieldString<NodeStatusDump_board_version_LENGTH>& mutable_board_version() { return board_version_; }
    inline void set_board_version(const ::EmbeddedProto::FieldString<NodeStatusDump_board_version_LENGTH>& rhs) { board_version_.set(rhs); }
    inline const ::EmbeddedProto::FieldString<NodeStatusDump_board_version_LENGTH>& get_board_version() const { return board_version_; }
    inline const char* board_version() const { return board_version_.get_const(); }

    static constexpr char const* MUSCLE_CNT_NAME = "muscle_cnt";
    inline void clear_muscle_cnt() { muscle_cnt_.clear(); }
    inline void set_muscle_cnt(const uint32_t& value) { muscle_cnt_ = value; }
    inline void set_muscle_cnt(const uint32_t&& value) { muscle_cnt_ = value; }
    inline uint32_t& mutable_muscle_cnt() { return muscle_cnt_.get(); }
    inline const uint32_t& get_muscle_cnt() const { return muscle_cnt_.get(); }
    inline uint32_t muscle_cnt() const { return muscle_cnt_.get(); }

    static constexpr char const* LOG_INTERVAL_MS_NAME = "log_interval_ms";
    inline void clear_log_interval_ms() { log_interval_ms_.clear(); }
    inline void set_log_interval_ms(const uint32_t& value) { log_interval_ms_ = value; }
    inline void set_log_interval_ms(const uint32_t&& value) { log_interval_ms_ = value; }
    inline uint32_t& mutable_log_interval_ms() { return log_interval_ms_.get(); }
    inline const uint32_t& get_log_interval_ms() const { return log_interval_ms_.get(); }
    inline uint32_t log_interval_ms() const { return log_interval_ms_.get(); }

    static constexpr char const* VRD_SCALAR_NAME = "vrd_scalar";
    inline void clear_vrd_scalar() { vrd_scalar_.clear(); }
    inline void set_vrd_scalar(const float& value) { vrd_scalar_ = value; }
    inline void set_vrd_scalar(const float&& value) { vrd_scalar_ = value; }
    inline float& mutable_vrd_scalar() { return vrd_scalar_.get(); }
    inline const float& get_vrd_scalar() const { return vrd_scalar_.get(); }
    inline float vrd_scalar() const { return vrd_scalar_.get(); }

    static constexpr char const* VRD_OFFSET_NAME = "vrd_offset";
    inline void clear_vrd_offset() { vrd_offset_.clear(); }
    inline void set_vrd_offset(const float& value) { vrd_offset_ = value; }
    inline void set_vrd_offset(const float&& value) { vrd_offset_ = value; }
    inline float& mutable_vrd_offset() { return vrd_offset_.get(); }
    inline const float& get_vrd_offset() const { return vrd_offset_.get(); }
    inline float vrd_offset() const { return vrd_offset_.get(); }

    static constexpr char const* MAX_CURRENT_NAME = "max_current";
    inline void clear_max_current() { max_current_.clear(); }
    inline void set_max_current(const float& value) { max_current_ = value; }
    inline void set_max_current(const float&& value) { max_current_ = value; }
    inline float& mutable_max_current() { return max_current_.get(); }
    inline const float& get_max_current() const { return max_current_.get(); }
    inline float max_current() const { return max_current_.get(); }

    static constexpr char const* MIN_V_SUPPLY_NAME = "min_v_supply";
    inline void clear_min_v_supply() { min_v_supply_.clear(); }
    inline void set_min_v_supply(const float& value) { min_v_supply_ = value; }
    inline void set_min_v_supply(const float&& value) { min_v_supply_ = value; }
    inline float& mutable_min_v_supply() { return min_v_supply_.get(); }
    inline const float& get_min_v_supply() const { return min_v_supply_.get(); }
    inline float min_v_supply() const { return min_v_supply_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = compact_status_.serialize_with_id(static_cast<uint32_t>(FieldNumber::COMPACT_STATUS), buffer, false);
      }

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = loaded_settings_.serialize_with_id(static_cast<uint32_t>(FieldNumber::LOADED_SETTINGS), buffer, false);
      }

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = firmware_version_.serialize_with_id(static_cast<uint32_t>(FieldNumber::FIRMWARE_VERSION), buffer, false);
      }

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = board_version_.serialize_with_id(static_cast<uint32_t>(FieldNumber::BOARD_VERSION), buffer, false);
      }

      if((0U != muscle_cnt_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = muscle_cnt_.serialize_with_id(static_cast<uint32_t>(FieldNumber::MUSCLE_CNT), buffer, false);
      }

      if((0U != log_interval_ms_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = log_interval_ms_.serialize_with_id(static_cast<uint32_t>(FieldNumber::LOG_INTERVAL_MS), buffer, false);
      }

      if((0.0 != vrd_scalar_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = vrd_scalar_.serialize_with_id(static_cast<uint32_t>(FieldNumber::VRD_SCALAR), buffer, false);
      }

      if((0.0 != vrd_offset_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = vrd_offset_.serialize_with_id(static_cast<uint32_t>(FieldNumber::VRD_OFFSET), buffer, false);
      }

      if((0.0 != max_current_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = max_current_.serialize_with_id(static_cast<uint32_t>(FieldNumber::MAX_CURRENT), buffer, false);
      }

      if((0.0 != min_v_supply_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = min_v_supply_.serialize_with_id(static_cast<uint32_t>(FieldNumber::MIN_V_SUPPLY), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::COMPACT_STATUS:
            return_value = compact_status_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::LOADED_SETTINGS:
            return_value = loaded_settings_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::FIRMWARE_VERSION:
            return_value = firmware_version_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::BOARD_VERSION:
            return_value = board_version_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::MUSCLE_CNT:
            return_value = muscle_cnt_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::LOG_INTERVAL_MS:
            return_value = log_interval_ms_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::VRD_SCALAR:
            return_value = vrd_scalar_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::VRD_OFFSET:
            return_value = vrd_offset_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::MAX_CURRENT:
            return_value = max_current_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::MIN_V_SUPPLY:
            return_value = min_v_supply_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_compact_status();
      clear_loaded_settings();
      clear_firmware_version();
      clear_board_version();
      clear_muscle_cnt();
      clear_log_interval_ms();
      clear_vrd_scalar();
      clear_vrd_offset();
      clear_max_current();
      clear_min_v_supply();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::COMPACT_STATUS:
          name = COMPACT_STATUS_NAME;
          break;
        case FieldNumber::LOADED_SETTINGS:
          name = LOADED_SETTINGS_NAME;
          break;
        case FieldNumber::FIRMWARE_VERSION:
          name = FIRMWARE_VERSION_NAME;
          break;
        case FieldNumber::BOARD_VERSION:
          name = BOARD_VERSION_NAME;
          break;
        case FieldNumber::MUSCLE_CNT:
          name = MUSCLE_CNT_NAME;
          break;
        case FieldNumber::LOG_INTERVAL_MS:
          name = LOG_INTERVAL_MS_NAME;
          break;
        case FieldNumber::VRD_SCALAR:
          name = VRD_SCALAR_NAME;
          break;
        case FieldNumber::VRD_OFFSET:
          name = VRD_OFFSET_NAME;
          break;
        case FieldNumber::MAX_CURRENT:
          name = MAX_CURRENT_NAME;
          break;
        case FieldNumber::MIN_V_SUPPLY:
          name = MIN_V_SUPPLY_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = compact_status_.to_string(left_chars, indent_level + 2, COMPACT_STATUS_NAME, true);
      left_chars = loaded_settings_.to_string(left_chars, indent_level + 2, LOADED_SETTINGS_NAME, false);
      left_chars = firmware_version_.to_string(left_chars, indent_level + 2, FIRMWARE_VERSION_NAME, false);
      left_chars = board_version_.to_string(left_chars, indent_level + 2, BOARD_VERSION_NAME, false);
      left_chars = muscle_cnt_.to_string(left_chars, indent_level + 2, MUSCLE_CNT_NAME, false);
      left_chars = log_interval_ms_.to_string(left_chars, indent_level + 2, LOG_INTERVAL_MS_NAME, false);
      left_chars = vrd_scalar_.to_string(left_chars, indent_level + 2, VRD_SCALAR_NAME, false);
      left_chars = vrd_offset_.to_string(left_chars, indent_level + 2, VRD_OFFSET_NAME, false);
      left_chars = max_current_.to_string(left_chars, indent_level + 2, MAX_CURRENT_NAME, false);
      left_chars = min_v_supply_.to_string(left_chars, indent_level + 2, MIN_V_SUPPLY_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      NodeStatusCompact compact_status_;
      NodeSettings loaded_settings_;
      ::EmbeddedProto::FieldString<NodeStatusDump_firmware_version_LENGTH> firmware_version_;
      ::EmbeddedProto::FieldString<NodeStatusDump_board_version_LENGTH> board_version_;
      EmbeddedProto::uint32 muscle_cnt_ = 0U;
      EmbeddedProto::uint32 log_interval_ms_ = 0U;
      EmbeddedProto::floatfixed vrd_scalar_ = 0.0;
      EmbeddedProto::floatfixed vrd_offset_ = 0.0;
      EmbeddedProto::floatfixed max_current_ = 0.0;
      EmbeddedProto::floatfixed min_v_supply_ = 0.0;

};

class SMAStatusDump final: public ::EmbeddedProto::MessageInterface
{
  public:
    SMAStatusDump() = default;
    SMAStatusDump(const SMAStatusDump& rhs )
    {
      set_compact_status(rhs.get_compact_status());
      set_loaded_settings(rhs.get_loaded_settings());
      set_vld_scalar(rhs.get_vld_scalar());
      set_vld_offset(rhs.get_vld_offset());
      set_r_sns_ohms(rhs.get_r_sns_ohms());
      set_amp_gain(rhs.get_amp_gain());
      set_af_mohms(rhs.get_af_mohms());
      set_delta_mohms(rhs.get_delta_mohms());
      set_trainState(rhs.get_trainState());
      set_rctrl_kp(rhs.get_rctrl_kp());
      set_rctrl_ki(rhs.get_rctrl_ki());
      set_rctrl_kd(rhs.get_rctrl_kd());
    }

    SMAStatusDump(const SMAStatusDump&& rhs ) noexcept
    {
      set_compact_status(rhs.get_compact_status());
      set_loaded_settings(rhs.get_loaded_settings());
      set_vld_scalar(rhs.get_vld_scalar());
      set_vld_offset(rhs.get_vld_offset());
      set_r_sns_ohms(rhs.get_r_sns_ohms());
      set_amp_gain(rhs.get_amp_gain());
      set_af_mohms(rhs.get_af_mohms());
      set_delta_mohms(rhs.get_delta_mohms());
      set_trainState(rhs.get_trainState());
      set_rctrl_kp(rhs.get_rctrl_kp());
      set_rctrl_ki(rhs.get_rctrl_ki());
      set_rctrl_kd(rhs.get_rctrl_kd());
    }

    ~SMAStatusDump() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      COMPACT_STATUS = 1,
      LOADED_SETTINGS = 2,
      VLD_SCALAR = 3,
      VLD_OFFSET = 4,
      R_SNS_OHMS = 5,
      AMP_GAIN = 6,
      AF_MOHMS = 7,
      DELTA_MOHMS = 8,
      TRAINSTATE = 9,
      RCTRL_KP = 10,
      RCTRL_KI = 11,
      RCTRL_KD = 12
    };

    SMAStatusDump& operator=(const SMAStatusDump& rhs)
    {
      set_compact_status(rhs.get_compact_status());
      set_loaded_settings(rhs.get_loaded_settings());
      set_vld_scalar(rhs.get_vld_scalar());
      set_vld_offset(rhs.get_vld_offset());
      set_r_sns_ohms(rhs.get_r_sns_ohms());
      set_amp_gain(rhs.get_amp_gain());
      set_af_mohms(rhs.get_af_mohms());
      set_delta_mohms(rhs.get_delta_mohms());
      set_trainState(rhs.get_trainState());
      set_rctrl_kp(rhs.get_rctrl_kp());
      set_rctrl_ki(rhs.get_rctrl_ki());
      set_rctrl_kd(rhs.get_rctrl_kd());
      return *this;
    }

    SMAStatusDump& operator=(const SMAStatusDump&& rhs) noexcept
    {
      set_compact_status(rhs.get_compact_status());
      set_loaded_settings(rhs.get_loaded_settings());
      set_vld_scalar(rhs.get_vld_scalar());
      set_vld_offset(rhs.get_vld_offset());
      set_r_sns_ohms(rhs.get_r_sns_ohms());
      set_amp_gain(rhs.get_amp_gain());
      set_af_mohms(rhs.get_af_mohms());
      set_delta_mohms(rhs.get_delta_mohms());
      set_trainState(rhs.get_trainState());
      set_rctrl_kp(rhs.get_rctrl_kp());
      set_rctrl_ki(rhs.get_rctrl_ki());
      set_rctrl_kd(rhs.get_rctrl_kd());
      return *this;
    }

    static constexpr char const* COMPACT_STATUS_NAME = "compact_status";
    inline void clear_compact_status() { compact_status_.clear(); }
    inline void set_compact_status(const SMAStatusCompact& value) { compact_status_ = value; }
    inline void set_compact_status(const SMAStatusCompact&& value) { compact_status_ = value; }
    inline SMAStatusCompact& mutable_compact_status() { return compact_status_; }
    inline const SMAStatusCompact& get_compact_status() const { return compact_status_; }
    inline const SMAStatusCompact& compact_status() const { return compact_status_; }

    static constexpr char const* LOADED_SETTINGS_NAME = "loaded_settings";
    inline void clear_loaded_settings() { loaded_settings_.clear(); }
    inline void set_loaded_settings(const SMAControllerSettings& value) { loaded_settings_ = value; }
    inline void set_loaded_settings(const SMAControllerSettings&& value) { loaded_settings_ = value; }
    inline SMAControllerSettings& mutable_loaded_settings() { return loaded_settings_; }
    inline const SMAControllerSettings& get_loaded_settings() const { return loaded_settings_; }
    inline const SMAControllerSettings& loaded_settings() const { return loaded_settings_; }

    static constexpr char const* VLD_SCALAR_NAME = "vld_scalar";
    inline void clear_vld_scalar() { vld_scalar_.clear(); }
    inline void set_vld_scalar(const float& value) { vld_scalar_ = value; }
    inline void set_vld_scalar(const float&& value) { vld_scalar_ = value; }
    inline float& mutable_vld_scalar() { return vld_scalar_.get(); }
    inline const float& get_vld_scalar() const { return vld_scalar_.get(); }
    inline float vld_scalar() const { return vld_scalar_.get(); }

    static constexpr char const* VLD_OFFSET_NAME = "vld_offset";
    inline void clear_vld_offset() { vld_offset_.clear(); }
    inline void set_vld_offset(const float& value) { vld_offset_ = value; }
    inline void set_vld_offset(const float&& value) { vld_offset_ = value; }
    inline float& mutable_vld_offset() { return vld_offset_.get(); }
    inline const float& get_vld_offset() const { return vld_offset_.get(); }
    inline float vld_offset() const { return vld_offset_.get(); }

    static constexpr char const* R_SNS_OHMS_NAME = "r_sns_ohms";
    inline void clear_r_sns_ohms() { r_sns_ohms_.clear(); }
    inline void set_r_sns_ohms(const float& value) { r_sns_ohms_ = value; }
    inline void set_r_sns_ohms(const float&& value) { r_sns_ohms_ = value; }
    inline float& mutable_r_sns_ohms() { return r_sns_ohms_.get(); }
    inline const float& get_r_sns_ohms() const { return r_sns_ohms_.get(); }
    inline float r_sns_ohms() const { return r_sns_ohms_.get(); }

    static constexpr char const* AMP_GAIN_NAME = "amp_gain";
    inline void clear_amp_gain() { amp_gain_.clear(); }
    inline void set_amp_gain(const float& value) { amp_gain_ = value; }
    inline void set_amp_gain(const float&& value) { amp_gain_ = value; }
    inline float& mutable_amp_gain() { return amp_gain_.get(); }
    inline const float& get_amp_gain() const { return amp_gain_.get(); }
    inline float amp_gain() const { return amp_gain_.get(); }

    static constexpr char const* AF_MOHMS_NAME = "af_mohms";
    inline void clear_af_mohms() { af_mohms_.clear(); }
    inline void set_af_mohms(const float& value) { af_mohms_ = value; }
    inline void set_af_mohms(const float&& value) { af_mohms_ = value; }
    inline float& mutable_af_mohms() { return af_mohms_.get(); }
    inline const float& get_af_mohms() const { return af_mohms_.get(); }
    inline float af_mohms() const { return af_mohms_.get(); }

    static constexpr char const* DELTA_MOHMS_NAME = "delta_mohms";
    inline void clear_delta_mohms() { delta_mohms_.clear(); }
    inline void set_delta_mohms(const float& value) { delta_mohms_ = value; }
    inline void set_delta_mohms(const float&& value) { delta_mohms_ = value; }
    inline float& mutable_delta_mohms() { return delta_mohms_.get(); }
    inline const float& get_delta_mohms() const { return delta_mohms_.get(); }
    inline float delta_mohms() const { return delta_mohms_.get(); }

    static constexpr char const* TRAINSTATE_NAME = "trainState";
    inline void clear_trainState() { trainState_.clear(); }
    inline void set_trainState(const uint32_t& value) { trainState_ = value; }
    inline void set_trainState(const uint32_t&& value) { trainState_ = value; }
    inline uint32_t& mutable_trainState() { return trainState_.get(); }
    inline const uint32_t& get_trainState() const { return trainState_.get(); }
    inline uint32_t trainState() const { return trainState_.get(); }

    static constexpr char const* RCTRL_KP_NAME = "rctrl_kp";
    inline void clear_rctrl_kp() { rctrl_kp_.clear(); }
    inline void set_rctrl_kp(const float& value) { rctrl_kp_ = value; }
    inline void set_rctrl_kp(const float&& value) { rctrl_kp_ = value; }
    inline float& mutable_rctrl_kp() { return rctrl_kp_.get(); }
    inline const float& get_rctrl_kp() const { return rctrl_kp_.get(); }
    inline float rctrl_kp() const { return rctrl_kp_.get(); }

    static constexpr char const* RCTRL_KI_NAME = "rctrl_ki";
    inline void clear_rctrl_ki() { rctrl_ki_.clear(); }
    inline void set_rctrl_ki(const float& value) { rctrl_ki_ = value; }
    inline void set_rctrl_ki(const float&& value) { rctrl_ki_ = value; }
    inline float& mutable_rctrl_ki() { return rctrl_ki_.get(); }
    inline const float& get_rctrl_ki() const { return rctrl_ki_.get(); }
    inline float rctrl_ki() const { return rctrl_ki_.get(); }

    static constexpr char const* RCTRL_KD_NAME = "rctrl_kd";
    inline void clear_rctrl_kd() { rctrl_kd_.clear(); }
    inline void set_rctrl_kd(const float& value) { rctrl_kd_ = value; }
    inline void set_rctrl_kd(const float&& value) { rctrl_kd_ = value; }
    inline float& mutable_rctrl_kd() { return rctrl_kd_.get(); }
    inline const float& get_rctrl_kd() const { return rctrl_kd_.get(); }
    inline float rctrl_kd() const { return rctrl_kd_.get(); }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = compact_status_.serialize_with_id(static_cast<uint32_t>(FieldNumber::COMPACT_STATUS), buffer, false);
      }

      if(::EmbeddedProto::Error::NO_ERRORS == return_value)
      {
        return_value = loaded_settings_.serialize_with_id(static_cast<uint32_t>(FieldNumber::LOADED_SETTINGS), buffer, false);
      }

      if((0.0 != vld_scalar_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = vld_scalar_.serialize_with_id(static_cast<uint32_t>(FieldNumber::VLD_SCALAR), buffer, false);
      }

      if((0.0 != vld_offset_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = vld_offset_.serialize_with_id(static_cast<uint32_t>(FieldNumber::VLD_OFFSET), buffer, false);
      }

      if((0.0 != r_sns_ohms_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = r_sns_ohms_.serialize_with_id(static_cast<uint32_t>(FieldNumber::R_SNS_OHMS), buffer, false);
      }

      if((0.0 != amp_gain_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = amp_gain_.serialize_with_id(static_cast<uint32_t>(FieldNumber::AMP_GAIN), buffer, false);
      }

      if((0.0 != af_mohms_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = af_mohms_.serialize_with_id(static_cast<uint32_t>(FieldNumber::AF_MOHMS), buffer, false);
      }

      if((0.0 != delta_mohms_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = delta_mohms_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DELTA_MOHMS), buffer, false);
      }

      if((0U != trainState_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = trainState_.serialize_with_id(static_cast<uint32_t>(FieldNumber::TRAINSTATE), buffer, false);
      }

      if((0.0 != rctrl_kp_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = rctrl_kp_.serialize_with_id(static_cast<uint32_t>(FieldNumber::RCTRL_KP), buffer, false);
      }

      if((0.0 != rctrl_ki_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = rctrl_ki_.serialize_with_id(static_cast<uint32_t>(FieldNumber::RCTRL_KI), buffer, false);
      }

      if((0.0 != rctrl_kd_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = rctrl_kd_.serialize_with_id(static_cast<uint32_t>(FieldNumber::RCTRL_KD), buffer, false);
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::COMPACT_STATUS:
            return_value = compact_status_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::LOADED_SETTINGS:
            return_value = loaded_settings_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::VLD_SCALAR:
            return_value = vld_scalar_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::VLD_OFFSET:
            return_value = vld_offset_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::R_SNS_OHMS:
            return_value = r_sns_ohms_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::AMP_GAIN:
            return_value = amp_gain_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::AF_MOHMS:
            return_value = af_mohms_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::DELTA_MOHMS:
            return_value = delta_mohms_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::TRAINSTATE:
            return_value = trainState_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::RCTRL_KP:
            return_value = rctrl_kp_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::RCTRL_KI:
            return_value = rctrl_ki_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::RCTRL_KD:
            return_value = rctrl_kd_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_compact_status();
      clear_loaded_settings();
      clear_vld_scalar();
      clear_vld_offset();
      clear_r_sns_ohms();
      clear_amp_gain();
      clear_af_mohms();
      clear_delta_mohms();
      clear_trainState();
      clear_rctrl_kp();
      clear_rctrl_ki();
      clear_rctrl_kd();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::COMPACT_STATUS:
          name = COMPACT_STATUS_NAME;
          break;
        case FieldNumber::LOADED_SETTINGS:
          name = LOADED_SETTINGS_NAME;
          break;
        case FieldNumber::VLD_SCALAR:
          name = VLD_SCALAR_NAME;
          break;
        case FieldNumber::VLD_OFFSET:
          name = VLD_OFFSET_NAME;
          break;
        case FieldNumber::R_SNS_OHMS:
          name = R_SNS_OHMS_NAME;
          break;
        case FieldNumber::AMP_GAIN:
          name = AMP_GAIN_NAME;
          break;
        case FieldNumber::AF_MOHMS:
          name = AF_MOHMS_NAME;
          break;
        case FieldNumber::DELTA_MOHMS:
          name = DELTA_MOHMS_NAME;
          break;
        case FieldNumber::TRAINSTATE:
          name = TRAINSTATE_NAME;
          break;
        case FieldNumber::RCTRL_KP:
          name = RCTRL_KP_NAME;
          break;
        case FieldNumber::RCTRL_KI:
          name = RCTRL_KI_NAME;
          break;
        case FieldNumber::RCTRL_KD:
          name = RCTRL_KD_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = compact_status_.to_string(left_chars, indent_level + 2, COMPACT_STATUS_NAME, true);
      left_chars = loaded_settings_.to_string(left_chars, indent_level + 2, LOADED_SETTINGS_NAME, false);
      left_chars = vld_scalar_.to_string(left_chars, indent_level + 2, VLD_SCALAR_NAME, false);
      left_chars = vld_offset_.to_string(left_chars, indent_level + 2, VLD_OFFSET_NAME, false);
      left_chars = r_sns_ohms_.to_string(left_chars, indent_level + 2, R_SNS_OHMS_NAME, false);
      left_chars = amp_gain_.to_string(left_chars, indent_level + 2, AMP_GAIN_NAME, false);
      left_chars = af_mohms_.to_string(left_chars, indent_level + 2, AF_MOHMS_NAME, false);
      left_chars = delta_mohms_.to_string(left_chars, indent_level + 2, DELTA_MOHMS_NAME, false);
      left_chars = trainState_.to_string(left_chars, indent_level + 2, TRAINSTATE_NAME, false);
      left_chars = rctrl_kp_.to_string(left_chars, indent_level + 2, RCTRL_KP_NAME, false);
      left_chars = rctrl_ki_.to_string(left_chars, indent_level + 2, RCTRL_KI_NAME, false);
      left_chars = rctrl_kd_.to_string(left_chars, indent_level + 2, RCTRL_KD_NAME, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      SMAStatusCompact compact_status_;
      SMAControllerSettings loaded_settings_;
      EmbeddedProto::floatfixed vld_scalar_ = 0.0;
      EmbeddedProto::floatfixed vld_offset_ = 0.0;
      EmbeddedProto::floatfixed r_sns_ohms_ = 0.0;
      EmbeddedProto::floatfixed amp_gain_ = 0.0;
      EmbeddedProto::floatfixed af_mohms_ = 0.0;
      EmbeddedProto::floatfixed delta_mohms_ = 0.0;
      EmbeddedProto::uint32 trainState_ = 0U;
      EmbeddedProto::floatfixed rctrl_kp_ = 0.0;
      EmbeddedProto::floatfixed rctrl_ki_ = 0.0;
      EmbeddedProto::floatfixed rctrl_kd_ = 0.0;

};

class NodeCommand final: public ::EmbeddedProto::MessageInterface
{
  public:
    NodeCommand() = default;
    NodeCommand(const NodeCommand& rhs )
    {
      if(rhs.get_which_command() != which_command_)
      {
        // First delete the old object in the oneof.
        clear_command();
      }

      switch(rhs.get_which_command())
      {
        case FieldNumber::RESET:
          set_reset(rhs.get_reset());
          break;

        case FieldNumber::ENABLE:
          set_enable(rhs.get_enable());
          break;

        case FieldNumber::DISABLE:
          set_disable(rhs.get_disable());
          break;

        case FieldNumber::SET_MODE:
          set_set_mode(rhs.get_set_mode());
          break;

        case FieldNumber::SET_SETPOINT:
          set_set_setpoint(rhs.get_set_setpoint());
          break;

        case FieldNumber::CONFIGURE_SETTINGS:
          set_configure_settings(rhs.get_configure_settings());
          break;

        case FieldNumber::STATUS:
          set_status(rhs.get_status());
          break;

        case FieldNumber::SILENCE_NODE:
          set_silence_node(rhs.get_silence_node());
          break;

        default:
          break;
      }

    }

    NodeCommand(const NodeCommand&& rhs ) noexcept
    {
      if(rhs.get_which_command() != which_command_)
      {
        // First delete the old object in the oneof.
        clear_command();
      }

      switch(rhs.get_which_command())
      {
        case FieldNumber::RESET:
          set_reset(rhs.get_reset());
          break;

        case FieldNumber::ENABLE:
          set_enable(rhs.get_enable());
          break;

        case FieldNumber::DISABLE:
          set_disable(rhs.get_disable());
          break;

        case FieldNumber::SET_MODE:
          set_set_mode(rhs.get_set_mode());
          break;

        case FieldNumber::SET_SETPOINT:
          set_set_setpoint(rhs.get_set_setpoint());
          break;

        case FieldNumber::CONFIGURE_SETTINGS:
          set_configure_settings(rhs.get_configure_settings());
          break;

        case FieldNumber::STATUS:
          set_status(rhs.get_status());
          break;

        case FieldNumber::SILENCE_NODE:
          set_silence_node(rhs.get_silence_node());
          break;

        default:
          break;
      }

    }

    ~NodeCommand() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      RESET = 1,
      ENABLE = 2,
      DISABLE = 3,
      SET_MODE = 4,
      SET_SETPOINT = 5,
      CONFIGURE_SETTINGS = 6,
      STATUS = 7,
      SILENCE_NODE = 8
    };

    NodeCommand& operator=(const NodeCommand& rhs)
    {
      if(rhs.get_which_command() != which_command_)
      {
        // First delete the old object in the oneof.
        clear_command();
      }

      switch(rhs.get_which_command())
      {
        case FieldNumber::RESET:
          set_reset(rhs.get_reset());
          break;

        case FieldNumber::ENABLE:
          set_enable(rhs.get_enable());
          break;

        case FieldNumber::DISABLE:
          set_disable(rhs.get_disable());
          break;

        case FieldNumber::SET_MODE:
          set_set_mode(rhs.get_set_mode());
          break;

        case FieldNumber::SET_SETPOINT:
          set_set_setpoint(rhs.get_set_setpoint());
          break;

        case FieldNumber::CONFIGURE_SETTINGS:
          set_configure_settings(rhs.get_configure_settings());
          break;

        case FieldNumber::STATUS:
          set_status(rhs.get_status());
          break;

        case FieldNumber::SILENCE_NODE:
          set_silence_node(rhs.get_silence_node());
          break;

        default:
          break;
      }

      return *this;
    }

    NodeCommand& operator=(const NodeCommand&& rhs) noexcept
    {
      if(rhs.get_which_command() != which_command_)
      {
        // First delete the old object in the oneof.
        clear_command();
      }

      switch(rhs.get_which_command())
      {
        case FieldNumber::RESET:
          set_reset(rhs.get_reset());
          break;

        case FieldNumber::ENABLE:
          set_enable(rhs.get_enable());
          break;

        case FieldNumber::DISABLE:
          set_disable(rhs.get_disable());
          break;

        case FieldNumber::SET_MODE:
          set_set_mode(rhs.get_set_mode());
          break;

        case FieldNumber::SET_SETPOINT:
          set_set_setpoint(rhs.get_set_setpoint());
          break;

        case FieldNumber::CONFIGURE_SETTINGS:
          set_configure_settings(rhs.get_configure_settings());
          break;

        case FieldNumber::STATUS:
          set_status(rhs.get_status());
          break;

        case FieldNumber::SILENCE_NODE:
          set_silence_node(rhs.get_silence_node());
          break;

        default:
          break;
      }

      return *this;
    }

    FieldNumber get_which_command() const { return which_command_; }

    static constexpr char const* RESET_NAME = "reset";
    inline bool has_reset() const
    {
      return FieldNumber::RESET == which_command_;
    }
    inline void clear_reset()
    {
      if(FieldNumber::RESET == which_command_)
      {
        which_command_ = FieldNumber::NOT_SET;
        command_.reset_.~ResetCommand();
      }
    }
    inline void set_reset(const ResetCommand& value)
    {
      if(FieldNumber::RESET != which_command_)
      {
        init_command(FieldNumber::RESET);
      }
      command_.reset_ = value;
    }
    inline void set_reset(const ResetCommand&& value)
    {
      if(FieldNumber::RESET != which_command_)
      {
        init_command(FieldNumber::RESET);
      }
      command_.reset_ = value;
    }
    inline ResetCommand& mutable_reset()
    {
      if(FieldNumber::RESET != which_command_)
      {
        init_command(FieldNumber::RESET);
      }
      return command_.reset_;
    }
    inline const ResetCommand& get_reset() const { return command_.reset_; }
    inline const ResetCommand& reset() const { return command_.reset_; }

    static constexpr char const* ENABLE_NAME = "enable";
    inline bool has_enable() const
    {
      return FieldNumber::ENABLE == which_command_;
    }
    inline void clear_enable()
    {
      if(FieldNumber::ENABLE == which_command_)
      {
        which_command_ = FieldNumber::NOT_SET;
        command_.enable_.~EnableCommand();
      }
    }
    inline void set_enable(const EnableCommand& value)
    {
      if(FieldNumber::ENABLE != which_command_)
      {
        init_command(FieldNumber::ENABLE);
      }
      command_.enable_ = value;
    }
    inline void set_enable(const EnableCommand&& value)
    {
      if(FieldNumber::ENABLE != which_command_)
      {
        init_command(FieldNumber::ENABLE);
      }
      command_.enable_ = value;
    }
    inline EnableCommand& mutable_enable()
    {
      if(FieldNumber::ENABLE != which_command_)
      {
        init_command(FieldNumber::ENABLE);
      }
      return command_.enable_;
    }
    inline const EnableCommand& get_enable() const { return command_.enable_; }
    inline const EnableCommand& enable() const { return command_.enable_; }

    static constexpr char const* DISABLE_NAME = "disable";
    inline bool has_disable() const
    {
      return FieldNumber::DISABLE == which_command_;
    }
    inline void clear_disable()
    {
      if(FieldNumber::DISABLE == which_command_)
      {
        which_command_ = FieldNumber::NOT_SET;
        command_.disable_.~DisableCommand();
      }
    }
    inline void set_disable(const DisableCommand& value)
    {
      if(FieldNumber::DISABLE != which_command_)
      {
        init_command(FieldNumber::DISABLE);
      }
      command_.disable_ = value;
    }
    inline void set_disable(const DisableCommand&& value)
    {
      if(FieldNumber::DISABLE != which_command_)
      {
        init_command(FieldNumber::DISABLE);
      }
      command_.disable_ = value;
    }
    inline DisableCommand& mutable_disable()
    {
      if(FieldNumber::DISABLE != which_command_)
      {
        init_command(FieldNumber::DISABLE);
      }
      return command_.disable_;
    }
    inline const DisableCommand& get_disable() const { return command_.disable_; }
    inline const DisableCommand& disable() const { return command_.disable_; }

    static constexpr char const* SET_MODE_NAME = "set_mode";
    inline bool has_set_mode() const
    {
      return FieldNumber::SET_MODE == which_command_;
    }
    inline void clear_set_mode()
    {
      if(FieldNumber::SET_MODE == which_command_)
      {
        which_command_ = FieldNumber::NOT_SET;
        command_.set_mode_.~SetModeCommand();
      }
    }
    inline void set_set_mode(const SetModeCommand& value)
    {
      if(FieldNumber::SET_MODE != which_command_)
      {
        init_command(FieldNumber::SET_MODE);
      }
      command_.set_mode_ = value;
    }
    inline void set_set_mode(const SetModeCommand&& value)
    {
      if(FieldNumber::SET_MODE != which_command_)
      {
        init_command(FieldNumber::SET_MODE);
      }
      command_.set_mode_ = value;
    }
    inline SetModeCommand& mutable_set_mode()
    {
      if(FieldNumber::SET_MODE != which_command_)
      {
        init_command(FieldNumber::SET_MODE);
      }
      return command_.set_mode_;
    }
    inline const SetModeCommand& get_set_mode() const { return command_.set_mode_; }
    inline const SetModeCommand& set_mode() const { return command_.set_mode_; }

    static constexpr char const* SET_SETPOINT_NAME = "set_setpoint";
    inline bool has_set_setpoint() const
    {
      return FieldNumber::SET_SETPOINT == which_command_;
    }
    inline void clear_set_setpoint()
    {
      if(FieldNumber::SET_SETPOINT == which_command_)
      {
        which_command_ = FieldNumber::NOT_SET;
        command_.set_setpoint_.~SetSetpointCommand();
      }
    }
    inline void set_set_setpoint(const SetSetpointCommand& value)
    {
      if(FieldNumber::SET_SETPOINT != which_command_)
      {
        init_command(FieldNumber::SET_SETPOINT);
      }
      command_.set_setpoint_ = value;
    }
    inline void set_set_setpoint(const SetSetpointCommand&& value)
    {
      if(FieldNumber::SET_SETPOINT != which_command_)
      {
        init_command(FieldNumber::SET_SETPOINT);
      }
      command_.set_setpoint_ = value;
    }
    inline SetSetpointCommand& mutable_set_setpoint()
    {
      if(FieldNumber::SET_SETPOINT != which_command_)
      {
        init_command(FieldNumber::SET_SETPOINT);
      }
      return command_.set_setpoint_;
    }
    inline const SetSetpointCommand& get_set_setpoint() const { return command_.set_setpoint_; }
    inline const SetSetpointCommand& set_setpoint() const { return command_.set_setpoint_; }

    static constexpr char const* CONFIGURE_SETTINGS_NAME = "configure_settings";
    inline bool has_configure_settings() const
    {
      return FieldNumber::CONFIGURE_SETTINGS == which_command_;
    }
    inline void clear_configure_settings()
    {
      if(FieldNumber::CONFIGURE_SETTINGS == which_command_)
      {
        which_command_ = FieldNumber::NOT_SET;
        command_.configure_settings_.~ConfigureSettingsCommand();
      }
    }
    inline void set_configure_settings(const ConfigureSettingsCommand& value)
    {
      if(FieldNumber::CONFIGURE_SETTINGS != which_command_)
      {
        init_command(FieldNumber::CONFIGURE_SETTINGS);
      }
      command_.configure_settings_ = value;
    }
    inline void set_configure_settings(const ConfigureSettingsCommand&& value)
    {
      if(FieldNumber::CONFIGURE_SETTINGS != which_command_)
      {
        init_command(FieldNumber::CONFIGURE_SETTINGS);
      }
      command_.configure_settings_ = value;
    }
    inline ConfigureSettingsCommand& mutable_configure_settings()
    {
      if(FieldNumber::CONFIGURE_SETTINGS != which_command_)
      {
        init_command(FieldNumber::CONFIGURE_SETTINGS);
      }
      return command_.configure_settings_;
    }
    inline const ConfigureSettingsCommand& get_configure_settings() const { return command_.configure_settings_; }
    inline const ConfigureSettingsCommand& configure_settings() const { return command_.configure_settings_; }

    static constexpr char const* STATUS_NAME = "status";
    inline bool has_status() const
    {
      return FieldNumber::STATUS == which_command_;
    }
    inline void clear_status()
    {
      if(FieldNumber::STATUS == which_command_)
      {
        which_command_ = FieldNumber::NOT_SET;
        command_.status_.~StatusCommand();
      }
    }
    inline void set_status(const StatusCommand& value)
    {
      if(FieldNumber::STATUS != which_command_)
      {
        init_command(FieldNumber::STATUS);
      }
      command_.status_ = value;
    }
    inline void set_status(const StatusCommand&& value)
    {
      if(FieldNumber::STATUS != which_command_)
      {
        init_command(FieldNumber::STATUS);
      }
      command_.status_ = value;
    }
    inline StatusCommand& mutable_status()
    {
      if(FieldNumber::STATUS != which_command_)
      {
        init_command(FieldNumber::STATUS);
      }
      return command_.status_;
    }
    inline const StatusCommand& get_status() const { return command_.status_; }
    inline const StatusCommand& status() const { return command_.status_; }

    static constexpr char const* SILENCE_NODE_NAME = "silence_node";
    inline bool has_silence_node() const
    {
      return FieldNumber::SILENCE_NODE == which_command_;
    }
    inline void clear_silence_node()
    {
      if(FieldNumber::SILENCE_NODE == which_command_)
      {
        which_command_ = FieldNumber::NOT_SET;
        command_.silence_node_.~SilenceNodeCommand();
      }
    }
    inline void set_silence_node(const SilenceNodeCommand& value)
    {
      if(FieldNumber::SILENCE_NODE != which_command_)
      {
        init_command(FieldNumber::SILENCE_NODE);
      }
      command_.silence_node_ = value;
    }
    inline void set_silence_node(const SilenceNodeCommand&& value)
    {
      if(FieldNumber::SILENCE_NODE != which_command_)
      {
        init_command(FieldNumber::SILENCE_NODE);
      }
      command_.silence_node_ = value;
    }
    inline SilenceNodeCommand& mutable_silence_node()
    {
      if(FieldNumber::SILENCE_NODE != which_command_)
      {
        init_command(FieldNumber::SILENCE_NODE);
      }
      return command_.silence_node_;
    }
    inline const SilenceNodeCommand& get_silence_node() const { return command_.silence_node_; }
    inline const SilenceNodeCommand& silence_node() const { return command_.silence_node_; }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      switch(which_command_)
      {
        case FieldNumber::RESET:
          if(has_reset() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_.reset_.serialize_with_id(static_cast<uint32_t>(FieldNumber::RESET), buffer, true);
          }
          break;

        case FieldNumber::ENABLE:
          if(has_enable() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_.enable_.serialize_with_id(static_cast<uint32_t>(FieldNumber::ENABLE), buffer, true);
          }
          break;

        case FieldNumber::DISABLE:
          if(has_disable() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_.disable_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DISABLE), buffer, true);
          }
          break;

        case FieldNumber::SET_MODE:
          if(has_set_mode() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_.set_mode_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SET_MODE), buffer, true);
          }
          break;

        case FieldNumber::SET_SETPOINT:
          if(has_set_setpoint() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_.set_setpoint_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SET_SETPOINT), buffer, true);
          }
          break;

        case FieldNumber::CONFIGURE_SETTINGS:
          if(has_configure_settings() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_.configure_settings_.serialize_with_id(static_cast<uint32_t>(FieldNumber::CONFIGURE_SETTINGS), buffer, true);
          }
          break;

        case FieldNumber::STATUS:
          if(has_status() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_.status_.serialize_with_id(static_cast<uint32_t>(FieldNumber::STATUS), buffer, true);
          }
          break;

        case FieldNumber::SILENCE_NODE:
          if(has_silence_node() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = command_.silence_node_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SILENCE_NODE), buffer, true);
          }
          break;

        default:
          break;
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::RESET:
          case FieldNumber::ENABLE:
          case FieldNumber::DISABLE:
          case FieldNumber::SET_MODE:
          case FieldNumber::SET_SETPOINT:
          case FieldNumber::CONFIGURE_SETTINGS:
          case FieldNumber::STATUS:
          case FieldNumber::SILENCE_NODE:
            return_value = deserialize_command(id_tag, buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_command();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::RESET:
          name = RESET_NAME;
          break;
        case FieldNumber::ENABLE:
          name = ENABLE_NAME;
          break;
        case FieldNumber::DISABLE:
          name = DISABLE_NAME;
          break;
        case FieldNumber::SET_MODE:
          name = SET_MODE_NAME;
          break;
        case FieldNumber::SET_SETPOINT:
          name = SET_SETPOINT_NAME;
          break;
        case FieldNumber::CONFIGURE_SETTINGS:
          name = CONFIGURE_SETTINGS_NAME;
          break;
        case FieldNumber::STATUS:
          name = STATUS_NAME;
          break;
        case FieldNumber::SILENCE_NODE:
          name = SILENCE_NODE_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = to_string_command(left_chars, indent_level + 2, true);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:



      FieldNumber which_command_ = FieldNumber::NOT_SET;
      union command
      {
        command() {}
        ~command() {}
        ResetCommand reset_;
        EnableCommand enable_;
        DisableCommand disable_;
        SetModeCommand set_mode_;
        SetSetpointCommand set_setpoint_;
        ConfigureSettingsCommand configure_settings_;
        StatusCommand status_;
        SilenceNodeCommand silence_node_;
      };
      command command_;

      void init_command(const FieldNumber field_id)
      {
        if(FieldNumber::NOT_SET != which_command_)
        {
          // First delete the old object in the oneof.
          clear_command();
        }

        // C++11 unions only support nontrivial members when you explicitly call the placement new statement.
        switch(field_id)
        {
          case FieldNumber::RESET:
            new(&command_.reset_) ResetCommand;
            break;

          case FieldNumber::ENABLE:
            new(&command_.enable_) EnableCommand;
            break;

          case FieldNumber::DISABLE:
            new(&command_.disable_) DisableCommand;
            break;

          case FieldNumber::SET_MODE:
            new(&command_.set_mode_) SetModeCommand;
            break;

          case FieldNumber::SET_SETPOINT:
            new(&command_.set_setpoint_) SetSetpointCommand;
            break;

          case FieldNumber::CONFIGURE_SETTINGS:
            new(&command_.configure_settings_) ConfigureSettingsCommand;
            break;

          case FieldNumber::STATUS:
            new(&command_.status_) StatusCommand;
            break;

          case FieldNumber::SILENCE_NODE:
            new(&command_.silence_node_) SilenceNodeCommand;
            break;

          default:
            break;
         }

         which_command_ = field_id;
      }

      void clear_command()
      {
        switch(which_command_)
        {
          case FieldNumber::RESET:
            ::EmbeddedProto::destroy_at(&command_.reset_);
            break;
          case FieldNumber::ENABLE:
            ::EmbeddedProto::destroy_at(&command_.enable_);
            break;
          case FieldNumber::DISABLE:
            ::EmbeddedProto::destroy_at(&command_.disable_);
            break;
          case FieldNumber::SET_MODE:
            ::EmbeddedProto::destroy_at(&command_.set_mode_);
            break;
          case FieldNumber::SET_SETPOINT:
            ::EmbeddedProto::destroy_at(&command_.set_setpoint_);
            break;
          case FieldNumber::CONFIGURE_SETTINGS:
            ::EmbeddedProto::destroy_at(&command_.configure_settings_);
            break;
          case FieldNumber::STATUS:
            ::EmbeddedProto::destroy_at(&command_.status_);
            break;
          case FieldNumber::SILENCE_NODE:
            ::EmbeddedProto::destroy_at(&command_.silence_node_);
            break;
          default:
            break;
        }
        which_command_ = FieldNumber::NOT_SET;
      }

      ::EmbeddedProto::Error deserialize_command(const FieldNumber field_id, 
                                    ::EmbeddedProto::ReadBufferInterface& buffer,
                                    const ::EmbeddedProto::WireFormatter::WireType wire_type)
      {
        ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
        
        if(field_id != which_command_)
        {
          init_command(field_id);
        }

        switch(which_command_)
        {
          case FieldNumber::RESET:
            return_value = command_.reset_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::ENABLE:
            return_value = command_.enable_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::DISABLE:
            return_value = command_.disable_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::SET_MODE:
            return_value = command_.set_mode_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::SET_SETPOINT:
            return_value = command_.set_setpoint_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::CONFIGURE_SETTINGS:
            return_value = command_.configure_settings_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::STATUS:
            return_value = command_.status_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::SILENCE_NODE:
            return_value = command_.silence_node_.deserialize_check_type(buffer, wire_type);
            break;
          default:
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS != return_value)
        {
          clear_command();
        }
        return return_value;
      }

#ifdef MSG_TO_STRING 
      ::EmbeddedProto::string_view to_string_command(::EmbeddedProto::string_view& str, const uint32_t indent_level, const bool first_field) const
      {
        ::EmbeddedProto::string_view left_chars = str;

        switch(which_command_)
        {
          case FieldNumber::RESET:
            left_chars = command_.reset_.to_string(left_chars, indent_level, RESET_NAME, first_field);
            break;
          case FieldNumber::ENABLE:
            left_chars = command_.enable_.to_string(left_chars, indent_level, ENABLE_NAME, first_field);
            break;
          case FieldNumber::DISABLE:
            left_chars = command_.disable_.to_string(left_chars, indent_level, DISABLE_NAME, first_field);
            break;
          case FieldNumber::SET_MODE:
            left_chars = command_.set_mode_.to_string(left_chars, indent_level, SET_MODE_NAME, first_field);
            break;
          case FieldNumber::SET_SETPOINT:
            left_chars = command_.set_setpoint_.to_string(left_chars, indent_level, SET_SETPOINT_NAME, first_field);
            break;
          case FieldNumber::CONFIGURE_SETTINGS:
            left_chars = command_.configure_settings_.to_string(left_chars, indent_level, CONFIGURE_SETTINGS_NAME, first_field);
            break;
          case FieldNumber::STATUS:
            left_chars = command_.status_.to_string(left_chars, indent_level, STATUS_NAME, first_field);
            break;
          case FieldNumber::SILENCE_NODE:
            left_chars = command_.silence_node_.to_string(left_chars, indent_level, SILENCE_NODE_NAME, first_field);
            break;
          default:
            break;
        }

        return left_chars;
      }

#endif // End of MSG_TO_STRING
};

template<
    uint32_t StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, 
    uint32_t StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH
>
class StatusResponse final: public ::EmbeddedProto::MessageInterface
{
  public:
    StatusResponse() = default;
    StatusResponse(const StatusResponse& rhs )
    {
      set_device(rhs.get_device());
      if(rhs.get_which_status_response() != which_status_response_)
      {
        // First delete the old object in the oneof.
        clear_status_response();
      }

      switch(rhs.get_which_status_response())
      {
        case FieldNumber::NODE_STATUS_COMPACT:
          set_node_status_compact(rhs.get_node_status_compact());
          break;

        case FieldNumber::NODE_STATUS_DUMP:
          set_node_status_dump(rhs.get_node_status_dump());
          break;

        case FieldNumber::SMA_STATUS_COMPACT:
          set_sma_status_compact(rhs.get_sma_status_compact());
          break;

        case FieldNumber::SMA_STATUS_DUMP:
          set_sma_status_dump(rhs.get_sma_status_dump());
          break;

        default:
          break;
      }

    }

    StatusResponse(const StatusResponse&& rhs ) noexcept
    {
      set_device(rhs.get_device());
      if(rhs.get_which_status_response() != which_status_response_)
      {
        // First delete the old object in the oneof.
        clear_status_response();
      }

      switch(rhs.get_which_status_response())
      {
        case FieldNumber::NODE_STATUS_COMPACT:
          set_node_status_compact(rhs.get_node_status_compact());
          break;

        case FieldNumber::NODE_STATUS_DUMP:
          set_node_status_dump(rhs.get_node_status_dump());
          break;

        case FieldNumber::SMA_STATUS_COMPACT:
          set_sma_status_compact(rhs.get_sma_status_compact());
          break;

        case FieldNumber::SMA_STATUS_DUMP:
          set_sma_status_dump(rhs.get_sma_status_dump());
          break;

        default:
          break;
      }

    }

    ~StatusResponse() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      DEVICE = 1,
      NODE_STATUS_COMPACT = 2,
      NODE_STATUS_DUMP = 3,
      SMA_STATUS_COMPACT = 4,
      SMA_STATUS_DUMP = 5
    };

    StatusResponse& operator=(const StatusResponse& rhs)
    {
      set_device(rhs.get_device());
      if(rhs.get_which_status_response() != which_status_response_)
      {
        // First delete the old object in the oneof.
        clear_status_response();
      }

      switch(rhs.get_which_status_response())
      {
        case FieldNumber::NODE_STATUS_COMPACT:
          set_node_status_compact(rhs.get_node_status_compact());
          break;

        case FieldNumber::NODE_STATUS_DUMP:
          set_node_status_dump(rhs.get_node_status_dump());
          break;

        case FieldNumber::SMA_STATUS_COMPACT:
          set_sma_status_compact(rhs.get_sma_status_compact());
          break;

        case FieldNumber::SMA_STATUS_DUMP:
          set_sma_status_dump(rhs.get_sma_status_dump());
          break;

        default:
          break;
      }

      return *this;
    }

    StatusResponse& operator=(const StatusResponse&& rhs) noexcept
    {
      set_device(rhs.get_device());
      if(rhs.get_which_status_response() != which_status_response_)
      {
        // First delete the old object in the oneof.
        clear_status_response();
      }

      switch(rhs.get_which_status_response())
      {
        case FieldNumber::NODE_STATUS_COMPACT:
          set_node_status_compact(rhs.get_node_status_compact());
          break;

        case FieldNumber::NODE_STATUS_DUMP:
          set_node_status_dump(rhs.get_node_status_dump());
          break;

        case FieldNumber::SMA_STATUS_COMPACT:
          set_sma_status_compact(rhs.get_sma_status_compact());
          break;

        case FieldNumber::SMA_STATUS_DUMP:
          set_sma_status_dump(rhs.get_sma_status_dump());
          break;

        default:
          break;
      }

      return *this;
    }

    static constexpr char const* DEVICE_NAME = "device";
    inline void clear_device() { device_.clear(); }
    inline void set_device(const Device& value) { device_ = value; }
    inline void set_device(const Device&& value) { device_ = value; }
    inline const Device& get_device() const { return device_.get(); }
    inline Device device() const { return device_.get(); }

    FieldNumber get_which_status_response() const { return which_status_response_; }

    static constexpr char const* NODE_STATUS_COMPACT_NAME = "node_status_compact";
    inline bool has_node_status_compact() const
    {
      return FieldNumber::NODE_STATUS_COMPACT == which_status_response_;
    }
    inline void clear_node_status_compact()
    {
      if(FieldNumber::NODE_STATUS_COMPACT == which_status_response_)
      {
        which_status_response_ = FieldNumber::NOT_SET;
        status_response_.node_status_compact_.~NodeStatusCompact();
      }
    }
    inline void set_node_status_compact(const NodeStatusCompact& value)
    {
      if(FieldNumber::NODE_STATUS_COMPACT != which_status_response_)
      {
        init_status_response(FieldNumber::NODE_STATUS_COMPACT);
      }
      status_response_.node_status_compact_ = value;
    }
    inline void set_node_status_compact(const NodeStatusCompact&& value)
    {
      if(FieldNumber::NODE_STATUS_COMPACT != which_status_response_)
      {
        init_status_response(FieldNumber::NODE_STATUS_COMPACT);
      }
      status_response_.node_status_compact_ = value;
    }
    inline NodeStatusCompact& mutable_node_status_compact()
    {
      if(FieldNumber::NODE_STATUS_COMPACT != which_status_response_)
      {
        init_status_response(FieldNumber::NODE_STATUS_COMPACT);
      }
      return status_response_.node_status_compact_;
    }
    inline const NodeStatusCompact& get_node_status_compact() const { return status_response_.node_status_compact_; }
    inline const NodeStatusCompact& node_status_compact() const { return status_response_.node_status_compact_; }

    static constexpr char const* NODE_STATUS_DUMP_NAME = "node_status_dump";
    inline bool has_node_status_dump() const
    {
      return FieldNumber::NODE_STATUS_DUMP == which_status_response_;
    }
    inline void clear_node_status_dump()
    {
      if(FieldNumber::NODE_STATUS_DUMP == which_status_response_)
      {
        which_status_response_ = FieldNumber::NOT_SET;
        status_response_.node_status_dump_.~NodeStatusDump<StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>();
      }
    }
    inline void set_node_status_dump(const NodeStatusDump<StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>& value)
    {
      if(FieldNumber::NODE_STATUS_DUMP != which_status_response_)
      {
        init_status_response(FieldNumber::NODE_STATUS_DUMP);
      }
      status_response_.node_status_dump_ = value;
    }
    inline void set_node_status_dump(const NodeStatusDump<StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>&& value)
    {
      if(FieldNumber::NODE_STATUS_DUMP != which_status_response_)
      {
        init_status_response(FieldNumber::NODE_STATUS_DUMP);
      }
      status_response_.node_status_dump_ = value;
    }
    inline NodeStatusDump<StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>& mutable_node_status_dump()
    {
      if(FieldNumber::NODE_STATUS_DUMP != which_status_response_)
      {
        init_status_response(FieldNumber::NODE_STATUS_DUMP);
      }
      return status_response_.node_status_dump_;
    }
    inline const NodeStatusDump<StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>& get_node_status_dump() const { return status_response_.node_status_dump_; }
    inline const NodeStatusDump<StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>& node_status_dump() const { return status_response_.node_status_dump_; }

    static constexpr char const* SMA_STATUS_COMPACT_NAME = "sma_status_compact";
    inline bool has_sma_status_compact() const
    {
      return FieldNumber::SMA_STATUS_COMPACT == which_status_response_;
    }
    inline void clear_sma_status_compact()
    {
      if(FieldNumber::SMA_STATUS_COMPACT == which_status_response_)
      {
        which_status_response_ = FieldNumber::NOT_SET;
        status_response_.sma_status_compact_.~SMAStatusCompact();
      }
    }
    inline void set_sma_status_compact(const SMAStatusCompact& value)
    {
      if(FieldNumber::SMA_STATUS_COMPACT != which_status_response_)
      {
        init_status_response(FieldNumber::SMA_STATUS_COMPACT);
      }
      status_response_.sma_status_compact_ = value;
    }
    inline void set_sma_status_compact(const SMAStatusCompact&& value)
    {
      if(FieldNumber::SMA_STATUS_COMPACT != which_status_response_)
      {
        init_status_response(FieldNumber::SMA_STATUS_COMPACT);
      }
      status_response_.sma_status_compact_ = value;
    }
    inline SMAStatusCompact& mutable_sma_status_compact()
    {
      if(FieldNumber::SMA_STATUS_COMPACT != which_status_response_)
      {
        init_status_response(FieldNumber::SMA_STATUS_COMPACT);
      }
      return status_response_.sma_status_compact_;
    }
    inline const SMAStatusCompact& get_sma_status_compact() const { return status_response_.sma_status_compact_; }
    inline const SMAStatusCompact& sma_status_compact() const { return status_response_.sma_status_compact_; }

    static constexpr char const* SMA_STATUS_DUMP_NAME = "sma_status_dump";
    inline bool has_sma_status_dump() const
    {
      return FieldNumber::SMA_STATUS_DUMP == which_status_response_;
    }
    inline void clear_sma_status_dump()
    {
      if(FieldNumber::SMA_STATUS_DUMP == which_status_response_)
      {
        which_status_response_ = FieldNumber::NOT_SET;
        status_response_.sma_status_dump_.~SMAStatusDump();
      }
    }
    inline void set_sma_status_dump(const SMAStatusDump& value)
    {
      if(FieldNumber::SMA_STATUS_DUMP != which_status_response_)
      {
        init_status_response(FieldNumber::SMA_STATUS_DUMP);
      }
      status_response_.sma_status_dump_ = value;
    }
    inline void set_sma_status_dump(const SMAStatusDump&& value)
    {
      if(FieldNumber::SMA_STATUS_DUMP != which_status_response_)
      {
        init_status_response(FieldNumber::SMA_STATUS_DUMP);
      }
      status_response_.sma_status_dump_ = value;
    }
    inline SMAStatusDump& mutable_sma_status_dump()
    {
      if(FieldNumber::SMA_STATUS_DUMP != which_status_response_)
      {
        init_status_response(FieldNumber::SMA_STATUS_DUMP);
      }
      return status_response_.sma_status_dump_;
    }
    inline const SMAStatusDump& get_sma_status_dump() const { return status_response_.sma_status_dump_; }
    inline const SMAStatusDump& sma_status_dump() const { return status_response_.sma_status_dump_; }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      if((static_cast<Device>(0) != device_.get()) && (::EmbeddedProto::Error::NO_ERRORS == return_value))
      {
        return_value = device_.serialize_with_id(static_cast<uint32_t>(FieldNumber::DEVICE), buffer, false);
      }

      switch(which_status_response_)
      {
        case FieldNumber::NODE_STATUS_COMPACT:
          if(has_node_status_compact() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = status_response_.node_status_compact_.serialize_with_id(static_cast<uint32_t>(FieldNumber::NODE_STATUS_COMPACT), buffer, true);
          }
          break;

        case FieldNumber::NODE_STATUS_DUMP:
          if(has_node_status_dump() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = status_response_.node_status_dump_.serialize_with_id(static_cast<uint32_t>(FieldNumber::NODE_STATUS_DUMP), buffer, true);
          }
          break;

        case FieldNumber::SMA_STATUS_COMPACT:
          if(has_sma_status_compact() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = status_response_.sma_status_compact_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SMA_STATUS_COMPACT), buffer, true);
          }
          break;

        case FieldNumber::SMA_STATUS_DUMP:
          if(has_sma_status_dump() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = status_response_.sma_status_dump_.serialize_with_id(static_cast<uint32_t>(FieldNumber::SMA_STATUS_DUMP), buffer, true);
          }
          break;

        default:
          break;
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::DEVICE:
            return_value = device_.deserialize_check_type(buffer, wire_type);
            break;

          case FieldNumber::NODE_STATUS_COMPACT:
          case FieldNumber::NODE_STATUS_DUMP:
          case FieldNumber::SMA_STATUS_COMPACT:
          case FieldNumber::SMA_STATUS_DUMP:
            return_value = deserialize_status_response(id_tag, buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_device();
      clear_status_response();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::DEVICE:
          name = DEVICE_NAME;
          break;
        case FieldNumber::NODE_STATUS_COMPACT:
          name = NODE_STATUS_COMPACT_NAME;
          break;
        case FieldNumber::NODE_STATUS_DUMP:
          name = NODE_STATUS_DUMP_NAME;
          break;
        case FieldNumber::SMA_STATUS_COMPACT:
          name = SMA_STATUS_COMPACT_NAME;
          break;
        case FieldNumber::SMA_STATUS_DUMP:
          name = SMA_STATUS_DUMP_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = device_.to_string(left_chars, indent_level + 2, DEVICE_NAME, true);
      left_chars = to_string_status_response(left_chars, indent_level + 2, false);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:


      EmbeddedProto::enumeration<Device> device_ = static_cast<Device>(0);

      FieldNumber which_status_response_ = FieldNumber::NOT_SET;
      union status_response
      {
        status_response() {}
        ~status_response() {}
        NodeStatusCompact node_status_compact_;
        NodeStatusDump<StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH> node_status_dump_;
        SMAStatusCompact sma_status_compact_;
        SMAStatusDump sma_status_dump_;
      };
      status_response status_response_;

      void init_status_response(const FieldNumber field_id)
      {
        if(FieldNumber::NOT_SET != which_status_response_)
        {
          // First delete the old object in the oneof.
          clear_status_response();
        }

        // C++11 unions only support nontrivial members when you explicitly call the placement new statement.
        switch(field_id)
        {
          case FieldNumber::NODE_STATUS_COMPACT:
            new(&status_response_.node_status_compact_) NodeStatusCompact;
            break;

          case FieldNumber::NODE_STATUS_DUMP:
            new(&status_response_.node_status_dump_) NodeStatusDump<StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>;
            break;

          case FieldNumber::SMA_STATUS_COMPACT:
            new(&status_response_.sma_status_compact_) SMAStatusCompact;
            break;

          case FieldNumber::SMA_STATUS_DUMP:
            new(&status_response_.sma_status_dump_) SMAStatusDump;
            break;

          default:
            break;
         }

         which_status_response_ = field_id;
      }

      void clear_status_response()
      {
        switch(which_status_response_)
        {
          case FieldNumber::NODE_STATUS_COMPACT:
            ::EmbeddedProto::destroy_at(&status_response_.node_status_compact_);
            break;
          case FieldNumber::NODE_STATUS_DUMP:
            ::EmbeddedProto::destroy_at(&status_response_.node_status_dump_);
            break;
          case FieldNumber::SMA_STATUS_COMPACT:
            ::EmbeddedProto::destroy_at(&status_response_.sma_status_compact_);
            break;
          case FieldNumber::SMA_STATUS_DUMP:
            ::EmbeddedProto::destroy_at(&status_response_.sma_status_dump_);
            break;
          default:
            break;
        }
        which_status_response_ = FieldNumber::NOT_SET;
      }

      ::EmbeddedProto::Error deserialize_status_response(const FieldNumber field_id, 
                                    ::EmbeddedProto::ReadBufferInterface& buffer,
                                    const ::EmbeddedProto::WireFormatter::WireType wire_type)
      {
        ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
        
        if(field_id != which_status_response_)
        {
          init_status_response(field_id);
        }

        switch(which_status_response_)
        {
          case FieldNumber::NODE_STATUS_COMPACT:
            return_value = status_response_.node_status_compact_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::NODE_STATUS_DUMP:
            return_value = status_response_.node_status_dump_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::SMA_STATUS_COMPACT:
            return_value = status_response_.sma_status_compact_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::SMA_STATUS_DUMP:
            return_value = status_response_.sma_status_dump_.deserialize_check_type(buffer, wire_type);
            break;
          default:
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS != return_value)
        {
          clear_status_response();
        }
        return return_value;
      }

#ifdef MSG_TO_STRING 
      ::EmbeddedProto::string_view to_string_status_response(::EmbeddedProto::string_view& str, const uint32_t indent_level, const bool first_field) const
      {
        ::EmbeddedProto::string_view left_chars = str;

        switch(which_status_response_)
        {
          case FieldNumber::NODE_STATUS_COMPACT:
            left_chars = status_response_.node_status_compact_.to_string(left_chars, indent_level, NODE_STATUS_COMPACT_NAME, first_field);
            break;
          case FieldNumber::NODE_STATUS_DUMP:
            left_chars = status_response_.node_status_dump_.to_string(left_chars, indent_level, NODE_STATUS_DUMP_NAME, first_field);
            break;
          case FieldNumber::SMA_STATUS_COMPACT:
            left_chars = status_response_.sma_status_compact_.to_string(left_chars, indent_level, SMA_STATUS_COMPACT_NAME, first_field);
            break;
          case FieldNumber::SMA_STATUS_DUMP:
            left_chars = status_response_.sma_status_dump_.to_string(left_chars, indent_level, SMA_STATUS_DUMP_NAME, first_field);
            break;
          default:
            break;
        }

        return left_chars;
      }

#endif // End of MSG_TO_STRING
};

template<
    uint32_t Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, 
    uint32_t Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH
>
class Response final: public ::EmbeddedProto::MessageInterface
{
  public:
    Response() = default;
    Response(const Response& rhs )
    {
      if(rhs.get_which_response() != which_response_)
      {
        // First delete the old object in the oneof.
        clear_response();
      }

      switch(rhs.get_which_response())
      {
        case FieldNumber::GENERAL_RESPONSE:
          set_general_response(rhs.get_general_response());
          break;

        case FieldNumber::STATUS_RESPONSE:
          set_status_response(rhs.get_status_response());
          break;

        default:
          break;
      }

    }

    Response(const Response&& rhs ) noexcept
    {
      if(rhs.get_which_response() != which_response_)
      {
        // First delete the old object in the oneof.
        clear_response();
      }

      switch(rhs.get_which_response())
      {
        case FieldNumber::GENERAL_RESPONSE:
          set_general_response(rhs.get_general_response());
          break;

        case FieldNumber::STATUS_RESPONSE:
          set_status_response(rhs.get_status_response());
          break;

        default:
          break;
      }

    }

    ~Response() override = default;

    enum class FieldNumber : uint32_t
    {
      NOT_SET = 0,
      GENERAL_RESPONSE = 1,
      STATUS_RESPONSE = 2
    };

    Response& operator=(const Response& rhs)
    {
      if(rhs.get_which_response() != which_response_)
      {
        // First delete the old object in the oneof.
        clear_response();
      }

      switch(rhs.get_which_response())
      {
        case FieldNumber::GENERAL_RESPONSE:
          set_general_response(rhs.get_general_response());
          break;

        case FieldNumber::STATUS_RESPONSE:
          set_status_response(rhs.get_status_response());
          break;

        default:
          break;
      }

      return *this;
    }

    Response& operator=(const Response&& rhs) noexcept
    {
      if(rhs.get_which_response() != which_response_)
      {
        // First delete the old object in the oneof.
        clear_response();
      }

      switch(rhs.get_which_response())
      {
        case FieldNumber::GENERAL_RESPONSE:
          set_general_response(rhs.get_general_response());
          break;

        case FieldNumber::STATUS_RESPONSE:
          set_status_response(rhs.get_status_response());
          break;

        default:
          break;
      }

      return *this;
    }

    FieldNumber get_which_response() const { return which_response_; }

    static constexpr char const* GENERAL_RESPONSE_NAME = "general_response";
    inline bool has_general_response() const
    {
      return FieldNumber::GENERAL_RESPONSE == which_response_;
    }
    inline void clear_general_response()
    {
      if(FieldNumber::GENERAL_RESPONSE == which_response_)
      {
        which_response_ = FieldNumber::NOT_SET;
        response_.general_response_.~GeneralResponse();
      }
    }
    inline void set_general_response(const GeneralResponse& value)
    {
      if(FieldNumber::GENERAL_RESPONSE != which_response_)
      {
        init_response(FieldNumber::GENERAL_RESPONSE);
      }
      response_.general_response_ = value;
    }
    inline void set_general_response(const GeneralResponse&& value)
    {
      if(FieldNumber::GENERAL_RESPONSE != which_response_)
      {
        init_response(FieldNumber::GENERAL_RESPONSE);
      }
      response_.general_response_ = value;
    }
    inline GeneralResponse& mutable_general_response()
    {
      if(FieldNumber::GENERAL_RESPONSE != which_response_)
      {
        init_response(FieldNumber::GENERAL_RESPONSE);
      }
      return response_.general_response_;
    }
    inline const GeneralResponse& get_general_response() const { return response_.general_response_; }
    inline const GeneralResponse& general_response() const { return response_.general_response_; }

    static constexpr char const* STATUS_RESPONSE_NAME = "status_response";
    inline bool has_status_response() const
    {
      return FieldNumber::STATUS_RESPONSE == which_response_;
    }
    inline void clear_status_response()
    {
      if(FieldNumber::STATUS_RESPONSE == which_response_)
      {
        which_response_ = FieldNumber::NOT_SET;
        response_.status_response_.~StatusResponse<Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>();
      }
    }
    inline void set_status_response(const StatusResponse<Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>& value)
    {
      if(FieldNumber::STATUS_RESPONSE != which_response_)
      {
        init_response(FieldNumber::STATUS_RESPONSE);
      }
      response_.status_response_ = value;
    }
    inline void set_status_response(const StatusResponse<Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>&& value)
    {
      if(FieldNumber::STATUS_RESPONSE != which_response_)
      {
        init_response(FieldNumber::STATUS_RESPONSE);
      }
      response_.status_response_ = value;
    }
    inline StatusResponse<Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>& mutable_status_response()
    {
      if(FieldNumber::STATUS_RESPONSE != which_response_)
      {
        init_response(FieldNumber::STATUS_RESPONSE);
      }
      return response_.status_response_;
    }
    inline const StatusResponse<Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>& get_status_response() const { return response_.status_response_; }
    inline const StatusResponse<Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>& status_response() const { return response_.status_response_; }


    ::EmbeddedProto::Error serialize(::EmbeddedProto::WriteBufferInterface& buffer) const override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;

      switch(which_response_)
      {
        case FieldNumber::GENERAL_RESPONSE:
          if(has_general_response() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = response_.general_response_.serialize_with_id(static_cast<uint32_t>(FieldNumber::GENERAL_RESPONSE), buffer, true);
          }
          break;

        case FieldNumber::STATUS_RESPONSE:
          if(has_status_response() && (::EmbeddedProto::Error::NO_ERRORS == return_value))
          {
            return_value = response_.status_response_.serialize_with_id(static_cast<uint32_t>(FieldNumber::STATUS_RESPONSE), buffer, true);
          }
          break;

        default:
          break;
      }

      return return_value;
    };

    ::EmbeddedProto::Error deserialize(::EmbeddedProto::ReadBufferInterface& buffer) override
    {
      ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
      ::EmbeddedProto::WireFormatter::WireType wire_type = ::EmbeddedProto::WireFormatter::WireType::VARINT;
      uint32_t id_number = 0;
      FieldNumber id_tag = FieldNumber::NOT_SET;

      ::EmbeddedProto::Error tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
      while((::EmbeddedProto::Error::NO_ERRORS == return_value) && (::EmbeddedProto::Error::NO_ERRORS == tag_value))
      {
        id_tag = static_cast<FieldNumber>(id_number);
        switch(id_tag)
        {
          case FieldNumber::GENERAL_RESPONSE:
          case FieldNumber::STATUS_RESPONSE:
            return_value = deserialize_response(id_tag, buffer, wire_type);
            break;

          case FieldNumber::NOT_SET:
            return_value = ::EmbeddedProto::Error::INVALID_FIELD_ID;
            break;

          default:
            return_value = skip_unknown_field(buffer, wire_type);
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS == return_value)
        {
          // Read the next tag.
          tag_value = ::EmbeddedProto::WireFormatter::DeserializeTag(buffer, wire_type, id_number);
        }
      }

      // When an error was detect while reading the tag but no other errors where found, set it in the return value.
      if((::EmbeddedProto::Error::NO_ERRORS == return_value)
         && (::EmbeddedProto::Error::NO_ERRORS != tag_value)
         && (::EmbeddedProto::Error::END_OF_BUFFER != tag_value)) // The end of the buffer is not an array in this case.
      {
        return_value = tag_value;
      }

      return return_value;
    };

    void clear() override
    {
      clear_response();

    }

    static char const* field_number_to_name(const FieldNumber fieldNumber)
    {
      char const* name = nullptr;
      switch(fieldNumber)
      {
        case FieldNumber::GENERAL_RESPONSE:
          name = GENERAL_RESPONSE_NAME;
          break;
        case FieldNumber::STATUS_RESPONSE:
          name = STATUS_RESPONSE_NAME;
          break;
        default:
          name = "Invalid FieldNumber";
          break;
      }
      return name;
    }

#ifdef MSG_TO_STRING

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str) const
    {
      return this->to_string(str, 0, nullptr, true);
    }

    ::EmbeddedProto::string_view to_string(::EmbeddedProto::string_view& str, const uint32_t indent_level, char const* name, const bool first_field) const override
    {
      ::EmbeddedProto::string_view left_chars = str;
      int32_t n_chars_used = 0;

      if(!first_field)
      {
        // Add a comma behind the previous field.
        n_chars_used = snprintf(left_chars.data, left_chars.size, ",\n");
        if(0 < n_chars_used)
        {
          // Update the character pointer and characters left in the array.
          left_chars.data += n_chars_used;
          left_chars.size -= n_chars_used;
        }
      }

      if(nullptr != name)
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "\"%s\": {\n", name);
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s\"%s\": {\n", indent_level, " ", name);
        }
      }
      else
      {
        if( 0 == indent_level)
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "{\n");
        }
        else
        {
          n_chars_used = snprintf(left_chars.data, left_chars.size, "%*s{\n", indent_level, " ");
        }
      }
      
      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      left_chars = to_string_response(left_chars, indent_level + 2, true);
  
      if( 0 == indent_level) 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n}");
      }
      else 
      {
        n_chars_used = snprintf(left_chars.data, left_chars.size, "\n%*s}", indent_level, " ");
      }

      if(0 < n_chars_used)
      {
        left_chars.data += n_chars_used;
        left_chars.size -= n_chars_used;
      }

      return left_chars;
    }

#endif // End of MSG_TO_STRING

  private:



      FieldNumber which_response_ = FieldNumber::NOT_SET;
      union response
      {
        response() {}
        ~response() {}
        GeneralResponse general_response_;
        StatusResponse<Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH> status_response_;
      };
      response response_;

      void init_response(const FieldNumber field_id)
      {
        if(FieldNumber::NOT_SET != which_response_)
        {
          // First delete the old object in the oneof.
          clear_response();
        }

        // C++11 unions only support nontrivial members when you explicitly call the placement new statement.
        switch(field_id)
        {
          case FieldNumber::GENERAL_RESPONSE:
            new(&response_.general_response_) GeneralResponse;
            break;

          case FieldNumber::STATUS_RESPONSE:
            new(&response_.status_response_) StatusResponse<Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_firmware_version_LENGTH, Response_status_response_StatusResponse_node_status_dump_NodeStatusDump_board_version_LENGTH>;
            break;

          default:
            break;
         }

         which_response_ = field_id;
      }

      void clear_response()
      {
        switch(which_response_)
        {
          case FieldNumber::GENERAL_RESPONSE:
            ::EmbeddedProto::destroy_at(&response_.general_response_);
            break;
          case FieldNumber::STATUS_RESPONSE:
            ::EmbeddedProto::destroy_at(&response_.status_response_);
            break;
          default:
            break;
        }
        which_response_ = FieldNumber::NOT_SET;
      }

      ::EmbeddedProto::Error deserialize_response(const FieldNumber field_id, 
                                    ::EmbeddedProto::ReadBufferInterface& buffer,
                                    const ::EmbeddedProto::WireFormatter::WireType wire_type)
      {
        ::EmbeddedProto::Error return_value = ::EmbeddedProto::Error::NO_ERRORS;
        
        if(field_id != which_response_)
        {
          init_response(field_id);
        }

        switch(which_response_)
        {
          case FieldNumber::GENERAL_RESPONSE:
            return_value = response_.general_response_.deserialize_check_type(buffer, wire_type);
            break;
          case FieldNumber::STATUS_RESPONSE:
            return_value = response_.status_response_.deserialize_check_type(buffer, wire_type);
            break;
          default:
            break;
        }

        if(::EmbeddedProto::Error::NO_ERRORS != return_value)
        {
          clear_response();
        }
        return return_value;
      }

#ifdef MSG_TO_STRING 
      ::EmbeddedProto::string_view to_string_response(::EmbeddedProto::string_view& str, const uint32_t indent_level, const bool first_field) const
      {
        ::EmbeddedProto::string_view left_chars = str;

        switch(which_response_)
        {
          case FieldNumber::GENERAL_RESPONSE:
            left_chars = response_.general_response_.to_string(left_chars, indent_level, GENERAL_RESPONSE_NAME, first_field);
            break;
          case FieldNumber::STATUS_RESPONSE:
            left_chars = response_.status_response_.to_string(left_chars, indent_level, STATUS_RESPONSE_NAME, first_field);
            break;
          default:
            break;
        }

        return left_chars;
      }

#endif // End of MSG_TO_STRING
};

} // End of namespace tfnode
#endif // TFNODE-MESSAGES_H