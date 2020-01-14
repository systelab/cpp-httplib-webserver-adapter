#pragma once

#include "WebServerAdapterInterface/Model/Request.h"
#include "WebServerAdapterInterface/Model/Reply.h"


namespace systelab { namespace web_server { namespace httplib { namespace test {

	struct ServerTestData
	{
		Request request;
		Reply reply;
	};

	class ServerTestDataBuilder
	{
	public:
		static std::vector<ServerTestData> build();
	};

}}}}
