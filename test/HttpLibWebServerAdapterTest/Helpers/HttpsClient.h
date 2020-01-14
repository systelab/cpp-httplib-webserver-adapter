#pragma once

#include "HttpClient.h"


namespace systelab { namespace web_server { namespace httplib { namespace test {

	class HttpsClient : public HttpClient
	{
	public:
		HttpsClient();
		virtual ~HttpsClient();

		std::unique_ptr<Reply> send(const std::string& serverAddress, unsigned int port, const Request&) const;
	};

}}}}

