#include "websocket_server.hpp"

#include "config.hpp"
#include "roah/logger.hpp"

#include <nlohmann/json.hpp>

#include <unordered_set>

namespace {
const roah::Logger logger{ "WebsocketServer" };
}  // namespace

roah::logging::webv::WebsocketServer::WebsocketServer(const Config & config)
    : config_{ config }
    , server_{ static_cast<int>(config.getWebsocketServerPort()), config.getServerHost() }
    , started_{ false }
{}

roah::logging::webv::WebsocketServer::~WebsocketServer() noexcept
{
    this->stop();
}

bool
roah::logging::webv::WebsocketServer::start()
{
    if (this->started_)
    {
        return true;
    }

    if (const auto ret = this->server_.listen(); !ret.first)
    {
        ROAH_ERROR(logger, "Failed to start WebsocketServer: {}", ret.second);
        return false;
    }

    this->server_.setOnClientMessageCallback([this](std::shared_ptr<ix::ConnectionState> connection_state,
                                                    ix::WebSocket &                      ws,
                                                    const ix::WebSocketMessagePtr &      msg) {
        this->_onClientMessageReceived(std::move(connection_state), ws, msg);
    });

    this->server_.start();
    this->started_ = true;

    return true;
}

void
roah::logging::webv::WebsocketServer::stop()
{
    if (this->started_)
    {
        this->server_.stop();
        this->started_ = false;
    }
}

void
roah::logging::webv::WebsocketServer::_onClientMessageReceived(std::shared_ptr<ix::ConnectionState> && connection_state,
                                                               ix::WebSocket &                         ws,
                                                               const ix::WebSocketMessagePtr &         msg)
{
    switch (msg->type)
    {
    case ix::WebSocketMessageType::Open:  //
        ROAH_INFO(logger,
                  "Websocket connection opened: {}:{}",
                  connection_state->getRemoteIp(),
                  connection_state->getRemotePort());
        this->_reportClientCount(this->server_.getClients().size());
        break;

    case ix::WebSocketMessageType::Close:  //
        ROAH_INFO(logger,
                  "Websocket connection closed: {}:{} (code: {}, reason: {})",
                  connection_state->getRemoteIp(),
                  connection_state->getRemotePort(),
                  msg->closeInfo.code,
                  msg->closeInfo.reason);

        this->_reportClientCount(this->server_.getClients().size() - 1);

        // 削除されるクライアントを Subscribers クライアントから削除する
        for (auto iter = this->subscribers_.begin(); iter != this->subscribers_.end();)
        {
            if (const auto ptr = iter->lock(); ptr && ptr.get() != &ws)
            {
                ++iter;
            }
            else
            {
                iter = this->subscribers_.erase(iter);
            }
        }
        break;

    case ix::WebSocketMessageType::Message:  //
        try
        {
            const auto j_data = nlohmann::json::parse(msg->str);
            const auto type   = j_data.at("type").get<std::string>();
            if (type == "subscribe")
            {
                for (const auto clients = this->server_.getClients(); const auto & client : clients)
                {
                    if (client.get() == &ws)
                    {
                        this->subscribers_.emplace_back(client);
                        break;
                    }
                }
                // 念のため, 重複, 無効なクライアントを Subscribers クライアントから削除する
                std::unordered_set<std::shared_ptr<ix::WebSocket>> subscribers;
                for (const auto & subscriber : this->subscribers_)
                {
                    if (const auto ptr = subscriber.lock(); ptr)
                    {
                        subscribers.insert(ptr);
                    }
                }
                this->subscribers_.clear();
                this->subscribers_.insert(this->subscribers_.end(),
                                          std::make_move_iterator(subscribers.begin()),
                                          std::make_move_iterator(subscribers.end()));
                ROAH_INFO(logger,
                          "Client subscribed to updates: {}:{}",
                          connection_state->getRemoteIp(),
                          connection_state->getRemotePort());
            }
            else if (type == "logs")
            {
                // 内容をそのまま Subscribers クライアントへ転送する
                // ... と思ったけどバリデートしたほうが良いかな
                for (const auto & subscriber : this->subscribers_)
                {
                    if (const auto ptr = subscriber.lock(); ptr)
                    {
                        ptr->sendUtf8Text(msg->str);
                    }
                }
            }
        }
        catch (const std::exception & e)
        {
            ROAH_ERROR(logger, "Failed to parse message: {}", e.what());
        }
        break;

    default:
        // IGNORE
        break;
    }
}

void
roah::logging::webv::WebsocketServer::_reportClientCount(const std::size_t count)
{
    nlohmann::json j_data{
        { "type", "client-count" },
        { "count", count },
    };
    const auto j_str = j_data.dump();
    for (const auto & client : this->server_.getClients())
    {
        client->sendUtf8Text(j_str);
    }
}
