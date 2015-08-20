/*
 * test8.cpp
 *
 *  Created on: 2015年6月16日
 *      Author: Administrator
 */

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <boost/flyweight.hpp>

using namespace std;
using namespace boost::flyweights;

class Output
{
public:
	template <typename T>
	Output& operator << (const T& v) {
		cout << v;
		return *this;
	}
};

template <class Archiver>
struct Base
{
	virtual void serialize(Archiver& ar)	= 0;
};

template <class Archiver>
struct Values
{
	void reg(Base<Archiver>& v) {
		base_list_.push_back(&v);
	}

	void serialize(Archiver& ar) {
		for (auto it = base_list_.begin(); it != base_list_.end(); ++it) {
			(*it)->serialize(ar);
		}
	}

private:
	vector<Base<Archiver> *>	base_list_;
};

template <typename T, class Archiver1, class Archiver2 = void, class Archiver3 = void>
class Value;

template <typename T, class Archiver1>
class Value<T, Archiver1, void, void> : public Base<Archiver1>
{
public:
	Value(Values<Archiver1>& values, const string& name, const T& def = T()) :
		name_(name), v_(def) {
		values.reg(*this);
	}

public:
	const T& get() const {return v_;}
	void set(const T& v) {v_ = v;}

	operator T() {return v_;}
	T& operator= (const T& v) {set(v); return *this;}

public:
	virtual void serialize(Archiver1& ar) {
		ar << name_.get() << ":" << v_ << ",";
	}

private:
	flyweight<string>			name_;
	T							v_;
};

template <typename T, class Archiver1, class Archiver2>
class Value<T, Archiver1, Archiver2, void> :
	public Base<Archiver1>,
	public Base<Archiver2>
{
public:
	Value(Values<Archiver1>& values1, Values<Archiver2>& values2, const string& name, const T& def = T()) :
		name_(name), v_(def) {
		values1.reg(*this);
		values2.reg(*this);
	}

public:
	const T& get() const {return v_;}
	void set(const T& v) {v_ = v;}

	operator T() {return v_;}
	T& operator= (const T& v) {set(v); return *this;}

public:
	virtual void serialize(Archiver1& ar) {
		ar << name_.get() << ":" << v_ << ",";
	}
	virtual void serialize(Archiver2& ar) {
		ar << name_.get() << ":" << v_ << ",";
	}

private:
	flyweight<string>			name_;
	T							v_;
};

template <typename T, class Archiver1, class Archiver2, class Archiver3>
class Value :
	public Base<Archiver1>,
	public Base<Archiver2>,
	public Base<Archiver3>
{
public:
	Value(Values<Archiver1>& values1, Values<Archiver2>& values2, Values<Archiver3>& values3, const string& name, const T& def = T()) :
		name_(name), v_(def) {
		values1.reg(*this);
		values2.reg(*this);
		values3.reg(*this);
	}

public:
	const T& get() const {return v_;}
	void set(const T& v) {v_ = v;}

	operator T() {return v_;}
	T& operator= (const T& v) {set(v); return *this;}

public:
	virtual void serialize(Archiver1& ar) {
		ar << name_.get() << ":" << v_ << ",";
	}
	virtual void serialize(Archiver2& ar) {
		ar << name_.get() << ":" << v_ << ",";
	}
	virtual void serialize(Archiver3& ar) {
		ar << name_.get() << ":" << v_ << ",";
	}

private:
	flyweight<string>			name_;
	T							v_;
};


template <class Archiver>
class Host
{
public:
	void serialize(Archiver& ar) {
		values_.serialize(ar);
	}

protected:
	Values<Archiver>			values_;
};

class C : public Values<ostream>, public Values<Output>
{
public:


private:
	Value<int, ostream>			v1{*this, "v1"};
	Value<int, Output, ostream>
								v2{*this, *this, "v2"};
};


template <class Final>
class DBTemplate
{
public:
	struct Data
	{
	private:
		Final&					c_;
	public:
		Data(Final& c) : c_(c) {}

		template <typename Archiver>
		void serialize(Archiver& ar);
	};

public:
	template <typename Archiver>
	void serialize(Archiver& ar) {
		Data data(static_cast<Final &>(*this));
		data.serialize(ar);
	}

private:
};

class Cc : public DBTemplate<Cc>
{
	friend class DBTemplate<Cc>::Data;
private:
	string						name_{"haha"};
};

template <>
template <typename Archiver>
void DBTemplate<Cc>::Data::serialize(Archiver& ar) {
	ar << c_.name_;
}

int main() {
	C c;
	Output o;
	Values<ostream>(c).serialize(cout);
	cout << endl;
	Values<Output>(c).serialize(o);

	cout << endl;

	Cc cc;
	cc.serialize(cout);

	cout << endl;

	return 0;
}


