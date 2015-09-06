/*
 * Session.h
 *
 *  Created on: 2015年6月18日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_SESSION_SESSION_H_
#define ZERTCORE_UTILS_SESSION_SESSION_H_

#include <pch.h>
#include <utils/types.h>

#include <object/ActiveObjectManager.h>
#include <object/ActiveObjectTraits.h>
#include <utils/updatelist/DynamicList.h>
#include <utils/updatelist/UpdateList.h>

#include <core/Runtime.h>

#ifndef ZC_SESSION_CMD_SLOT_SIZE
#  define ZC_SESSION_CMD_SLOT_SIZE			32
#endif

namespace zertcore { namespace suit {
using namespace zertcore::object;
using namespace zertcore::utils;
using namespace zertcore::net;

namespace __ {

struct NonConnection
{
	void send(const SharedBuffer& buff) {}
	void write(const SharedBuffer& buff) {}
};

}

template <u32 S, class D, class C = __::NonConnection>
class Session;

}}

namespace zertcore { namespace object {
using namespace zertcore::suit;
/**
 * ActiveObjectTraits
 */
template <u32 S, class D, class C>
struct ActiveObjectTraits<Session<S, D,C> > : public ObjectTraits<Session<S,D,C> >
{
	typedef uuid_t							id_type;

	static const char*			TABLE_NAME;
	static const char*			RPC_NAME;
	static const char*			SYNC_NAME;

	static const tick_type		DefaultExpiredTick; // default 10 second
};

template <u32 S, class D, class C>
const char*						ActiveObjectTraits<Session<S,D,C> >::TABLE_NAME = NULL;

template <u32 S, class D, class C>
const char*						ActiveObjectTraits<Session<S,D,C> >::RPC_NAME = NULL;

template <u32 S, class D, class C>
const char*						ActiveObjectTraits<Session<S,D,C> >::SYNC_NAME = "*sess";

template <u32 S, class D, class C>
const tick_type					ActiveObjectTraits<Session<S,D,C> >::DefaultExpiredTick = 1000 * 60 * 3;

}}

namespace zertcore { namespace suit {

/**
 * In the connection handshake, use DH algorithm set up the private key.
 * and then use the key to encrypt the transfer data.
 */
/**
 * Session was bridge or agent between server and game world
 * one session would hold several connections.
 *
 * if want to sync Data in template, make it serializable & unserializable
 */
/**
 * For using of session slave,
 *
 *
 * class Player
 * {
 * public:
 * 	void d();
 * private:
 * 	Session::ptr				session_;
 * };
 *
 */
template <u32 MaxMessage, class Data, class Connection>
class Session :
		public ActiveObject<Session<MaxMessage, Data, Connection> >
{
	typedef Session<MaxMessage, Data, Connection>
											self;
	typedef ActiveObject<Session<MaxMessage, Data, Connection> >
											parent;
public:
	typedef Data							data_type;
	typedef typename ObjectTraits<Connection>::ptr
											connection_ptr;
public:
	typedef unordered_set<connection_ptr>	connection_set_type;
	typedef circular_buffer<SharedBuffer>	message_buffer_type;

public:
	typedef ThreadHandler<void (typename self::ptr)>
											on_create_handler_type;
	typedef ThreadHandler<void (typename self::ptr)>
											on_sync_handler_type;
	typedef ThreadHandler<void (typename self::ptr, SharedBuffer)>
											on_data_handler_type;

public:
	Session() : is_gate_(false), rev_msg_buffer_(MaxMessage), sed_msg_buffer_(MaxMessage),
		priority_(PRIORITY_LOW) {}

	virtual ~Session() {}

public:
	virtual bool init();
	static void setCreateHandler(const on_create_handler_type& handler) {
		on_create_handler_ = handler;
	}
	static void setSyncHandler(const on_sync_handler_type& handler) {
		on_sync_handler_ = handler;
	}
	static void setDataHandler(const on_data_handler_type& handler) {
		on_data_handler_ = handler;
	}

public:
	void setPrivateKey(const string& key) { private_key_ = key; }
	string getPrivateKey() const { return private_key_; }

	void setPriority(const priority_type& pri) { priority_ = pri; }
	priority_type getPriorty() const { return priority_; }

	void setGate() {is_gate_ = true;}
	bool isGate() const  {return is_gate_;}

public:
	bool runPrepare();
	bool runCommand();

public:
	virtual void onSync();

public:
	data_type& data() { return data_; }
	const data_type& data() const { return data_; }

public:
	void addConnection(connection_ptr conn);
	void eraseConnection(connection_ptr conn);

public:
	/**
	 * can be called multi-thread
	 */
	void send(const SharedBuffer& buff);

private:
	void sendBuffers();

public:
	bool pushMessage(const SharedBuffer& buff, connection_ptr conn);
	bool popMessage(SharedBuffer& buff);
	bool hasMessage() const;

public:
	template <class Archiver>
	void serialize(Archiver& archiver) {
		doSerializeWithData(archiver, is_base_of<SerializableBase, data_type>());
		parent::serialize(archiver);
	}

	template <class Archiver>
	bool unserialize(Archiver& archiver) {
		parent::unserialize(archiver);
		return doUnserializeWithData(archiver, is_base_of<UnserializableBase, data_type>());
	}


	/**
	 * make serialization if the data was serializable
	 */
private:
	template <class Archiver>
	void doSerializeWithData(Archiver& archiver, const true_type& _) {
		doSerialize(archiver);
		archiver & data_;
	}
	template <class Archiver>
	void doSerializeWithData(Archiver& archiver, const false_type& _) {
		doSerialize(archiver);
	}

	template <class Archiver>
	void doSerialize(Archiver& archiver) {
		archiver["*pri"] & priority_;

		if (is_gate_) {
			archiver["*rev"] & rev_msg_buffer_;
			rev_msg_buffer_.clear();
		}
		else {
			archiver["*sed"] & sed_msg_buffer_;
			sed_msg_buffer_.clear();
		}
	}

private:
	template <class Archiver>
	bool doUnserializeWithData(Archiver& archiver, const true_type& _) {
		archiver & data_;
		return doUnserialize(archiver);
	}
	template <class Archiver>
	bool doUnserializeWithData(Archiver& archiver, const false_type& _) {
		return doUnserialize(archiver);
	}
	template <class Archiver>
	bool doUnserialize(Archiver& archiver) {
		archiver["*pri"] & priority_;
		bool ret = false;

		if (!is_gate_) {
			do {
				spinlock_guard_type guard(rev_lock_);
				ret = archiver["*rev"] & rev_msg_buffer_;
			}
			while(false);

			if (ret) {
				runPrepare();
			}
		}
		else {
			ret = archiver["*sed"] & sed_msg_buffer_;
		}

		return ret;
	}

	ZC_TO_STRING("id" << this->template getId() << "is_gate" << is_gate_);

private:
	bool						is_gate_;
	string						private_key_;
	data_type					data_;

private:
	spinlock_type				rev_lock_;
	message_buffer_type			rev_msg_buffer_,
								sed_msg_buffer_;

private:
	priority_type				priority_;

private:
	connection_set_type			connection_set_;


private:
	static on_create_handler_type
								on_create_handler_;
	static on_sync_handler_type	on_sync_handler_;
	static on_data_handler_type	on_data_handler_;
};

}}


namespace zertcore { namespace suit {

/*
 * SessionManager
 */
template <u32 MaxMessage, class Data, class Connection = __::NonConnection>
class SessionManager :
		public ActiveObjectManager<SessionManager<MaxMessage, Data, Connection>, Session<MaxMessage, Data, Connection> >
{
public:
	enum {
		THREAD_INDEX						= 2,
	};

public:
	typedef Session<MaxMessage, Data, Connection>
											session_type;
	typedef typename session_type::ptr		session_ptr;
	typedef unordered_set<session_ptr>		session_ptr_set_type;
	typedef UpdateList<session_ptr_set_type>
											session_update_list_type;

public:
	session_ptr createSession();

public:
	bool setup();

public:
	void registerSync(session_ptr session);

private:
	u32 handleSessions();

private:
	session_update_list_type	sync_list_;
private:
	uuid_t						id_counter_{0};
	bool						is_setup_{false};
};

}}


#endif /* UTILS_SESSION_SESSION_H_ */
