#include "stdafx.h"
#include "BaseServerTest.h"
#include "ServerTestData.h"

#include "HttpLibWebServerAdapter/Client.h"
#include "HttpLibWebServerAdapter/Server.h"

#include "TestUtilitiesInterface/EntityComparator.h"


using namespace testing;
using namespace systelab::test_utility;

namespace systelab { namespace web_server { namespace httplib { namespace test {

	class ServerTest : public BaseServerTest
	{
	public:
		void SetUp()
		{
			BaseServerTest::SetUp();
		}

		std::unique_ptr<IServer> buildServer(Configuration& configuration) override
		{
			return std::make_unique<Server>(configuration);
		}

		std::unique_ptr<IClient> buildClient(const std::string& hostAddress, unsigned int port) override
		{
			return std::make_unique<Client>(hostAddress, port);
		}
	};

	TEST_F(ServerTest, testSetupSecondServerThrowsException)
	{
		Configuration configuration;
		configuration.setPort(m_port);
		configuration.setThreadPoolSize(5);

		auto secondServer = buildServer(configuration);
		EXPECT_THROW(secondServer->start(), std::runtime_error);
	}

	class ParametricServerTest : public ServerTest,
								 public WithParamInterface<ServerTestData>
	{};

	TEST_P(ParametricServerTest, testSendRequestCallsWebService)
	{
		Request request = GetParam().request;
		Request expectedRequest = addClientHeaders(request);
		EXPECT_CALL(*m_webService, processProxy(isEqTo(expectedRequest)));

		m_httpClient->send(request);
	}

	TEST_P(ParametricServerTest, testSendRequestReturnsExpectedReply)
	{
		m_defaultReply = GetParam().reply;
		auto reply = m_httpClient->send(GetParam().request);

		Reply expectedReply = addServerHeaders(m_defaultReply);
		ASSERT_TRUE(reply != NULL);
		EXPECT_TRUE(EntityComparator()(expectedReply, *reply));
	}

	INSTANTIATE_TEST_CASE_P(Server, ParametricServerTest, ValuesIn(ServerTestDataBuilder::build()));

}}}}
