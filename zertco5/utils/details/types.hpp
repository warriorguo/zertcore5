/*
 * types.hpp
 *
 *  Created on: 2014-4-28
 *      Author: Administrator
 */

namespace zertcore { namespace utils {

template <typename T, typename enable_if<is_arithmetic<T>, int>::type>
inline void clearUp(T& v) {
	return v = 0;
}

template <>
inline void clearUp(string& v) {
	return v.clear();
}

template <typename T, class Alloc>
inline void clearUp(std::list<T, Alloc>& v) {
	v.clear();
}

template <typename T, class Alloc>
inline void clearUp(std::vector<T, Alloc>& v) {
	v.clear();
}

template <typename T, class Alloc>
inline void clearUp(std::deque<T, Alloc>& v) {
	v.clear();
}

template <typename T, class Alloc, class Compare>
inline void clearUp(std::set<T, Compare, Alloc>& v) {
	v.clear();
}

template <typename T, class Alloc, class Compare>
inline void clearUp(std::multiset<T, Compare, Alloc>& v) {
	v.clear();
}

template <typename T, typename U, class Alloc, class Compare>
inline void clearUp(std::map<T, U, Compare, Alloc>& v) {
	v.clear();
}

template <typename T, typename U, class Alloc, class Compare>
inline void clearUp(std::multimap<T, U, Compare, Alloc>& v) {
	v.clear();
}

}}

namespace zertcore { namespace utils {

template <typename T, typename enable_if<is_arithmetic<T>, int>::type>
inline bool isEmpty(const T& v) {
	return v == 0;
}

template <>
inline bool isEmpty(const string& v) {
	return v.empty();
}

template <typename T, class Alloc>
inline bool isEmpty(const std::list<T, Alloc>& v) {
	return v.empty();
}

template <typename T, class Alloc>
inline bool isEmpty(const std::vector<T, Alloc>& v) {
	return v.empty();
}

template <typename T, class Alloc>
inline bool isEmpty(const std::deque<T, Alloc>& v) {
	return v.empty();
}

template <typename T, class Alloc, class Compare>
inline bool isEmpty(const std::set<T, Compare, Alloc>& v) {
	return v.empty();
}

template <typename T, class Alloc, class Compare>
inline bool isEmpty(const std::multiset<T, Compare, Alloc>& v) {
	return v.empty();
}

template <typename T, typename U, class Alloc, class Compare>
inline bool isEmpty(const std::map<T, U, Compare, Alloc>& v) {
	return v.empty();
}

template <typename T, typename U, class Alloc, class Compare>
inline bool isEmpty(const std::multimap<T, U, Compare, Alloc>& v) {
	return v.empty();
}

}}

namespace zertcore {

template <class T>
inline static T* purePtr(T* p) {return p;}

template <class T>
inline static T* purePtr(SMART_PTR(T) p) {return p.get();}

}


