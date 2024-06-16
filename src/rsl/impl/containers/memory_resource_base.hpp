#pragma once

#include "../util/primitives.hpp"
#include "../util/type_util.hpp"

namespace rsl
{
	template <
		typename T, typename Alloc, bool AllowRawDataAccess = false, typename AllocMessage = void, typename ConstructMessage = void,
		typename FreeMessage = void,
		typename DestroyMessage = void>
	struct memory_resource_base
	{
		using value_type = T;
		using allocator_type = Alloc;

		virtual ~memory_resource_base() = default;

		[[nodiscard]] constexpr value_type* data() noexcept
			requires AllowRawDataAccess;

		[[nodiscard]] constexpr const value_type* data() const noexcept
			requires AllowRawDataAccess;

	protected:
		virtual void alloc(optional_param_t<AllocMessage, ref_signal>) = 0;
		virtual void construct(optional_param_t<AllocMessage, ref_signal>) = 0;
		virtual void free(optional_param_t<AllocMessage, ref_signal>) = 0;
		virtual void destroy(optional_param_t<AllocMessage, ref_signal>) = 0;

		Alloc m_alloc;
		value_type* m_ptr;
	};
} // namespace rsl

#include "memory_resource_base.inl"
