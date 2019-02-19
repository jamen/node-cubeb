#ifndef CUBEB_JS_H
#define CUBEB_JS_H

#include "napi.h"
#include "cubeb/cubeb.h"
#include <iostream>

#define DEBUG(x) x

void JsObjectToCubebParams(Napi::Value, cubeb_stream_params *);

class CubebJs : public Napi::ObjectWrap<CubebJs> {
    public:
        static Napi::Object Init(Napi::Env, Napi::Object);
        CubebJs(const Napi::CallbackInfo&);

        cubeb * internal_context;

    private:
        static Napi::FunctionReference constructor;

        Napi::Value Init(const Napi::CallbackInfo&);
        Napi::Value GetBackendId(const Napi::CallbackInfo&);
        Napi::Value GetMaxChannelCount(const Napi::CallbackInfo&);
        Napi::Value GetMinLatency(const Napi::CallbackInfo&);
        Napi::Value GetPreferredSampleRate(const Napi::CallbackInfo&);
        Napi::Value Destroy(const Napi::CallbackInfo&);
};

void NoopJs(const Napi::CallbackInfo&);

class CubebJsStream : public Napi::ObjectWrap<CubebJsStream> {
    public:
        static Napi::Object Init(Napi::Env, Napi::Object);
        CubebJsStream(const Napi::CallbackInfo&);

        cubeb * internal_context;
        cubeb_stream * internal_stream;

    private:
        static Napi::FunctionReference constructor;
};

struct CubebJsUserData {
    Napi::Function js_data_callback;
    Napi::Function js_state_callback;
};

long CubebJsDataCallback(cubeb_stream *, void *, void const *, void *, long);
void CubebJsStateCallback(cubeb_stream *, void *, cubeb_state);

class CubebJsDevice : public Napi::ObjectWrap<CubebJsDevice> {
    public:
        static Napi::Object Init(Napi::Env, Napi::Object);
        CubebJsDevice(const Napi::CallbackInfo&);

        cubeb_device_info * internal_device_info;

    private:
        static Napi::FunctionReference constructor;
};

#endif