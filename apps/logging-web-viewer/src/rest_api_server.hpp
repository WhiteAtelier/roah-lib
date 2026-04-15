#ifndef ROAH_LOGGING_WEB_VIEWER_REST_API_SERVER_HPP
#define ROAH_LOGGING_WEB_VIEWER_REST_API_SERVER_HPP

#include <IXHttpServer.h>

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
    bool           started_;
    ix::HttpServer server_;
};

}  // namespace roah::logging::webv

#endif
