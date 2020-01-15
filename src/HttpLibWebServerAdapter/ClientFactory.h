#pragma once

#include <memory>
#include <string>

#include "WebServerAdapterInterface/IClientFactory.h"


namespace systelab { namespace web_server {
	class IClient;
}}

namespace systelab { namespace web_server { namespace httplib {

	class ClientFactory : public IClientFactory
	{
	public:
		ClientFactory();
		virtual ~ClientFactory();

		std::unique_ptr<IClient> buildHTTPClient(const std::string& serverAddress, unsigned int port) const override;
		std::unique_ptr<IClient> buildHTTPSClient(const std::string& serverAddress, unsigned int port) const override;
	};

}}}

