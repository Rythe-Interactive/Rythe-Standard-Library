#pragma once

#include "fmt_include.hpp"

#define SPDLOG_HEADER_ONLY
#define SPDLOG_NO_EXCEPTIONS
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
