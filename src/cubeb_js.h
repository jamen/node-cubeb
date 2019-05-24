#pragma once

#include "napi.h"
#include "cubeb/cubeb.h"

void JsObjectToCubebParams(Napi::Value, cubeb_stream_params *);

class CubebJs : public Napi::ObjectWrap<CubebJs> {
    public:
        static Napi::Object Init(Napi::Env, Napi::Object);
        CubebJs(const Napi::CallbackInfo&);

        cubeb * internal_context;

    private:
        static Napi::FunctionReference constructor;

        Napi::Value GetBackendId(const Napi::CallbackInfo&);
        Napi::Value GetMaxChannelCount(const Napi::CallbackInfo&);
        Napi::Value GetMinLatency(const Napi::CallbackInfo&);
        Napi::Value GetPreferredSampleRate(const Napi::CallbackInfo&);
        Napi::Value CreateStream(const Napi::CallbackInfo&);
        Napi::Value EnumerateDevices(const Napi::CallbackInfo&);
        Napi::Value RegisterDeviceCollectionChanged(const Napi::CallbackInfo&);
};
