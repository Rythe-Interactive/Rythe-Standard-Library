#pragma once

#include "../containers/string_view.hpp"
#include "../time/time_point.hpp"
#include "../threading/thread_id.hpp"

#include "severity.hpp"

namespace rsl::log
{
	struct message
	{
		string_view loggerName;
		thread_id threadId;

		time::point32 timestamp;
		log::severity severity;
		string_view msg;
	};
}
