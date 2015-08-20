/*
 * Buff.h
 *
 *  Created on: 2015年6月20日
 *      Author: Administrator
 */

#ifndef ZERTCORE_SUIT_BUFF_BUFF_H_
#define ZERTCORE_SUIT_BUFF_BUFF_H_

#include <pch.h>
#include <utils/types.h>
#include <utils/serializable/SerializableObject.h>
#include <suit/utils/RateValue.h>

namespace zertcore { namespace suit {
using namespace zertcore::utils;

typedef uuid_t								phase_type;

class Buff;
class BuffCeptor;


typedef unordered_map<const Buff *, ulong>	buff_ptr_map_type;
typedef unordered_set<BuffCeptor *>			buffceptor_ptr_list_type;
}}

namespace zertcore { namespace suit {

/**
 * Buff
 */
class Buff : noncopyable
{
public:
	Buff();
	Buff(const double& rate, const double& value, const ulong& tick);

	virtual ~Buff();

public:
	void setHost(const uuid_t& host) {host_ = host;}
	void setVia(const uuid_t& via) {via_ = via;}
	uuid_t getHost() const {return host_;}
	uuid_t getVia() const {return via_;}

public:
	double getValue() const {return rv_.value;}
	double getRate() const {return rv_.rate;}

	ulong getTick() const {return tick_;}

public:
	void link(BuffCeptor& bc) const {
		buffceptor_ptr_list_.insert(&bc);
	}
	void unlink(BuffCeptor& bc) const {
		buffceptor_ptr_list_.erase(&bc);
	}
	void unlinkAll() const;

private:
	uuid_t						host_;
	uuid_t						via_;

	RateValue					rv_;
	ulong						tick_;

private:
	mutable buffceptor_ptr_list_type
								buffceptor_ptr_list_;
};

/**
 * BuffCeptor
 */
class BuffCeptor
{
public:
	BuffCeptor();

public:
	void add(const Buff& buff);
	void erase(const Buff& buff);

public:
	double getValue() const {return rv_.value;}
	double getRate() const {return rv_.rate;}

	double calc(const double& v) const;

	BuffCeptor& operator += (const Buff& buff) {
		add(buff);
		return *this;
	}
	BuffCeptor& operator -= (const Buff& buff) {
		erase(buff);
		buff.unlink(*this);
		return *this;
	}

public:
	void updateTick(const ulong& tick = 1);

	template <typename Handler>
	void foreach(Handler handler) {
		for (auto it = buff_ptr_map_.begin(); it != buff_ptr_map_.end(); ) {
			auto hit = it++;
			handler(*(hit->first));
		}
	}

public:
	bool empty() const {return buff_ptr_map_.empty();}

private:
	void refresh();

private:
	RateValue					rv_;
	buff_ptr_map_type			buff_ptr_map_;
};

}}


#endif /* UTILS_BUFF_BUFF_H_ */
