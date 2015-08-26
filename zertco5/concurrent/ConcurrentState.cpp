/*
 * Signal.cpp
 *
 *  Created on: 2014��10��13��
 *      Author: Administrator
 */
#include <pch.h>

#include <log/Log.h>
#include <thread/ThreadPool.h>

#include "ConcurrentState.h"
#include "coroutine/Coroutine.h"

#include <details.h>

namespace zertcore { namespace concurrent {
namespace def {
static RuntimeContext::ptr nonerror_context;
}

}}


namespace zertcore { namespace concurrent {

ConcurrentState::ConcurrentState() : target_(0) {
	current_ = 0;
	tid_ = Thread::getCurrentTid();
	pause_ctx_ = nullptr;
}

ConcurrentState::ConcurrentState(const callback_type& handler) :
		target_(0), cb_handler_(handler) {
	current_ = 0;
	tid_ = Thread::getCurrentTid();
	pause_ctx_ = nullptr;
}

#ifdef ZC_ENABLE_THREADHANDLER_MULTICAST
ConcurrentState::ConcurrentState(const callback_type& handler, const tid_type& tid) :
		target_(0), cb_handler_(handler, tid) {
	current_ = 0;
	tid_ = Thread::getCurrentTid();
	pause_ctx_ = nullptr;
}
#endif

ConcurrentState::ConcurrentState(const callback_type& handler, const thread_ids_flag_type& tids) :
		target_(0), cb_handler_(handler, tids) {
	current_ = 0;
	tid_ = Thread::getCurrentTid();
	pause_ctx_ = nullptr;
}

ConcurrentState::~ConcurrentState() {
//	ZCLOG(NOTE) << "~ConcurrentState()" << End;

	if (cb_handler_ && current_ >= target_) {
		cb_handler_.setParams(context_);
		cb_handler_.push();
	}

	if (pause_ctx_) {
		pause_ctx_ = nullptr;
	}
}

void ConcurrentState::
init() {
	def::nonerror_context = RuntimeContext::create();
}

bool ConcurrentState::
onComplete(const callback_type& handler) {
	cb_handler_ = handler;
	return true;
}

/**
handler_type& ConcurrentState::getCompleteHandler() {
	ConcurrentStateInfoProvider::Cell::ptr cell = ConcurrentStateInfoProvider::Instance().get(index_);
	if (!cell) {
		throw Error("ConcurrentStateInfoProvider::Instance().get is NULL");
	}

	return cb_handler_;
}


void ConcurrentState::
handleCallback() {
	ZC_ASSERT(cb_handler_);
	cb_handler_(context_);
}
*/

ConcurrentState::
operator bool() const {
	if (pause_ctx_) return true;
	return cb_handler_? true: false;
}

bool ConcurrentState::
isError() const {
	if (!context_) {
		return false;
	}

	return context_->error;
}

void ConcurrentState::
setError(const Error& error) {
	if (!context_) {
		context_ = RuntimeContext::create();
	}

	context_->error = error;
}

#ifndef ZC_DISABLE_COROUTINE

void ConcurrentState::
setCoroutineCtx(cofcell_type::ptr co) {
	pause_ctx_ = co;
}

void ConcurrentState::
releaseCoroutineCtx() {
	if (pause_ctx_)
		pause_ctx_->release();

	pause_ctx_ = nullptr;
}

void ConcurrentState::
yeild() {
	/**
	 * just make sure this state wouldnt release.
	 */
	save_me_ = thisPtr();
	pause_ctx_ = Coroutine::Instance().getLastFCell();
	Coroutine::Instance().jumpBack(pause_ctx_);
}

void ConcurrentState::
resume() {
	save_me_.reset();
	Coroutine::Instance().jumpTo(pause_ctx_);
}

#endif

void ConcurrentState::
setTimeout(const tick_type& intval) {
	timer_ = timer_type::create();

	timer_->onExpired(bind(&ConcurrentState::onTimeout, this, _1));
	timer_->start(intval);
}

void ConcurrentState::
onTimeout(const tick_type& intval) {
	spinlock_guard_type guard(lock_);
	setError(error::TIME_OUT);

	if (pause_ctx_) {
		ThreadPool::Instance().push(thisPtr());
	}

	if (cb_handler_) {
		cb_handler_.setParams(context_);
		ZC_ASSERT(cb_handler_.push());
		cb_handler_ = cbt_handler_type();
	}

	ZCLOG(NOTE) << "State completed for Time out" << End;
}

bool ConcurrentState::
complete(const Error& error) {
	spinlock_guard_type guard(lock_);

	/**
	if (!cb_handler_) {
		return false;
	}
	*/

	if (!error) {
		RuntimeContext& ctx = Thread::getCurrentRuntimeContext();
		if (ctx) {
			setError(ctx.error);
		}
	}
	else {
		setError(error);
	}

	if (++current_ >= target_) {
		/**
		handler_type cb(bind(&ConcurrentState::handleCallback, thisPtr()));
		cb.setThreadIndex(cb_handler_.getThreadIndex());

		ZC_ASSERT(cb.push());
		*/

		if (cb_handler_) {
			if (context_)
				cb_handler_.setParams(context_);
			else
				cb_handler_.setParams(def::nonerror_context);

			/**
			 * if pause context set, resume it after the callback running
			 */
			if (pause_ctx_) {
				task_type task(cb_handler_, cb_handler_.getThreadIndex());
				task.state = thisPtr();

				ThreadPool::Instance().push(task, cb_handler_.getPriority());
			}
			else {
				ZC_ASSERT(cb_handler_.push());
			}
			cb_handler_.clear();
		}
		else if (pause_ctx_) {
			ThreadPool::Instance().push(thisPtr());
		}

//		ZCLOG(NOTE) << "State completed and CB Handler pushed" << End;
	}
/**
	else {
//		ZCLOG(NOTE) << "current=" << current_ << " target=" << target_ << End;
	}
*/
	return true;
}

ConcurrentState& ConcurrentState::
operator= (const callback_type& handler) {
	cb_handler_ = handler;
//	target_ = 0;

	return *this;
}

bool ConcurrentState::
listen() {
	target_++;
	return true;
}

}}
