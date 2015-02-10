/*
 * pch.h
 *
 *  Created on: 2014��10��8��
 *      Author: Administrator
 */

#ifndef ZERTCORE_PCH_H_
#define ZERTCORE_PCH_H_

#include <cstdio>
#include <string>

#include <ctime>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include <array>
#include <map>
#include <set>
#include <list>
#include <future>
#include <utility>
#include <vector>
#include <deque>

#include <cmath>
#include <memory>
#include <random>

#include <ctime>
#include <sys/time.h>

#include <pthread.h>

#include <boost/array.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <boost/circular_buffer.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/timer/timer.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/type_traits.hpp>
#include <boost/utility.hpp>
#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/signals2/signal.hpp>

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

#include <boost/thread.hpp>
#include <boost/dynamic_bitset.hpp>

#include <boost/function_types/parameter_types.hpp>
#include <boost/smart_ptr/detail/spinlock.hpp>

namespace zertcore {
using namespace boost;
}

namespace zertcore {
#define SMART_PTR(x)						::boost::shared_ptr<x >
#define WEAK_PTR(x)							::boost::weak_ptr<x >
}

#endif /* PCH_H_ */
