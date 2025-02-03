#pragma once

#include <sstream>
#include <thread>

#include "spdlog_include.hpp"

namespace rsl::log
{
	using logger_ptr = std::shared_ptr<spdlog::logger>;

	struct impl
	{
		rsl::cstring logFile = "logs/rythe.log";
		//logger_ptr logger;
		logger_ptr fileLogger;
		logger_ptr logger = spdlog::stdout_color_mt("console-logger");
		logger_ptr undecoratedLogger = spdlog::stdout_color_mt("undecorated-logger");
		// async::rw_spinlock threadNamesLock;
		std::unordered_map<std::thread::id, std::string> threadNames;

		static impl& get()
		{
			static impl m_instance;
			return m_instance;
		}
	};

	const static inline std::chrono::time_point<std::chrono::high_resolution_clock> genesis =
		std::chrono::high_resolution_clock::now();

	class genesis_formatter_flag : public spdlog::custom_flag_formatter
	{
	public:
		void format(
			[[maybe_unused]] const spdlog::details::log_msg& msg, [[maybe_unused]] const std::tm& tm_time,
			spdlog::memory_buf_t& dest
		) override
		{
			// get seconds since engine start
			const auto now = std::chrono::high_resolution_clock::now();
			const auto time_since_genesis = now - genesis;
			const auto seconds =
				std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(time_since_genesis).count();

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
			[[maybe_unused]] const spdlog::details::log_msg& msg, [[maybe_unused]] const std::tm& tm_time,
			spdlog::memory_buf_t& dest
		) override
		{
			thread_local static std::string* thread_ident;

			if (!thread_ident)
			{
				auto& inst = impl::get();
				// async::readonly_guard guard(inst.threadNamesLock);

				if (inst.threadNames.count(std::this_thread::get_id()))
				{
					thread_ident = &inst.threadNames.at(std::this_thread::get_id());
				}
				else
				{
					std::ostringstream oss;
					oss << std::this_thread::get_id();
					{
						// async::readwrite_guard wguard(inst.threadNamesLock);
						thread_ident = &inst.threadNames[std::this_thread::get_id()];
					}
					*thread_ident = oss.str();

					// NOTE(algo-ryth-mix): this conversion is not portable
					// thread_ident = std::to_string(rythe::core::force_value_cast<uint>(std::this_thread::get_id()));
				}
			}

			dest.append(thread_ident->data(), thread_ident->data() + thread_ident->size());
		}
		std::unique_ptr<custom_flag_formatter> clone() const override
		{
			return spdlog::details::make_unique<thread_name_formatter_flag>();
		}
	};


	[[rythe_always_inline]] static void
	initLogger(logger_ptr& logger, [[maybe_unused]] std::string_view loggerName = "")
	{
		auto f = std::make_unique<spdlog::pattern_formatter>();

		f->add_flag<thread_name_formatter_flag>('f');
		f->add_flag<genesis_formatter_flag>('*');

		f->set_pattern("T+ %* [%^%=7l%$] [%=13!f] : %v");

		logger->set_formatter(std::move(f));
	}

	[[maybe_unused]] [[rythe_always_inline]] static void setDefaultLogger(const logger_ptr& newLogger)
	{
		impl::get().logger = newLogger;
	}

	enum struct severity
	{
		trace, // lowest severity
		debug,
		info,
		warn,
		error,
		fatal // highest severity
	};

	[[nodiscard]] [[rythe_always_inline]] constexpr static spdlog::level::level_enum args2spdlog(severity s)
	{
		switch (s)
		{
			case severity::trace: return spdlog::level::trace;
			case severity::debug: return spdlog::level::debug;
			case severity::info: return spdlog::level::info;
			case severity::warn: return spdlog::level::warn;
			case severity::error: return spdlog::level::err;
			case severity::fatal: return spdlog::level::critical;
		}
		return spdlog::level::err;
	}

	/** @brief prints a log line, using the specified `severity`
	 *  @param s The severity you wan't to report this log with
	 *  @param format The format string you want to print
	 *  @param a The arguments to the format string
	 *  @note This uses fmt lib style syntax check
	 *         https://fmt.dev/latest/syntax.html
	 */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void println(severity s, const FormatString& format, Args&&... a)
	{
		impl::get().logger->log(args2spdlog(s), format, std::forward<Args>(a)...);
	}

	/** @brief same as println but uses the undecorated logger */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void undecoratedln(severity s, const FormatString& format, Args&&... a)
	{
		// I need to get back to this
		impl::get().undecoratedLogger->log(args2spdlog(s), format, std::forward<Args>(a)...);
	}

	/** @brief prints a log line, using the specified `severity`
	 *  @param level selects the severity level you are interested in
	 */
	[[rythe_always_inline]] static void filter(severity level)
	{
		auto& inst = impl::get();
		inst.logger->set_level(args2spdlog(level));
		inst.undecoratedLogger->set_level(args2spdlog(level));
	}

	/** @brief same as println but with severity = trace */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void undecoratedTrace(const FormatString& format, Args&&... a)
	{
		undecoratedln(severity::trace, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = debug */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void undecoratedDebug(const FormatString& format, Args&&... a)
	{
		undecoratedln(severity::debug, format, std::forward<Args>(a)...);
	}

	/** @brief same as undecoratedln but with severity = info */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void undecoratedInfo(const FormatString& format, Args&&... a)
	{
		undecoratedln(severity::info, format, std::forward<Args>(a)...);
	}

	/** @brief same as undecoratedln but with severity = warn */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void undecoratedWarn(const FormatString& format, Args&&... a)
	{
		undecoratedln(severity::warn, format, std::forward<Args>(a)...);
	}

	/** @brief same as undecoratedln but with severity = error */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void undecoratedError(const FormatString& format, Args&&... a)
	{
		undecoratedln(severity::error, format, std::forward<Args>(a)...);
	}

	/** @brief same as undecoratedln but with severity = fatal */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void undecoratedFatal(const FormatString& format, Args&&... a)
	{
		undecoratedln(severity::fatal, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = trace */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void trace(const FormatString& format, Args&&... a)
	{
		println(severity::trace, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = debug */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void debug(const FormatString& format, Args&&... a)
	{
		println(severity::debug, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = info */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void info(const FormatString& format, Args&&... a)
	{
		println(severity::info, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = warn */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void warn(const FormatString& format, Args&&... a)
	{
		println(severity::warn, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = error */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void error(const FormatString& format, Args&&... a)
	{
		println(severity::error, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = fatal */
	template <class... Args, class FormatString>
	[[rythe_always_inline]] void fatal(const FormatString& format, Args&&... a)
	{
		println(severity::fatal, format, std::forward<Args>(a)...);
	}

	namespace internal
	{
		inline byte _setup_impl()
		{
			auto& inst = impl::get();

			auto f = std::make_unique<spdlog::pattern_formatter>();
			f->set_pattern("%^%v%$");
			inst.undecoratedLogger->set_formatter(std::move(f));

			inst.fileLogger = spdlog::rotating_logger_mt(inst.logFile, inst.logFile, 1'048'576, 5);
			initLogger(inst.logger);
			initLogger(inst.fileLogger);

#if defined(RYTHE_KEEP_CONSOLE) || defined(RYTHE_DEBUG)
			//inst.logger = inst.consoleLogger;
#else
			inst.logger = inst.fileLogger;
#endif

#if defined(RYTHE_LOG_TRACE)
			filter(severity::trace);
#elif defined(RYTHE_LOG_DEBUG)
			filter(severity::debug);
#elif defined(RYTHE_LOG_INFO)
			filter(severity::info);
#elif defined(RYTHE_LOG_WARN)
			filter(severity::warn);
#elif defined(RYTHE_LOG_ERROR)
			filter(severity::error);
#elif defined(RYTHE_LOG_FATAL)
			filter(severity::fatal);
#elif defined(RYTHE_DEBUG)
			filter(severity::debug);
#else
			filter(severity::info);
#endif

			undecoratedInfo("== Initializing Logger");
			return 0;
		}
	} // namespace internal

	/** @brief sets up logging (do not call, invoked by engine) */
	inline void setup()
	{
		[[maybe_unused]] static auto v = internal::_setup_impl();
	}
} // namespace rsl::log
#undef logger
