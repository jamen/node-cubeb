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

//
// Cubeb: A context which manages streams and devices.
//

Napi::FunctionReference CubebJs::constructor;

Napi::Object CubebJs::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "Cubeb", {
        InstanceMethod("getBackendId", &CubebJs::GetBackendId),
        InstanceMethod("getMaxChannelCount", &CubebJs::GetMaxChannelCount),
        InstanceMethod("getMinLatency", &CubebJs::GetMinLatency),
        InstanceMethod("getPreferredSampleRate", &CubebJs::GetPreferredSampleRate),
        InstanceMethod("createStream", &CubebJs::CreateStream),

        // Export cubeb_sample_format values
        StaticValue("CUBEB_SAMPLE_S16LE", Napi::Number::New(env, CUBEB_SAMPLE_S16LE), napi_static),
        StaticValue("CUBEB_SAMPLE_S16BE", Napi::Number::New(env, CUBEB_SAMPLE_S16BE), napi_static),
        StaticValue("CUBEB_SAMPLE_S16NE", Napi::Number::New(env, CUBEB_SAMPLE_S16NE), napi_static),
        StaticValue("CUBEB_SAMPLE_FLOAT32LE", Napi::Number::New(env, CUBEB_SAMPLE_FLOAT32LE), napi_static),
        StaticValue("CUBEB_SAMPLE_FLOAT32BE", Napi::Number::New(env, CUBEB_SAMPLE_FLOAT32BE), napi_static),
        StaticValue("CUBEB_SAMPLE_FLOAT32NE", Napi::Number::New(env, CUBEB_SAMPLE_FLOAT32NE), napi_static),

        // Export cubeb_channel_layout values
        StaticValue("CUBEB_LAYOUT_UNDEFINED", Napi::Number::New(env, CUBEB_LAYOUT_UNDEFINED), napi_static),
        StaticValue("CUBEB_LAYOUT_MONO", Napi::Number::New(env, CUBEB_LAYOUT_MONO), napi_static),
        StaticValue("CUBEB_LAYOUT_MONO_LFE", Napi::Number::New(env, CUBEB_LAYOUT_MONO_LFE), napi_static),
        StaticValue("CUBEB_LAYOUT_STEREO", Napi::Number::New(env, CUBEB_LAYOUT_STEREO), napi_static),
        StaticValue("CUBEB_LAYOUT_STEREO_LFE", Napi::Number::New(env, CUBEB_LAYOUT_STEREO_LFE), napi_static),
        StaticValue("CUBEB_LAYOUT_3F", Napi::Number::New(env, CUBEB_LAYOUT_3F), napi_static),
        StaticValue("CUBEB_LAYOUT_3F_LFE", Napi::Number::New(env, CUBEB_LAYOUT_3F_LFE), napi_static),
        StaticValue("CUBEB_LAYOUT_2F1", Napi::Number::New(env, CUBEB_LAYOUT_2F1), napi_static),
        StaticValue("CUBEB_LAYOUT_2F1_LFE", Napi::Number::New(env, CUBEB_LAYOUT_2F1_LFE), napi_static),
        StaticValue("CUBEB_LAYOUT_3F1", Napi::Number::New(env, CUBEB_LAYOUT_3F1), napi_static),
        StaticValue("CUBEB_LAYOUT_3F1_LFE", Napi::Number::New(env, CUBEB_LAYOUT_3F1_LFE), napi_static),
        StaticValue("CUBEB_LAYOUT_2F2", Napi::Number::New(env, CUBEB_LAYOUT_2F2), napi_static),
        StaticValue("CUBEB_LAYOUT_2F2_LFE", Napi::Number::New(env, CUBEB_LAYOUT_2F2_LFE), napi_static),
        StaticValue("CUBEB_LAYOUT_QUAD", Napi::Number::New(env, CUBEB_LAYOUT_QUAD), napi_static),
        StaticValue("CUBEB_LAYOUT_QUAD_LFE", Napi::Number::New(env, CUBEB_LAYOUT_QUAD_LFE), napi_static),
        StaticValue("CUBEB_LAYOUT_3F2", Napi::Number::New(env, CUBEB_LAYOUT_3F2), napi_static),
        StaticValue("CUBEB_LAYOUT_3F2_LFE", Napi::Number::New(env, CUBEB_LAYOUT_3F2_LFE), napi_static),
        StaticValue("CUBEB_LAYOUT_3F2_BACK", Napi::Number::New(env, CUBEB_LAYOUT_3F2_BACK), napi_static),
        StaticValue("CUBEB_LAYOUT_3F2_LFE_BACK", Napi::Number::New(env, CUBEB_LAYOUT_3F2_LFE_BACK), napi_static),
        StaticValue("CUBEB_LAYOUT_3F3R_LFE", Napi::Number::New(env, CUBEB_LAYOUT_3F3R_LFE), napi_static),
        StaticValue("CUBEB_LAYOUT_3F4_LFE", Napi::Number::New(env, CUBEB_LAYOUT_3F4_LFE), napi_static),

        // Export chbeb_stream_prefs values
        StaticValue("CUBEB_STREAM_PREF_NONE", Napi::Number::New(env, CUBEB_STREAM_PREF_NONE), napi_static),
        StaticValue("CUBEB_STREAM_PREF_LOOPBACK", Napi::Number::New(env, CUBEB_STREAM_PREF_LOOPBACK), napi_static),

        // Export cubeb_state values
        StaticValue("CUBEB_STATE_STARTED", Napi::Number::New(env, CUBEB_STATE_STARTED), napi_static),
        StaticValue("CUBEB_STATE_STOPPED", Napi::Number::New(env, CUBEB_STATE_STOPPED), napi_static),
        StaticValue("CUBEB_STATE_DRAINED", Napi::Number::New(env, CUBEB_STATE_DRAINED), napi_static),
        StaticValue("CUBEB_STATE_ERROR", Napi::Number::New(env, CUBEB_STATE_ERROR), napi_static),

        // Export cubeb_device_type values
        StaticValue("CUBEB_DEVICE_TYPE_UNKNOWN", Napi::Number::New(env, CUBEB_DEVICE_TYPE_UNKNOWN), napi_static),
        StaticValue("CUBEB_DEVICE_TYPE_INPUT", Napi::Number::New(env, CUBEB_DEVICE_TYPE_INPUT), napi_static),
        StaticValue("CUBEB_DEVICE_TYPE_OUTPUT", Napi::Number::New(env, CUBEB_DEVICE_TYPE_OUTPUT), napi_static),

        // Export cubeb_device_state values
        StaticValue("CUBEB_DEVICE_STATE_DISABLED", Napi::Number::New(env, CUBEB_DEVICE_STATE_DISABLED), napi_static),
        StaticValue("CUBEB_DEVICE_STATE_UNPLUGGED", Napi::Number::New(env, CUBEB_DEVICE_STATE_UNPLUGGED), napi_static),
        StaticValue("CUBEB_DEVICE_STATE_ENABLED", Napi::Number::New(env, CUBEB_DEVICE_STATE_ENABLED), napi_static),

        // Export cubeb_device_fmt values
        StaticValue("CUBEB_DEVICE_FMT_S16LE", Napi::Number::New(env, CUBEB_DEVICE_FMT_S16LE), napi_static),
        StaticValue("CUBEB_DEVICE_FMT_S16BE", Napi::Number::New(env, CUBEB_DEVICE_FMT_S16BE), napi_static),
        StaticValue("CUBEB_DEVICE_FMT_F32LE", Napi::Number::New(env, CUBEB_DEVICE_FMT_F32LE), napi_static),
        StaticValue("CUBEB_DEVICE_FMT_F32BE", Napi::Number::New(env, CUBEB_DEVICE_FMT_F32BE), napi_static),
        StaticValue("CUBEB_DEVICE_FMT_S16NE", Napi::Number::New(env, CUBEB_DEVICE_FMT_S16NE), napi_static),
        StaticValue("CUBEB_DEVICE_FMT_F32NE", Napi::Number::New(env, CUBEB_DEVICE_FMT_F32NE), napi_static),
        StaticValue("CUBEB_DEVICE_FMT_S16_MASK", Napi::Number::New(env, CUBEB_DEVICE_FMT_S16_MASK), napi_static),
        StaticValue("CUBEB_DEVICE_FMT_F32_MASK", Napi::Number::New(env, CUBEB_DEVICE_FMT_F32_MASK), napi_static),
        StaticValue("CUBEB_DEVICE_FMT_ALL", Napi::Number::New(env, CUBEB_DEVICE_FMT_ALL), napi_static),

        // Export cubeb_device_pref values
        StaticValue("CUBEB_DEVICE_PREF_NONE", Napi::Number::New(env, CUBEB_DEVICE_PREF_NONE), napi_static),
        StaticValue("CUBEB_DEVICE_PREF_MULTIMEDIA", Napi::Number::New(env, CUBEB_DEVICE_PREF_MULTIMEDIA), napi_static),
        StaticValue("CUBEB_DEVICE_PREF_VOICE", Napi::Number::New(env, CUBEB_DEVICE_PREF_VOICE), napi_static),
        StaticValue("CUBEB_DEVICE_PREF_NOTIFICATION", Napi::Number::New(env, CUBEB_DEVICE_PREF_NOTIFICATION), napi_static),
        StaticValue("CUBEB_DEVICE_PREF_ALL", Napi::Number::New(env, CUBEB_DEVICE_PREF_ALL), napi_static)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    // TODO: Figure out how to use finalizeCallback in classes to destroy the Cubeb context when garbage collecting.

    exports.Set("Cubeb", func);

    return exports;
}

CubebJs::CubebJs(const Napi::CallbackInfo& info) : Napi::ObjectWrap<CubebJs>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    int argc = info.Length();
    char const * context_name;
    char const * backend_name = "";

    if (argc < 1 || argc > 2) {
        Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
        return;
    }

    if (info[0].IsString()) {
        std::string name = info[0].As<Napi::String>().Utf8Value();
        context_name = name.c_str();
    } else {
        Napi::TypeError::New(env, "Context name must be a string").ThrowAsJavaScriptException();
        return;
    }

    if (argc == 2) {
        if (info[1].IsString()) {
            std::string name = info[1].As<Napi::String>().Utf8Value();
            backend_name = name.c_str();
        } else {
            Napi::TypeError::New(env, "Backend name must be a string").ThrowAsJavaScriptException();
            return;
        }
    }

    this->internal_context = NULL;
    int status = cubeb_init(&this->internal_context, context_name, backend_name);

    if (status == CUBEB_ERROR) {
        Napi::TypeError::New(env, "Failed to initialize Cubeb context").ThrowAsJavaScriptException();
    }
}

Napi::Value CubebJs::GetBackendId(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int argc = info.Length();

    if (argc != 0) {
        Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    const char * backend_name = cubeb_get_backend_id(this->internal_context);

    return Napi::String::New(env, backend_name);
}

Napi::Value CubebJs::GetMaxChannelCount(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int argc = info.Length();

    if (argc != 0) {
        Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    uint32_t max_channels;
    int status = cubeb_get_max_channel_count(this->internal_context, &max_channels);

    if (status == CUBEB_OK) {
        return Napi::Number::New(env, max_channels);
    } else {
        // TODO: Throw status errors
        return env.Null();
    }
}

Napi::Value CubebJs::GetMinLatency(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int argc = info.Length();

    if (argc > 1) {
        Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    cubeb_stream_params * params = NULL;
    JsObjectToCubebParams(info[0], params);

    uint32_t latency_frames = 0;
    int status = cubeb_get_min_latency(this->internal_context, params, &latency_frames);

    if (status == CUBEB_OK) {
        return Napi::Number::New(env, latency_frames);
    } else {
        Napi::TypeError::New(env, "Failed to get minimum latency").ThrowAsJavaScriptException();
        // TODO: Throw status errors
        return env.Null();
    }
}

Napi::Value CubebJs::GetPreferredSampleRate(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int argc = info.Length();

    if (argc > 0) {
        Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    uint32_t sample_rate = 0;

    int status = cubeb_get_preferred_sample_rate(this->internal_context, &sample_rate);

    if (status == CUBEB_OK) {
        return Napi::Number::New(env, sample_rate);
    } else {
        // TODO: Throw status errors
        return env.Null();
    }
}

// Napi::Value CubebJs::Destroy(const Napi::CallbackInfo& info) {
//     Napi::Env env = info.Env();
//     int argc = info.Length();
//
//     if (argc > 0) {
//         Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
//         return env.Null();
//     }
//
//     cubeb_destroy(this->internal_context);
//
//     return Napi::Boolean::New(env, true);
// }

Napi::Value CubebJs::CreateStream(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return env.Null();
}

Napi::Value CubebJs::EnumerateDevices(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return env.Null();
}

Napi::Value CubebJs::RegisterDeviceCollectionChanged(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return env.Null();
}

//
// CubebStream: An audio sink or source stream
//

void NoopJs(const Napi::CallbackInfo&) {}

Napi::FunctionReference CubebJsStream::constructor;

Napi::Object CubebJsStream::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "CubebStream", {
        InstanceMethod("start", &CubebJsStream::Start),
        InstanceMethod("stop", &CubebJsStream::Stop),
        InstanceMethod("resetDefaultDevice", &CubebJsStream::ResetDefaultDevice),
        InstanceMethod("getPosition", &CubebJsStream::GetPosition),
        InstanceMethod("getLatency", &CubebJsStream::GetLatency),
        InstanceMethod("setVolume", &CubebJsStream::SetVolume),
        InstanceMethod("setPanning", &CubebJsStream::SetPanning),
        InstanceMethod("getCurrentDevice", &CubebJsStream::GetCurrentDevice),
        InstanceMethod("registerDeviceChangedCallback", &CubebJsStream::RegisterDeviceChangedCallback)
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
    return nframes;
}

void CubebJsStateCallback(cubeb_stream * stream, void * user_ptr, cubeb_state state) {

}

Napi::Value CubebJsStream::Start(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return env.Null();
}

Napi::Value CubebJsStream::Stop(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return env.Null();
}

Napi::Value CubebJsStream::ResetDefaultDevice(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return env.Null();
}

Napi::Value CubebJsStream::GetPosition(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return env.Null();
}

Napi::Value CubebJsStream::GetLatency(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return env.Null();
}

Napi::Value CubebJsStream::SetVolume(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return env.Null();
}

Napi::Value CubebJsStream::SetPanning(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return env.Null();
}

Napi::Value CubebJsStream::GetCurrentDevice(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return env.Null();
}

Napi::Value CubebJsStream::RegisterDeviceChangedCallback(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return env.Null();
}

//
// CubebDevice: An audio device used as a sink or source from a stream.
//

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

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    CubebJs::Init(env, exports);
    CubebJsStream::Init(env, exports);
    CubebJsDevice::Init(env, exports);
    return exports;
}

NODE_API_MODULE(cubeb_js, Init)