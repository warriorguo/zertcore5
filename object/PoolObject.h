/*
 * PoolObject.h
 *
 *  Created on: 2014Äê11ÔÂ20ÈÕ
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
		public ObjectBase<Final>
{
public:
	void operator delete(void *ptr) {
		if (pobject_pool_.is_from((Final *)ptr))
			pobject_pool_.destroy((Final *)ptr);
		else
			::operator delete(ptr);
	}

public:
	static typename _Traits::ptr create() {
		typename _Traits::ptr ptr((Final *)PoolObject<Final, _Traits>::pobject_pool_.construct());
		return ptr;
	}

private:
	static object_pool<Final>	pobject_pool_;
};

template<class Final, class _Traits>
object_pool<Final>				PoolObject<Final, _Traits>::pobject_pool_;

}}


#endif /* POOLOBJECT_H_ */
