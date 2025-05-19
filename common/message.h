#ifndef MESSAGE
#define MESSAGE

#include <cstddef>

#include "message_type.h"

class Message {
public:
    virtual ~Message() = default;

    virtual void serialize(uint8_t* buffer) const = 0;
    virtual size_t serialized_size() const = 0;
    virtual MessageType type() const = 0;
};
#endif
