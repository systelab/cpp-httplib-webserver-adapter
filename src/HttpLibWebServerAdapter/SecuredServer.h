#pragma once

#include "Server.h"


namespace systelab { namespace web_server { namespace httplib {

	class SecuredServer : public Server
	{
	public:
		SecuredServer(const Configuration&);
		virtual ~SecuredServer();

	protected:
		virtual std::unique_ptr<::httplib::Server> buildHttpLibServer() const;
	};

}}}

