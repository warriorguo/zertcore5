/*
 * PoolObject.h
 *
 *  Created on: 2014��11��20��
 *      Author: Administrator
 */

#ifndef ZERTCORE_POOLOBJECT_H_
#define ZERTCORE_POOLOBJECT_H_

#include <pch.h>

#include <utils/types.h>
#include <object/ObjectBase.h>
#include <object/ObjectTraits.h>

namespace zertcore{ namespace object {

/**
 * PoolObject
 */
template <class Final, class _Traits = ObjectTraits<Final> >
class PoolObject :
		public ObjectBase<Final>,
		public enable_shared_from_this<Final>,
		noncopyable
{
public:
	typedef typename _Traits::ptr			ptr;
	const static typename _Traits::ptr		Null;

public:
	void operator delete(void *ptr) {
#ifndef ZC_RELEASE
		ZC_DEBUG_ASSERT(ptr == ((Final *)ptr)->raw_ptr_);
		((Final *)ptr)->raw_ptr_ = nullptr;
#endif
/**
		spinlock_guard_type guard(lock_);

		if (pobject_pool_.is_from((Final *)ptr)) {
			pobject_pool_.destroy((Final *)ptr);
		}
		else
*/
			::operator delete(ptr);
	}

public:
	ptr thisPtr() {
		return getThisPtr(is_pointer<ptr>());
	}
	ptr thisPtr() const {
		return getThisPtr(is_pointer<ptr>());
	}

public:
	static typename _Traits::ptr create() {
		Final* raw_ptr = new Final();//PoolObject<Final, _Traits>::pobject_pool_.construct();//
		typename _Traits::ptr ptr(raw_ptr);
#ifndef ZC_RELEASE
		ptr->raw_ptr_ = raw_ptr;
#endif
		return ptr;
	}

#ifdef ZC_COMPILE
# include "details/PoolObjectCreate.ipp"
#else
	template<typename T, typename... Tn>
	static typename _Traits::ptr create(T p1, Tn... args) {
		typename _Traits::ptr ptr((Final *)PoolObject<Final, _Traits>::
				pobject_pool_.construct(p1, args...));
		return ptr;
	}
#endif

private:
	ptr getThisPtr(const true_type& _) {
		/**
		 * if was pure pointer, the share_from_this() wont work
		 */
		return this;
	}

	ptr getThisPtr(const false_type& _) {
		return this->template shared_from_this();
	}

#ifndef ZC_RELEASE
private:
	void*						raw_ptr_;
#endif
/**
protected:
	static spinlock_type		lock_;

private:
	static object_pool<Final>	pobject_pool_;
*/
};

template<class Final, class _Traits>
const typename _Traits::ptr		PoolObject<Final, _Traits>::Null = nullptr;

/**
template<class Final, class _Traits>
object_pool<Final>				PoolObject<Final, _Traits>::pobject_pool_;


template<class Final, class _Traits>
spinlock_type					PoolObject<Final, _Traits>::lock_;
*/
}}


#endif /* POOLOBJECT_H_ */
