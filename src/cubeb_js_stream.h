#pragma once

#include "napi.h"
#include "cubeb/cubeb.h"

void NoopJs(const Napi::CallbackInfo&);

class CubebJsStream : public Napi::ObjectWrap<CubebJsStream> {
    public:
        static Napi::Object Init(Napi::Env, Napi::Object);
        CubebJsStream(const Napi::CallbackInfo&);

        cubeb * internal_context;
        cubeb_stream * internal_stream;

    private:
        static Napi::FunctionReference constructor;

        Napi::Value Start(const Napi::CallbackInfo&);
        Napi::Value Stop(const Napi::CallbackInfo&);
        Napi::Value ResetDefaultDevice(const Napi::CallbackInfo&);
        Napi::Value GetPosition(const Napi::CallbackInfo&);
        Napi::Value GetLatency(const Napi::CallbackInfo&);
        Napi::Value SetVolume(const Napi::CallbackInfo&);
        Napi::Value SetPanning(const Napi::CallbackInfo&);
        Napi::Value GetCurrentDevice(const Napi::CallbackInfo&);
        Napi::Value RegisterDeviceChangedCallback(const Napi::CallbackInfo&);
};

struct CubebJsUserData {
    Napi::Function js_data_callback;
    Napi::Function js_state_callback;
};

long CubebJsDataCallback(cubeb_stream *, void *, void const *, void *, long);
void CubebJsStateCallback(cubeb_stream *, void *, cubeb_state);