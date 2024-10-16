#pragma once

#include <sstream>
#include <thread>

#define SPDLOG_HEADER_ONLY
#define FMT_CONSTEVAL

#include "spdlog_include.hpp"

#include "../math/color.hpp"
#include "../math/quaternion/quaternion.hpp"
#include "../math/vector/vector.hpp"
#include "../util/primitives.hpp"

namespace fmt
{
	struct gog
	{
		std::thread t;
	};

	// template <>
	// struct formatter<std::thread::id>
	//{
	//	constexpr const char* parse(format_parse_context& ctx)
	//	{
	//		auto it = ctx.begin(), end = ctx.end();

	//		if (!it)
	//			return nullptr;

	//		if (it != end && *it != '}')
	//			throw format_error("invalid format");
	//		return it++;
	//	}

	//	template <typename FormatContext>
	//	auto format(const std::thread::id& p, FormatContext& ctx) {
	//		std::ostringstream oss;
	//		oss << p;
	//		return format_to(ctx.out(), "{}", oss.str());
	//	}
	//};


	template <>
	struct formatter<rsl::math::vec2>
	{
		// Presentation format: 'f' - fixed, 'e' - exponential.
		char presentation = 'f';

		// Parses format specifications of the form ['f' | 'e'].
		constexpr const char* parse(format_parse_context& ctx)
		{
			// auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) // c++11
			// [ctx.begin(), ctx.end()) is a character range that contains a part of
			// the format string starting from the format specifications to be parsed,
			// e.g. in
			//
			//   fmt::format("{:f} - point of interest", point{1, 2});
			//
			// the range will contain "f} - point of interest". The formatter should
			// parse specifiers until '}' or the end of the range. In this example
			// the formatter should parse the 'f' specifier and return an iterator
			// pointing to '}'.

			// Parse the presentation format and store it in the formatter:
			auto it = ctx.begin(), end = ctx.end();

			if (!it)
				return nullptr;

			if (it != end && (*it == 'f' || *it == 'e'))
				presentation = *it++;

			// Check if reached the end of the range:
			if (it != end && *it != '}')
				throw format_error("invalid format");

			// Return an iterator past the end of the parsed range:
			return it;
		}

		// Formats the point p using the parsed format specification (presentation)
		// stored in this formatter.
		template <typename FormatContext>
		auto format(const rsl::math::vec2& p, FormatContext& ctx)
		{
			// auto format(const point &p, FormatContext &ctx) -> decltype(ctx.out()) // c++11
			// ctx.out() is an output iterator to write to.
			return format_to(
				ctx.out(),
				presentation == 'f' ? "({:.1f}, {:.1f})" : "({:.1e}, {:.1e})",
				p.x, p.y
			);
		}
	};

	template <>
	struct formatter<rsl::math::ivec2>
	{

		constexpr const char* parse(format_parse_context& ctx)
		{
			auto it = ctx.begin(), end = ctx.end();

			if (!it)
				return nullptr;

			if (it != end && *it != '}')
				throw format_error("invalid format");
			return it++;
		}

		template <typename FormatContext>
		auto format(const rsl::math::ivec2& p, FormatContext& ctx)
		{
			return format_to(
				ctx.out(),
				"({}, {})",
				p.x, p.y
			);
		}
	};

	template <>
	struct formatter<rsl::math::vec3>
	{
		char presentation = 'f';

		constexpr const char* parse(format_parse_context& ctx)
		{
			auto it = ctx.begin(), end = ctx.end();

			if (!it)
				return nullptr;

			if (it != end && (*it == 'f' || *it == 'e'))
				presentation = *it++;

			if (it != end && *it != '}')
				throw format_error("invalid format");

			return it;
		}

		template <typename FormatContext>
		auto format(const rsl::math::vec3& p, FormatContext& ctx)
		{
			return format_to(
				ctx.out(),
				presentation == 'f' ? "({:.1f}, {:.1f}, {:.1f})" : "({:.1e}, {:.1e}, {:.1e})",
				p.x, p.y, p.z
			);
		}
	};

	template <>
	struct formatter<rsl::math::ivec3>
	{

		constexpr const char* parse(format_parse_context& ctx)
		{
			auto it = ctx.begin(), end = ctx.end();

			if (!it)
				return nullptr;

			if (it != end && *it != '}')
				throw format_error("invalid format");
			return it++;
		}

		template <typename FormatContext>
		auto format(const rsl::math::ivec3& p, FormatContext& ctx)
		{
			return format_to(
				ctx.out(),
				"({}, {}, {})",
				p.x, p.y, p.z
			);
		}
	};

	template <>
	struct formatter<rsl::math::vec4>
	{
		char presentation = 'f';

		constexpr const char* parse(format_parse_context& ctx)
		{
			auto it = ctx.begin(), end = ctx.end();

			if (!it)
				return nullptr;

			if (it != end && (*it == 'f' || *it == 'e'))
				presentation = *it++;

			if (it != end && *it != '}')
				throw format_error("invalid format");

			return it;
		}

		template <typename FormatContext>
		auto format(const rsl::math::vec4& p, FormatContext& ctx)
		{
			return format_to(
				ctx.out(),
				presentation == 'f' ? "({:.1f}, {:.1f}, {:.1f}, {:.1f})" : "({:.1e}, {:.1e}, {:.1e}, {:.1e})",
				p.x, p.y, p.z, p.w
			);
		}
	};

	template <>
	struct formatter<rsl::math::mat4> : fmt::formatter<std::string>
	{
		char presentation = 'f';

		constexpr const char* parse(format_parse_context& ctx)
		{
			auto it = ctx.begin(), end = ctx.end();

			if (!it)
				return nullptr;

			if (it != end && (*it == 'f' || *it == 'e'))
				presentation = *it++;

			if (it != end && *it != '}')
				throw format_error("invalid format");

			return it;
		}

		template <typename FormatContext>
		auto format(const rsl::math::mat4& p, FormatContext& ctx) const -> decltype(ctx.out())
		{
			return format_to(
				ctx.out(),
				presentation == 'f' ? "({:.1f}, {:.1f}, {:.1f}, {:.1f})\n"
									  "({:.1f}, {:.1f}, {:.1f}, {:.1f})\n"
									  "({:.1f}, {:.1f}, {:.1f}, {:.1f})\n"
									  "({:.1f}, {:.1f}, {:.1f}, {:.1f})"
									: "({:.1e}, {:.1e}, {:.1e}, {:.1e})\n"
									  "({:.1e}, {:.1e}, {:.1e}, {:.1e})\n"
									  "({:.1e}, {:.1e}, {:.1e}, {:.1e})\n"
									  "({:.1e}, {:.1e}, {:.1e}, {:.1e})",
				p[0][0], p[0][1], p[0][2], p[0][3],
				p[1][0], p[1][1], p[1][2], p[1][3],
				p[2][0], p[2][1], p[2][2], p[2][3],
				p[3][0], p[3][1], p[3][2], p[3][3]
			);
		}
	};

	template <>
	struct formatter<rsl::math::color>
	{
		char presentation = 'f';

		constexpr const char* parse(format_parse_context& ctx)
		{
			auto it = ctx.begin(), end = ctx.end();

			if (!it)
				return nullptr;

			if (it != end && (*it == 'f' || *it == 'e'))
				presentation = *it++;

			if (it != end && *it != '}')
				throw format_error("invalid format");

			return it;
		}

		template <typename FormatContext>
		auto format(const rsl::math::color& p, FormatContext& ctx)
		{
			return format_to(
				ctx.out(),
				presentation == 'f' ? "({:.1f}, {:.1f}, {:.1f}, {:.1f})" : "({:.1e}, {:.1e}, {:.1e}, {:.1e})",
				p.r, p.g, p.b, p.a
			);
		}
	};

	template <>
	struct formatter<rsl::math::quat>
	{
		char presentation = 'f';

		constexpr const char* parse(format_parse_context& ctx)
		{
			auto it = ctx.begin(), end = ctx.end();

			if (!it)
				return nullptr;

			if (it != end && (*it == 'f' || *it == 'e'))
				presentation = *it++;

			if (it != end && *it != '}')
				throw format_error("invalid format");

			return it;
		}

		template <typename FormatContext>
		auto format(const rsl::math::quat& p, FormatContext& ctx)
		{
			return format_to(
				ctx.out(),
				presentation == 'f' ? "(({:.1f}, {:.1f}, {:.1f}),r: {:.1f})" : "(({:.1e}, {:.1e}, {:.1e}),r: {:.1e})",
				p.i, p.j, p.k, p.w
			);
		}
	};

	// template <>
	// struct formatter<std::string> {
	//	char presentation = 'f';

	//	constexpr const char* parse(format_parse_context& ctx) {
	//		auto it = ctx.begin(), end = ctx.end();

	//		if (!it)
	//			return nullptr;

	//		if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;

	//		if (it != end && *it != '}')
	//			throw format_error("invalid format");

	//		return it;
	//	}

	//	template <typename FormatContext>
	//	auto format(const std::string& p, FormatContext& ctx) {
	//		return format_to(ctx.out(), "{}", p);
	//	}
	//};
} // namespace fmt

namespace rsl::log
{
	using logger_ptr = std::shared_ptr<spdlog::logger>;

	struct impl
	{
		rsl::cstring logFile = "logs/rythe.log";
		logger_ptr logger;
		logger_ptr fileLogger;
		logger_ptr consoleLogger = spdlog::stdout_color_mt("console-logger");
		logger_ptr undecoratedLogger = spdlog::stdout_color_mt("undecorated-logger");
		// async::rw_spinlock threadNamesLock;
		std::unordered_map<std::thread::id, std::string> threadNames;

		static impl& get()
		{
			static impl m_instance;
			return m_instance;
		}
	};

	const static inline std::chrono::time_point<std::chrono::high_resolution_clock> genesis = std::chrono::high_resolution_clock::now();

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
			const auto seconds = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(time_since_genesis).count();

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
			// std::string thread_ident;
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


	inline static void initLogger(logger_ptr& logger, [[maybe_unused]] std::string_view loggerName = "")
	{
		auto f = std::make_unique<spdlog::pattern_formatter>();

		f->add_flag<thread_name_formatter_flag>('f');
		f->add_flag<genesis_formatter_flag>('*');

		f->set_pattern("T+ %* [%^%=7l%$] [%=13!f] : %v");

		logger->set_formatter(std::move(f));
	}

	inline void setDefaultLogger(const logger_ptr& newLogger)
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

	constexpr severity severity_trace = severity::trace;
	constexpr severity severity_debug = severity::debug;
	constexpr severity severity_info = severity::info;
	constexpr severity severity_warn = severity::warn;
	constexpr severity severity_error = severity::error;
	constexpr severity severity_fatal = severity::fatal;

	constexpr spdlog::level::level_enum args2spdlog(severity s)
	{
		switch (s)
		{
			case severity_trace: return spdlog::level::trace;
			case severity_debug: return spdlog::level::debug;
			case severity_info: return spdlog::level::info;
			case severity_warn: return spdlog::level::warn;
			case severity_error: return spdlog::level::err;
			case severity_fatal: return spdlog::level::critical;
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
	void println(severity s, const FormatString& format, Args&&... a)
	{
		impl::get().logger->log(args2spdlog(s), format, std::forward<Args>(a)...);
	}

	/** @brief same as println but uses the undecorated logger */
	template <class... Args, class FormatString>
	void undecoratedln(severity s, const FormatString& format, Args&&... a)
	{
		// I need to get back to this
		impl::get().undecoratedLogger->log(args2spdlog(s), format, std::forward<Args>(a)...);
	}

	/** @brief prints a log line, using the specified `severity`
	 *  @param level selects the severity level you are interested in
	 */
	inline void filter(severity level)
	{
		auto& inst = impl::get();
		inst.logger->set_level(args2spdlog(level));
		inst.undecoratedLogger->set_level(args2spdlog(level));
	}

	/** @brief same as println but with severity = trace */
	template <class... Args, class FormatString>
	void undecoratedTrace(const FormatString& format, Args&&... a)
	{
		undecoratedln(severity::trace, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = debug */
	template <class... Args, class FormatString>
	void undecoratedDebug(const FormatString& format, Args&&... a)
	{
		undecoratedln(severity::debug, format, std::forward<Args>(a)...);
	}

	/** @brief same as undecoratedln but with severity = info */
	template <class... Args, class FormatString>
	void undecoratedInfo(const FormatString& format, Args&&... a)
	{
		undecoratedln(severity::info, format, std::forward<Args>(a)...);
	}

	/** @brief same as undecoratedln but with severity = warn */
	template <class... Args, class FormatString>
	void undecoratedWarn(const FormatString& format, Args&&... a)
	{
		undecoratedln(severity::warn, format, std::forward<Args>(a)...);
	}

	/** @brief same as undecoratedln but with severity = error */
	template <class... Args, class FormatString>
	void undecoratedError(const FormatString& format, Args&&... a)
	{
		undecoratedln(severity::error, format, std::forward<Args>(a)...);
	}

	/** @brief same as undecoratedln but with severity = fatal */
	template <class... Args, class FormatString>
	void undecoratedFatal(const FormatString& format, Args&&... a)
	{
		undecoratedln(severity::fatal, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = trace */
	template <class... Args, class FormatString>
	void trace(const FormatString& format, Args&&... a)
	{
		println(severity::trace, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = debug */
	template <class... Args, class FormatString>
	void debug(const FormatString& format, Args&&... a)
	{
		println(severity::debug, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = info */
	template <class... Args, class FormatString>
	void info(const FormatString& format, Args&&... a)
	{
		println(severity::info, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = warn */
	template <class... Args, class FormatString>
	void warn(const FormatString& format, Args&&... a)
	{
		println(severity::warn, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = error */
	template <class... Args, class FormatString>
	void error(const FormatString& format, Args&&... a)
	{
		println(severity::error, format, std::forward<Args>(a)...);
	}

	/** @brief same as println but with severity = fatal */
	template <class... Args, class FormatString>
	void fatal(const FormatString& format, Args&&... a)
	{
		println(severity::fatal, format, std::forward<Args>(a)...);
	}

	namespace detail
	{
		inline byte _setup_impl()
		{
			auto& inst = impl::get();

			auto f = std::make_unique<spdlog::pattern_formatter>();
			f->set_pattern("%v");
			inst.undecoratedLogger->set_formatter(std::move(f));

			inst.fileLogger = spdlog::rotating_logger_mt(inst.logFile, inst.logFile, 1'048'576, 5);
			initLogger(inst.consoleLogger);
			initLogger(inst.fileLogger);

#if defined(RYTHE_KEEP_CONSOLE) || defined(RYTHE_DEBUG)
			inst.logger = inst.consoleLogger;
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
	} // namespace detail

	/** @brief sets up logging (do not call, invoked by engine) */
	inline void setup()
	{
		[[maybe_unused]] static auto v = detail::_setup_impl();
	}
} // namespace rsl::log
#undef logger
