#include "node_cubeb.h"

Napi::FunctionReference NodeCubebStream::constructor;

Napi::Object NodeCubebStream::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "CubebStream", {
        // InstanceMethod("streamInit", &NodeCubebStream::Init),
        // InstanceMethod("streamDestroy", &NodeCubebStream::Destroy),
        // InstanceMethod("streamStart", &NodeCubebStream::Start),
        // InstanceMethod("", ),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    // TODO: Figure out how to use finalizeCallback in classes to destroy the Cubeb context when garbage collecting.

    exports.Set("CubebStream", func);

    return exports;
}


NodeCubebStream::NodeCubebStream(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NodeCubebStream>(info) {
    Napi::Env env = info.Env();
    int argc = info.Length();

    this->internal_context = NULL;
    char const * stream_name = "";
    cubeb_devid input_device = NULL;
    cubeb_stream_params * output_stream_params = NULL;
    cubeb_devid output_device = NULL;
    cubeb_stream_params * input_stream_params = NULL;
    uint32_t latency_frames = 0;

    this->internal_context = info[0].As<NodeCubeb>().internal_context;

    std::string name = info[1].As<Napi::String>().Utf8Value();
    stream_name = name.c_str();

    if (info[2].IsNumber()) {
        input_device = static_cast<cubeb_devid>(info[2].As<Napi::Number>().Int32Value());
    }

    if (info[3].IsObject()) {
        *input_stream_params = {};
        Napi::Object js_params = info[3].As<Napi::Object>();
        JsParamsToCubebParams(js_params, input_stream_params);
    }

    if (info[4].IsNumber()) {
        output_device = static_cast<cubeb_devid>(info[4].As<Napi::Number>().Int32Value());
    }

    if (info[5].IsObject()) {
        *output_stream_params = {};
        Napi::Object js_params = info[5].As<Napi::Object>();
        JsParamsToCubebParams(js_params, output_stream_params);
    }

    uint32_t latency_frames = info[6].As<Napi::Number>().Int32Value();

    this->internal_stream = NULL;

    int status = cubeb_stream_init(
        this->internal_context,
        &this->internal_stream,
        stream_name,
        input_device,
        input_stream_params,
        output_device,
        output_stream_params,
        latency_frames
    );

    if (status == CUBEB_OK) {

    }
}

// Napi::Value NodeCubebStream::Destroy(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubebStream::Start(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubebStream::Stop(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubebStream::ResetDefaultDevice(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubebStream::GetPosition(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubebStream::GetLatency(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubebStream::SetVolume(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubebStream::SetPanning(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubebStream::GetCurrentDevice(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubebStream::SDeviceDestroy(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubebStream::RegisterDeviceChangedCallback(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubebStream::UserPtr(const Napi::CallbackInfo& info) {}