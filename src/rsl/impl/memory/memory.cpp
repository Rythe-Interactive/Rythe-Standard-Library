#include "memory.hpp"

namespace rsl
{
	default_pmu_allocator allocator_context::defaultGlobalAllocator{};
	pmu_allocator* allocator_context::globalAllocator = &defaultGlobalAllocator;
	thread_local pmu_allocator* allocator_context::threadSpecificAllocator = globalAllocator;
} // namespace rsl
