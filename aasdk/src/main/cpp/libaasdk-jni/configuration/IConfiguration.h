#pragma once

#include <string>

namespace configuration
{

class IConfiguration
{
public:
    typedef IConfiguration* Pointer;

    virtual ~IConfiguration() = default;

    virtual std::string huName() const = 0;
    virtual std::string model() const = 0;
    virtual std::string year() const = 0;
    virtual std::string huMake() const = 0;
    virtual std::string huModel() const = 0;
    virtual std::string swVersion() const = 0;
    virtual bool playMediaDuringVr() const = 0;
    virtual bool leftHandDrive() const = 0;
    virtual bool hideClock() const = 0;
};

}

