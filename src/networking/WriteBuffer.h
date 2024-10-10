// WriteBuffer.h
#ifndef WRITE_BUFFER_H
#define WRITE_BUFFER_H

#include <WriteBufferInterface.h>
#include <cstdint>
#include <vector>
#include <cstring> // For memcpy

class WriteBuffer : public ::EmbeddedProto::WriteBufferInterface
{
public:
    // Constructors
    WriteBuffer(uint8_t* data, const uint32_t capacity)
        : data_(data), capacity_(capacity), index_(0) {}

    WriteBuffer(std::vector<uint8_t>& data)
        : data_(data.data()), capacity_(data.size()), index_(0) {}

    virtual ~WriteBuffer() {}

    // Delete all data in the buffer.
    void clear() override
    {
        index_ = 0;
    }

    // Obtain the total number of bytes currently stored in the buffer.
    uint32_t get_size() const override
    {
        return index_;
    }

    // Obtain the total number of bytes which can at most be stored in the buffer.
    uint32_t get_max_size() const override
    {
        return capacity_;
    }

    // Obtain the total number of bytes still available in the buffer.
    uint32_t get_available_size() const override
    {
        return capacity_ - index_;
    }

    // Push a single byte into the buffer.
    bool push(const uint8_t byte) override
    {
        if (index_ < capacity_)
        {
            data_[index_++] = byte;
            return true;
        }
        return false;
    }

    // Push an array of bytes into the buffer.
    bool push(const uint8_t* bytes, const uint32_t length) override
    {
        if ((index_ + length) <= capacity_)
        {
            memcpy(&data_[index_], bytes, length);
            index_ += length;
            return true;
        }
        return false;
    }

    // Reset the buffer index to the beginning
    void reset()
    {
        index_ = 0;
    }

    // Optional: Provide access to the buffer data
    const uint8_t* get_data() const
    {
        return data_;
    }

private:
    uint8_t* data_;     // Pointer to the buffer data
    uint32_t capacity_; // Total capacity of the buffer
    uint32_t index_;    // Current write index
};

#endif // WRITE_BUFFER_H
