#include "stdafx.h"
#include "HttpLibWebServerAdapter/Services/RequestURIParserService.h"

#include "WebServerAdapterInterface/Model/Request.h"
#include "WebServerAdapterTestUtilities/Builders/RequestQueryStringsBuilder.h"
#include "TestUtilitiesInterface/EntityComparator.h"

#include <string>
#include <memory>


using namespace systelab::test_utility;
using namespace systelab::web_server::test_utility;

namespace systelab { namespace web_server { namespace httplib { namespace test {

	struct RequestURIParserServiceTestData
	{
		std::string inputURI;
		bool expectedResult;
		std::string expectedURI;
		RequestQueryStrings expectedQueryStrings;
	};

	RequestURIParserServiceTestData requestURIParserServiceTestData[] =
	{
		// Valid URIs
		{ "/rest/api/users/1",				true,		"/rest/api/users/1",		RequestQueryStrings() },
		{ "/rest/api/users/1+1",			true,		"/rest/api/users/1 1",		RequestQueryStrings() },
		{ "/rest/api/users/1++1",			true,		"/rest/api/users/1  1",		RequestQueryStrings() },
		{ "/rest/api/users/1%20",			true,		"/rest/api/users/1 ",		RequestQueryStrings() },
		{ "/rest/api/users/1%201",			true,		"/rest/api/users/1 1",		RequestQueryStrings() },

		// Valid URIs with query strings
		{
			"/rest/api/users?",				true,		"/rest/api/users",
			RequestQueryStrings()
		},
		{
			"/rest/api/users?id=1",			true,		"/rest/api/users",
			RequestQueryStringsBuilder().addItem("id", "1").getEntity()
		},
		{
			"/rest/api/users?id=ABC",		true,		"/rest/api/users",
			RequestQueryStringsBuilder().addItem("id", "ABC").getEntity()
		},
		{
			"/rest/api/users?id=..",		true,		"/rest/api/users",
			RequestQueryStringsBuilder().addItem("id", "..").getEntity()
		},
		{
			"/rest/api/users?id1=A&id2=B",	true,		"/rest/api/users",
			RequestQueryStringsBuilder().addItem("id1", "A").addItem("id2", "B").getEntity()
		},

		// Invalid URIS
		{ "/rest/api/users/%",				false,		"",							RequestQueryStrings() },
		{ "/rest/api/users/%2",				false,		"",							RequestQueryStrings() },
		{ "/rest/api/users/%XX",			false,		"",							RequestQueryStrings() },
		{ "/rest/api/users/%G2",			false,		"",							RequestQueryStrings() },
		{ "/../files/images",				false,		"",							RequestQueryStrings() },
		{ "/../files/images?id=1",			false,		"",							RequestQueryStrings() },
		{ "",								false,		"",							RequestQueryStrings() },
		{ "\\",								false,		"",							RequestQueryStrings() },
		{ "rest/api/users/1",				false,		"",							RequestQueryStrings() },
	};


	class RequestURIParserServiceTest : public ::testing::TestWithParam<RequestURIParserServiceTestData>
	{
	public:
		void SetUp()
		{
			m_service = std::make_unique<RequestURIParserService>();
		}

	protected:
		std::unique_ptr<RequestURIParserService> m_service;
	};


	TEST_P(RequestURIParserServiceTest, testParse)
	{
		Request request;
		request.setURI(GetParam().inputURI);
		bool result = m_service->parse(request);
		ASSERT_EQ(GetParam().expectedResult, result);

		if (result)
		{
			ASSERT_EQ(GetParam().expectedURI, request.getURI());
			ASSERT_TRUE(EntityComparator()(GetParam().expectedQueryStrings, request.getQueryStrings()));
		}
	}

	INSTANTIATE_TEST_CASE_P(WebServer, RequestURIParserServiceTest, testing::ValuesIn(requestURIParserServiceTestData));

}}}}
