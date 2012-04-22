#include "persistent-data-structure.h"

CAbstractPersistentDataStructure::CAbstractPersistentDataStructure():
	version_count(1),
	current_version(0)
{
}

CAbstractPersistentDataStructure &CAbstractPersistentDataStructure::SetVersion(size_t version)
{
	if (version >= version_count)
	{
		throw std::out_of_range("version out of bounds");
	}

	current_version = version;
	return *this;
}

size_t CAbstractPersistentDataStructure::GetVersionCount() const
{
	return version_count;
}

size_t CAbstractPersistentDataStructure::GetCurrentVersion() const
{
	return current_version;
}

size_t CAbstractPersistentDataStructure::AddVersion()
{
	return ++version_count;
}
