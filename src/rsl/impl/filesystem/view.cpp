#include "view.hpp"

namespace rsl::filesystem
{
    file_traits view::file_info() const {}
    filesystem_traits view::filesystem_info() const {}
    result<dynamic_array<view>> view::ls() const {}
    result<byte_view> view::read() {}
    result<void> view::write(byte_view data) {}
    result<void> view::append(byte_view data) {}
    result<void> view::flush() {}
}
