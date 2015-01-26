/*
 * IOServiceDetails.hpp
 *
 *  Created on: 2015Äê1ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef ZERTCORE_IOSERVICEDETAILS_HPP_
#define ZERTCORE_IOSERVICEDETAILS_HPP_

#include <pch.h>

namespace zertcore{ namespace net{

template <class Final>
bool IOService<Final>::
init(const ServiceConfig& config) {
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

	for (u32 i = 0; i < config.thread_nums; ++i) {
		if (!ThreadPool::Instance().registerExclusiveHandler(
				bind(&self::mainThread, this), true))
			return false;
	}

	return true;
}

}}


#endif /* IOSERVICEDETAILS_HPP_ */
