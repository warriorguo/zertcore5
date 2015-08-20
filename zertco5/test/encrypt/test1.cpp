/*
 * test1.cpp
 *
 *  Created on: 2015年6月18日
 *      Author: Administrator
 */

#include <utils/crypto/Crypto.h>

using namespace zertcore::utils;

int main() {
	Crypto::randkey_type rdk;
	Crypto::prepareRandkey(rdk, "show me the money");

	SharedBuffer sb;
	sb.assign(std::string("operation cwal"));

	ZC_ASSERT( Crypto::doXor(sb, rdk) );
	::printf("%s\n", (const char *)sb.data());

	ZC_ASSERT( Crypto::doXor(sb, rdk) );

	::printf("%s\n", (const char *)sb.data());

	return 0;
}
