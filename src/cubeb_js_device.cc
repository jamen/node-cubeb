#include "cubeb_js_device.h"

//
// CubebDevice: An audio device used as a sink or source from a stream.
//

Napi::FunctionReference CubebJsDevice::constructor;

Napi::Object CubebJsDevice::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "Device", {

    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    // TODO: Figure out how to use finalizeCallback in classes to destroy the Cubeb context when garbage collecting.

    exports.Set("Device", func);

    return exports;
}