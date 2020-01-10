#include "ServerFactory.h"

#include "SecuredServer.h"
#include "Server.h"

#include "WebServerAdapterInterface/Model/Configuration.h"
#include "WebServerAdapterInterface/Model/SecurityConfiguration.h"


namespace systelab { namespace web_server { namespace httplib {

	ServerFactory::ServerFactory()
	{
	}

	ServerFactory::~ServerFactory() = default;

	std::unique_ptr<IServer> ServerFactory::buildServer(const Configuration& configuration) const
	{
		const SecurityConfiguration& securityConfiguration = configuration.getSecurityConfiguration();
		if (securityConfiguration.isHTTPSEnabled())
		{
			return std::make_unique<SecuredServer>(configuration);
		}
		else
		{
			return std::make_unique<Server>(configuration);
		}
	}

}}}
