#pragma once

#include <d2d1helper.h>

#define F35_NS			fxxxv
#define USING_F35_NS	using namespace F35_NS

namespace F35_NS
{
	struct ChartDataPoint: D2D_VECTOR_4F
	{
		D2D_VECTOR_4F raw;
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

		static void safeRelease(Interface **ppInterfaceToRelease)
		{
			if (*ppInterfaceToRelease != NULL)
			{
				(*ppInterfaceToRelease)->Release();
				(*ppInterfaceToRelease) = NULL;
			}
		}

	public:
		ResourceHolder() : pResource(NULL) { }
		ResourceHolder(Interface*p): pResource(p) { }
		ResourceHolder(ResourceHolder && rh) : ResourceHolder(rh.pResource) { }
		ResourceHolder & operator= (ResourceHolder && rh)
		{
			safeRelease(&this->pResource);
			this->pResource = rh.pResource; // move value
			rh.pResource = NULL;
			return *this;
		}

		ResourceHolder & operator= (Interface*p)
		{
			safeRelease(&this->pResource);
			this->pResource = p;
			return *this;
		}

		operator bool() { return pResource != NULL; }

		operator BOOL() { return pResource != NULL; }

		Interface * operator& () const { return pResource; }

		operator Interface *() { return pResource; }

		operator const Interface *() const { return pResource; }

		virtual ~ResourceHolder() { safeRelease(&pResource); }
	};


	static D2D1_RECT_F MakeRectRatios(D2D1_RECT_F src_rect, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);

	static D2D1_RECT_F MakeRectRatios(D2D1_RECT_F src_rect, D2D1_RECT_F ratio_rect);

	static D2D1_POINT_2F MakePointRatio(D2D1_POINT_2F pt0, D2D1_POINT_2F pt1, FLOAT ratio);

}
