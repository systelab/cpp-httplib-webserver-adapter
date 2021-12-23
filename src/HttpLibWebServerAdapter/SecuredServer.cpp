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
		std::string serverCertificate = securityConfiguration.getServerCertificate()();
		std::string serverPrivateKey = securityConfiguration.getServerPrivateKey()();
		std::string serverDHParam = securityConfiguration.getServerDHParam()();
		unsigned int tlsSupportMask = buildTLSSupportMask(securityConfiguration);
		if (securityConfiguration.isMutualSSLEnabled())
		{
			std::string clientCertificate = securityConfiguration.getClientCertificate();
			httpLibServer.reset(new ::httplib::SSLServer(serverCertificate, serverPrivateKey, serverDHParam, clientCertificate, tlsSupportMask));
		}
		else
		{
			httpLibServer.reset(new ::httplib::SSLServer(serverCertificate, serverPrivateKey, serverDHParam, "", tlsSupportMask));
		}
 
		configureRoutes(*httpLibServer);
		httpLibServer->set_gzip_compression_enabled(m_configuration->isGZIPCompressionEnabled());

		return httpLibServer;
	}

	unsigned int SecuredServer::buildTLSSupportMask(const SecurityConfiguration& securityConfiguration) const
	{
		unsigned int tlsSupportMask = 0;
		if (!securityConfiguration.isTLSv10Enabled())
		{
			tlsSupportMask |= SSL_OP_NO_TLSv1;
		}

		if (!securityConfiguration.isTLSv11Enabled())
		{
			tlsSupportMask |= SSL_OP_NO_TLSv1_1;
		}

		if (!securityConfiguration.isTLSv12Enabled())
		{
			tlsSupportMask |= SSL_OP_NO_TLSv1_2;
		}

#if (OPENSSL_VERSION_NUMBER >= 0x10101000)
		if (!securityConfiguration.isTLSv13Enabled())
		{
			tlsSupportMask |= SSL_OP_NO_TLSv1_3;
		}
#else
		if (securityConfiguration.isTLSv13Enabled())
		{
			throw std::runtime_error("TLS v1.3 is not supported before OpenSSL 1.1.1");
		}
#endif

		return tlsSupportMask;
	}

}}}
