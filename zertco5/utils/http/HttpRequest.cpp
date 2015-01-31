/*
 * HttpRequest.cpp
 *
 *  Created on: 2015��1��11��
 *      Author: Administrator
 */

#include "HttpRequest.h"

namespace zertcore { namespace net { namespace details {

HttpRequest::HttpRequest() : action_(ACTION_NONE), status_(STATUS_NONE) {
	// TODO Auto-generated constructor stub

}

HttpRequest::~HttpRequest() {
	// TODO Auto-generated destructor stub
}

const string& HttpRequest::
getKey(const string& key) const {
	const static string sv;
	key_value_map_type::const_iterator it = key_value_map_.find(key);
	if (it != key_value_map_.end()) {
		return it->second;
	}

	return sv;
}

HttpRequest::iterator_pair_type HttpRequest::
getKeys() const {
	return iterator_pair_type(key_value_map_.begin(), key_value_map_.end());
}

bool HttpRequest::
parse(const SharedBuffer& buffer, u32& handle_size) {
	if (buffer.empty())
		return false;

	if (status_ == STATUS_UPLOADING) {
		return handlePostUploading(buffer, handle_size);
	}

	handle_size = 0;
	status_ = STATUS_PARSING;

	for (u32 i = 0; i < buffer.size(); ++i) {
		if (buffer[i] == '\n') {
			/**
			 * the http header has been done
			 */
			if (i - handle_size < 3) {
				if (action_ == ACTION_POST) {
					status_ = STATUS_UPLOADING;

					u32 post_handle_size = 0;

					/**
					 * nothing post to the stream
					 */
					if (i >= buffer.size() - 1) {
						status_ = STATUS_DONE;
					}
					else if (!handlePostUploading(buffer.slice(i + 1, buffer.size() - i - 1),
												post_handle_size)) {
						return false;
					}

					if (post_handle_size) {
						handle_size = buffer.size();
					}
					else {
						handle_size = i + 1;
					}
				}
				else {
					status_ = STATUS_DONE;
					handle_size = buffer.size();
				}

				return true;
			}
			else if (!parseSentence(buffer.slice(handle_size, i - handle_size - 1))) {
				return false;
			}

			handle_size = i + 1;
		}
	}

	return true;
}

bool HttpRequest::
handlePostUploading(const SharedBuffer& content, u32& handle_size) {
	if (status_ != STATUS_UPLOADING)
		return false;

	u32 length = lexical_cast<u32>(getKey("Content-Length"));
	if (length == 0)
		return false;

	handle_size = 0;

	if (content.size() >= length) {
		post_ = content.slice(0, length);
		handle_size = length;

		status_ = STATUS_DONE;
	}

	return true;
}

bool HttpRequest::
parseSentence(const SharedBuffer& sentence) {
	const static u32 cGet = ZC_MAKE_U32('G','E','T', ' ');
	const static u32 cPost = ZC_MAKE_U32('P', 'O', 'S', 'T');

	u32 uri_offset = 0;

	switch(*((const u32 *)sentence.data())) {
	case cPost:
		uri_offset = 5;
		action_ = ACTION_POST;
		break;

	case cGet:
		uri_offset = 4;
		action_ = ACTION_GET;
		break;

	default:
		for (size_t i = 0; i < sentence.size() - 2; ++i) {
			if (sentence[i] == ':') {
				string key(&sentence[0], i);
				string value(&sentence[i + 2]);

				key_value_map_.insert(key_value_map_type::value_type(key, value));
				break;
			}
		}
		break;
	}

	if (uri_offset > 0) {
		for (size_t i = uri_offset; i < sentence.size(); ++i) {
			if (sentence[i] == ' ') {
				uri_ = sentence.slice(uri_offset, i - uri_offset);
				break;
			}
		}
	}

	return true;
}

} /* namespace details */} /* namespace net */} /* namespace zertcore */
