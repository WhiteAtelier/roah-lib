// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#ifndef ROAH_LOGGING_WEB_VIEWER_WEBSOCKET_SERVER_HPP
#define ROAH_LOGGING_WEB_VIEWER_WEBSOCKET_SERVER_HPP

#include <IXWebSocketServer.h>

#include <memory>
#include <vector>

namespace roah::logging::webv {

class Config;

class WebsocketServer
{
public:
    WebsocketServer(const Config & config);
    ~WebsocketServer() noexcept;

    bool
    start();

    void
    stop();

private:
    void
    _onClientMessageReceived(std::shared_ptr<ix::ConnectionState> && connection_state,
                             ix::WebSocket &                         ws,
                             const ix::WebSocketMessagePtr &         msg);

    void
    _reportClientCount(const std::size_t count, ix::WebSocket * const dst);

    const Config &                            config_;
    ix::WebSocketServer                       server_;
    std::vector<std::weak_ptr<ix::WebSocket>> subscribers_;
    bool                                      started_;
};

}  // namespace roah::logging::webv

#endif
