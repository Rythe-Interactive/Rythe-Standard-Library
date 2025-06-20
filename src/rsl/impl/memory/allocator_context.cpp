#include "allocator_context.hpp"

namespace rsl
{
	default_pmu_allocator allocator_context::defaultGlobalAllocator{};
	pmu_allocator* allocator_context::globalAllocator = &defaultGlobalAllocator;
	default_pmu_allocator allocator_context::defaultThreadSpecificAllocator{};
	thread_local pmu_allocator* allocator_context::threadSpecificAllocator = &defaultThreadSpecificAllocator;
} // namespace rsl
