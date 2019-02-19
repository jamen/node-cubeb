#include "cubeb_js.h"

Napi::FunctionReference CubebJsDevice::constructor;

Napi::Object CubebJsDevice::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "CubebDevice", {

    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    // TODO: Figure out how to use finalizeCallback in classes to destroy the Cubeb context when garbage collecting.

    exports.Set("CubebDevice", func);

    return exports;
}