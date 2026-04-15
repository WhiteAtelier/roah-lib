// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

#include "rest_api_server.hpp"

#include "binpack.hpp"
#include "config.hpp"
#include "roah/logger.hpp"
#include "roah/url_parser.hpp"
#include "roah/url_parts.hpp"

#include <nlohmann/json.hpp>

#include <cstring>
#include <format>
#include <memory>
#include <unordered_map>

namespace {
roah::Logger _logger{ "RestAPIServer" };
}  // namespace

roah::logging::webv::RestAPIServer::RestAPIServer(const Config & config)
    : config_{ config }
    , server_{ static_cast<int>(config.getHttpServerPort()), config.getServerHost() }
    , started_{ false }
{}

roah::logging::webv::RestAPIServer::~RestAPIServer() noexcept
{
    this->stop();
}

bool
roah::logging::webv::RestAPIServer::start()
{
    if (this->started_)
    {
        return true;
    }

    if (const auto ret = this->server_.listen(); !ret.first)
    {
        ROAH_ERROR(_logger, "Failed to start REST API server: {}", ret.second);
        return false;
    }

    this->server_.setOnConnectionCallback(
        [this](ix::HttpRequestPtr request, std::shared_ptr<ix::ConnectionState> connection_state) {
            return this->_onRequest(std::move(request), std::move(connection_state));
        });

    this->server_.start();
    this->started_ = true;
    return true;
}

void
roah::logging::webv::RestAPIServer::stop()
{
    if (this->started_)
    {
        this->server_.stop();
        this->started_ = false;
    }
}

ix::HttpResponsePtr
roah::logging::webv::RestAPIServer::_onRequest(ix::HttpRequestPtr &&                   request,
                                               std::shared_ptr<ix::ConnectionState> && connection_state)
{
    static const std::unordered_map<std::string, std::string> mime_types{
        { "html", "text/html" },        { "css", "text/css" },      { "js", "application/javascript" },
        { "png", "image/png" },         { "jpg", "image/jpeg" },    { "jpeg", "image/jpeg" },
        { "gif", "image/gif" },         { "svg", "image/svg+xml" }, { "ico", "image/x-icon" },
        { "json", "application/json" }, { "txt", "text/plain" }
    };

    ix::HttpResponsePtr response;

    if (request->method == "GET")
    {
        static const URLParser api_wsport{ "/api/wsport" };
        static const URLParser file_resolver{ "/*path" };

        std::unique_ptr<URLParts> url_parts;
        if (url_parts = api_wsport.match(request->uri); url_parts)
        {
            // api/wsport
            nlohmann::json payload{ { "port", this->config_.getWebsocketServerPort() } };
            response = std::make_shared<ix::HttpResponse>(  //
                200,
                "OK",
                ix::HttpErrorCode::Ok,
                ix::WebSocketHttpHeaders{
                    { "Content-Type", "application/json; charset=utf-8" },
                },
                payload.dump());
        }
        else if (url_parts = file_resolver.match(request->uri); url_parts)
        {
            // SPA file resolver
            const auto  path = url_parts->getParam("path");
            auto        ret  = embedded::getFile(path);
            std::string mime_type;
            if (ret.first != nullptr)
            {
                const auto pos = path.find_last_of('.');
                if (pos != std::string::npos)
                {
                    // 拡張子から MIME タイプを推測
                    const auto ext = path.substr(pos + 1);
                    if (const auto iter = mime_types.find(ext); iter != mime_types.cend())
                    {
                        mime_type = iter->second;
                    }
                }
            }
            else
            {
                ret       = embedded::getFile("index.html");
                mime_type = "text/html";
            }
            if (ret.first != nullptr && !mime_type.empty())
            {
                std::string buffer(ret.second, 0);
                std::memcpy(buffer.data(), ret.first, ret.second);
                response = std::make_shared<ix::HttpResponse>(
                    200,
                    "OK",
                    ix::HttpErrorCode::Ok,
                    ix::WebSocketHttpHeaders{
                        { "Content-Type", std::format("{}; charset=utf-8", mime_type) },
                    },
                    std::move(buffer));
            }
        }

        if (!response)
        {
            response = std::make_shared<ix::HttpResponse>(
                404,
                "Not Found",
                ix::HttpErrorCode::Ok,
                ix::WebSocketHttpHeaders{ { "Content-Type", "text/html; charset=utf-8" } },
                "<html><body><h1>404</h1><p>Not Found</p></body></html>");
        }
        // Add CORS headers for API responses
        response->headers["Access-Control-Allow-Origin"] = "*";
    }
    else if (request->method == "OPTIONS")
    {
        // CORS preflight request
        response
            = std::make_shared<ix::HttpResponse>(204,
                                                 "No Content",
                                                 ix::HttpErrorCode::Ok,
                                                 ix::WebSocketHttpHeaders{
                                                     { "Access-Control-Allow-Origin", "*" },
                                                     { "Access-Control-Allow-Methods", "GET, OPTIONS" },
                                                     { "Access-Control-Allow-Headers", "Content-Type, Authorization" },
                                                     { "Access-Control-Max-Age", "86400" },
                                                 },
                                                 "");
    }
    else
    {
        response = std::make_shared<ix::HttpResponse>(
            404,
            "Not Found",
            ix::HttpErrorCode::Ok,
            ix::WebSocketHttpHeaders{ { "Content-Type", "text/html; charset=utf-8" } },
            "<html><body><h1>404</h1><p>Not Found</p></body></html>");
    }

    if (response)
    {
        ROAH_INFO(_logger,
                  "{} {} by {} -> {}",
                  request->method,
                  request->uri,
                  connection_state->getRemoteIp(),
                  response->statusCode);
    }

    return response;
}
