/*
 * WorldObject.h
 *
 *  Created on: 2014��11��24��
 *      Author: Administrator
 */

#ifndef ZERTCORE_WORLDOBJECT_H_
#define ZERTCORE_WORLDOBJECT_H_

#include <pch.h>

#include <core/Runtime.h>
#include <object/PoolObject.h>

#include "ActiveObjectTraits.h"

namespace zertcore { namespace object {
using namespace zertcore::utils;
}}

namespace zertcore { namespace object {

template <class Object>
struct ActiveObjectManagerBase
{
	virtual ~ActiveObjectManagerBase() {}

	virtual void saveObject(const typename ActiveObjectTraits<Object>::id_type& id)
											= 0;
	virtual bool reloadObject(const typename ActiveObjectTraits<Object>::id_type& id,
			typename ActiveObjectTraits<Object>::ptr object)
											= 0;
	virtual bool setupSlave(const typename ActiveObjectTraits<Object>::id_type& id,
			typename ActiveObjectTraits<Object>::ptr object)
											= 0;
};

/**
 * ActiveObject<Final>
 */
template <class Final>
class ActiveObject :
		public PoolObject<Final>,
		public Serializable<Final>,
		public Unserializable<Final>
{
public:
	typedef typename ActiveObjectTraits<Final>::id_type
											id_type;
	typedef ActiveObjectManagerBase<Final>	manager_base_type;

public:
	virtual ~ActiveObject() {}

public:
	virtual bool init()						= 0;
	virtual void deinit() {}

public:
	virtual void refresh() {}

public:
	virtual bool reload() {
		return getManager().reloadObject(id_, this->template thisPtr());
	}
	virtual void save() {
		getManager().saveObject(id_);
	}

public:
	virtual bool sync();
	virtual void onSync() {}

	bool setMaster();
	bool setSlave();

public:
	id_type getId() const {return id_;}
	void setId(const id_type& id) {id_ = id;}

public:
	manager_base_type& getManager() {
		ZC_ASSERT( manager_base_ );
		return *manager_base_;
	}
	void setManager(manager_base_type& manager) {
		manager_base_ = &manager;
	}

private:
	id_type						id_{0};
	manager_base_type*			manager_base_{nullptr};
	role_type					role_{ROLE_NONE};
};

}}


#endif /* WORLDOBJECT_H_ */
