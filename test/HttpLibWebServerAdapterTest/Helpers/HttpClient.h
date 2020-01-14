#pragma once

namespace httplib {
	struct Request;
	struct Response;
}

namespace systelab { namespace web_server {
	class Reply;
	class Request;
}}

namespace systelab { namespace web_server { namespace httplib { namespace test {

	class HttpClient
	{
	public:
		HttpClient();
		virtual ~HttpClient();

		virtual std::unique_ptr<Reply> send(const std::string& serverAddress, unsigned int port, const Request&) const;

	protected:
		void fillHttplibRequest(const Request&, ::httplib::Request&) const;
		std::unique_ptr<Reply> buildReply(::httplib::Response) const;
	};

}}}}

