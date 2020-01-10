#pragma once

#include <exception>
#include <string>
#include <vector>
#include <httplib/httplib.h>

#include "WebServerAdapterInterface/IServer.h"


namespace systelab { namespace web_server {
	class Configuration;
}}

namespace systelab { namespace web_server { namespace httplib {

	class WebServicesMgr;

	class Server : public IServer
	{
	public:
		Server(const Configuration&);
		virtual ~Server();

		void registerWebService(std::unique_ptr<IWebService>) override;

		bool isRunning() const override;
		void start() override;
		void stop() override;

	protected:
		void runThread();

		virtual std::unique_ptr<::httplib::Server> buildHttpLibServer() const;
		void configureRoutes(::httplib::Server&) const;
		void handleRequest(const ::httplib::Request&, ::httplib::Response&) const;

	protected:
		std::unique_ptr<Configuration> m_configuration;
		std::unique_ptr<WebServicesMgr> m_webServicesMgr;
		std::unique_ptr<::httplib::Server> m_httpLibServer;

		bool m_running;
		std::thread m_thread;
	};

}}}

