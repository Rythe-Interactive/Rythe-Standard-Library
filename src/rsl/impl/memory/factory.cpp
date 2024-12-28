#include "factory.hpp"

namespace rsl
{
	void* type_erased_factory::construct(void* ptr, size_type count) const
	{
		return m_constructFunc ? (m_constructFunc)(ptr, count) : nullptr;
	}

	void* type_erased_factory::move(void* dst, void* src, size_type count) const
	{
		return m_moveFunc ? (m_moveFunc)(dst, src, count) : nullptr;
	}

	void type_erased_factory::destroy(void* ptr, size_type count) const noexcept
	{
		rsl_assert_frequent(m_destroyFunc);
		(m_destroyFunc)(ptr, count);
	}

	size_type type_erased_factory::typeSize() const noexcept
	{
		return m_typeSize;
	}

	bool type_erased_factory::trivialCopy() const noexcept
	{
		return m_triviallyCopyable;
	}
} // namespace rsl
