/*
 * nullval.h
 *
 *  Created on: 2013-7-30
 *      Author: Administrator
 */

#ifndef ZERTCORE_NULLVAL_H_
#define ZERTCORE_NULLVAL_H_

namespace zertcore {

const static struct Null : noncopyable {
/**
	operator std::string() const {
		return std::string();
	}
	operator mongo::Query() const {
		return mongo::BSONObj();
	}
	operator mongo::BSONObj() const {
		return mongo::BSONObj();
	}
	operator mongo::BSONElement() const {
		return mongo::BSONElement();
	}
	operator time_type() const {
		return time_type(0);
	}
	operator any() const {
		return any();
	}
*/
	template <typename T>
	operator T() const {
		return T();
	}

	template <typename T>
	operator T* () const {
		return nullptr;
	}
/**
	template <typename T>
	operator SMART_PTR(T)() const {
		return SMART_PTR(T)();
	}
	template <typename T>
	operator vector<T>() const {
		return vector<T>();
	}
	template <typename T>
	operator list<T>() const {
		return list<T>();
	}
	template <typename T>
	operator set<T>() const {
		return set<T>();
	}
	template <typename T>
	operator deque<T>() const {
		return deque<T>();
	}
	template <typename T, typename R>
	operator map<T, R>() const {
		return deque<T, R>();
	}
	template <typename T>
	operator function<T>() const {
		return function<T>();
	}
*/
	operator char() const {return 0;}
	operator bool() const {return false;}
	operator short() const {return 0;}
	operator int() const {return 0;}
	operator long() const {return 0;}
	operator long long() const {return 0;}

	operator unsigned char() const {return 0;}
	operator unsigned short() const {return 0;}
	operator unsigned int() const {return 0;}
	operator unsigned long() const {return 0;}
	operator unsigned long long() const {return 0;}

	operator float() const {return 0;}
	operator double() const {return 0;}

	/**
	template <typename T, typename _ = void>
	operator T() const {
		return T();
	}

	template <typename T, typename enable_if<is_arithmetic<T> >::type>
	operator T() const {
		return (T)0;
	}
	*/
} null = {};

template <typename T>
inline static bool IsNull(const T& value) {
	return !value;
}

}


#endif /* NULLVAL_H_ */
