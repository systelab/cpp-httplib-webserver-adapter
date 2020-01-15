#include "ClientFactory.h"

#include "Client.h"
#include "SecuredClient.h"


namespace systelab { namespace web_server { namespace httplib {

	ClientFactory::ClientFactory() = default;
	ClientFactory::~ClientFactory() = default;

	std::unique_ptr<IClient> ClientFactory::buildHTTPClient(const std::string& serverAddress, unsigned int port) const
	{
		return std::make_unique<Client>(serverAddress, port);
	}

	std::unique_ptr<IClient> ClientFactory::buildHTTPSClient(const std::string& serverAddress, unsigned int port) const
	{
		return std::make_unique<SecuredClient>(serverAddress, port);
	}

}}}
