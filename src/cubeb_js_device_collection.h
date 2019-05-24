#pragma once

#include "napi.h"
#include "cubeb/cubeb.h"

class CubebJsDeviceCollection : public Napi::ObjectWrap<CubebJsDeviceCollection> {
    public:
        static Napi::Object Init(Napi::Env, Napi::Object);
        CubebJsDeviceCollection(const Napi::CallbackInfo&);

        cubeb_device_collection * internal_device_collection;

    private:
        static Napi::FunctionReference constructor;
};