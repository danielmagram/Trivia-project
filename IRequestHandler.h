#pragma once
#include <vector>
#include <ctime>
#include <memory>


class IRequestHandler;

struct RequestInfo
{
    unsigned char id; // The message code
    std::time_t receivalTime; // When the message arrived
    std::vector<unsigned char> buffer; // The JSON payload bytes
};

struct RequestResult
{
    std::vector<unsigned char> response; 
    std::unique_ptr<IRequestHandler> newHandler;
};

class IRequestHandler
{
public:
    virtual ~IRequestHandler() = default;
    virtual bool isRequestRelevant(const RequestInfo& info) const = 0;
    virtual RequestResult handleRequest(const RequestInfo& info) = 0;
    virtual void onClientDisconnected() = 0;
};