#include "logger.hpp"

#include "time/time_point.hpp"
#include "time/stopwatch.hpp"

#include "severity.hpp"

#include "logging.hpp"
#include "containers/string.hpp"

namespace rsl
{
	namespace
	{
		class logger_name_formatter : public spdlog::custom_flag_formatter
		{
		public:
			void format(
				const spdlog::details::log_msg& msg, [[maybe_unused]] const std::tm& tmTime, spdlog::memory_buf_t& dest
			) override
			{
				// append to data
				dest.append(msg.logger_name.begin(), msg.logger_name.end());
			}

			// generates a new formatter flag
			[[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override
			{
				return spdlog::details::make_unique<logger_name_formatter>();
			}
		};

		class genesis_formatter_flag : public spdlog::custom_flag_formatter
		{
		public:
			void format(
				[[maybe_unused]] const spdlog::details::log_msg& msg, [[maybe_unused]] const std::tm& tmTime,
				spdlog::memory_buf_t& dest
			) override
			{
				// get seconds since engine start
				const auto now = time::main_clock.current_point();
				const auto timeSinceGenesis = now - time::genesis;
				const auto seconds = timeSinceGenesis.seconds();

				// convert to "--s.ms---"
				const auto str = std::to_string(seconds);

				// append to data
				dest.append(str.data(), str.data() + str.size());
			}

			// generates a new formatter flag
			std::unique_ptr<custom_flag_formatter> clone() const override
			{
				return spdlog::details::make_unique<genesis_formatter_flag>();
			}
		};

		class thread_name_formatter_flag : public spdlog::custom_flag_formatter
		{
			void format(
				[[maybe_unused]] const spdlog::details::log_msg& msg, [[maybe_unused]] const std::tm& tmTime,
				spdlog::memory_buf_t& dest
			) override
			{
				thread_local static std::string* threadIdent;

				if (!threadIdent)
				{
					auto& inst = logging_context::get();
					// async::readonly_guard guard(inst.threadNamesLock);

					if (inst.threadNames.count(std::this_thread::get_id()))
					{
						threadIdent = &inst.threadNames.at(std::this_thread::get_id());
					}
					else
					{
						std::ostringstream oss;
						oss << std::this_thread::get_id();
						{
							// async::readwrite_guard wguard(inst.threadNamesLock);
							threadIdent = &inst.threadNames[std::this_thread::get_id()];
						}
						*threadIdent = oss.str();
					}
				}

				dest.append(threadIdent->data(), threadIdent->data() + threadIdent->size());
			}

			std::unique_ptr<custom_flag_formatter> clone() const override
			{
				return spdlog::details::make_unique<thread_name_formatter_flag>();
			}
		};
	}

	struct logger::impl : public spdlog::logger
	{
		dynamic_string name;
		log::severity logLevel;

		void log(log::severity s, const source_location& location, string_view format, fmt::format_args args) noexcept
		{
			spdlog::level::level_enum lvl = rsl::log::internal::rythe_to_spdlog(s);
			bool log_enabled = should_log(lvl);
			bool traceback_enabled = tracer_.enabled();
			if (!log_enabled && !traceback_enabled)
			{
				return;
			}

			spdlog::memory_buf_t buf;
			fmt::vformat_to(fmt::appender(buf), fmt::string_view(format.data(), format.size()), args);

			spdlog::details::log_msg log_msg(
				spdlog::source_loc(location.file_name(), static_cast<int>(location.line()), location.file_name()),
				name_,
				lvl,
				spdlog::string_view_t(buf.data(), buf.size()));
			log_it_(log_msg, log_enabled, traceback_enabled);
		}
	};

	void logger::log(log::severity s, format_string format, fmt::format_args args) noexcept
	{
		m_impl->log(s, format.srcLoc, format.str, args);
	}
} // namespace rsl
