#pragma once

#include "IHttpLibResponseBuilderService.h"


namespace systelab { namespace web_server { namespace httplib {

	class HttpLibResponseBuilderService : public IHttpLibResponseBuilderService
	{
	public:
		HttpLibResponseBuilderService();
		virtual ~HttpLibResponseBuilderService();

		void build(const Reply&, ::httplib::Response&) const override;
	};

}}}
