#include "cubeb_js.h"

void NoopJs(const Napi::CallbackInfo&) {}

Napi::FunctionReference CubebJsStream::constructor;

Napi::Object CubebJsStream::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "CubebStream", {
        // InstanceMethod("streamInit", &CubebJsStream::Init),
        // InstanceMethod("streamDestroy", &CubebJsStream::Destroy),
        // InstanceMethod("streamStart", &CubebJsStream::Start),
        // InstanceMethod("", ),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    // TODO: Figure out how to use finalizeCallback in classes to destroy the Cubeb context when garbage collecting.

    exports.Set("CubebStream", func);

    return exports;
}

CubebJsStream::CubebJsStream(const Napi::CallbackInfo& info) : Napi::ObjectWrap<CubebJsStream>(info) {
    Napi::Env env = info.Env();
    int argc = info.Length();

    // Cubeb params to be constructed from JS params.
    char const * stream_name = "";
    cubeb_devid input_devid = NULL;
    cubeb_stream_params * output_stream_params = NULL;
    cubeb_devid output_devid = NULL;
    cubeb_stream_params * input_stream_params = NULL;
    uint32_t latency_frames = 0;

    // JS-defined functions passed into Cubeb's callbacks.
    struct CubebJsUserData user_data = {
        Napi::Function::New(env, NoopJs),
        Napi::Function::New(env, NoopJs)
    };

    // Cubeb context
    this->internal_context = NULL;
    CubebJs * context = Napi::ObjectWrap<CubebJs>::Unwrap(info[0].As<Napi::Object>());
    this->internal_context = context->internal_context;

    // Stream name
    std::string name = info[1].As<Napi::String>().Utf8Value();
    stream_name = name.c_str();

    // Input device
    if (info[2].IsNumber()) {
        CubebJsDevice * js_device_info = Napi::ObjectWrap<CubebJsDevice>::Unwrap(info[2].As<Napi::Object>());
        input_devid = js_device_info->internal_device_info->devid;
    }

    // Input stream params
    JsObjectToCubebParams(info[3], input_stream_params);

    // Output device
    if (info[4].IsNumber()) {
        CubebJsDevice * js_device_info = Napi::ObjectWrap<CubebJsDevice>::Unwrap(info[4].As<Napi::Object>());
        output_devid = js_device_info->internal_device_info->devid;
    }

    // Output stream params
    JsObjectToCubebParams(info[5], output_stream_params);

    // latency frames
    latency_frames = info[6].As<Napi::Number>().Int32Value();

    // Data callback
    if (info[7].IsFunction()) {
        user_data.js_data_callback = info[7].As<Napi::Function>();
    }

    // State callback
    if (info[8].IsFunction()) {
        user_data.js_state_callback = info[8].As<Napi::Function>();
    }

    this->internal_stream = NULL;

    int status = cubeb_stream_init(
        this->internal_context,
        &this->internal_stream,
        stream_name,
        input_devid,
        input_stream_params,
        output_devid,
        output_stream_params,
        latency_frames,
        CubebJsDataCallback,
        CubebJsStateCallback,
        &user_data
    );
}

long CubebJsDataCallback(cubeb_stream * stream, void * user_ptr, void const * input_buffer, void * output_buffer, long nframes) {

}

void CubebJsStateCallback(cubeb_stream * stream, void * user_ptr, cubeb_state state) {

}

// Napi::Value CubebJsStream::Destroy(const Napi::CallbackInfo& info) {}

// Napi::Value CubebJsStream::Start(const Napi::CallbackInfo& info) {}

// Napi::Value CubebJsStream::Stop(const Napi::CallbackInfo& info) {}

// Napi::Value CubebJsStream::ResetDefaultDevice(const Napi::CallbackInfo& info) {}

// Napi::Value CubebJsStream::GetPosition(const Napi::CallbackInfo& info) {}

// Napi::Value CubebJsStream::GetLatency(const Napi::CallbackInfo& info) {}

// Napi::Value CubebJsStream::SetVolume(const Napi::CallbackInfo& info) {}

// Napi::Value CubebJsStream::SetPanning(const Napi::CallbackInfo& info) {}

// Napi::Value CubebJsStream::GetCurrentDevice(const Napi::CallbackInfo& info) {}

// Napi::Value CubebJsStream::SDeviceDestroy(const Napi::CallbackInfo& info) {}

// Napi::Value CubebJsStream::RegisterDeviceChangedCallback(const Napi::CallbackInfo& info) {}

// Napi::Value CubebJsStream::UserPtr(const Napi::CallbackInfo& info) {}