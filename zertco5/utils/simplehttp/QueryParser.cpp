/*
 * QueryParser.cpp
 *
 *  Created on: 2015年10月16日
 *      Author: Administrator
 */

#include "HttpParser.h"
#include "utils.h"

namespace zertcore { namespace utils {

const string& HttpContext::
getQuery(const string& key) {
	static string sv;

	if (query_.empty()) {
		parseQuery();
	}

	auto it = query_.find(key);
	if (it == query_.end())
		return sv;

	return it->second;
}

void HttpContext::
parseQuery() {
	string key, value;

	key.resize(ZC_HTTP_QUERY_KEY_MAX_SIZE + 1);
	value.resize(ZC_HTTP_QUERY_VALUE_MAX_SIZE + 1);

	enum {
		STATE_NONE,
		STATE_KEY,
		STATE_VALUE,
	};

	enum {
		L_NONE,
		L_SIGN_PERCENT,
		L_SIGN_PERCENT2,
	};

	u32 index = 0;
	u32 state = STATE_KEY;
	u32 l = L_NONE;
	SharedBuffer::byte ch = 0;

	key[0] = 0; value[0] = 0;

	u32 i = 0;
	for (; i < header.uri.size() && header.uri[i] != '?'; ++i);

	for (++i; i < header.uri.size(); ++i) {
		const SharedBuffer::byte c = header.uri[i];

		switch(l) {
		case L_SIGN_PERCENT:
			if (!mcodes[c | 0x20]) {
				ch = c;
				break;
			}

			ch = ch * 16 + (mcodes[c | 0x20]);
			l = L_SIGN_PERCENT2;
			continue;

		case L_SIGN_PERCENT2:
			if (!mcodes[c | 0x20]) {
				ch = c;
				break;
			}

			ch = ch * 16 + (mcodes[c | 0x20]);
			l = L_NONE;
			break;

		default:
			ch = c;

			switch (ch) {
			case '&':
				if (key[0] == 0)
					break;

				value[index] = 0;
				query_[key] = value;

				key[0] = 0;
				index = 0;

				state = STATE_KEY;
				continue;

			case '=':
				key[index] = 0;

				index = 0;
				value[index] = 0;

				state = STATE_VALUE;
				continue;

			case '%':
				l = L_SIGN_PERCENT;
				continue;

			case '+':
				continue;
			}

			break;
		}

		switch (state) {
		case STATE_KEY:
			if (index >= ZC_HTTP_QUERY_KEY_MAX_SIZE)
				break;

			key[index++] = ch;
			break;

		case STATE_VALUE:
			if (index >= ZC_HTTP_QUERY_VALUE_MAX_SIZE)
				break;

			value[index++] = ch;
			break;
		}

		ch = 0;
	}

	if (key[0] != 0) {
		query_[key] = value;
	}

}

}}

