#pragma once

#include "napi.h"
#include "cubeb/cubeb.h"

class CubebJsDevice : public Napi::ObjectWrap<CubebJsDevice> {
    public:
        static Napi::Object Init(Napi::Env, Napi::Object);
        CubebJsDevice(const Napi::CallbackInfo&);

        cubeb_device_info * internal_device_info;

    private:
        static Napi::FunctionReference constructor;
};
