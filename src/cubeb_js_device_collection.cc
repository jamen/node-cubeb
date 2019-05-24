#include "cubeb_js_device_collection.h"

//
// CubebDevice: An audio device used as a sink or source from a stream.
//

Napi::FunctionReference CubebJsDeviceCollection::constructor;

Napi::Object CubebJsDeviceCollection::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "DeviceCollection", {

    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    // TODO: Figure out how to use finalizeCallback in classes to destroy the Cubeb context when garbage collecting.

    exports.Set("DeviceCollection", func);

    return exports;
}