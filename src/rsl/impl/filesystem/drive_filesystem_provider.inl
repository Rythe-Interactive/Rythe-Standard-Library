﻿#pragma once

namespace rsl::filesystem
{
    template <string_like Domain, string_like ... Domains>
    drive_filesystem_provider::drive_filesystem_provider(const string_view rootPath, Domain&& domain, Domains&&... domains)
        : filesystem_provider(rsl::forward<Domain>(domain), rsl::forward<Domains>(domains)...),
          m_rootPath(dynamic_string::from_view(rootPath)) {}
}
