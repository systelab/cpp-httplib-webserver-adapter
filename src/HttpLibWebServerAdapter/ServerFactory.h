#pragma once

#include "WebServerAdapterInterface/IServerFactory.h"

#include <memory>


namespace systelab { namespace web_server {
	class Configuration;
}}

namespace systelab { namespace web_server { namespace httplib {

	class ServerFactory : public IServerFactory
	{
	public:
		ServerFactory();
		virtual ~ServerFactory();

		std::unique_ptr<IServer> buildServer(const Configuration&) const;
	};

}}}

