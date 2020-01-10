#include "stdafx.h"
#include "HttpLibWebServerAdapter/Server.h"

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

	namespace {
		struct ServerTestData
		{
			Request request;
			Reply reply;
		};

		std::vector<ServerTestData> testData =
		{
			{
				Request("GET", "/rest/api/health", {}, 1, 1, { {"Content-Type", "application/json"} }, ""),
				Reply(Reply::StatusType::OK, { {"Content-Type", "application/json"} }, "{ \"status\": \"running\" }")
			},
			{
				Request("POST", "/rest/api/users",{}, 1, 1,{ { "Content-Length", "25" },{ "Content-Type", "text/plain" } }, "Request content goes here"),
				Reply(Reply::StatusType::OK,{ { "Content-Type", "text/plain" },{ "Authorization", "Bearer 12345679012345689" } }, "Reply goes here")
			}
		};
	}

	class ServerTest : public TestWithParam<ServerTestData>
	{
	public:
		void SetUp()
		{
			setUpServer();
			setUpWebService();
			setUpHttpClient();
		}

		void TearDown()
		{
			m_httpClient.reset();
			m_server.reset();
		}

		void setUpServer()
		{
			m_hostAddress = "127.0.0.1";
			m_port = 9999;

			Configuration configuration;
			configuration.setPort(m_port);
			configuration.setThreadPoolSize(5);

			m_server = std::make_unique<Server>(configuration);
			m_server->start();

			while (!m_server->isRunning())
			{
			}
		}

		void setUpWebService()
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

		void setUpHttpClient()
		{
			m_httpClient = std::make_unique<HttpClient>();
		}

		Request addClientHeaders(const Request& request)
		{
			Request newRequest = request;
			newRequest.getHeaders().addHeader("Accept", "*/*");
			newRequest.getHeaders().addHeader("Connection", "close");
			newRequest.getHeaders().addHeader("Host", m_hostAddress + ":" + std::to_string(m_port));
			newRequest.getHeaders().addHeader("REMOTE_ADDR", m_hostAddress);
			newRequest.getHeaders().addHeader("User-Agent", "cpp-httplib/0.5");

			return newRequest;
		}

		Reply addServerHeaders(const Reply& reply)
		{
			Reply newReply = reply;
			newReply.addHeader("Accept-Ranges", "bytes");
			newReply.addHeader("Connection", "close");
			newReply.addHeader("Content-Length", std::to_string(reply.getContent().size()));

			return newReply;
		}

	protected:
		std::unique_ptr<Server> m_server;
		std::unique_ptr<HttpClient> m_httpClient;
		MockWebService* m_webService;

		std::string m_basePath;
		Reply m_defaultReply;

		std::string m_hostAddress;
		unsigned int m_port;
	};


	TEST_P(ServerTest, testSendRequestCallsWebService)
	{
		Request request = GetParam().request;
		Request expectedRequest = addClientHeaders(request);
		EXPECT_CALL(*m_webService, processProxy(isEqTo(expectedRequest)));

		m_httpClient->send(m_hostAddress, m_port, request);
	}

	TEST_P(ServerTest, testSendRequestReturnsExpectedReply)
	{
		m_defaultReply = GetParam().reply;
		auto reply = m_httpClient->send(m_hostAddress, m_port, GetParam().request);

		Reply expectedReply = addServerHeaders(m_defaultReply);
		ASSERT_TRUE(reply != NULL);
		EXPECT_TRUE(EntityComparator()(expectedReply, *reply));
	}

	INSTANTIATE_TEST_CASE_P(Server, ServerTest, testing::ValuesIn(testData));

}}}}
