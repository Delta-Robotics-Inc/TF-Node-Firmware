// ReadBuffer.h
#ifndef READ_BUFFER_H
#define READ_BUFFER_H

#include <ReadBufferInterface.h>
#include <cstdint>
#include <vector>
class ReadBuffer : public ::EmbeddedProto::ReadBufferInterface
{
public:
    // Constructors
    ReadBuffer(const uint8_t* data, const uint32_t size)
        : data_(data), max_size_(size), index_(0) {}

    ReadBuffer(const std::vector<uint8_t>& data)
        : data_(data.data()), max_size_(data.size()), index_(0) {}

    virtual ~ReadBuffer() {}

    // Obtain the total number of bytes currently stored in the buffer.
    uint32_t get_size() const override
    {
        return max_size_ - index_;
    }

    // Obtain the total number of bytes which can at most be stored in the buffer.
    uint32_t get_max_size() const override
    {
        return max_size_;
    }

    // Peek at the next byte without advancing the index.
    bool peek(uint8_t& byte) const override
    {
        if (index_ < max_size_)
        {
            byte = data_[index_];
            return true;
        }
        return false;
    }

    // Advance the internal read index by one when the buffer is not empty.
    bool advance() override
    {
        if (index_ < max_size_)
        {
            ++index_;
            return true;
        }
        return false;
    }

    // Advance the internal read index by n_bytes.
    bool advance(const uint32_t n_bytes) override
    {
        if ((index_ + n_bytes) <= max_size_)
        {
            index_ += n_bytes;
            return true;
        }
        else
        {
            // Advance as much as possible and set index to max_size_
            index_ = max_size_;
            return false;
        }
    }

    // Pop the next byte from the buffer.
    bool pop(uint8_t& byte) override
    {
        if (index_ < max_size_)
        {
            byte = data_[index_++];
            return true;
        }
        return false;
    }

    // Reset the buffer index to the beginning
    void reset()
    {
        index_ = 0;
    }

private:
    const uint8_t* data_; // Pointer to the buffer data
    uint32_t max_size_;   // Total size of the buffer
    uint32_t index_;      // Current read index
};

#endif // READ_BUFFER_H