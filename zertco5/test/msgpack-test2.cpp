/*
 * msgpack-test2.cpp
 *
 *  Created on: 2015年2月3日
 *      Author: Administrator
 */

#include <msgpack.hpp>
#include <vector>
#include <string>
#include <iostream>

void f() {
	;
}

int main() {
    // This is target object.
    std::vector<std::string> target;
    target.push_back("Hello,");
    target.push_back("World!");

    // Serialize it.
    msgpack::sbuffer sbuf;  // simple buffer
    msgpack::pack(&sbuf, target);

    std::cout << sbuf.size() << std::endl;

    // Deserialize the serialized data.
    msgpack::unpacked msg;    // includes memory pool and deserialized object
    msgpack::unpack(msg, sbuf.data(), sbuf.size());
    msgpack::object obj = msg.get();

    msgpack::sbuffer otherbuf;
    msgpack::pack(&otherbuf, obj);
    msgpack::unpacked othermsg;
    msgpack::unpack(othermsg, otherbuf.data(), otherbuf.size());
    msgpack::object otherobj = othermsg.get();

    // Print the deserialized object to stdout.
    std::cout << otherobj << std::endl;    // ["Hello," "World!"]

    // Convert the deserialized object to staticaly typed object.
    std::vector<std::string> result;
    obj.convert(&result);

}


