/*
 * config.h
 *
 *  Created on: 2015年6月4日
 *      Author: Administrator
 */

#ifndef ZERTCORE_THREAD_CONFIG_H_
#define ZERTCORE_THREAD_CONFIG_H_

#include <utils/types.h>

namespace zertcore { namespace concurrent {

typedef function<void ()>					init_handler_type;
typedef function<void ()>					after_all_init_handler_type;

}}

#endif /* THREAD_CONFIG_H_ */
