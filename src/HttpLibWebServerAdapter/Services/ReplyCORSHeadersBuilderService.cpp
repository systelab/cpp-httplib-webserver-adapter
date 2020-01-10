#include "ReplyCORSHeadersBuilderService.h"

#include "WebServerAdapterInterface/Model/CORSConfiguration.h"
#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"


namespace systelab { namespace web_server { namespace httplib {

	ReplyCORSHeadersBuilderService::ReplyCORSHeadersBuilderService(const CORSConfiguration& corsConfiguration)
		:m_corsConfiguration(corsConfiguration)
	{
	}

	ReplyCORSHeadersBuilderService::~ReplyCORSHeadersBuilderService() = default;

	void ReplyCORSHeadersBuilderService::addCORSHeaders(const Request& request,
														Reply& reply) const
	{
		if (m_corsConfiguration.isEnabled() && request.getHeaders().hasHeader("Origin"))
		{
			std::string origin = request.getHeaders().getHeader("Origin");
			if (m_corsConfiguration.isAllowedOrigin(origin))
			{
				reply.addHeader("Access-Control-Allow-Origin", origin);
			}

			std::string exposedHeadersStr = getCommaSeparatedList(m_corsConfiguration.getExposedHeaders());
			reply.addHeader("Access-Control-Expose-Headers", exposedHeadersStr);

			std::string allowedHeadersStr = getCommaSeparatedList(m_corsConfiguration.getAllowedHeaders());
			reply.addHeader("Access-Control-Allow-Headers", allowedHeadersStr);

			std::string allowedMethodsStr = getCommaSeparatedList(m_corsConfiguration.getAllowedMethods());
			reply.addHeader("Access-Control-Allow-Methods", allowedMethodsStr);

			reply.addHeader("Access-Control-Allow-Credentials", m_corsConfiguration.areAllowedCredentials() ? "true" : "false");
			reply.addHeader("Access-Control-Max-Age", std::to_string(m_corsConfiguration.getMaxAge()));
		}
	}

	std::string ReplyCORSHeadersBuilderService::getCommaSeparatedList(const std::set<std::string>& items) const
	{
		std::string itemsStr = "";
		for (auto item : items)
		{
			itemsStr += (itemsStr != "") ? "," : "";
			itemsStr += item;
		}

		return itemsStr;
	}

}}}
