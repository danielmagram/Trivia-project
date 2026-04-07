#pragma once
#include <vector>
#include <ctime>


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
    IRequestHandler* newHandler; 
};

class IRequestHandler
{
public:
    virtual ~IRequestHandler() = default;
    virtual bool isRequestRelevant(RequestInfo info) = 0;
    virtual RequestResult handleRequest(RequestInfo info) = 0;
};