#pragma once
#include "../sink.hpp"

namespace rsl::log
{
    class file_sink : public sink
    {
    public:
        using sink::sink;
        file_sink(string_view fileName, size_type maxFileCount = npos) noexcept;

        void set_file_name(string_view fileName) noexcept;
        void set_max_file_count(size_type maxFileCount) noexcept;

        void log([[maybe_unused]] formatter& formatter, [[maybe_unused]] const message& msg) override{}
        void flush() override {}

    private:
        string_view m_fileName = "logs/rythe.log";
        size_type m_maxFileCount = npos;
    };
}
