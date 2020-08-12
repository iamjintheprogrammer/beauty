#pragma once

#include <beauty/certificate.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <vector>
#include <thread>
#include <optional>

namespace asio = boost::asio;

namespace beauty {
// --------------------------------------------------------------------------
class application {
public:
    application();
    application(certificates&& c);

    ~application();

    // Start the thread pool, running the event loop, not blocking
    void start(int concurrency = 1);

    // Check is the thread pool is started
    bool is_started() const;

    // Run the event loop in the current thread, blocking
    void run();

    // Stop the event loop
    void stop();

    asio::io_context& ioc() { return _ioc; }
    asio::ssl::context& ssl_context() { return _ssl_context; }
    bool is_ssl_activated() const { return (bool)_certificates; }

    static application& Instance();
    static application& Instance(certificates&& c);

private:
    asio::io_context            _ioc;
    asio::executor_work_guard<asio::io_context::executor_type> _work;
    asio::ssl::context          _ssl_context;
    std::optional<certificates> _certificates;

    std::vector<std::thread>    _threads;

private:
    void load_server_certificates();
};

// --------------------------------------------------------------------------
// Singleton direct access
// --------------------------------------------------------------------------
void start(int concurrency = 1);
bool is_started();
void run();
void stop();
}
