#pragma once

namespace rsl::filesystem
{
    template <derived_from<filesystem_provider> ProviderType, typename... Args>
    void filesystem_registry::register_provider(Args&&... args)
    {
        register_provider(temporary_object<ProviderType>::create_in_place(rsl::forward<Args>(args)...));
    }
}
