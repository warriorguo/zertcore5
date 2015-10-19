/*
 * PoolObjectDetails.hpp
 *
 *  Created on: 2015年10月18日
 *      Author: Administrator
 */

#ifndef ZERTCORE_OBJECT_DETAILS_POOLOBJECTDETAILS_HPP_
#define ZERTCORE_OBJECT_DETAILS_POOLOBJECTDETAILS_HPP_

#include "../PoolObject.h"

namespace zertcore{ namespace object {

template <class Final, class _Traits>
void PoolObject<Final, _Traits>::
operator delete(void *ptr) {
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

template <class Final, class _Traits>
typename _Traits::ptr PoolObject<Final, _Traits>::
create() {
	Final* raw_ptr = new Final();//PoolObject<Final, _Traits>::pobject_pool_.construct();//
	typename _Traits::ptr ptr(raw_ptr);
#ifndef ZC_RELEASE
	ptr->raw_ptr_ = raw_ptr;
#endif
	return ptr;
}

#ifdef ZC_COMPILE
#  include "PoolObjectCreateImpl.ipp"
#endif

}}


#endif /* OBJECT_DETAILS_POOLOBJECTDETAILS_HPP_ */
