#pragma once
#include "../sink.hpp"

#include "../../filesystem/view.hpp"

namespace rsl::log
{
    class console_sink : public sink
    {
    public:
        void log(formatter& formatter, const message& msg) override;
        void flush() override;

    private:
        fs::view m_stdout = "stdout://"_fsview;
    };
}
