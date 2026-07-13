// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_LOGGING_WEB_VIEWER_REST_API_SERVER_HPP
#define ROAH_LOGGING_WEB_VIEWER_REST_API_SERVER_HPP

#include <ixwebsocket/IXHttpServer.h>

namespace roah::logging::webv {

class Config;

class RestAPIServer
{
public:
    RestAPIServer(const Config & config);
    ~RestAPIServer() noexcept;

    bool
    start();

    void
    stop();

private:
    ix::HttpResponsePtr
    _onRequest(ix::HttpRequestPtr && request, std::shared_ptr<ix::ConnectionState> && connectionState);

    const Config & config_;
    ix::HttpServer server_;
    bool           started_;
};

}  // namespace roah::logging::webv

#endif
