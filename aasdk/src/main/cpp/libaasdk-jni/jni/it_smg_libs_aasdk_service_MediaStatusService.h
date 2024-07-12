#include <JNIBase.h>
#include <service/MediaStatusService.h>
#include "it_smg_libs_aasdk_service_Service.h"

using namespace service;

class JMediaStatusService : public JService, JNIBase {
public:
    typedef JMediaStatusService* Pointer;

    JMediaStatusService(JNIEnv *env, jobject jmediastatusservice, jobject jmessenger, jobject jmediastatusevent);
    ~JMediaStatusService();

    void initJavaMethods() override;

    static jfieldID handleId;
    static JMediaStatusService::Pointer getJMediaStatusService(JNIEnv* env, jobject jmediastatusservice);
    IService::Pointer getService() override;

    void start();
    void stop();
    void onError(const aasdk::error::Error& e) override;
private:
    MediaStatusService::Pointer mediaStatusService_;
//    jmethodID onErrorMethodId_;
};