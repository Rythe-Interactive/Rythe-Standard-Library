#pragma once
#include "../sink.hpp"

namespace rsl::log
{
    class void_sink : public sink
    {
    public:
        void log([[maybe_unused]] formatter& formatter, [[maybe_unused]] const message& msg) override {}
        void flush() override {}
    };
}
