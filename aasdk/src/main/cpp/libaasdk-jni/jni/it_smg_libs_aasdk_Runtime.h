#pragma once

#include <JNIBase.h>
#include <boost/asio/io_service.hpp>
#include <thread>
#include "io/IOService.h"

class JRuntime : JNIBase {
public:
    typedef JRuntime* Pointer;

    JRuntime(JNIEnv* env, jobject androidApp);
    ~JRuntime();

    void startIOServiceWorkers(int threads);
    void stopIOServiceWorkers();

    static aasdk::io::ioService& ioService();

    static jfieldID handleId;
    static JRuntime::Pointer getJRuntime(JNIEnv* env, jobject jvideooutput);

private:
    using ThreadPool = std::vector<std::thread>;

//    aasdk::io::ioService ioService_;
    boost::asio::io_service ioService_;
    std::unique_ptr<boost::asio::io_service::work> work_;
    ThreadPool threadPool_;

    static JRuntime::Pointer instance_;

//    std::atomic<int> i;
    int i;
    std::mutex m;
    std::condition_variable v;

};
