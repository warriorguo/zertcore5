/*
 * Crypto.h
 *
 *  Created on: 2015年4月28日
 *      Author: Administrator
 */

#ifndef ZERTCORE_UTILS_CRYPTO_CRYPTO_H_
#define ZERTCORE_UTILS_CRYPTO_CRYPTO_H_

#include <pch.h>
#include <utils/types.h>

#include <utils/buffer/SharedBuffer.h>

#ifndef ZC_ENCRYPT_RANDKEY_LENGTH
#  define ZC_ENCRYPT_RANDKEY_LENGTH			256
#endif

namespace zertcore{ namespace utils{

/**
 * provides the normal math algorithm
 */
class Crypto
{
public:
	static string sha256(const string& str);
	static string md5(const string& str);

public:
	typedef array<u8, ZC_ENCRYPT_RANDKEY_LENGTH>
											randkey_type;

public:
	static bool prepareRandkey(randkey_type& randkey, const string& password);
	/**
	 * if need compress the data, do it before encrypt
	 */
	static bool doXor(SharedBuffer& input, const randkey_type& randkey);

public:
	static string genRandomKey();
};

}}


#endif /* UTILS_CRYPTO_CRYPTO_H_ */
