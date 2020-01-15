#pragma once

#include "Client.h"


namespace systelab { namespace web_server { namespace httplib {

	class SecuredClient : public Client
	{
	public:
		SecuredClient(const std::string& serverAddress, unsigned int port);
		virtual ~SecuredClient();

		virtual std::unique_ptr<Reply> send(const Request&) const override;
	};

}}}

