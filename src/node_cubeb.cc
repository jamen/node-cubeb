#include "node_cubeb.h"

void JsParamsToCubebParams(Napi::Object js_params, cubeb_stream_params *params) {
    params->rate = js_params.Get("rate").As<Napi::Number>().Int32Value();
    params->channels = js_params.Get("channels").As<Napi::Number>().Int32Value();
    params->format = static_cast<cubeb_sample_format>(js_params.Get("format").As<Napi::Number>().Int32Value());
    params->layout = js_params.Get("layout").As<Napi::Number>().Int32Value();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // Export cubeb_sample_format values
    exports.Set("CUBEB_SAMPLE_S16LE", Napi::Number::New(env, CUBEB_SAMPLE_S16LE));
    exports.Set("CUBEB_SAMPLE_S16BE", Napi::Number::New(env, CUBEB_SAMPLE_S16BE));
    exports.Set("CUBEB_SAMPLE_S16NE", Napi::Number::New(env, CUBEB_SAMPLE_S16NE));
    exports.Set("CUBEB_SAMPLE_FLOAT32LE", Napi::Number::New(env, CUBEB_SAMPLE_FLOAT32LE));
    exports.Set("CUBEB_SAMPLE_FLOAT32BE", Napi::Number::New(env, CUBEB_SAMPLE_FLOAT32BE));
    exports.Set("CUBEB_SAMPLE_FLOAT32NE", Napi::Number::New(env, CUBEB_SAMPLE_FLOAT32NE));

    // Export cubeb_channel_layout values
    exports.Set("CUBEB_LAYOUT_UNDEFINED", Napi::Number::New(env, CUBEB_LAYOUT_UNDEFINED));
    exports.Set("CUBEB_LAYOUT_MONO", Napi::Number::New(env, CUBEB_LAYOUT_MONO));
    exports.Set("CUBEB_LAYOUT_MONO_LFE", Napi::Number::New(env, CUBEB_LAYOUT_MONO_LFE));
    exports.Set("CUBEB_LAYOUT_STEREO", Napi::Number::New(env, CUBEB_LAYOUT_STEREO));
    exports.Set("CUBEB_LAYOUT_STEREO_LFE", Napi::Number::New(env, CUBEB_LAYOUT_STEREO_LFE));
    exports.Set("CUBEB_LAYOUT_3F", Napi::Number::New(env, CUBEB_LAYOUT_3F));
    exports.Set("CUBEB_LAYOUT_3F_LFE", Napi::Number::New(env, CUBEB_LAYOUT_3F_LFE));
    exports.Set("CUBEB_LAYOUT_2F1", Napi::Number::New(env, CUBEB_LAYOUT_2F1));
    exports.Set("CUBEB_LAYOUT_2F1_LFE", Napi::Number::New(env, CUBEB_LAYOUT_2F1_LFE));
    exports.Set("CUBEB_LAYOUT_3F1", Napi::Number::New(env, CUBEB_LAYOUT_3F1));
    exports.Set("CUBEB_LAYOUT_3F1_LFE", Napi::Number::New(env, CUBEB_LAYOUT_3F1_LFE));
    exports.Set("CUBEB_LAYOUT_2F2", Napi::Number::New(env, CUBEB_LAYOUT_2F2));
    exports.Set("CUBEB_LAYOUT_2F2_LFE", Napi::Number::New(env, CUBEB_LAYOUT_2F2_LFE));
    exports.Set("CUBEB_LAYOUT_QUAD", Napi::Number::New(env, CUBEB_LAYOUT_QUAD));
    exports.Set("CUBEB_LAYOUT_QUAD_LFE", Napi::Number::New(env, CUBEB_LAYOUT_QUAD_LFE));
    exports.Set("CUBEB_LAYOUT_3F2", Napi::Number::New(env, CUBEB_LAYOUT_3F2));
    exports.Set("CUBEB_LAYOUT_3F2_LFE", Napi::Number::New(env, CUBEB_LAYOUT_3F2_LFE));
    exports.Set("CUBEB_LAYOUT_3F2_BACK", Napi::Number::New(env, CUBEB_LAYOUT_3F2_BACK));
    exports.Set("CUBEB_LAYOUT_3F2_LFE_BACK", Napi::Number::New(env, CUBEB_LAYOUT_3F2_LFE_BACK));
    exports.Set("CUBEB_LAYOUT_3F3R_LFE", Napi::Number::New(env, CUBEB_LAYOUT_3F3R_LFE));
    exports.Set("CUBEB_LAYOUT_3F4_LFE", Napi::Number::New(env, CUBEB_LAYOUT_3F4_LFE));

    // Export chbeb_stream_prefs values
    exports.Set("CUBEB_STREAM_PREF_NONE", Napi::Number::New(env, CUBEB_STREAM_PREF_NONE));
    exports.Set("CUBEB_STREAM_PREF_LOOPBACK", Napi::Number::New(env, CUBEB_STREAM_PREF_LOOPBACK));

    // Export cubeb_state values
    exports.Set("CUBEB_STATE_STARTED", Napi::Number::New(env, CUBEB_STATE_STARTED));
    exports.Set("CUBEB_STATE_STOPPED", Napi::Number::New(env, CUBEB_STATE_STOPPED));
    exports.Set("CUBEB_STATE_DRAINED", Napi::Number::New(env, CUBEB_STATE_DRAINED));
    exports.Set("CUBEB_STATE_ERROR", Napi::Number::New(env, CUBEB_STATE_ERROR));

    // Export cubeb_device_type values
    exports.Set("CUBEB_DEVICE_TYPE_UNKNOWN", Napi::Number::New(env, CUBEB_DEVICE_TYPE_UNKNOWN));
    exports.Set("CUBEB_DEVICE_TYPE_INPUT", Napi::Number::New(env, CUBEB_DEVICE_TYPE_INPUT));
    exports.Set("CUBEB_DEVICE_TYPE_OUTPUT", Napi::Number::New(env, CUBEB_DEVICE_TYPE_OUTPUT));

    // Export cubeb_device_state values
    exports.Set("CUBEB_DEVICE_STATE_DISABLED", Napi::Number::New(env, CUBEB_DEVICE_STATE_DISABLED));
    exports.Set("CUBEB_DEVICE_STATE_UNPLUGGED", Napi::Number::New(env, CUBEB_DEVICE_STATE_UNPLUGGED));
    exports.Set("CUBEB_DEVICE_STATE_ENABLED", Napi::Number::New(env, CUBEB_DEVICE_STATE_ENABLED));

    // Export cubeb_device_fmt values
    exports.Set("CUBEB_DEVICE_FMT_S16LE", Napi::Number::New(env, CUBEB_DEVICE_FMT_S16LE));
    exports.Set("CUBEB_DEVICE_FMT_S16BE", Napi::Number::New(env, CUBEB_DEVICE_FMT_S16BE));
    exports.Set("CUBEB_DEVICE_FMT_F32LE", Napi::Number::New(env, CUBEB_DEVICE_FMT_F32LE));
    exports.Set("CUBEB_DEVICE_FMT_F32BE", Napi::Number::New(env, CUBEB_DEVICE_FMT_F32BE));
    exports.Set("CUBEB_DEVICE_FMT_S16NE", Napi::Number::New(env, CUBEB_DEVICE_FMT_S16NE));
    exports.Set("CUBEB_DEVICE_FMT_F32NE", Napi::Number::New(env, CUBEB_DEVICE_FMT_F32NE));
    exports.Set("CUBEB_DEVICE_FMT_S16_MASK", Napi::Number::New(env, CUBEB_DEVICE_FMT_S16_MASK));
    exports.Set("CUBEB_DEVICE_FMT_F32_MASK", Napi::Number::New(env, CUBEB_DEVICE_FMT_F32_MASK));
    exports.Set("CUBEB_DEVICE_FMT_ALL", Napi::Number::New(env, CUBEB_DEVICE_FMT_ALL));

    // Export cubeb_device_pref values
    exports.Set("CUBEB_DEVICE_PREF_NONE", Napi::Number::New(env, CUBEB_DEVICE_PREF_NONE));
    exports.Set("CUBEB_DEVICE_PREF_MULTIMEDIA", Napi::Number::New(env, CUBEB_DEVICE_PREF_MULTIMEDIA));
    exports.Set("CUBEB_DEVICE_PREF_VOICE", Napi::Number::New(env, CUBEB_DEVICE_PREF_VOICE));
    exports.Set("CUBEB_DEVICE_PREF_NOTIFICATION", Napi::Number::New(env, CUBEB_DEVICE_PREF_NOTIFICATION));
    exports.Set("CUBEB_DEVICE_PREF_ALL", Napi::Number::New(env, CUBEB_DEVICE_PREF_ALL));

    NodeCubeb::Init(env, exports);
    NodeCubebStream::Init(env, exports);

    return exports;
}

NODE_API_MODULE(node_cubeb, Init)