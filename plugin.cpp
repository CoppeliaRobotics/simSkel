// Copyright 2016 Coppelia Robotics AG. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// -------------------------------------------------------------------
// Authors:
// Federico Ferri <federico.ferri.it at gmail dot com>
// -------------------------------------------------------------------

#include <vector>
#include <memory>

#include "config.h"
#include "plugin.h"
#include "simPlusPlus/Plugin.h"
#include "simPlusPlus/Handles.h"
#include "stubs.h"

// an example data structure to hold data across multiple calls
struct ExampleObject
{
    int a = 0;
    int b = 0;
    std::vector<int> seq;

    using Ptr = std::shared_ptr<ExampleObject>;
};

class Plugin : public sim::Plugin
{
public:
    void onStart()
    {
        if(!registerScriptStuff())
            throw std::runtime_error("script stuff initialization failed");

        setExtVersion("Example Plugin Skeleton");
        setBuildDate(BUILD_DATE);
    }

    void onScriptStateDestroyed(int scriptID)
    {
        for(const ExampleObject::Ptr &obj : handles.find(scriptID))
            handles.remove(obj);
    }

    void createObject(createObject_in *in, createObject_out *out)
    {
        ExampleObject::Ptr obj = std::make_shared<ExampleObject>();

        out->handle = handles.add(obj, in->_.scriptID);
    }

    void destroyObject(destroyObject_in *in, destroyObject_out *out)
    {
        ExampleObject::Ptr obj = handles.get(in->handle);

        handles.remove(obj);
    }

    void setData(setData_in *in, setData_out *out)
    {
        ExampleObject::Ptr obj = handles.get(in->handle);

        if(!obj->seq.empty())
            sim::addLog(sim_verbosity_warnings, "current sequence not empty");

        obj->a = in->a;
        obj->b = in->b;
    }

    void compute(compute_in *in, compute_out *out)
    {
        ExampleObject::Ptr obj = handles.get(in->handle);

        obj->seq.push_back(obj->a + obj->b);
        obj->a = obj->b;
        obj->b = obj->seq.back();
        out->currentSize = obj->seq.size();
    }

    void getOutput(getOutput_in *in, getOutput_out *out)
    {
        ExampleObject::Ptr obj = handles.get(in->handle);

        out->output = obj->seq;
    }

private:
    sim::Handles<ExampleObject::Ptr> handles{"Example.Object"};
};

SIM_PLUGIN(PLUGIN_NAME, PLUGIN_VERSION, Plugin)
#include "stubsPlusPlus.cpp"
