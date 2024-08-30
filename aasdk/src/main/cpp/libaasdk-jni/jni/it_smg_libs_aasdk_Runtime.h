#pragma once

#include <JNIBase.h>
#include <boost/asio/io_service.hpp>
#include <thread>

class JRuntime : JNIBase {
public:
    typedef JRuntime* Pointer;

    JRuntime(JNIEnv* env, jobject androidApp);
    ~JRuntime();

    void startIOServiceWorkers(int threads);
    void stopIOServiceWorkers();

    static boost::asio::io_service& ioService();

private:
    using ThreadPool = std::vector<std::thread>;

    void initJavaExecptionHandler(JNIEnv* env);

    boost::asio::io_service ioService_;
    boost::asio::io_service::work work_;
    ThreadPool threadPool_;

    static JRuntime::Pointer instance_;
    static jclass cls_;

    std::atomic<int> i;
    std::mutex m;
    std::condition_variable v;

};
