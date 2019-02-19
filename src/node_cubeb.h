#ifndef NODECUBEB_H
#define NODECUBEB_H

#include "napi.h"
#include "cubeb/cubeb.h"
#include <iostream>

#define DEBUG(x) x

void JsParamsToCubebParams(Napi::Object, cubeb_stream_params *);

class NodeCubeb : public Napi::ObjectWrap<NodeCubeb> {
    public:
        static Napi::Object Init(Napi::Env, Napi::Object);
        NodeCubeb(const Napi::CallbackInfo&);

        cubeb * internal_context;

    private:
        static Napi::FunctionReference constructor;

        Napi::Value Init(const Napi::CallbackInfo&);
        Napi::Value GetBackendId(const Napi::CallbackInfo&);
        Napi::Value GetMaxChannelCount(const Napi::CallbackInfo&);
        Napi::Value GetMinLatency(const Napi::CallbackInfo&);
        Napi::Value GetPreferredSampleRate(const Napi::CallbackInfo&);
        Napi::Value Destroy(const Napi::CallbackInfo&);
};

class NodeCubebStream : public Napi::ObjectWrap<NodeCubebStream> {
    public:
        static Napi::Object Init(Napi::Env, Napi::Object);
        NodeCubebStream(const Napi::CallbackInfo&);

        cubeb * internal_context;
        cubeb_stream * internal_stream;

    private:
        static Napi::FunctionReference constructor;
};

#endif