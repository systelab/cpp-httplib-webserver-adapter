#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include "HttpLibWebServerAdapter/ServerFactory.h"

#include "WebServerAdapterInterface/IServer.h"
#include "WebServerAdapterInterface/IWebService.h"
#include "WebServerAdapterInterface/Model/Configuration.h"
#include "WebServerAdapterInterface/Model/Request.h"
#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/SecurityConfiguration.h"


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
	return []()
	{
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


class SimpleWebService : public systelab::web_server::IWebService
{
public:
	SimpleWebService() = default;
	virtual ~SimpleWebService() = default;
	
	std::unique_ptr<systelab::web_server::Reply> process(const systelab::web_server::Request& request) const override
	{
		auto reply = std::make_unique<systelab::web_server::Reply>();
		reply->setStatus(systelab::web_server::Reply::OK);
		reply->setContent(request.getContent());
		reply->addHeader("Content-Type", "text/plain");

		return reply;
	}
};

struct CmdArguments
{
	bool enableTLSv10;
	bool enableTLSv11;
	bool enableTLSv12;
	bool enableTLSv13;
};

std::unique_ptr<CmdArguments> parseArguments(int argc, char* argv[])
{
	auto cmdArgs = std::make_unique<CmdArguments>();
	cmdArgs->enableTLSv10 = true;
	cmdArgs->enableTLSv11 = true;
	cmdArgs->enableTLSv12 = true;
	cmdArgs->enableTLSv13 = true;

	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg == "--no_tls_v1_0")
		{
			cmdArgs->enableTLSv10 = false;
		}
		else if (arg == "--no_tls_v1_1")
		{
			cmdArgs->enableTLSv11 = false;
		}
		else if (arg == "--no_tls_v1_2")
		{
			cmdArgs->enableTLSv12 = false;
		}
		else if (arg == "--no_tls_v1_3")
		{
			cmdArgs->enableTLSv13 = false;
		}
		else
		{
			std::cout << "Unrecognized argument: " << arg << std::endl;
			return nullptr;
		}
	}

	return cmdArgs;
}

int main(int argc, char* argv[])
{
	auto cmdArgs = parseArguments(argc, argv);
	if (!cmdArgs)
	{
		std::cout << "Usage: " << argv[0] << " [--no_tls_v1_0] [--no_tls_v1_1] [--no_tls_v1_2] [--no_tls_v1_3]" << std::endl;
		std::cout << std::endl;
		std::cout << " --no_tls_v1_0 Enable support for TLS v1.0" << std::endl;
		std::cout << " --no_tls_v1_1 Enable support for TLS v1.1" << std::endl;
		std::cout << " --no_tls_v1_2 Enable support for TLS v1.2" << std::endl;
		std::cout << " --no_tls_v1_3 Enable support for TLS v1.3" << std::endl;
		return 0;
	}

	try
	{
		systelab::web_server::Configuration configuration;
		configuration.setHostAddress("127.0.0.1");
		configuration.setPort(9999);
		configuration.setThreadPoolSize(5);

		auto& securityConfiguration = configuration.getSecurityConfiguration();
		securityConfiguration.setHTTPSEnabled(true);
		securityConfiguration.setServerCertificate(getServerCertificate());
		securityConfiguration.setServerPrivateKey(getServerPrivateKey());
		securityConfiguration.setServerDHParam([]() { return ""; });

		securityConfiguration.setTLSv10Enabled(cmdArgs->enableTLSv10);
		securityConfiguration.setTLSv11Enabled(cmdArgs->enableTLSv11);
		securityConfiguration.setTLSv12Enabled(cmdArgs->enableTLSv12);
		securityConfiguration.setTLSv13Enabled(cmdArgs->enableTLSv13);

		systelab::web_server::httplib::ServerFactory serverFactory;
		auto server = serverFactory.buildServer(configuration);
		server->registerWebService(std::make_unique<SimpleWebService>());
		server->start();

		while (!server->isRunning())
		{
		}

		std::cout << "Server is running on https://127.0.0.1:9999..." << std::endl;
		std::cout << "Supported TLS versions:" << std::endl;
		std::cout << " - TLS v1.0: " << (cmdArgs->enableTLSv10 ? "enabled" : "disabled") << std::endl;
		std::cout << " - TLS v1.1: " << (cmdArgs->enableTLSv11 ? "enabled" : "disabled") << std::endl;
		std::cout << " - TLS v1.2: " << (cmdArgs->enableTLSv12 ? "enabled" : "disabled") << std::endl;
		std::cout << " - TLS v1.3: " << (cmdArgs->enableTLSv13 ? "enabled" : "disabled") << std::endl;

		while (server->isRunning())
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
	catch (...)
	{
		std::cout << "ERROR: Unknown error on web server" << std::endl;
		return 1;
	}

	return 0;
}

