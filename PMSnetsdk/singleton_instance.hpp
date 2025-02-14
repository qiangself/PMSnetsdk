#ifndef _SINGLE_INSTAN_H_
#define _SINGLE_INSTAN_H_
//
// PLEASE READ: Do you really need a singleton?
//
// Singletons make it hard to determine the lifetime of an object, which can
// lead to buggy code and spurious crashes.
//
// Instead of adding another singleton into the mix, try to identify either:
//   a) An existing singleton that can manage your object's lifetime
//   b) Locations where you can deterministically create the object and pass
//      into other objects
//
// If you absolutely need a singleton, please keep them as trivial as possible
// and ideally a leaf dependency. Singletons get problematic when they attempt
// to do too much in their destructor or have circular dependencies.
//

#include "lock_guard.hpp"

#if _MSC_VER >= 1800

#include <atomic>
#include <stack>
#include <functional>

namespace base {

template <class T>
class singleton_instance {
public:
	template <typename ...Args>
	static T* instance(Args&& ...args)
	{
		// 使用带原子锁的DoubleCheckNull 进行最大层度的线程安全校验
		T* tmp = instance_.load( std::memory_order::memory_order_acquire );
		if ( !tmp ) {
			std::lock_guard<std::recursive_mutex> guard( lock_ );
			tmp = instance_.load( std::memory_order::memory_order_relaxed );
			if ( !tmp ) {
				tmp = new T(std::forward<Args>(args)...);
				associated_ = 0;
				instance_.store( tmp, std::memory_order::memory_order_release );
			}
		}
		return tmp;
	}

	static void associate( T *ptr )
	{
		// 使用带原子锁的DoubleCheckNull 进行最大层度的线程安全校验
		T* tmp = instance_.load( std::memory_order::memory_order_acquire );
		if ( !tmp ) {
			std::lock_guard<std::recursive_mutex> guard( lock_ );
			tmp = instance_.load( std::memory_order::memory_order_relaxed );
			if ( !tmp ) {
				tmp = ptr;
				associated_ = 1;
				instance_.store( tmp, std::memory_order::memory_order_release );
			}
		}
	}

	static void release()
	{
		T* tmp = instance_.load( std::memory_order::memory_order_acquire );
		if ( tmp && !associated_ ) {
			std::lock_guard<std::recursive_mutex> guard( lock_ );
			tmp = instance_.load( std::memory_order::memory_order_relaxed );
			if ( tmp && !associated_ ) {
				delete tmp;
				instance_.store( nullptr, std::memory_order::memory_order_release );
			}
		}
	}

private:
	singleton_instance() {}
	~singleton_instance() {}

	singleton_instance( const singleton_instance& ) = delete;
	singleton_instance( singleton_instance&& ) = delete;
	singleton_instance& operator=( const singleton_instance& ) = delete;
	singleton_instance& operator=( singleton_instance&& ) = delete;

private:
	static std::atomic<T*> instance_;
	static std::recursive_mutex lock_;
	static int associated_;
};

template <class T>
std::atomic<T*> singleton_instance<T>::instance_ = nullptr;

template <class T>
std::recursive_mutex singleton_instance<T>::lock_;

template <class T>
int singleton_instance<T>::associated_ = 0;
}  // base

#else

#include "unique_ptr.hpp"

/*
	简单的单件模型, 无需做任何说明
	anderson 2015-7-23
	*/
namespace base {

template <class T, class LOCK = std::recursive_mutex>
class singleton_instance :
	public T {
public:
	//在第一次实例化的时候使用DoubleCheckNull方式检测并发性
	static T* instance() {
		if (!__instance) {
			std::lock_guard<std::recursive_mutex> guard( __locker );
			if (!__instance) {
				__instance = new T;
			}
		}
		return __instance;
	}

	static void associate(T *ptr)
	{
		if (!__instance) {
			std::lock_guard<std::recursive_mutex> guard( __locker );
			if (!__instance) {
				__instance = ptr;
				__associated = 1;
			}
		}
	}

	static void release() {
		if (__instance && !__associated) {
			std::lock_guard<std::recursive_mutex> guard( __locker );
			if (__instance && !__associated) {
				delete __instance;
				__instance = NULL;
			}
		}
	}

private:
	singleton_instance();
	~singleton_instance();

	singleton_instance( const singleton_instance& singletonhandler );
	singleton_instance& operator=( const singleton_instance& singletonhandler );

	static T* __instance;
	static LOCK __locker;
	static int __associated;
};

template <class T, class LOCK>
T *singleton_instance<T, LOCK>::__instance = NULL;

template <class T, class LOCK>
LOCK singleton_instance<T, LOCK>::__locker;

template <class T, class LOCK>
int singleton_instance<T, LOCK>::__associated = 0;
};

#endif

#endif