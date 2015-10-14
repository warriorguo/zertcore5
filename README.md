This is my own server framework covering the mainly requests of my work.
It has evolved into the 5 version. The last version 4 framework was used in a mobile game named Clash of Leagues, but it got deprecated since it lacks of many core modules such as multi-thread, RPC etc.

The goals of this framework:
	1. Define the way to develop server application.
	2. Speed is important but Safe & Stable is more important.
	3. Good framework guide good coding habbit.
	4. Try best to hide the details of multi-thread, multi-server communication, make the functions easy to understand and use.

Features:

**Serialization**:
I have done many work to make the data serialization precisely and easy to use. 
Currently, it supports BSON(using Mongodb) and Msgpack.

	Serializer<BSONIStream> in;
	Unserializer<BSONOStream> out;

or

	Serializer<MsgpackIStream> in;
	Unserializer<MsgpackOStream> out;

for using of in & out was very simple,

	in["key1"] & 1;
	in["key2"] & "hello there";
	in["key3"]["sub_key1"]["sub_key2"] & 3.14159265;
	
	// in was something like {"key1":1,"key2":"hello there","key3":{"sub_key1":{"sub_key2":3.14159265}}}
	
	out.buffer(in.buffer()); //in.buffer() return the byte stream and out.buffer() read the stream
	
	int v1;
	string v2;
	double v3;
	out["key1"] & v1; // 1
	out["key2"] & v2; // "hello there"
	out["key3"]["sub_key1"]["sub_key2"] & v3; // 3.14159265

It support the staple STL containers.

	map<string, int> v4;
	v4["map_key1"] = 1;
	v4["map_key2"] = 2;
	v4["map_key3"] = 3;
	
	in["key4"] & v4;
	
	vector<int> v5;
	v5.push_back(4);
	v5.push_back(5);
	v5.push_back(6);
	
	in["key5"] & v5;
	
	set<string> v6;
	v6.insert("s1");
	v6.insert("s2");
	
	in["key6"] & v6;
	
	v4.clear();
	v5.clear();
	v6.clear();
	
	out.buffer(in.buffer());
	
	out["key4"] & v4; // map_key1 -> 1, map_key2 -> 2, map_key3 -> 3
	out["key5"] & v5; // [1,2,3]
	out["key6"] & v6; // ["s1","s2"]
	
It also very easy to define your own structure to support serialization

	struct Person : Serializable<Person>, Unserializable<Person>
	{
		int id;
		string name;
		int age;
	
		template <class Archiver>
		void serialize(Archiver& archiver) const {
			archiver["id"] & id;
			archiver["name"] & name;
			archiver["age"] & age;
		}
		
		template <class Archiver>
		bool unserialize(Archiver& archiver) {
			archiver["id"] & id;
			archiver["name"] & name;
			archiver["age"] & age;

			return id > 0;
		}
		
	};
	
	Person p1, p2;
	p1.id = 1; p1.name = "Peter"; p1.age = 23;
	p2.id = 2; p2.name = "Marry"; p2.age = 21;
	in["p1"] & p1;
	in["p2"] & p2;
	
	out.buffer(in.buffer());
	
	memset(&p1, 0, sizeof(p1));
	memset(&p2, 0, sizeof(p2));
	out["p1"] & p1; //p1.id = 1; p1.name = "Peter"; p1.age = 23;
	out["p2"] & p2; //p2.id = 2; p2.name = "Marry"; p2.age = 21;

*SerializableObject*, a somehow syntactic sugar to make things more lightly.

	// you could change the Person struct like this
	struct Person : public SerializableObject
	{
		ZCVAL(int, id); // the key would be id
		ZCVAL(string, name); // the key would be name
		ZCVAL(int, age); // the key would be age
	};

**Thread**:
I made a rule that the application must define how many threads it would take before the initial the thread pool and DO NOT support dynamic launch new thread then.
Every thread was hold a task priority list, any task would be running in the thread would push into the list. 

*ThreadHandler*, a TR1 function like template class that support thread.
	
	ThreadHandler<void (int, string)> thread_handler; // the handler running thread was default to the thread not declare
	thread_handler.setThreadIndex(1); // or set it to run in the thread 1,
	thread_handler.setParams(1234, "this was a string parameter");
	thread_handler.push(); // push it to thread 1 list
	
ThreadSingleton, a singleton for each thread.
	
	class Derived : public ThreadSingleton<Derived> {};

**RPC**:
Based on Network, Thread and Serialization, RPC now were support two way to send & receive message between servers.

*Call*, was the classical way to call the remote functions, in the server side, bind a function with a key, and the client call the key and get the return value.
	
	//rpc::oarchiver_type was Unserializer<BSONOStream>
	//rpc::iarchiver_type was Serializer<BSONIStream>
	
	//The reason why using BSON not Msgpack is the BSON supported BINDATA
	
	RPC.registerRPCHandler("echo", [] (key_type, rpc::oarchiver_type params, rpc::iarchiver_type& ret_data) {
		string text;
		params["text"] & text;
		ret_data["text"] & text;
	});

to call the *echo*,

	rpc::iarchiver_type in;
	in["text"] & "hello there";
		
	RPC.asyncCall("echo", in, [] (key_type key, Error error, rpc::oarchiver_type out) {
		// handle the callback
		string text;
		out["text"] & text;

		if (error) {
			ZCLOG(NOTE) << "Got Error:" << error << End;
		}

		ZCLOG(NOTE) << "Got return with key=" << key << " ret=" << text << End;
	});

*Notify*, some something like publisher & subscriber way.

	RPC.registerDataSyncHandler("chat", [](key_type, rpc::oarchiver_type out) {
		string text;
		out["msg"] & text;
		
		ZCLOG(NOTE) << "Got a message :" << text << End;
	});
	
You would run the code in many servers.
	
to notify the *chat*,
	
	rpc::iarchiver_type in;
	in["msg"] & "some message here";
	RPC.notify("chat", in);
	
All the servers would receive the "some message here" message!

Notice, *Call* mechanism could bind many handlers with same key. And the Call API provided more strategies.
For instance.

	RPC.registerRPCHandler("write_object", [] (key_type, rpc::oarchiver_type params, rpc::iarchiver_type& ret_data) {
		// dump the object data to database.
	});
	
The code was running in many servers, they all would handle the "write_object" action.
	
	Person someone;
	someone.id = 1;
	rpc::iarchiver_type in;
	in & someone;
	
The SAME ID would lead to the SAME remote handler!
	
	RPC.asyncCall("write_object", in, [] (key_type key, Error error, rpc::oarchiver_type out) {
		// handle the callback
	}, someone.id, rpc::RPCIDBaseFetcher()); // <- NOTICE here
			
The strategy would call the remote one which reacted the fastest.
			
	RPC.asyncCall("write_object", in, [] (key_type key, Error error, rpc::oarchiver_type out) {
		// handle the callback
	}, 0, rpc::RPCLessReactTimeFetcher()); // <- NOTICE here
			
The strategy would call ALL those remote handlers on the same time
			
	RPC.asyncCall("write_object", in, [] (key_type key, Error error, rpc::oarchiver_type out) {
		// handle the callback
	}, 0, rpc::RPCAllFetcher()); // <- NOTICE here


**ActiveObject**:
Based on *PoolObject*, *Coroutine*, *Serialization*, *Database* and *RPC*, *ActiveObject* provide a easy way to manage the Object that easy to read & write from database, cache, and transfer through network.

Setting:
	class TestObject;

	template <>
	struct ActiveObjectTraits<TestObject> : public ObjectTraits<TestObject>
	{
		typedef uuid_t id_type;

		static const char* TABLE_NAME;
		static const char* RPC_NAME;
		static const char* SYNC_NAME;

		static const tick_type DefaultExpiredTick; // default 10 second
		// if the object didnt do any thing, 10s later it would be erase from memory 
	};

	const char* ActiveObjectTraits<TestObject>::TABLE_NAME = "test_tb"; // read & write it via Database, set the table name
	const char* ActiveObjectTraits<TestObject>::RPC_NAME = NULL;	// read & write it via RPC, the key name
	const tick_type	ActiveObjectTraits<TestObject>::DefaultExpiredTick = 100000;
	const char* ActiveObjectTraits<TestObject>::SYNC_NAME = "test_object"; // do the Synchronization

Define the object,

	class TestObject :
		public ActiveObject<TestObject>
	{
	public:
		virtual bool init() {return true;}

	public:
		string getName() const {return name_;}
		void setName(const string& name) {name_ = name;}

	public:
		template <class Archiver>
		void serialize(Archiver& archiver) const {
			archiver["age"] & age_;
			archiver["name"] & name_;
		}

		template <class Archiver>
		bool unserialize(Archiver& archiver) {
			return
				(archiver["age"] & age_) &&
				(archiver["name"] & name_);
		}

	private:
		u32 age_;
		string name_;

		ZC_TO_STRING("age" << age_ << "name" << name_)
	};

Define the object manager,

	#define FROM_MONGODB 1
	class TestObjectManager : public ActiveObjectManager<TestObjectManager, TestObject>
	{
	public:
		enum {
			THREAD_INDEX = 2,  // the manager would running the thread 2
		};
	public:
		virtual void init() {
			ActiveObjectManager<TestObjectManager, TestObject>::init();
			
			 // add Mongodb data support
			dp_manager::Instance().reg(FROM_MONGODB, // Data provide Index
				new io::MongoDBDataProvider<TestObject>);
		}
	};

how to use:

	uuid_t id = 1; // read id = 1 recording from database
	
Create:
	
	TestObject::ptr obj = TestObjectManager::Instance().create(i, FROM_MONGODB);
	
Get:

	obj = TestObjectManager::Instance().get(id, FROM_MONGODB);
	
If the object were existed in cache, it would refresh the expired timer and return directly.
Otherwise, NOTICE, it would make a request to provider, and the provider would work in OTHER THREAD. and here would YEILD, the *Coroutine* would JUMP to another task in the thread list until the provider responsed.

Save:

	obj->save(id);
	
How it would save the way denpending on the way you get&create it, here it save to the mongodb provider.

Reload:
	
	obj->reload(id);
	
*Synchronization*, it was a very interesting idea that the *ActiveObject* implemented. It provided *ActiveObject* synchronize data in Master & Slave mode, and as the usual, it was very easy to use.

in the one server,

	obj->setMaster();
	obj->sync();

in others servers,

	obj->setSlave();
	obj->sync();
	
the `obj` get synchronized now!

*More feature* on the TODO list:
Simple Http Server:
	this method will be rewritten.
Event Bus:
	this method will be redesigned. 

And waiting for more!

