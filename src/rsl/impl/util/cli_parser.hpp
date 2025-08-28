#pragma once

#include <argh.h>

namespace rsl
{
    // TODO(Glyn): make a version of argh that uses rsl instead

    // Based on Argh: https://github.com/adishavit/argh
    // Copyright (c) 2016, Adi Shavit
    // All rights reserved.

    using cli_parser_string_stream = argh::string_stream;
    using cli_parser = argh::parser;
}
