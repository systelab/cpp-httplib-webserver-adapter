#include "stdafx.h"
#include "HttpLibWebServerAdapter/Services/RequestHandlingService.h"

#include "HttpLibWebServerAdapter/Model/WebServicesMgr.h"

#include "WebServerAdapterInterface/Model/Request.h"
#include "WebServerAdapterInterface/Model/Reply.h"

#include "WebServerAdapterTestUtilities/Mocks/MockWebService.h"

#include "TestUtilitiesInterface/EntityComparator.h"

#include <string>
#include <memory>


using namespace testing;
using namespace systelab::test_utility;
using namespace systelab::web_server::test_utility;

namespace systelab { namespace web_server { namespace httplib { namespace test {

	class RequestHandlingServiceTest : public ::testing::Test
	{
	public:
		void SetUp()
		{
			setUpWebServicesMgr();

			m_service = std::make_unique<RequestHandlingService>(m_webServicesMgr);
		}

		void setUpWebServicesMgr()
		{
			m_defaultGetReply.setStatus(Reply::OK);
			m_webService1 = new MockWebService();
			ON_CALL(*m_webService1, processProxy(_)).WillByDefault(Invoke(
				[this](const Request& request) -> Reply*
				{
					return (request.getMethod() == "GET") ? new Reply(m_defaultGetReply) : NULL;
				}
			));

			m_defaultPostReply.setStatus(Reply::CREATED);
			m_webService2 = new MockWebService();
			ON_CALL(*m_webService2, processProxy(_)).WillByDefault(Invoke(
				[this](const Request& request) -> Reply*
				{
					return (request.getMethod() == "POST") ? new Reply(m_defaultPostReply) : NULL;
				}
			));

			m_webServicesMgr.addWebService(std::unique_ptr<MockWebService>(m_webService1));
			m_webServicesMgr.addWebService(std::unique_ptr<MockWebService>(m_webService2));
		}

	protected:
		std::unique_ptr<RequestHandlingService> m_service;
		WebServicesMgr m_webServicesMgr;

		MockWebService* m_webService1;
		MockWebService* m_webService2;
		Reply m_defaultGetReply;
		Reply m_defaultPostReply;
	};


	TEST_F(RequestHandlingServiceTest, testProcessRequestForGETReturnsDefaultGetReply)
	{
		Request request;
		request.setMethod("GET");
		auto reply = m_service->processRequest(request);
		ASSERT_TRUE(reply != NULL);
		ASSERT_TRUE(EntityComparator()(m_defaultGetReply, *reply));
	}

	TEST_F(RequestHandlingServiceTest, testProcessRequestForPOSTReturnsDefaultPostReply)
	{
		Request request;
		request.setMethod("POST");
		auto reply = m_service->processRequest(request);
		ASSERT_TRUE(reply != NULL);
		ASSERT_TRUE(EntityComparator()(m_defaultPostReply, *reply));
	}

	TEST_F(RequestHandlingServiceTest, testProcessRequestForDELETEReturnsNotFoundReply)
	{
		Request request;
		request.setMethod("DELETE");
		auto reply = m_service->processRequest(request);

		Reply expectedReply;
		expectedReply.setStatus(Reply::NOT_FOUND);
		ASSERT_TRUE(reply != NULL);
		ASSERT_TRUE(EntityComparator()(expectedReply, *reply));
	}

}}}}
