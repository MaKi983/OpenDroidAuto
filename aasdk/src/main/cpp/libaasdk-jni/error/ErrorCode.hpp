/*
*  This file is part of aasdk library project.
*  Copyright (C) 2018 f1x.studio (Michal Szwaj)
*
*  aasdk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.

*  aasdk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with aasdk. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdint.h>


namespace aasdk
{
namespace error
{

enum class ErrorCode
{
    NONE = 0,
    USB_CLAIM_INTERFACE = 1,
    USB_INVALID_CONFIG_DESCRIPTOR = 2,
    USB_OBTAIN_INTERFACE_DESCRIPTOR = 3,
    USB_EMPTY_INTERFACES = 4,
    USB_INVALID_DEVICE_ENDPOINTS = 5,
    USB_INVALID_TRANSFER_METHOD = 6,
    USB_TRANSFER_ALLOCATION = 7,
    USB_LIST_DEVICES = 8,
    USB_OBTAIN_CONFIG_DESCRIPTOR = 9,
    USB_TRANSFER = 10,
    DATA_SINK_COMMIT_OVERFLOW = 11,
    DATA_SINK_CONSUME_UNDERFLOW = 12,
    USB_AOAP_PROTOCOL_VERSION = 13,
    USB_AOAP_DEVICE_NOT_FOUND = 14,
    SSL_READ_CERTIFICATE = 15,
    SSL_READ_PRIVATE_KEY = 16,
    SSL_METHOD = 17,
    SSL_CONTEXT_CREATION = 18,
    SSL_USE_CERTIFICATE = 19,
    SSL_USE_PRIVATE_KEY = 20,
    SSL_HANDLER_CREATION = 21,
    SSL_READ_BIO_CREATION = 22,
    SSL_WRITE_BIO_CREATION = 23,
    SSL_HANDSHAKE = 24,
    SSL_WRITE = 25,
    SSL_READ = 26,
    SSL_BIO_READ = 27,
    SSL_BIO_WRITE = 28,
    MESSENGER_INTERTWINED_CHANNELS = 29,
    OPERATION_ABORTED = 30,
    OPERATION_IN_PROGRESS = 31,
    PARSE_PAYLOAD = 32,
    TCP_TRANSFER = 33,
    UNEXPECTED_ERROR = 34
};

    inline std::string errorCodeToString(ErrorCode errorCode) {
        switch (errorCode) {
            case ErrorCode::NONE:
                return "NONE";
            case ErrorCode::USB_CLAIM_INTERFACE:
                return "USB_CLAIM_INTERFACE";
            case ErrorCode::USB_INVALID_CONFIG_DESCRIPTOR:
                return "USB_INVALID_CONFIG_DESCRIPTOR";
            case ErrorCode::USB_OBTAIN_INTERFACE_DESCRIPTOR:
                return "USB_OBTAIN_INTERFACE_DESCRIPTOR";
            case ErrorCode::USB_EMPTY_INTERFACES:
                return "USB_EMPTY_INTERFACES";
            case ErrorCode::USB_INVALID_DEVICE_ENDPOINTS:
                return "USB_INVALID_DEVICE_ENDPOINTS";
            case ErrorCode::USB_INVALID_TRANSFER_METHOD:
                return "USB_INVALID_TRANSFER_METHOD";
            case ErrorCode::USB_TRANSFER_ALLOCATION:
                return "USB_TRANSFER_ALLOCATION";
            case ErrorCode::USB_LIST_DEVICES:
                return "USB_LIST_DEVICES";
            case ErrorCode::USB_OBTAIN_CONFIG_DESCRIPTOR:
                return "USB_OBTAIN_CONFIG_DESCRIPTOR";
            case ErrorCode::USB_TRANSFER:
                return "USB_TRANSFER";
            case ErrorCode::DATA_SINK_COMMIT_OVERFLOW:
                return "DATA_SINK_COMMIT_OVERFLOW";
            case ErrorCode::DATA_SINK_CONSUME_UNDERFLOW:
                return "DATA_SINK_CONSUME_UNDERFLOW";
            case ErrorCode::USB_AOAP_PROTOCOL_VERSION:
                return "USB_AOAP_PROTOCOL_VERSION";
            case ErrorCode::USB_AOAP_DEVICE_NOT_FOUND:
                return "USB_AOAP_DEVICE_NOT_FOUND";
            case ErrorCode::SSL_READ_CERTIFICATE:
                return "SSL_READ_CERTIFICATE";
            case ErrorCode::SSL_READ_PRIVATE_KEY:
                return "SSL_READ_PRIVATE_KEY";
            case ErrorCode::SSL_METHOD:
                return "SSL_METHOD";
            case ErrorCode::SSL_CONTEXT_CREATION:
                return "SSL_CONTEXT_CREATION";
            case ErrorCode::SSL_USE_CERTIFICATE:
                return "SSL_USE_CERTIFICATE";
            case ErrorCode::SSL_USE_PRIVATE_KEY:
                return "SSL_USE_PRIVATE_KEY";
            case ErrorCode::SSL_HANDLER_CREATION:
                return "SSL_HANDLER_CREATION";
            case ErrorCode::SSL_READ_BIO_CREATION:
                return "SSL_READ_BIO_CREATION";
            case ErrorCode::SSL_WRITE_BIO_CREATION:
                return "SSL_WRITE_BIO_CREATION";
            case ErrorCode::SSL_HANDSHAKE:
                return "SSL_HANDSHAKE";
            case ErrorCode::SSL_WRITE:
                return "SSL_WRITE";
            case ErrorCode::SSL_READ:
                return "SSL_READ";
            case ErrorCode::SSL_BIO_READ:
                return "SSL_BIO_READ";
            case ErrorCode::SSL_BIO_WRITE:
                return "SSL_BIO_WRITE";
            case ErrorCode::MESSENGER_INTERTWINED_CHANNELS:
                return "MESSENGER_INTERTWINED_CHANNELS";
            case ErrorCode::OPERATION_ABORTED:
                return "OPERATION_ABORTED";
            case ErrorCode::OPERATION_IN_PROGRESS:
                return "OPERATION_IN_PROGRESS";
            case ErrorCode::PARSE_PAYLOAD:
                return "PARSE_PAYLOAD";
            case ErrorCode::TCP_TRANSFER:
                return "TCP_TRANSFER";
            case ErrorCode::UNEXPECTED_ERROR:
                return "UNEXPECTED_ERROR";
            default:
                return "(unknown)";
        }
    }


}
}
