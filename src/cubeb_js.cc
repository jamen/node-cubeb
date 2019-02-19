#include "cubeb_js.h"

void JsObjectToCubebParams(Napi::Value value, cubeb_stream_params *params) {
    if (value.IsObject()) {
        Napi::Object js_params = value.As<Napi::Object>();
        params->rate = js_params.Get("rate").As<Napi::Number>().Int32Value();
        params->channels = js_params.Get("channels").As<Napi::Number>().Int32Value();
        params->format = static_cast<cubeb_sample_format>(js_params.Get("format").As<Napi::Number>().Int32Value());
        params->layout = js_params.Get("layout").As<Napi::Number>().Int32Value();
    } else if (value.IsNull()) {
        params = NULL;
    } else {
        // Invalid type.
    }
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    CubebJs::Init(env, exports);
    CubebJsStream::Init(env, exports);
    CubebJsDevice::Init(env, exports);
    return exports;
}

NODE_API_MODULE(cubeb_js, Init)