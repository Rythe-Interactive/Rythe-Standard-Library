#include "logger.hpp"

#include "time/time_point.hpp"
#include "time/stopwatch.hpp"

#include "logging.hpp"

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
} // namespace rsl
