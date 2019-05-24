#include "cubeb_js.h"
#include "cubeb_js_device_collection.h"
#include "cubeb_js_device.h"
#include "cubeb_js_stream.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    CubebJs::Init(env, exports);
    CubebJsDeviceCollection::Init(env, exports);
    CubebJsDevice::Init(env, exports);
    CubebJsStream::Init(env, exports);
    return exports;
}

NODE_API_MODULE(cubeb_js, Init)