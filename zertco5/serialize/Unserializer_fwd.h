/*
 * Unserializer_fwd.h
 *
 *  Created on: 2015年1月27日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UNSERIALIZER_FWD_H_
#define ZERTCORE_UNSERIALIZER_FWD_H_

#include "Archiver.h"

namespace zertcore {

template <class F>
struct Unserializable {};

}

namespace zertcore { namespace serialization {

/**
 * template for a OStream
 * struct OStream
 * {
 * typedef iterator iterator_type;
 *
 * template <typename T>
 * bool getValue(const key_type& key, T& value);
 *
 * template <typename T>
 * bool getValue(iterator_type& it, key_type& key, T& value);
 *
 * iterator_type begin();
 *
 * value_type getType();
 * data_type& data();
 *
 * };
 *
 */

/**
 * Serializer<Stream>
 */
template <class Stream>
class Unserializer
{
	typedef Unserializer<Stream>			self_type;
public:
	enum {
		FROM_NONE							= 0,
		FROM_KEY							= 1,
		FROM_ITERATOR						= 2,
	};

public:
	typedef key_type						key_type;

public:
	typedef Stream							stream_type;
	typedef Archiver<Stream>				archiver_type;
	typedef typename archiver_type::ptr		archiver_ptr;

public:
	typedef typename stream_type::iterator_type
											iterator_type;

public:
	Unserializer();
	Unserializer(const self_type& ar);
	Unserializer(self_type& ar);
	virtual ~Unserializer() {}

public:
	stream_type& stream() {return archiver_->stream();}
	const stream_type& stream() const {return archiver_->stream();}

public:
	self_type& operator= (const self_type& ar);

public:
	template <typename T>
	bool operator& (T& v) const;

public:
	const self_type& operator[] (const key_type& key) const {
		setKey(key);
		return *this;
	}
	const key_type& getKey() const;
	void setKey(const key_type& key) const;
	bool hasKey() const;
	const self_type& setIterator(iterator_type* iter) const;

public:
	template <typename T>
	bool getValue(T& v) const;
	bool getValue(self_type& v) const;

	template <typename T>
	bool getValue(iterator_type* iter, T& v) const;
	bool getValue(iterator_type* iter, self_type& v) const;

public:
	bool buffer(const SharedBuffer& buf);

public:
	value_type getType() const;

public:
	iterator_type begin() const;

private:
	mutable archiver_ptr		archiver_;

	mutable u32					from_type_;
	mutable iterator_type*		p_iterator_;
};

}}



#endif /* UNSERIALIZER_FWD_H_ */
