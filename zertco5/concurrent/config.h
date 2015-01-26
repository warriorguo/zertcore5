/*
 * ConcurrentConfig.h
 *
 *  Created on: 2014Äê11ÔÂ22ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_CONCURRENT_CONFIG_H_
#define ZERTCORE_CONCURRENT_CONFIG_H_

#include <pch.h>
#include <utils/types.h>

namespace zertcore { namespace concurrent {

/**
 * ConcurrentConfig
 */
struct ConcurrentConfig
{
	u32							thread_nums;
};

namespace details {
/**
typedef ::boost::thread::id					thread_id_type;


 * ThreadIndex

struct ThreadIndex
{
	thread_id_type				index;

	explicit ThreadIndex() {
		index = this_thread::get_id();
	}
	explicit ThreadIndex(const thread_id_type& i) : index(i) {}
	explicit ThreadIndex(const ThreadIndex& ti) : index(ti.index) {}

	ThreadIndex& operator= (thread_id_type i) {
		index = i;
		return *this;
	}
};
 */
}

}}

/**
namespace zertcore { namespace concurrent{
typedef details::ThreadIndex				thread_index_type;
}}
*/

namespace zertcore { namespace concurrent {
}}

#endif /* CONCURRENTCONFIG_H_ */
