#include <iostream>
#include <stdexcept>

#include "WebServerAdapterInterface/IServer.h"
#include "WebServerAdapterInterface/Model/Configuration.h"

#include "HttpLibWebServerAdapter/ServerFactory.h"



int main(int argc, char *argv[])
{
	systelab::web_server::Configuration configuration;
	configuration.setHostAddress("127.0.0.1");
	configuration.setPort(8080);
	configuration.setThreadPoolSize(5);
	
	systelab::web_server::httplib::ServerFactory serverFactory;
	auto server = serverFactory.buildServer(configuration);
	std::cout << "Server built on port" << configuration.getPort() << std::endl;
	std::cout << "Running: " << server->isRunning() << std::endl;

    return 0;
}
