#pragma once
#include "../containers/views.hpp"
#include "../memory/unique_object.hpp"
#include "../util/error_handling.hpp"

#include "filesystem_provider.hpp"

namespace rsl::filesystem
{
    class file_solution;

    class filesystem_registry
    {
    public:
        filesystem_registry() = default;
        virtual ~filesystem_registry() = default;

        template<derived_from<filesystem_provider> ProviderType, typename... Args>
        void register_provider(Args&&... args);
        void register_provider(temporary_object<filesystem_provider>&& provider);

        result<file_solution*> find_solution(string_view path, bool ignoreMultipleSolutions = false);

    protected:
        dynamic_map<string_view, dynamic_array<filesystem_provider*>> m_domainMap;
        dynamic_array<unique_object<filesystem_provider>> m_providers;
    };

    DECLARE_SINGLETON(filesystem_registry)
}

#include "filesystem_registry.inl"
