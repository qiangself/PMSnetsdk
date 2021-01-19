#pragma once

#if _MSC_VER >= 1800
#include <mutex>
#else

#include <windows.h>

namespace std {

template <class _Mutex_T>
class lock_guard {
public:
	explicit lock_guard( _Mutex_T &mtx ) : __MyMutex( mtx ) { __MyMutex.lock(); }
	~lock_guard() { __MyMutex.unlock(); } // class with destructor that unlocks mutex
private:
	lock_guard( const lock_guard& );
	lock_guard& operator=( const lock_guard& );
	_Mutex_T &__MyMutex;
};

class recursive_mutex {
public:
	recursive_mutex() { InitializeCriticalSection( &__critical_section_obj ); }
	~recursive_mutex() { DeleteCriticalSection( &__critical_section_obj ); }
	inline void lock() { EnterCriticalSection( &__critical_section_obj ); }
	inline void unlock() { LeaveCriticalSection( &__critical_section_obj ); }
	inline CRITICAL_SECTION* get() { return &__critical_section_obj; }
private:
	CRITICAL_SECTION	__critical_section_obj;
	recursive_mutex(const recursive_mutex &rf);
	recursive_mutex &operator=(const recursive_mutex &rf);
};

class mutex {
public:
	mutex() { __mutex_obj = ::CreateMutex(NULL, FALSE,  NULL); }
	~mutex() { if (__mutex_obj) ::CloseHandle(__mutex_obj); }
	inline void lock() { if (__mutex_obj ) ::WaitForSingleObject( __mutex_obj, INFINITE ); }
	inline void unlock() { if (__mutex_obj) ::ReleaseMutex( __mutex_obj ); }
	inline HANDLE *get () { return &__mutex_obj; }
private:
	HANDLE	__mutex_obj;
};

} // namespace base

#endif