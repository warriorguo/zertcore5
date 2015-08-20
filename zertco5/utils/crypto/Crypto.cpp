/*
 * Crypto.cpp
 *
 *  Created on: 2015年4月28日
 *      Author: Administrator
 */

#include "Crypto.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK		1

#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <cryptopp/md5.h>
#include <cryptopp/base64.h>
#include <cryptopp/integer.h>

namespace zertcore { namespace utils {

string Crypto::
sha256(const string& str) {
	string output;
	CryptoPP::SHA256 hash;
	byte digest[CryptoPP::SHA256::DIGESTSIZE];

	hash.CalculateDigest(digest,(const byte *)str.c_str(),str.size());

	CryptoPP::HexEncoder encoder;

	encoder.Attach(new CryptoPP::StringSink(output));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();

	return output;
}

string Crypto::
md5(const string& str) {
	string output;
	CryptoPP::Weak::MD5 hash;
	byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];

	hash.CalculateDigest(digest,(const byte *)str.c_str(),str.size());

	CryptoPP::HexEncoder encoder;

	encoder.Attach(new CryptoPP::StringSink(output));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();

	return output;
}

bool Crypto::
prepareRandkey(randkey_type& randkey, const string& password) {
	string en(move(md5(password)));
	size_t length = en.size();

	for (size_t i = 0; i < randkey.size(); ++i) {
		randkey[i] = (en[i % length] + i) % 0xff;
	}

	return true;
}

bool Crypto::
doXor(SharedBuffer& source, const randkey_type& randkey) {
	if (randkey.empty())
		return false;

	randkey_type rndkey(randkey);

	for (size_t i = 0, j = 0, a = 0; i < source.size(); ++i) {
		a = (a + 1) % rndkey.size();
		j = (rndkey[a] + j) % rndkey.size();

		SharedBuffer::byte tmp = rndkey[a];
		rndkey[a] = rndkey[j];
		rndkey[j] = tmp;

		source[i] = source[i] ^ rndkey[(rndkey[a] + rndkey[j]) % rndkey.size()];
	}

	return true;
}

}}
