#pragma once

#include <stdexcept>

class IPersistentDataStructure
{

	public:
		virtual ~IPersistentDataStructure() {}

		virtual IPersistentDataStructure &SetVersion(size_t) = 0;
		virtual size_t GetVersionCount() const = 0;
		virtual size_t GetCurrentVersion() const = 0;

};

class CAbstractPersistentDataStructure:
	public IPersistentDataStructure
{

	public:
		CAbstractPersistentDataStructure();

		virtual CAbstractPersistentDataStructure &SetVersion(size_t version);
		virtual size_t GetVersionCount() const;
		virtual size_t GetCurrentVersion() const;

	protected:
		size_t AddVersion();

	private:
		size_t version_count;
		size_t current_version;

};
