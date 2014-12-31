/*
 * ObjectTraits.h
 *
 *  Created on: 2014Äê11ÔÂ30ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_OBJECTTRAITS_H_
#define ZERTCORE_OBJECTTRAITS_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace object {

template <class Object>
struct ObjectTraits
{
	typedef SMART_PTR(Object)				ptr;
};

}}


#endif /* OBJECTTRAITS_H_ */
