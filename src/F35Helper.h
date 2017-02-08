#pragma once

#define F35_NS			fxxxv
#define USING_F35_NS	using namespace F35_NS

namespace F35_NS
{
	struct ChartDataPoint
	{
		FLOAT x, y, z, w;
		struct
		{
			FLOAT x, y, z, w;
		} raw;
	};

	class NonCopyable
	{
	protected:
		NonCopyable () {}
		virtual ~NonCopyable () {}
	private: 
		NonCopyable (const NonCopyable &) = delete;
		NonCopyable(NonCopyable &&) = delete;
		NonCopyable & operator= (const NonCopyable &) = delete;
		NonCopyable & operator= (NonCopyable &&) = delete;
	};

	class Lockable
	{
	private:
		CRITICAL_SECTION cs;
	protected:
		Lockable () { ::InitializeCriticalSection(&cs); }
		virtual ~Lockable ()
		{
			::EnterCriticalSection(&cs); 
			::DeleteCriticalSection(&cs);
		}
		void Lock(void) {::EnterCriticalSection(&cs);}
		void Unlock(void) {::LeaveCriticalSection(&cs);}
		BOOL TryLock(void) { return ::TryEnterCriticalSection(&cs); }
	};

	template<class Interface>
	class ResourceHolder
	{
	private:
		Interface *pResource;
		ResourceHolder(const ResourceHolder &) = delete;
		ResourceHolder & operator= (const ResourceHolder &) = delete;

		static void SafeRelease(Interface **ppInterfaceToRelease)
		{
			if (*ppInterfaceToRelease != NULL)
			{
				(*ppInterfaceToRelease)->Release();
				(*ppInterfaceToRelease) = NULL;
			}
		}

	public:
		ResourceHolder(Interface*p = NULL): pResource(p) { }
		ResourceHolder(ResourceHolder && rh) : ResourceHolder(rh.pResource) { }
		ResourceHolder & operator= (ResourceHolder && rh)
		{
			SafeRelease(&this->pResource);
			this->pResource = rh.pResource; // move value
			rh.pResource = NULL;
		}

		operator Interface *() { return pResource; }

		virtual ~ResourceHolder() { SafeRelease(&pResource); }
	};

}
