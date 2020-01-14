#include "stdafx.h"
#include "ServerTestData.h"


namespace systelab { namespace web_server { namespace httplib { namespace test {

	std::vector<ServerTestData> ServerTestDataBuilder::build()
	{
		return {
			{
				Request("GET", "/rest/api/health", {}, 1, 1, { {"Origin", "http://localhost:4200"} }, ""),
				Reply(Reply::StatusType::OK, { {"Content-Type", "application/json"} }, "{ \"status\": \"running\" }")
			},
			{
				Request("POST", "/rest/api/users/login", {}, 1, 1,{ { "Content-Length", "25" },{ "Content-Type", "text/plain" } }, "Request content goes here"),
				Reply(Reply::StatusType::CREATED,{ { "Content-Type", "text/plain" },{ "Authorization", "Bearer 12345679012345689" } }, "Reply goes here")
			},
			{
				Request("PUT", "/custom/url/here", {}, 1, 1,{ {"Authorization", "Bearer xxx.yyy.zzz"} }, ""),
				Reply(Reply::StatusType::NOT_FOUND,{ { "Content-Type", "application/json" },{ "Authorization", "Bearer 12345679012345689" } }, "{}")
			},
			{
				Request("PATCH", "/rest/api/patch/method", {}, 1, 1,{ {"Content-Type", "application/json"} }, "{ \"aaa\": \"bbb\" }"),
				Reply(Reply::StatusType::BAD_REQUEST,{ { "Content-Type", "application/json" },{ "Authorization", "Bearer 12345679012345689" } }, "{ \"ccc\": [1,2,3,4,5,6,7,8,9] }")
			},
			{
				Request("DELETE", "/rest/api/users/john",{}, 1, 1,{ { "CustomHeader", "CustomValue" } }, "Request content goes here"),
				Reply(Reply::StatusType::NO_CONTENT, {}, "")
			},
			{
				Request("OPTIONS", "/rest/api/users",{}, 1, 1,{ { "Origin", "http://localhost:4200" } }, ""),
				Reply(Reply::StatusType::INTERNAL_SERVER_ERROR,{ {"Access-Control-Allow-Origin", "http://localhost:4200"} }, "CORS reply")
			}
		};
	}

}}}}
