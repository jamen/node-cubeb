#include "node_cubeb.h"

Napi::FunctionReference NodeCubeb::constructor;

Napi::Object NodeCubeb::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "Cubeb", {
        InstanceMethod("getBackendId", &NodeCubeb::GetBackendId),
        InstanceMethod("getMaxChannelCount", &NodeCubeb::GetMaxChannelCount),
        InstanceMethod("getMinLatency", &NodeCubeb::GetMinLatency),
        InstanceMethod("getPreferredSampleRate", &NodeCubeb::GetPreferredSampleRate),
        InstanceMethod("destroy", &NodeCubeb::Destroy)
        // InstanceMethod("streamInit", &NodeCubeb::StreamInit),
        // InstanceMethod("streamDestroy", &NodeCubeb::StreamDestroy),
        // InstanceMethod("streamStart", &NodeCubeb::StreamStart),
        // InstanceMethod("", ),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    // TODO: Figure out how to use finalizeCallback in classes to destroy the Cubeb context when garbage collecting.

    exports.Set("Cubeb", func);

    return exports;
}

NodeCubeb::NodeCubeb(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NodeCubeb>(info) {
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

Napi::Value NodeCubeb::GetBackendId(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int argc = info.Length();

    if (argc != 0) {
        Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    const char * backend_name = cubeb_get_backend_id(this->internal_context);

    return Napi::String::New(env, backend_name);
}

Napi::Value NodeCubeb::GetMaxChannelCount(const Napi::CallbackInfo& info) {
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

Napi::Value NodeCubeb::GetMinLatency(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int argc = info.Length();

    if (argc > 1) {
        Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    cubeb_stream_params params = {};

    if (argc == 1) {
        Napi::Object js_params = info[0].As<Napi::Object>();
        JsParamsToCubebParams(js_params, &params);
    }

    uint32_t latency_frames = 0;

    int status = cubeb_get_min_latency(this->internal_context, &params, &latency_frames);

    if (status == CUBEB_OK) {
        return Napi::Number::New(env, latency_frames);
    } else {
        Napi::TypeError::New(env, "Failed to get minimum latency").ThrowAsJavaScriptException();
        // TODO: Throw status errors
        return env.Null();
    }
}

Napi::Value NodeCubeb::GetPreferredSampleRate(const Napi::CallbackInfo& info) {
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

Napi::Value NodeCubeb::Destroy(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int argc = info.Length();

    if (argc > 0) {
        Napi::TypeError::New(env, "Wrong amount of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    cubeb_destroy(this->internal_context);

    return Napi::Boolean::New(env, true);
}

// Napi::Value NodeCubeb::EnumerateDevices(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubeb::DeviceCollectionDestroy(const Napi::CallbackInfo& info) {}

// Napi::Value NodeCubeb::RegisterDeviceCollectionChanged(const Napi::CallbackInfo& info) {}