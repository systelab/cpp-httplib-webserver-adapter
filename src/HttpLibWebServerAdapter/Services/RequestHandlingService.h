#pragma once

#include "IRequestHandlingService.h"


namespace systelab { namespace web_server { namespace httplib {

	class WebServicesMgr;

	class RequestHandlingService : public IRequestHandlingService
	{
	public:
		RequestHandlingService(WebServicesMgr&);
		virtual ~RequestHandlingService();

		std::unique_ptr<Reply> processRequest(const Request&) const;

	private:
		WebServicesMgr& m_webServicesMgr;
	};

}}}

