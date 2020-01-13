#include "Server.h"

#include "Model/WebServicesMgr.h"
#include "Services/HttpLibRequestParserService.h"
#include "Services/HttpLibResponseBuilderService.h"
#include "Services/ReplyCORSHeadersBuilderService.h"
#include "Services/RequestHandlingService.h"

#include "WebServerAdapterInterface/IWebService.h"
#include "WebServerAdapterInterface/Model/Configuration.h"
#include "WebServerAdapterInterface/Model/Request.h"
#include "WebServerAdapterInterface/Model/Reply.h"


namespace systelab { namespace web_server { namespace httplib {

	Server::Server(const Configuration& configuration)
		:m_configuration(new Configuration(configuration))
		,m_webServicesMgr(new WebServicesMgr())
		,m_httpLibServer()
		,m_thread()
	{
	}

	Server::~Server()
	{
		stop();
	}

	void Server::registerWebService(std::unique_ptr<IWebService> webService)
	{
		m_webServicesMgr->addWebService(std::move(webService));
	}

	bool Server::isRunning() const
	{
		return m_httpLibServer && m_httpLibServer->is_running();
	}

	void Server::start()
	{
		if (isRunning())
		{
			throw std::runtime_error("Server already running");
		}

		if (m_thread.joinable())
		{
			m_thread.detach();
		}

		if (!m_httpLibServer)
		{
			m_httpLibServer = buildHttpLibServer();
		}

		std::thread t(std::bind(&Server::runThread, this));
		m_thread.swap(t);
	}

	void Server::stop()
	{
		if (m_httpLibServer && m_httpLibServer->is_running())
		{
			m_httpLibServer->stop();

			if (m_thread.joinable())
			{
				m_thread.join();
			}
		}
	}

	std::unique_ptr<::httplib::Server> Server::buildHttpLibServer() const
	{
		auto httpLibServer = std::unique_ptr<::httplib::Server>(new ::httplib::Server());
		configureRoutes(*httpLibServer);
		httpLibServer->set_gzip_compression_enabled(m_configuration->isGZIPCompressionEnabled());

		return httpLibServer;
	}

	void Server::configureRoutes(::httplib::Server& httpLibServer) const
	{
		httpLibServer.Get(".+", [this](const ::httplib::Request& request, ::httplib::Response& response)
		{
			this->handleRequest(request, response);
		});

		httpLibServer.Post(".+", [this](const ::httplib::Request& request, ::httplib::Response& response)
		{
			this->handleRequest(request, response);
		});

		httpLibServer.Put(".+", [this](const ::httplib::Request& request, ::httplib::Response& response)
		{
			this->handleRequest(request, response);
		});

		httpLibServer.Patch(".+", [this](const ::httplib::Request& request, ::httplib::Response& response)
		{
			this->handleRequest(request, response);
		});

		httpLibServer.Delete(".+", [this](const ::httplib::Request& request, ::httplib::Response& response)
		{
			this->handleRequest(request, response);
		});

		httpLibServer.Options(".+", [this](const ::httplib::Request& request, ::httplib::Response& response)
		{
			this->handleRequest(request, response);
		});
	}

	void Server::handleRequest(const ::httplib::Request& httpLibRequest, ::httplib::Response& httpLibResponse) const
	{
		HttpLibRequestParserService requestParserService;
		auto request = requestParserService.parse(httpLibRequest);

		std::unique_ptr<Reply> reply;
		if (request)
		{
			RequestHandlingService requestHandlingService(*m_webServicesMgr);
			reply = requestHandlingService.processRequest(*request);
		}
		else
		{
			reply = std::unique_ptr<Reply>(new Reply());
			reply->setStatus(Reply::BAD_REQUEST);
		}

		ReplyCORSHeadersBuilderService replyCORSHeadersBuilderService(m_configuration->getCORSConfiguration());
		replyCORSHeadersBuilderService.addCORSHeaders(*request, *reply);

		HttpLibResponseBuilderService responseBuilderService;
		responseBuilderService.build(*reply, httpLibResponse);
	}

	void Server::runThread()
	{
		std::string hostAddress = m_configuration->getHostAddress();
		unsigned int port = m_configuration->getPort();
		m_httpLibServer->listen(hostAddress.c_str(), port);
	}

}}}
