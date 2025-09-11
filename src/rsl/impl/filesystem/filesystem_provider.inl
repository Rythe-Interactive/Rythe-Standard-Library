#pragma once

namespace rsl::filesystem
{
    template <string_like Domain, string_like ... Domains>
    filesystem_provider::filesystem_provider(Domain&& domain, Domains&&... domains)
    {
        register_domain(domain);
        (register_domain(domains), ...);
    }
}
