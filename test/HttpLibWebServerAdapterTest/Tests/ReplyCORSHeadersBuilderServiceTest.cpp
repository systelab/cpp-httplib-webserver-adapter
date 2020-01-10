#include "stdafx.h"
#include "HttpLibWebServerAdapter/Services/ReplyCORSHeadersBuilderService.h"

#include "WebServerAdapterInterface/Model/CORSConfiguration.h"
#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"
#include "WebServerAdapterInterface/Model/RequestHeaders.h"

#include "WebServerAdapterTestUtilities/Builders/CORSConfigurationBuilder.h"

#include <string>
#include <memory>


using namespace systelab::web_server::test_utility;

namespace systelab { namespace web_server { namespace httplib { namespace test {

	struct ReplyCORSHeadersBuilderServiceTestData
	{
		CORSConfiguration corsConfiguration;
		std::string requestOrigin;
		std::map<std::string, std::string> expectedReplyHeaders;
	};

	ReplyCORSHeadersBuilderServiceTestData replyCORSHeadersBuilderServiceTestData[] =
	{
		{
			CORSConfigurationBuilder().setEnabled(false).getEntity(),
			std::string(""),
			{}
		},
		{
			CORSConfigurationBuilder().setEnabled(true).getEntity(),
			std::string(""),
			{}
		},
		{
			CORSConfigurationBuilder()
				.setEnabled(true).setAllowedCredentials(true).setMaxAge(123456)
				.getEntity(),
			std::string("http://127.0.0.1:4200"),
			{
				{ "Access-Control-Expose-Headers", "" },
				{ "Access-Control-Allow-Headers", "" },
				{ "Access-Control-Allow-Methods", "" },
				{ "Access-Control-Allow-Credentials", "true" },
				{ "Access-Control-Max-Age", "123456" },
			}
		},
		{
			CORSConfigurationBuilder()
				.setEnabled(true).setAllowedCredentials(false).setMaxAge(654321)
				.addAllowedOrigin("*").addExposedHeader("Content-Type")
				.addAllowedHeader("Authorization").addAllowedMethod("POST")
				.getEntity(),
			std::string("http://127.0.0.1:4200"),
			{
				{ "Access-Control-Allow-Origin", "http://127.0.0.1:4200" },
				{ "Access-Control-Expose-Headers", "Content-Type" },
				{ "Access-Control-Allow-Headers", "Authorization" },
				{ "Access-Control-Allow-Methods", "POST" },
				{ "Access-Control-Allow-Credentials", "false" },
				{ "Access-Control-Max-Age", "654321" },
			}
		},
		{
			CORSConfigurationBuilder()
				.setEnabled(true).setAllowedCredentials(true).setMaxAge(333)
				.addAllowedOrigin("http://127.0.0.1:4200")
				.addExposedHeader("Content-Type").addExposedHeader("Authorization")
				.addAllowedHeader("Origin").addAllowedHeader("Accept")
				.addAllowedMethod("GET").addAllowedMethod("POST").addAllowedMethod("DELETE")
				.getEntity(),
			std::string("http://127.0.0.1:4200"),
			{
				{ "Access-Control-Allow-Origin", "http://127.0.0.1:4200" },
				{ "Access-Control-Expose-Headers", "Authorization,Content-Type" },
				{ "Access-Control-Allow-Headers", "Accept,Origin" },
				{ "Access-Control-Allow-Methods", "DELETE,GET,POST" },
				{ "Access-Control-Allow-Credentials", "true" },
				{ "Access-Control-Max-Age", "333" },
			}
		},
		{
			CORSConfigurationBuilder()
				.setEnabled(true).setAllowedCredentials(true).setMaxAge(444)
				.addAllowedOrigin("http://127.0.0.1:8080").addAllowedOrigin("http://127.0.0.1:8081")
				.addExposedHeader("Content-Type").addExposedHeader("Authorization").addExposedHeader("Origin").addExposedHeader("Accept")
				.addAllowedHeader("Origin").addAllowedHeader("Accept").addAllowedHeader("Authorization").addAllowedHeader("Content-Type")
				.addAllowedMethod("GET").addAllowedMethod("POST").addAllowedMethod("DELETE").addAllowedMethod("OPTIONS")
				.getEntity(),
			std::string("http://127.0.0.1:4200"),
			{
				{ "Access-Control-Expose-Headers", "Accept,Authorization,Content-Type,Origin" },
				{ "Access-Control-Allow-Headers", "Accept,Authorization,Content-Type,Origin" },
				{ "Access-Control-Allow-Methods", "DELETE,GET,OPTIONS,POST" },
				{ "Access-Control-Allow-Credentials", "true" },
				{ "Access-Control-Max-Age", "444" },
			}
		}
	};


	class ReplyCORSHeadersBuilderServiceTest : public ::testing::TestWithParam<ReplyCORSHeadersBuilderServiceTestData>
	{
	public:
		void SetUp()
		{
			if (!GetParam().requestOrigin.empty())
			{
				m_request.getHeaders().addHeader("Origin", GetParam().requestOrigin);
			}

			m_service = std::make_unique<ReplyCORSHeadersBuilderService>(GetParam().corsConfiguration);
		}

		testing::AssertionResult compareReplyHeaders(std::map<std::string, std::string> expected,
													 std::map<std::string, std::string> actual)
		{
			if (expected.size() != actual.size())
			{
				return testing::AssertionFailure() << "Different headers size (expected='" << expected.size() << "', "
												   << "actual='" << actual.size() << "')";
			}

			for (auto header : expected)
			{
				std::string expectedHeaderName = header.first;
				std::string expectedHeaderValue = header.second;

				if (actual.find(expectedHeaderName) == actual.end())
				{
					return testing::AssertionFailure() << "Expected header '" << expectedHeaderName << "' not found";
				}

				if (actual[expectedHeaderName] != expectedHeaderValue)
				{
					return testing::AssertionFailure() << "Header '" << expectedHeaderName
													   << "' is different: expected='"
													   << expectedHeaderValue << "', actual='"
													   << actual[expectedHeaderName] << "'";
				}
			}

			return testing::AssertionSuccess();
		}

	protected:
		std::unique_ptr<ReplyCORSHeadersBuilderService> m_service;
		Request m_request;
	};


	TEST_P(ReplyCORSHeadersBuilderServiceTest, testAddCORSHeaders)
	{
		Reply reply;
		m_service->addCORSHeaders(m_request, reply);

		auto actualReplyHeaders = reply.getHeaders();
		ASSERT_TRUE(compareReplyHeaders(GetParam().expectedReplyHeaders, actualReplyHeaders));
	}

	INSTANTIATE_TEST_CASE_P(WebServer, ReplyCORSHeadersBuilderServiceTest, testing::ValuesIn(replyCORSHeadersBuilderServiceTestData));

}}}}
