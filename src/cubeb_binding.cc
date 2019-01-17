#include "napi.h"
#include "cubeb/cubeb.h"

Napi::Value NodeCubebInit(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int argc = info.Length();
    char const * context_name;
    char const * backend_name = "";

    if (argc < 1 || argc > 2) {
        Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info[0].IsString()) {
        std::string name = info[0].As<Napi::String>().Utf8Value();
        context_name = name.c_str();
    } else {
        Napi::TypeError::New(env, "Context name must be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (argc == 2) {
        if (info[1].IsString()) {
            std::string name = info[1].As<Napi::String>().Utf8Value();
            backend_name = name.c_str();
        } else {
            Napi::TypeError::New(env, "Backend name must be a string").ThrowAsJavaScriptException();
            return env.Null();
        }
    }

    cubeb * ctx;
    cubeb_init(&ctx, context_name, backend_name);

    // TODO: Use finalizeCallback to destroy context at GC time
    return Napi::External<cubeb *>::New(env, &ctx);
}

Napi::Value CubebGetBackendId(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int argc = info.Length();

    if (argc != 1) {
        Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    cubeb * ctx = *info[0].As<Napi::External<cubeb *>>().Data();

    const char * backend_name = cubeb_get_backend_id(ctx);

    return Napi::String::New(env, backend_name);
}

Napi::Value CubebGetMaxChannelCount(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int argc = info.Length();

    if (argc != 1) {
        Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    cubeb * ctx = *info[0].As<Napi::External<cubeb *>>().Data();

    uint32_t max_channels;
    int status = cubeb_get_max_channel_count(ctx, &max_channels);

    if (status == CUBEB_OK) {
        return Napi::Number::New(env, max_channels);
    } else {
        // TODO: Throw status errors
        return env.Null();
    }
}

Napi::Value CubebGetMinLatency(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int argc = info.Length();

    if (argc != 2) {
        Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    cubeb * ctx = *info[0].As<Napi::External<cubeb *>>().Data();

    // Napi::Object js_params = info[1].As<Napi::Object>();
    cubeb_stream_params params;

    params.format = CUBEB_SAMPLE_FLOAT32NE;
    params.rate = 44100;
    params.channels = 2;
    params.prefs = CUBEB_STREAM_PREF_NONE;

    // params.channels = js_params.Get("channels").As<Napi::Number>().Int32Value();
    // params.rate = js_params.Get("sampleRate").As<Napi::Number>().Int32Value();

    // std::string format_type;
    // std::string format_endianness;

    // // Construct cubeb_stream_params from an audio-format object
    // if (js_params.Has("type")) {
    //     format_type = js_params.Get("type").As<Napi::String>();
    // } else {
    //     Napi::TypeError::New(env, "Format type required").ThrowAsJavaScriptException();
    //     return env.Null();
    // }

    // if (js_params.Has("endianness")) {
    //     format_endianness = js_params.Get("endianness").As<Napi::String>();
    // }

    // if (format_type == "int16") {
    //     if (format_endianness == "le") {
    //         params.format = CUBEB_SAMPLE_S16LE;
    //     } else if (format_endianness == "be") {
    //         params.format = CUBEB_SAMPLE_S16BE;
    //     } else {
    //         params.format = CUBEB_SAMPLE_S16NE;
    //     }
    // } else if (format_type == "float32") {
    //     if (format_endianness == "le") {
    //         params.format = CUBEB_SAMPLE_FLOAT32LE;
    //     } else if (format_endianness == "be") {
    //         params.format = CUBEB_SAMPLE_FLOAT32BE;
    //     } else {
    //         params.format = CUBEB_SAMPLE_FLOAT32NE;
    //     }
    // } else {
    //     Napi::TypeError::New(env, "Unsupported format").ThrowAsJavaScriptException();
    //     return env.Null();
    // }

    // // TODO: Layout and prefs
    // params.layout = CUBEB_LAYOUT_UNDEFINED;
    // params.prefs = CUBEB_STREAM_PREF_NONE;

    uint32_t latency_frames;
    int status = cubeb_get_min_latency(ctx, &params, &latency_frames);

    if (status == CUBEB_OK) {
        return Napi::Number::New(env, latency_frames);
    } else {
        // TODO: Throw status errors
        return env.Null();
    }
}

// Napi::Value CubebGetPreferredSampleRate(const Napi::CallbackInfo& info) {}

// Napi::Value CubebDestroy(const Napi::CallbackInfo& info) {}

// Napi::Value CubebStreamInit(const Napi::CallbackInfo& info) {}
// Napi::Value CubebStreamDestroy(const Napi::CallbackInfo& info) {}
// Napi::Value CubebStreamStart(const Napi::CallbackInfo& info) {}
// Napi::Value CubebStreamStop(const Napi::CallbackInfo& info) {}
// Napi::Value CubebStreamResetDefaultDevice(const Napi::CallbackInfo& info) {}
// Napi::Value CubebStreamGetPosition(const Napi::CallbackInfo& info) {}
// Napi::Value CubebStreamGetLatency(const Napi::CallbackInfo& info) {}
// Napi::Value CubebStreamSetVolume(const Napi::CallbackInfo& info) {}
// Napi::Value CubebStreamSetPanning(const Napi::CallbackInfo& info) {}
// Napi::Value CubebStreamGetCurrentDevice(const Napi::CallbackInfo& info) {}
// Napi::Value CubebStreamSDeviceDestroy(const Napi::CallbackInfo& info) {}
// Napi::Value CubebStreamRegisterDeviceChangedCallback(const Napi::CallbackInfo& info) {}

// Dont know what "user data pointer" is yet.

// Napi::Value CubebStreamUserPtr(const Napi::CallbackInfo& info) {}

// Dont know if these should be implemented directly

// Napi::Value CubebEnumerateDevices(const Napi::CallbackInfo& info) {}
// Napi::Value CubebDeviceCollectionDestroy(const Napi::CallbackInfo& info) {}
// Napi::Value CubebRegisterDeviceCollectionChanged(const Napi::CallbackInfo& info) {}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.DefineProperties({
        Napi::PropertyDescriptor::Function("init", NodeCubebInit),
        Napi::PropertyDescriptor::Function("getBackendId", CubebGetBackendId),
        Napi::PropertyDescriptor::Function("getMaxChannelCount", CubebGetMaxChannelCount),
        Napi::PropertyDescriptor::Function("getMinLatency", CubebGetMinLatency),
        // Napi::PropertyDescriptor::Function("", CubebGetPreferredSampleRate),
        // Napi::PropertyDescriptor::Function("", CubebDestroy),
        // Napi::PropertyDescriptor::Function("", CubebStreamInit),
        // Napi::PropertyDescriptor::Function("", CubebStreamDestroy),
        // Napi::PropertyDescriptor::Function("", CubebStreamStart),
        // Napi::PropertyDescriptor::Function("", ),
    });

    return exports;
}

NODE_API_MODULE(node_cubeb, Init)