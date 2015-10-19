/*
 * HttpRequest.h
 *
 *  Created on: 2015��1��11��
 *      Author: Administrator
 */

#ifndef ZERTCORE_HTTPREQUEST_H_
#define ZERTCORE_HTTPREQUEST_H_

#include <pch.h>

#include <utils/types.h>
#include <utils/buffer/SharedBuffer.h>

namespace zertcore { namespace net { namespace details {
using namespace zertcore::utils;

/**
 * HttpRequest
 */
class HttpRequest
{
public:
	enum action_type {
		ACTION_NONE							= 0,
		ACTION_GET							= 1,
		ACTION_POST							= 2,
	};

	enum status_type {
		STATUS_NONE							= 0,
		STATUS_PARSING						= 1,
		STATUS_UPLOADING					= 2,
		STATUS_DONE							= 10,
	};

public:
	typedef map<string, string>				key_value_map_type;
	typedef pair<key_value_map_type::const_iterator,
	key_value_map_type::const_iterator>		iterator_pair_type;

public:

public:
	HttpRequest();
	virtual ~HttpRequest();

public:
	/**
	 * handle_size to get the handled size of the content
	 * return false if parse failed
	 */
	bool parse(const SharedBuffer& buffer, u32& handle_size);
	bool isDone() const {return status_ == STATUS_DONE;}

public:
	action_type getAction() const {return action_;}
	const string& getURI() const {return uri_;}

	const string& getKey(const string& key) const;
	iterator_pair_type getKeys() const;

	const SharedBuffer& getPost() const {return post_;}

private:
	bool parseSentence(const SharedBuffer& sentence);
	bool handlePostUploading(const SharedBuffer& content, u32& handle_size);

private:
	action_type					action_;
	status_type					status_;

	key_value_map_type			key_value_map_;
	string						uri_;
	SharedBuffer				post_;
};

}}}

#endif /* HTTPREQUEST_H_ */
