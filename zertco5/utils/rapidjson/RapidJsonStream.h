/*
 * RapidJsonStream.h
 *
 *  Created on: 2015年7月1日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_RAPIDJSON_RAPIDJSONSTREAM_H_
#define ZERTCORE_UTILS_RAPIDJSON_RAPIDJSONSTREAM_H_

#include <pch.h>
#include <pch.h>
#include <utils/types.h>
#include <log/Log.h>

#include <serialize/config.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace zertcore { namespace utils { namespace json {
using namespace zertcore::serialization;
using namespace rapidjson;
}}}


namespace zertcore { namespace utils { namespace json {

/**
 * RapidJsonIStream
 */
class RapidJsonIStream : noncopyable
{
public:
	RapidJsonIStream();
	RapidJsonIStream(RapidJsonIStream& stream);

public:
	void setListSize(const size_t& size) {
		ZC_ASSERT(type_ == TYPE_ARRAY);
		document_.SetArray();
		document_.Reserve(size, document_.GetAllocator());
	}
	void setObjectSize(const size_t& size) {
		ZC_ASSERT(type_ == TYPE_OBJECT);
		document_.SetObject();
	}

public:
	template <typename T>
	void addList(const T& v) {
		document_.PushBack(v, document_.GetAllocator());
	}
	template <typename T>
	void addObject(const key_type& key, const T& v) {
		document_.AddMember(key, v, document_.GetAllocator());
	}

public:
	void setType(const value_type& t) {
		type_ = t;
		switch(type_) {
		case TYPE_OBJECT:
			document_.SetArray();
			break;
		case TYPE_ARRAY:
			document_.SetObject();
			break;

		default:
			break;
		}
	}

public:
	const Document& data() const {
		return document_;
	}
	Document& data() {
		return document_;
	}
	bool initData() {return true;}

public:
	struct WriterBufferWrapper
	{
		typedef Document::Ch				Ch;

		SharedBuffer&			buffer;
		WriterBufferWrapper(SharedBuffer& b) : buffer(b) {}

	    void Put(Ch c) { buffer.add(&c, sizeof(c)); }
	    void Flush() {}

	    void Clear() { buffer.clear(); }

	    Ch* Push(size_t count) {
	    	u32 size = buffer.size();
	    	buffer.resize(size + count);
	    	return & buffer[size];
	    }
	    void Pop(size_t count) {
	    	ZC_ASSERT(count <= buffer.size());
	    	buffer.resize(buffer.size() - count);
	    }
	};

	SharedBuffer buffer() const {
		SharedBuffer buf;
		WriterBufferWrapper bw(buf);
		Writer<WriterBufferWrapper> writer(bw);

		document_.Accept(writer);
		return buf;
	}

private:
	u32							type_;
	Document					document_;
};

}}}

namespace zertcore { namespace utils { namespace json {

/**
 * RapidJsonOStream
 */
class RapidJsonOStream : noncopyable
{
public:
	struct Iterator
	{
		Document::ConstValueIterator		array_iter;
		Document::ConstMemberIterator		object_iter;

	};
	typedef Iterator						iterator_type;

public:
	RapidJsonOStream() : type_(TYPE_NONE) {}

public:
	bool getValue(const key_type& key, i8& value);
	bool getValue(iterator_type& it, key_type& key, i8& value);

	bool getValue(const key_type& key, i16& value);
	bool getValue(iterator_type& it, key_type& key, i16& value);

	bool getValue(const key_type& key, i32& value);
	bool getValue(iterator_type& it, key_type& key, i32& value);

	bool getValue(const key_type& key, i64& value);
	bool getValue(iterator_type& it, key_type& key, i64& value);

	bool getValue(const key_type& key, u8& value);
	bool getValue(iterator_type& it, key_type& key, u8& value);

	bool getValue(const key_type& key, u16& value);
	bool getValue(iterator_type& it, key_type& key, u16& value);

	bool getValue(const key_type& key, u32& value);
	bool getValue(iterator_type& it, key_type& key, u32& value);

	bool getValue(const key_type& key, u64& value);
	bool getValue(iterator_type& it, key_type& key, u64& value);

	bool getValue(const key_type& key, f32& value);
	bool getValue(iterator_type& it, key_type& key, f32& value);

	bool getValue(const key_type& key, f64& value);
	bool getValue(iterator_type& it, key_type& key, f64& value);

	bool getValue(const key_type& key, bool& value);
	bool getValue(iterator_type& it, key_type& key, bool& value);

	bool getValue(const key_type& key, string& value);
	bool getValue(iterator_type& it, key_type& key, string& value);

	bool getValue(const key_type& key, RapidJsonOStream& value);
	bool getValue(iterator_type& it, key_type& key, RapidJsonOStream& value);

public:
	value_type getType(const key_type& key) const;
	value_type getType(const iterator_type& it) const;

	value_type getType() const {
		return type_;
	}
public:
	iterator_type begin() {
		iterator_type it;
		if (type_ == TYPE_ARRAY) {
			it.array_iter = document_.Begin();
		}
		else if (type_ == TYPE_ARRAY) {
			it.object_iter = document_.MemberBegin();
		}
		else {
			/**
			 * should not reach here
			 */
			ZC_ASSERT(false);
		}

		return it;
	}
	bool moveIterator(iterator_type& it) {
		if (type_ == TYPE_ARRAY) {
			++it.array_iter;
			return true;
		}
		else if (type_ == TYPE_ARRAY) {
			++it.object_iter;
			return true;
		}
		else {
			/**
			 * should not reach here
			 */
			ZC_ASSERT(false);
		}

		return false;
	}
	bool isIteratorEnd(const iterator_type& it) {
		if (type_ == TYPE_ARRAY) {
			if (document_.End() == it.array_iter)
				return true;

			return false;
		}
		else if (type_ == TYPE_ARRAY) {
			if (document_.MemberEnd() == it.object_iter)
				return true;
		}
		else {
			/**
			 * should not reach here
			 */
			ZC_ASSERT(false);
		}

		return true;
	}
	const Value& getIteratorReference(iterator_type& it, key_type& key) {
		if (type_ == TYPE_ARRAY) {
			return *it.array_iter;
		}
		else if (type_ == TYPE_ARRAY) {
			key = it.object_iter->name.getString();
			return it.object_iter->value;
		}
		else {
			/**
			 * should not reach here
			 */
			ZC_ASSERT(false);
		}
		static Value v;
		return v;
	}

public:
	bool buffer(const SharedBuffer& source);
	bool data(const Document& d);
	Document& data() {return document_;}
	bool initData();

private:
	value_type					type_;
	Document					document_;
};

}}}


#endif /* UTILS_RAPIDJSON_RAPIDJSONSTREAM_H_ */
