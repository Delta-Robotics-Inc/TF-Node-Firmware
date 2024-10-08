// ReadBuffer.h
#ifndef READ_BUFFER_H
#define READ_BUFFER_H

#include <ReadBufferInterface.h>
#include <cstdint>
#include <vector>

class ReadBuffer : public ::EmbeddedProto::ReadBufferInterface
{
public:
    ReadBuffer(const uint8_t* data, const uint32_t size)
        : data_(data), size_(size), index_(0) {}

    ReadBuffer(const std::vector<uint8_t>& data)
        : data_(data.data()), size_(data.size()), index_(0) {}

    virtual ~ReadBuffer() {}

    // Get the next byte from the buffer
    bool get(uint8_t& byte) override
    {
        if (index_ < size_)
        {
            byte = data_[index_++];
            return true;
        }
        return false;
    }

    // Peek at the next byte in the buffer without advancing the index
    bool peek(uint8_t& byte) override
    {
        if (index_ < size_)
        {
            byte = data_[index_];
            return true;
        }
        return false;
    }

    // Advance the buffer index by n bytes
    bool advance(const uint32_t n) override
    {
        if ((index_ + n) <= size_)
        {
            index_ += n;
            return true;
        }
        return false;
    }

    // Get the total number of bytes left in the buffer
    uint32_t get_size() const override
    {
        return size_ - index_;
    }

    // Reset the buffer index to the beginning
    void reset()
    {
        index_ = 0;
    }

private:
    const uint8_t* data_;
    uint32_t size_;
    uint32_t index_;
};

#endif // READ_BUFFER_H
