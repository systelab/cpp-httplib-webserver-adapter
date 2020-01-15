#pragma once

#include <memory>
#include <string>

#include "WebServerAdapterInterface/IClient.h"


namespace httplib {
	struct Response;
}

namespace systelab { namespace web_server { namespace httplib {

	class Client : public IClient
	{
	public:
		Client(const std::string& serverAddress, unsigned int port);
		virtual ~Client();

		virtual std::unique_ptr<Reply> send(const Request&) const override;

	protected:
		std::unique_ptr<Reply> buildReply(::httplib::Response) const;

	protected:
		std::string m_serverAddress;
		unsigned int m_port;
	};

}}}

