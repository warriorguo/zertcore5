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
#include <algorithm>

#include <ctime>
#include <sys/time.h>
#include <ucontext.h>

#include <pthread.h>

#ifdef ZC_DISABLE_BOOST_FUNCTION
#  include <functional>
#else
#  include <boost/function.hpp>
#endif

#include <boost/array.hpp>
#include <boost/asio.hpp>
/**
#include <boost/asio/ssl.hpp>
*/
#include <boost/asio/deadline_timer.hpp>

#include <boost/circular_buffer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/timer/timer.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/type_traits.hpp>
#include <boost/utility.hpp>
/**
#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
*/
#include <boost/utility/result_of.hpp>
#include <boost/signals2/signal.hpp>

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/enable_shared_from_this.hpp>

/**
#include <boost/bimap/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/bimap/multiset_of.hpp>

#include <boost/flyweight.hpp>
#include <boost/thread.hpp>
#include <boost/dynamic_bitset.hpp>

#include <boost/function_types/parameter_types.hpp>
#include <boost/smart_ptr/detail/spinlock.hpp>
*/

namespace zertcore {
using namespace boost;
}

namespace zertcore {

#ifdef ZC_DISABLE_BOOST_FUNCTION
using ::std::function;
using ::std::bind;
#endif

using ::std::string;
using ::std::list;
using ::std::deque;
using ::std::map;
using ::std::multimap;
using ::boost::unordered_map;
using ::boost::unordered_multimap;
using ::std::vector;
using ::std::set;
using ::std::multiset;
using ::boost::unordered_set;
using ::boost::unordered_multiset;
using ::std::pair;
using ::std::move;
using ::std::forward;
using ::std::for_each;
using ::std::auto_ptr;

}

namespace zertcore {
#define SMART_PTR(x)						::boost::shared_ptr<x >
#define WEAK_PTR(x)							::boost::weak_ptr<x >
}

#endif /* PCH_H_ */
