#include "Configuration.h"

namespace configuration {

Configuration::Configuration() {}

std::string Configuration::huName() const {
    return huName_;
}

void Configuration::huName(std::string huName) {
    huName_ = huName;
}

std::string Configuration::model() const {
    return model_;
}

void Configuration::model(std::string model) {
    model_ = model;
}

std::string Configuration::year() const {
    return year_;
}

void Configuration::year(std::string year) {
    year_ = year;
}

std::string Configuration::huMake() const {
    return huMake_;
}

void Configuration::huMake(std::string huMake) {
    huMake_ = huMake;
}

std::string Configuration::huModel() const {
    return huModel_;
}

void Configuration::huModel(std::string huModel) {
    huModel_ = huModel;
}

std::string Configuration::swVersion() const {
    return swVersion_;
}

void Configuration::swVersion(std::string swVersion) {
    swVersion_ = swVersion;
}

bool Configuration::playMediaDuringVr() const {
    return playMediaDuringVr_;
}

void Configuration::playMediaDuringVr(bool playMediaDuringVr) {
    playMediaDuringVr_ = playMediaDuringVr;
}

bool Configuration::leftHandDrive() const {
    return leftHandDrive_;
}

void Configuration::leftHandDrive(bool leftHandDrive) {
    leftHandDrive_ = leftHandDrive;
}

bool Configuration::hideClock() const {
    return hideClock_;
}

void Configuration::hideClock(bool hideClock) {
    hideClock_ = hideClock;
}

}