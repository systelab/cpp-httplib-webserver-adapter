#include "RequestHandlingService.h"

#include "Model/WebServicesMgr.h"

#include "WebServerAdapterInterface/IWebService.h"
#include "WebServerAdapterInterface/Model/CORSConfiguration.h"
#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"

#include <algorithm>
#include <iterator>
#include <sstream>


namespace systelab { namespace web_server { namespace httplib {

	RequestHandlingService::RequestHandlingService(WebServicesMgr& webServicesMgr)
		:m_webServicesMgr(webServicesMgr)
	{
	}

	RequestHandlingService::~RequestHandlingService() = default;

	std::unique_ptr<Reply> RequestHandlingService::processRequest(const Request& request) const
	{
		unsigned nWebServices = m_webServicesMgr.getWebServiceCount();
		for (unsigned int i = 0; i < nWebServices; i++)
		{
			const IWebService* webService = m_webServicesMgr.getWebService(i);
			std::unique_ptr<Reply> reply = webService->process(request);
			if (reply)
			{
				return std::move(reply);
			}
		}

		std::unique_ptr<Reply> notFoundReply(new Reply());
		notFoundReply->setStatus(Reply::NOT_FOUND);
		return std::move(notFoundReply);
	}

}}}
