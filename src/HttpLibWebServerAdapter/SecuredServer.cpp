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
		std::unique_ptr<::httplib::SSLServer> httpLibServer;

		const auto& securityConfiguration = m_configuration->getSecurityConfiguration();
		std::string serverCertificate = securityConfiguration.getServerCertificate();
		std::string serverPrivateKey = securityConfiguration.getServerPrivateKey();
		std::string serverDHParam = securityConfiguration.getServerDHParam();
		if (securityConfiguration.isMutualSSLEnabled())
		{
			std::string clientCertificate = securityConfiguration.getClientCertificate();
			httpLibServer.reset(new ::httplib::SSLServer(serverCertificate, serverPrivateKey, serverDHParam, clientCertificate));
		}
		else
		{
			httpLibServer.reset(new ::httplib::SSLServer(serverCertificate, serverPrivateKey, serverDHParam, ""));
		}
 
		configureRoutes(*httpLibServer);
		httpLibServer->set_gzip_compression_enabled(m_configuration->isGZIPCompressionEnabled());

		return httpLibServer;
	}

}}}
