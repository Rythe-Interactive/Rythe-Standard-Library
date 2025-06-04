#pragma once

#include "../memory/unique_object.hpp"

#include "severity.hpp"

namespace rsl::log
{
	class sink
	{
	public:

		virtual void log(const  &msg) = 0;
		virtual void flush() = 0;

        void filter(severity s) noexcept;
		severity filter_severity() const noexcept;
		bool should_log(severity s) const noexcept;

	private:
		struct impl;
		unique_object<impl> m_impl;

	};
}
