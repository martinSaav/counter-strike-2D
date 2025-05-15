#ifndef MESSAGE
#define MESSAGE

#include "message_type.h"
#include <cstddef>

class Message {

    virtual void serialize(uint8_t* buffer) const = 0;

    virtual size_t serialized_size() const = 0;
};
#endif