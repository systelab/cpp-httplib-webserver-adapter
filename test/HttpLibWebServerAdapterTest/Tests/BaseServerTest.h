#pragma once

#include "WebServerAdapterInterface/IClient.h"
#include "WebServerAdapterInterface/IServer.h"
#include "WebServerAdapterInterface/IWebService.h"
#include "WebServerAdapterInterface/Model/Configuration.h"
#include "WebServerAdapterInterface/Model/Request.h"
#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/SecurityConfiguration.h"

#include "WebServerAdapterTestUtilities/Mocks/MockWebService.h"

#include "TestUtilitiesInterface/EntityComparator.h"


using namespace testing;
using namespace systelab::test_utility;
using namespace systelab::web_server::test_utility;

namespace systelab { namespace web_server { namespace httplib { namespace test {

	class BaseServerTest : public Test
	{
	public:
		void SetUp();
		void TearDown();

		void setUpServer();
		void setUpWebService();
		void setUpHttpClient();

		virtual std::unique_ptr<IServer> buildServer(Configuration&) = 0;
		virtual std::unique_ptr<IClient> buildClient(const std::string& hostAddress, unsigned int port) = 0;

		Request addClientHeaders(const Request&);
		Reply addServerHeaders(const Reply&);

	protected:
		std::unique_ptr<IServer> m_server;
		std::unique_ptr<IClient> m_httpClient;
		MockWebService* m_webService;

		std::string m_basePath;
		Reply m_defaultReply;

		std::string m_hostAddress;
		unsigned int m_port;
	};

}}}}
