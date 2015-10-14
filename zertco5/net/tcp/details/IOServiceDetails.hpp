/*
 * IOServiceDetails.hpp
 *
 *  Created on: 2015��1��13��
 *      Author: Administrator
 */

#ifndef ZERTCORE_IOSERVICEDETAILS_HPP_
#define ZERTCORE_IOSERVICEDETAILS_HPP_

#include <pch.h>
#include <log/Log.h>

namespace zertcore{ namespace net{ namespace tcp {

template <class Final>
bool IOService<Final>::
setup(const ServiceConfig& config) {

#ifdef ZC_ENABLE_SSL
	enable_ssl_ = config.enable_ssl;
	if (enable_ssl_) {
		ssl_password_ = config.ssl_config.password;
		ssl_context_.set_options(
				asio::ssl::context::default_workarounds
				| asio::ssl::context::no_sslv2
				| asio::ssl::context::single_dh_use);
		ssl_context_.set_password_callback(bind(&self::getSSLPassword, this));
		ssl_context_.use_certificate_chain_file(config.ssl_config.certificate_chain_file);
		ssl_context_.use_private_key_file(config.ssl_config.private_key_file,
				asio::ssl::context::pem);
		ssl_context_.use_tmp_dh_file(config.ssl_config.dh_file);
	}
#endif

	for (u32 i = 0; i < config.thread_nums; ++i) {
		if (!ThreadPool::Instance().registerExclusiveHandler(
				bind(&self::mainThread, this), bind(&self::onStart, this))) {
			ZCLOG(ERROR) << "register exclusive handler" << End;
			return false;
		}
	}

	time_checker_.expires_from_now(posix_time::seconds(60));
	time_checker_.async_wait(bind(&self::handleCheckTimeout, this, _1));
	return true;
}

template <class Final>
void IOService<Final>::
handleCheckTimeout(const system::error_code& err) {
	time_checker_.expires_from_now(posix_time::seconds(60));
	time_checker_.async_wait(bind(&self::handleCheckTimeout, this, _1));
}

}}}


#endif /* IOSERVICEDETAILS_HPP_ */
