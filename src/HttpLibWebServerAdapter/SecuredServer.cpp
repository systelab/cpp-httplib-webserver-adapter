#include "SecuredServer.h"

#include "WebServerAdapterInterface/Model/Configuration.h"


namespace systelab { namespace web_server { namespace httplib {

	SecuredServer::SecuredServer(const Configuration& configuration)
		:Server(configuration)
	{
	}

	SecuredServer::~SecuredServer()
	{
		stop();
	}

	std::unique_ptr<::httplib::Server> SecuredServer::buildHttpLibServer() const
	{
		auto httpLibServer = std::unique_ptr<::httplib::Server>(new ::httplib::Server());
		configureRoutes(*httpLibServer);

		return httpLibServer;
	}

}}}
