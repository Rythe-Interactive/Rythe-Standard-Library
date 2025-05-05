#include "allocator_storage.hpp"

#include "allocator_context.hpp"

namespace rsl
{
	allocator_storage<polymorphic_allocator>::allocator_storage() noexcept
		: value(allocator_context::threadSpecificAllocator)
	{
	}
} // namespace rsl
