/*
 * HttpParser.cpp
 *
 *  Created on: 2015年9月15日
 *      Author: Administrator
 */

#include <pch.h>
#include <utils/types.h>
#include <log/Log.h>

#include "HttpParser.h"

namespace zertcore { namespace utils {

enum state_type {
	STATE_NONE								= 0,
	STATE_HEADER,
	STATE_HEADER_ERROR,

	STATE_UPLOADING,
	STATE_UPLOADING_ERROR,

	STATE_DONE
};

enum {
	TOKEN_NONE								= 0,
	TOKEN_HTTP								= 1,
	TOKEN_GET								= 2,
	TOKEN_POST								= 3,
	TOKEN_HOST								= 4,
	TOKEN_CONTENTLENGTH						= 5,
	TOKEN_USERAGENT							= 6,
	TOKEN_ACCEPTLANGUAGE					= 7,
	TOKEN_ACCEPTENCODING					= 8,
	TOKEN_ACCEPT							= 9,
	TOKEN_REFERER							= 10,
	TOKEN_COOKIE							= 11,
	TOKEN_CONNECTION						= 12,
	TOKEN_UNKNOWN,
};
typedef u32									token_type;

class Chars;
struct HttpHeaderContext;

namespace helper {

bool handleGet(Chars& chars, HttpHeaderContext&);
bool handlePost(Chars& chars, HttpHeaderContext&);

bool handleHost(Chars& chars, HttpHeaderContext&);
bool handleUserAgent(Chars& chars, HttpHeaderContext&);
bool handleContentLength(Chars& chars, HttpHeaderContext&);
bool handleAcceptLanguage(Chars& chars, HttpHeaderContext&);
bool handleAcceptEncoding(Chars& chars, HttpHeaderContext&);
bool handleAccept(Chars& chars, HttpHeaderContext&);
bool handleReferer(Chars& chars, HttpHeaderContext&);
bool handleCookie(Chars& chars, HttpHeaderContext&);
bool handleConnection(Chars& chars, HttpHeaderContext&);
bool handleUnknown(Chars& chars, HttpHeaderContext&);

}

#define _L_TOKEN_MAX_KEYS					3

/**
 * TokenElement
 */
struct TokenElement
{
	typedef bool (*handler_type)(Chars&, HttpHeaderContext&);

	token_type					token;
	u32							keys[_L_TOKEN_MAX_KEYS];
	handler_type				hanlder;
};

static TokenElement TokensLib[] = {
		{0},
		{TOKEN_HTTP,			{ZC_MAKE_U32('H','T','T','P'), 0, 0}, NULL},
		{TOKEN_GET,				{ZC_MAKE_U32('G','E','T',' '), 0, 0}, & helper::handleGet},
		{TOKEN_POST,			{ZC_MAKE_U32('P','O','S','T'), 0, 0}, & helper::handlePost},
		{TOKEN_HOST,			{ZC_MAKE_U32('H','o','s','t'), 0, 0}, & helper::handleHost},
		{TOKEN_CONTENTLENGTH,	{ZC_MAKE_U32('C','o','n','t'), ZC_MAKE_U32('e','n','t','-'), ZC_MAKE_U32('L','e','n','g')}, & helper::handleContentLength},
		{TOKEN_USERAGENT,		{ZC_MAKE_U32('U','s','e','r'), 0, 0}, & helper::handleUserAgent},
		{TOKEN_ACCEPTLANGUAGE,	{ZC_MAKE_U32('A','c','c','e'), ZC_MAKE_U32('p','t','-','L'), 0}, & helper::handleAcceptLanguage},
		{TOKEN_ACCEPTENCODING,	{ZC_MAKE_U32('A','c','c','e'), ZC_MAKE_U32('p','t','-','E'), 0}, & helper::handleAcceptEncoding},
		{TOKEN_ACCEPT,			{ZC_MAKE_U32('A','c','c','e'), 0, 0}, & helper::handleAccept},
		{TOKEN_REFERER,			{ZC_MAKE_U32('R','e','f','e'), 0, 0}, & helper::handleReferer},
		{TOKEN_COOKIE,			{ZC_MAKE_U32('C','o','o','k'), 0, 0}, & helper::handleCookie},
		{TOKEN_CONNECTION,		{ZC_MAKE_U32('C','o','n','n'), 0, 0}, & helper::handleConnection},
		{0}
};

static TokenElement UnknownToken = {
		TOKEN_UNKNOWN, {0}, & helper::handleUnknown
};

/**
 * CharMatcher
 */
struct Chars
{
private:
	SharedBuffer				stream_;
	u32							current_index_;

public:
	Chars() : current_index_(0) {}

public:
	const SharedBuffer& stream() const {
		return stream_;
	}

	SharedBuffer left() {
		if (current_index_ >= stream_.size()) {
			return SharedBuffer();
		}
		return stream_.slice(current_index_);
	}

	void setStream(const SharedBuffer& sb) {
		stream_ = sb;
		current_index_ = 0;
	}

	const SharedBuffer::byte* data(const u32& index = 0) {
		if (current_index_ + index >= stream_.size()) {
			return 0;
		}
		return & stream_[current_index_ + index];
	}

	SharedBuffer::byte current(const u32& index = 0) {
		if (current_index_ + index >= stream_.size()) {
			return 0;
		}
		return stream_[current_index_ + index];
	}

	bool moveNext(const u32& index = 1) {
		if (current_index_ + index > stream_.size()) {
			return false;
		}

		current_index_ += index;
		return true;
	}

	u32 match(const SharedBuffer::byte& b) {
		for (u32 i = 0; ; ++i) {
			SharedBuffer::byte ch = current(i);
			if (!ch) return max_u32;
			if (ch == b) {
				return i;
			}
		}

		return max_u32;
	}

	u32 matchLine() {
		for (u32 i = 0; ; ++i) {
			SharedBuffer::byte ch = current(i);
			if (!ch || ch == '\r' || ch == '\n')
				return i;
		}

		return max_u32;
	}

	SharedBuffer getBuffer(const u32& from, const u32& to) {
		if (to <= from) return SharedBuffer();
		return stream_.slice(current_index_ + from, to - from);
	}
	SharedBuffer getBuffer(const u32& to) {
		return stream_.slice(current_index_ , to);
	}

	bool isMatch(const u32* keys) {
		if (!keys || *keys == 0) return false;

		for (u32 i = 0; i < _L_TOKEN_MAX_KEYS; ++i) {
			if (sizeof(u32) * (i + 1) + current_index_ >= stream_.size()) {
				return false;
			}

			if (*(keys + i) == 0)
				return true;

			if (*((const u32 *)(stream_.data() + current_index_ + (sizeof(u32) * i))) != *(keys + i))
				return false;
		}

		return true;
	}

	bool jumpLine() {
		for (u32 i = 0; ; ++i) {
			SharedBuffer::byte ch = current(i);
			if (ch == '\n') {
				return moveNext(i + 1);
			}
			else if (!ch) {
				return false;
			}
		}

		return false;
	}

	bool jumpSpace() {
		SharedBuffer::byte ch = current();

		while(ch == ' ' || ch == '	') {
			if (!moveNext()) return false;
			ch = current();
		}
		return true;
	}
};

/**
 * LexicalAnalyzer
 */
class HttpLexicalAnalyzer
{
	friend class HttpParser;

public:
	TokenElement* nextToken() {
		if (!chars_.jumpSpace()) return NULL;

		/**
		 * the header got end
		 */
		if (chars_.current() == '\n' || (chars_.current() == '\r' && chars_.current(1) == '\n'))
			return NULL;

		/**
		 * the token lib was just 10+ lines, so just went through all the recordings.
		 */
		for (u32 i = 1; TokensLib[i].token; ++i) {
			if (chars_.isMatch(TokensLib[i].keys)) {
				return & TokensLib[i];
			}
		}

		return & UnknownToken;
	}

	void setStream(const SharedBuffer& sb) {
		chars_.setStream(sb);
	}

private:
	Chars						chars_;
};

/**
 * HttpParser
 */
class HttpParser
{
public:
	HttpParser(HttpContext& context) : state_(STATE_NONE), context_(context) {}

	ret_type execute() {
		while(true) {
			switch (state_) {
			case STATE_NONE:
				if (!handleHeader()) return RET_ERROR;
				break;

			case STATE_UPLOADING:
			case STATE_HEADER:
				if (!handleBody()) return RET_ERROR;
				if (state_ == STATE_UPLOADING)
					return RET_CONTINUE;
				break;

			case STATE_HEADER_ERROR:
			case STATE_UPLOADING_ERROR:
				return RET_ERROR;

			case STATE_DONE:
				return RET_OK;
			}
		}

		return RET_CONTINUE;
	}

	void setStream(const SharedBuffer& sb) {
		analyzer_.setStream(sb);
	}

private:
	bool handleHeader() {
		state_ = STATE_HEADER;

		TokenElement* ptoken = analyzer_.nextToken();
		if (!ptoken || (ptoken->token != TOKEN_GET && ptoken->token != TOKEN_POST)) {
			ZCLOG(ERROR) << "Expect GET or POST token" << End;
			state_ = STATE_HEADER_ERROR;
			return false;
		}

		if (!ptoken->hanlder(analyzer_.chars_, context_.header)) {
			ZCLOG(ERROR) << "handler failed" << End;
			state_ = STATE_HEADER_ERROR;
			return false;
		}

		if (!analyzer_.chars_.jumpLine()) {
			ZCLOG(ERROR) << "Jump line failed" << End;
			state_ = STATE_HEADER_ERROR;
			return false;
		}

		while(true) {
			ptoken = analyzer_.nextToken();
			if (!ptoken) break;

			if (!ptoken->hanlder(analyzer_.chars_, context_.header)) {
				ZCLOG(ERROR) << "Token:" << (u32)ptoken->token << " handler failed" << End;
				state_ = STATE_HEADER_ERROR;
				return false;
			}

			if (!analyzer_.chars_.jumpLine()) {
				ZCLOG(ERROR) << "Jump line failed" << End;
				state_ = STATE_HEADER_ERROR;
				return false;
			}
		}

		if (!analyzer_.chars_.jumpLine()) {
			ZCLOG(ERROR) << "Jump line failed" << End;
			state_ = STATE_HEADER_ERROR;
			return false;
		}

		return true;
	}

	bool handleBody() {
		if (context_.header.method != METHOD_POST) {
			state_ = STATE_DONE;
			return true;
		}

		if (context_.header.content_length == 0) {
			ZCLOG(ERROR) << "Content-Length was 0" << End;
			state_ = STATE_UPLOADING_ERROR;
			return false;
		}

		context_.post.add(analyzer_.chars_.left());

		if (context_.post.size() >= context_.header.content_length) {
			state_ = STATE_DONE;
		}
		else {
			state_ = STATE_UPLOADING;
		}

		return true;
	}

private:
	state_type					state_;
	HttpContext&				context_;

private:
	HttpLexicalAnalyzer			analyzer_;
};


/**
 * Parse the http request
 */
ret_type parseHttp(const SharedBuffer& sb, HttpContext& context) {
	if (!context.parser) {
		context.parser = new HttpParser(context);
	}

	context.parser->setStream(sb);
	return context.parser->execute();
}

namespace helper {

static bool handleGetOrPost(const u32& offset, Chars& chars, HttpHeaderContext& header) {
	chars.moveNext(offset);
	u32 index = chars.match(' ');

	if (index == max_u32) {
		return false;
	}

	header.uri = chars.getBuffer(index);
	if (!chars.moveNext(index + 1)) {
		return false;
	}

#ifndef ZC_RELEASE
	if (chars.current(0) != 'H' || chars.current(1) != 'T' || chars.current(2) != 'T' ||
			chars.current(3) != 'P' || chars.current(4) != '/' || chars.current(5) != '1' || chars.current(6) != '.') {
		return false;
	}
#endif

	if (!chars.moveNext(7))
		return false;

	if (chars.current() == '1') {
		header.version = 11;
	}
	else if (chars.current() == '0') {
		header.version = 10;
	}

	return true;
}

static bool handleExpr(Chars& chars, u32& to) {
	u32 from = chars.match(':');
	if (from == max_u32)
		return false;

	chars.moveNext(from + 1);
	if (!chars.jumpSpace())
		return false;

	to = chars.matchLine();
	if (to == max_u32) {
		return false;
	}

	return true;
}

bool handleGet(Chars& chars, HttpHeaderContext& header) {
	header.method = METHOD_GET;
	return handleGetOrPost(4, chars, header);
}

bool handlePost(Chars& chars, HttpHeaderContext& header) {
	header.method = METHOD_POST;
	return handleGetOrPost(5, chars, header);
}

bool handleHost(Chars& chars, HttpHeaderContext& header) {
	u32 to = 0;
	if (!handleExpr(chars, to))
		return false;

	header.host = chars.getBuffer(to);
	return true;
}

bool handleUserAgent(Chars& chars, HttpHeaderContext& header) {
	u32 to = 0;
	if (!handleExpr(chars, to))
		return false;

	header.user_agent = chars.getBuffer(to);
	return true;
}

bool handleContentLength(Chars& chars, HttpHeaderContext& header) {
	u32 to = 0;
	if (!handleExpr(chars, to))
		return false;

	header.content_length = atoi((const char *) chars.data());
	return true;
}

bool handleAcceptLanguage(Chars& chars, HttpHeaderContext& header) {
	u32 to = 0;
	if (!handleExpr(chars, to))
		return false;

	header.accept_language = chars.getBuffer(to);
	return true;
}

bool handleAcceptEncoding(Chars& chars, HttpHeaderContext& header) {
	u32 to = 0;
	if (!handleExpr(chars, to))
		return false;

	header.accept_encoding = chars.getBuffer(to);
	return true;
}

bool handleAccept(Chars& chars, HttpHeaderContext& header) {
	u32 to = 0;
	if (!handleExpr(chars, to))
		return false;

	header.accept = chars.getBuffer(to);
	return true;
}

bool handleReferer(Chars& chars, HttpHeaderContext& header) {
	u32 to = 0;
	if (!handleExpr(chars, to))
		return false;

	header.referer = chars.getBuffer(to);
	return true;
}

bool handleCookie(Chars& chars, HttpHeaderContext& header) {
	u32 to = 0;
	if (!handleExpr(chars, to))
		return false;

	header.cookie = chars.getBuffer(to);
	return true;
}

bool handleConnection(Chars& chars, HttpHeaderContext& header) {
	u32 to = 0;
	if (!handleExpr(chars, to))
		return false;

	if (chars.current() == 'k') {
		header.keep_alive = true;
	}

	return true;
}

bool handleUnknown(Chars& chars, HttpHeaderContext& header) {
	return true;
}


}

}}

namespace zertcore { namespace utils {

HttpContext::~HttpContext() {
	if (parser) delete parser;
}

}}


