#pragma once

namespace rythe {
    template<typename Func>
    constexpr bool is_function_ptr_v = std::is_empty_v<Func> || std::is_pointer_v<Func>;
}
