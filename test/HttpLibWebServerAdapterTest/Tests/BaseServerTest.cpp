#include "stdafx.h"
#include "BaseServerTest.h"

#include "Helpers/HttpClient.h"

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

	void BaseServerTest::SetUp()
	{
		setUpServer();
		setUpWebService();
		setUpHttpClient();
	}

	void BaseServerTest::TearDown()
	{
		m_httpClient.reset();
		m_server.reset();
	}

	void BaseServerTest::setUpServer()
	{
		m_hostAddress = "127.0.0.1";
		m_port = 9999;

		Configuration configuration;
		configuration.setPort(m_port);
		configuration.setThreadPoolSize(5);

		m_server = buildServer(configuration);
		m_server->start();

		while (!m_server->isRunning())
		{
		}
	}

	void BaseServerTest::setUpWebService()
	{
		m_defaultReply.setStatus(Reply::OK);
		m_defaultReply.setContent("Expected reply goes here");
		m_defaultReply.addHeader("Content-Type", "text/plain");
		m_defaultReply.addHeader("Authorization", "Bearer 12345679012345689");

		auto webService = std::make_unique<MockWebService>();
		ON_CALL(*webService, processProxy(_)).WillByDefault(Invoke(
			[this](const Request& request) -> Reply*
			{
				return new Reply(m_defaultReply);
			}
		));

		m_webService = webService.get();
		m_server->registerWebService(std::move(webService));
	}

	void BaseServerTest::setUpHttpClient()
	{
		m_httpClient = buildClient();
	}

	Request BaseServerTest::addClientHeaders(const Request& request)
	{
		Request newRequest = request;
		newRequest.getHeaders().addHeader("Accept", "*/*");
		newRequest.getHeaders().addHeader("Connection", "close");
		newRequest.getHeaders().addHeader("Host", m_hostAddress + ":" + std::to_string(m_port));
		newRequest.getHeaders().addHeader("REMOTE_ADDR", m_hostAddress);
		newRequest.getHeaders().addHeader("User-Agent", "cpp-httplib/0.5");
		newRequest.getHeaders().addHeader("Content-Length", std::to_string(request.getContent().size()));

		if (!newRequest.getHeaders().hasHeader("Content-Type") && 
			(request.getMethod() != "GET") &&
			(request.getMethod() != "OPTIONS"))
		{
			newRequest.getHeaders().addHeader("Content-Type", "text/plain");
		}

		return newRequest;
	}

	Reply BaseServerTest::addServerHeaders(const Reply& reply)
	{
		Reply newReply = reply;
		newReply.addHeader("Accept-Ranges", "bytes");
		newReply.addHeader("Connection", "close");
		newReply.addHeader("Content-Length", std::to_string(reply.getContent().size()));
		if (!newReply.hasHeader("Content-Type"))
		{
			newReply.addHeader("Content-Type", "text/plain");
		}

		return newReply;
	}

}}}}
