#include "stdafx.h"
#include "BaseServerTest.h"
#include "ServerTestData.h"

#include "HttpLibWebServerAdapter/SecuredClient.h"
#include "HttpLibWebServerAdapter/SecuredServer.h"

#include "TestUtilitiesInterface/EntityComparator.h"


using namespace testing;
using namespace systelab::test_utility;

namespace systelab { namespace web_server { namespace httplib { namespace test {

	class SecuredServerTest : public BaseServerTest
							, public WithParamInterface<ServerTestData>
	{
	public:
		void SetUp()
		{
			BaseServerTest::SetUp();
		}

		void TearDown()
		{
			BaseServerTest::TearDown();
		}

		std::unique_ptr<IServer> buildServer(Configuration& configuration) override
		{
			auto serverCertificate = getServerCertificate();
			auto serverPrivateKey = getServerPrivateKey();

			auto& securityConfiguration = configuration.getSecurityConfiguration();
			securityConfiguration.setHTTPSEnabled(true);
			securityConfiguration.setServerCertificate(serverCertificate);
			securityConfiguration.setServerPrivateKey(serverPrivateKey);
			securityConfiguration.setServerDHParam([]() { return std::string(""); });
			securityConfiguration.setMutualSSLEnabled(false);

			return std::make_unique<SecuredServer>(configuration);
		}

		std::unique_ptr<IClient> buildClient(const std::string& hostAddress, unsigned int port) override
		{
			return std::make_unique<SecuredClient>(hostAddress, port);
		}

		std::function<std::string()> getServerCertificate()
		{
			return []()
			{
				std::stringstream ss;
				ss << "-----BEGIN CERTIFICATE-----" << std::endl;
				ss << "MIIDdDCCAlwCCQDydCZYRyLoODANBgkqhkiG9w0BAQsFADB8MQswCQYDVQQGEwJH" << std::endl;
				ss << "QjESMBAGA1UECAwJQmFyY2Vsb25hMRIwEAYDVQQHDAlCYXJjZWxvbmExGDAWBgNV" << std::endl;
				ss << "BAoMD0dsb2JhbCBTZWN1cml0eTEWMBQGA1UECwwNSVQgRGVwYXJ0bWVudDETMBEG" << std::endl;
				ss << "A1UEAwwKd2VyZmVuLmNvbTAeFw0xOTA0MDExNzU0MzVaFw0yOTAzMjkxNzU0MzVa" << std::endl;
				ss << "MHwxCzAJBgNVBAYTAkdCMRIwEAYDVQQIDAlCYXJjZWxvbmExEjAQBgNVBAcMCUJh" << std::endl;
				ss << "cmNlbG9uYTEYMBYGA1UECgwPR2xvYmFsIFNlY3VyaXR5MRYwFAYDVQQLDA1JVCBE" << std::endl;
				ss << "ZXBhcnRtZW50MRMwEQYDVQQDDAp3ZXJmZW4uY29tMIIBIjANBgkqhkiG9w0BAQEF" << std::endl;
				ss << "AAOCAQ8AMIIBCgKCAQEAqML1B7VW66CjzQBais4/Jz05gsZ3GWNGA6VKv2TX0VMp" << std::endl;
				ss << "+HnlXhJc+0WXZCLLSiKAwk70qQCdO2YNXg6sor5PcD1JvoFwQ8Kg34t3tZfiK48r" << std::endl;
				ss << "0UDuoXTbUfvucN0UhHQg+yaz/2QKqzRE796d89RLmnSMPpiPN3t/bNrwx7KX1FGu" << std::endl;
				ss << "Ppgow/AYI8419/LCvjl6+E5H7sy2j/svMaQJoRqguAaA+OG0PmLoKx2dgiX1kvXB" << std::endl;
				ss << "xN8vgJxLfpUDjW/JqupReezZbVOdd51RJ4WGgnyhp09EkIhOW2ijmWglw5aO6rmM" << std::endl;
				ss << "0UKqnp1uljomD8SBo3TpTpAj61Vl/2FqSi/szKoOdwIDAQABMA0GCSqGSIb3DQEB" << std::endl;
				ss << "CwUAA4IBAQBzOU0vnuR1kkFDULEkUdhLJ6hx3E/7rkHHlM9W2tdr49dJyIF2XIBY" << std::endl;
				ss << "LeO3tvo779D4ZWDvgQTgne+A7WLNBOOrRvvyZYD/sZEtqTzOFmjkQB9DvaIbvC3O" << std::endl;
				ss << "OP26dJnCkHNp0qvjyjyMZmjtJJKHkkpVc11Z/NjcOJIBczookgAEnZnyrBfA6QwF" << std::endl;
				ss << "juvkma05VOwuDrA+83+bzYFoFX65narVk4wtInVu9m4AZZTS+36ut6nopdMIpJHh" << std::endl;
				ss << "0ysT1+M0SVievi7e2scwIJ0xxbC219kO+oU7ormcJn0h1gUD7+rbMtxTgy58oVaQ" << std::endl;
				ss << "Ofe2qbaNiAhMKCHvUBUzupfpMTA1FWNZ" << std::endl;
				ss << "-----END CERTIFICATE-----" << std::endl;

				return ss.str();
			};
		}

		std::function<std::string()> getServerPrivateKey()
		{
			return []() {
				std::stringstream ss;
				ss << "-----BEGIN RSA PRIVATE KEY-----" << std::endl;
				ss << "MIIEowIBAAKCAQEAqML1B7VW66CjzQBais4/Jz05gsZ3GWNGA6VKv2TX0VMp+Hnl" << std::endl;
				ss << "XhJc+0WXZCLLSiKAwk70qQCdO2YNXg6sor5PcD1JvoFwQ8Kg34t3tZfiK48r0UDu" << std::endl;
				ss << "oXTbUfvucN0UhHQg+yaz/2QKqzRE796d89RLmnSMPpiPN3t/bNrwx7KX1FGuPpgo" << std::endl;
				ss << "w/AYI8419/LCvjl6+E5H7sy2j/svMaQJoRqguAaA+OG0PmLoKx2dgiX1kvXBxN8v" << std::endl;
				ss << "gJxLfpUDjW/JqupReezZbVOdd51RJ4WGgnyhp09EkIhOW2ijmWglw5aO6rmM0UKq" << std::endl;
				ss << "np1uljomD8SBo3TpTpAj61Vl/2FqSi/szKoOdwIDAQABAoIBAQCLXAsJ5Ya0p3/8" << std::endl;
				ss << "XslmDHD8T0O5qXhaX69GfW2fZYAO3LUw+kAdkd50pQAicdUcECfxzU+ea+JrVZe+" << std::endl;
				ss << "TzHiZnOlBQdrqOnzGrrMPxiIvbbIL7+IWi106NHf/sHlM7uosVk27LwBLzTcMhqn" << std::endl;
				ss << "VldrLamND6dsXYEO2Q3d72KcFe/pJH7RsHMuhc54VKU8VPR1jPB3AptquueCadXg" << std::endl;
				ss << "g7cYis5WHPwGyzr6ksL1oMtS4mIkqJRoTtvIlyBN7oXPPRmOngvbUMl118YpgYys" << std::endl;
				ss << "yG4ZWXOaRWfx905Z4W/JLBCpSoD4uf1xjyYaqrhH5EGXPhn1jj7Fd9k1mCWsUdGf" << std::endl;
				ss << "2aNbBnc5AoGBANeft0FohBKOzWegiBJV65cIbIEnosQKGhrvh8xppFQgLHCC/mt8" << std::endl;
				ss << "V9ed5pfElju1oB5WkAVUI1HI/OgcvyTHWwHOatof/6NaxknQgGoyJjgHibejfFvb" << std::endl;
				ss << "ABh2ih3c5Q0h3yMwaKRt1ajqHfD0YQ0vVHBQpsmZosSSkapoCmaNlGbbAoGBAMhc" << std::endl;
				ss << "0YHVzzsTmGe+6RMeBuXQsdK/sqltuX6VqyYIOvjd8Hw/HUFcVoGGIBcPR262vvQv" << std::endl;
				ss << "+MxWGFSxJgNfm/k8jGPu051+vC5d/Luw7bhBJVmYKRqMxPQDsJocCF9qQI4jmr8Z" << std::endl;
				ss << "Cp2XBbJtLiR8ranEEq1u+P4llfy7bVN8Khj/IuOVAoGAXGSRr7NLNHCAT3EYHVM3" << std::endl;
				ss << "ueXgKI+FTR9PBE+zDsINqgaHREa9Ie2L7lY4oruX7EntLFLHL6FX6QmIZSU8OlQ6" << std::endl;
				ss << "CNIcMw8XCTYyVRrEi7ylZ8jDGC8zcGCs786LbDRk6XWkGX36uMpfjRuUbNUqaO3o" << std::endl;
				ss << "7p+AeE4e0sBDLpOaMUtlVZUCgYAkt63FJ1OEtgCV8oxv7m7BKs+LmYm6NttpcTAe" << std::endl;
				ss << "FiAIf1OU0VQhKDTKI7Q5xZkHlwHDag8kExr+WkBnGdxRfmET5y5sjE8cCClKlA/c" << std::endl;
				ss << "VkPr+VB/BxEIzqxMcZqvMZ+G7Mts/b/ORWDL4+oy0Y5ZsA7kLZP/rw+fcxGD3Rsx" << std::endl;
				ss << "3X+dSQKBgA/RvLyMoXKdpEqD49DONjeRk0iCGSudqR4/ijPmha4EjE2gPe21T3Er" << std::endl;
				ss << "kTI0YJPS/OYjQsXWrz5Nfq0/k9aclFLKnWuuefo4uvw0IbcG33tzONczMol9tAIf" << std::endl;
				ss << "JFzwr9WrepdgTArcz0wLnEo30FCLUVm/dhx05+tkrtdtnAk79aQP" << std::endl;
				ss << "-----END RSA PRIVATE KEY-----" << std::endl;

				return ss.str();
			};
		}
	};

	TEST_P(SecuredServerTest, testSendRequestCallsWebService)
	{
		Request request = GetParam().request;
		Request expectedRequest = addClientHeaders(request);
		EXPECT_CALL(*m_webService, processProxy(isEqTo(expectedRequest)));

		m_httpClient->send(request);
	}

	TEST_P(SecuredServerTest, testSendRequestReturnsExpectedReply)
	{
		m_defaultReply = GetParam().reply;
		auto reply = m_httpClient->send(GetParam().request);

		Reply expectedReply = addServerHeaders(m_defaultReply);
		ASSERT_TRUE(reply != NULL);
		EXPECT_TRUE(EntityComparator()(expectedReply, *reply));
	}

	INSTANTIATE_TEST_SUITE_P(SecuredServer, SecuredServerTest, testing::ValuesIn(ServerTestDataBuilder::build()));

}}}}
