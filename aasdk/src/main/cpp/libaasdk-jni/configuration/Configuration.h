#pragma once

#include "IConfiguration.h"

namespace configuration
{

class Configuration : public IConfiguration {
public:
    typedef Configuration* Pointer;

    Configuration();

    std::string huName() const override;
    void huName(std::string huName);
    std::string model() const override;
    void model(std::string model);
    std::string year() const override;
    void year(std::string year);
    std::string huMake() const override;
    void huMake(std::string huMake);
    std::string huModel() const override;
    void huModel(std::string huModel);
    std::string swVersion() const override;
    void swVersion(std::string swVersion);
    bool playMediaDuringVr() const override;
    void playMediaDuringVr(bool playMediaDuringVr);
    bool leftHandDrive() const override;
    void leftHandDrive(bool leftHandDrive);
    bool hideClock() const override;
    void hideClock(bool hideClock);

private:
    std::string huName_;
    std::string model_;
    std::string year_;
    std::string huMake_;
    std::string huModel_;
    std::string swVersion_;
    bool playMediaDuringVr_;
    bool leftHandDrive_;
    bool hideClock_;
};

}

