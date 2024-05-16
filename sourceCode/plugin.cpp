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
#include <simPlusPlus/Plugin.h>
#include <simPlusPlus/Handles.h>
#include "stubs.h"

// an example data structure to hold data across multiple calls
struct ExampleObject
{
    int a = 0;
    int b = 0;
    std::vector<int> seq;

    // convenience alias for using shared_ptr
    using Ptr = std::shared_ptr<ExampleObject>;
};

class Plugin : public sim::Plugin
{
public:
    void onInit()
    {
        if(!registerScriptStuff())
            throw std::runtime_error("script stuff initialization failed");

        setExtVersion("Example Plugin Skeleton");
        setBuildDate(BUILD_DATE);
    }

    void createObject(createObject_in *in, createObject_out *out)
    {
        // create a new ExampleObject (using shared pointers)
        ExampleObject::Ptr obj = std::make_shared<ExampleObject>();

        // add to the list of handles, and return its handle
        out->handle = handles.add(obj, in->_.scriptID);
    }

    void destroyObject(destroyObject_in *in, destroyObject_out *out)
    {
        // get the object by handle (will throw an exception if it doesn't
        // exist; exception will show up as Lua errors)
        ExampleObject::Ptr obj = handles.get(in->handle);

        // remove the object from the list of handles (will also cause object
        // destruction if refcount drops to zero)
        handles.remove(obj);
    }

    void onScriptStateAboutToBeDestroyed(int scriptHandle, int scriptUid)
    {
        // when a script ends, we usually want to destroy all objects created
        // from within that script
        for(const ExampleObject::Ptr &obj : handles.find(scriptHandle))
            handles.remove(obj);
    }

    void setData(setData_in *in, setData_out *out)
    {
        // an example method

        ExampleObject::Ptr obj = handles.get(in->handle);

        if(!obj->seq.empty())
            sim::addLog(sim_verbosity_warnings, "current sequence not empty");

        obj->a = in->a;
        obj->b = in->b;
    }

    void compute(compute_in *in, compute_out *out)
    {
        // another example method

        ExampleObject::Ptr obj = handles.get(in->handle);

        obj->seq.push_back(obj->a + obj->b);
        obj->a = obj->b;
        obj->b = obj->seq.back();
        out->currentSize = obj->seq.size();
    }

    void getOutput(getOutput_in *in, getOutput_out *out)
    {
        // another example method

        ExampleObject::Ptr obj = handles.get(in->handle);

        out->output = obj->seq;
    }

private:
    // the set of handles to ExampleObject
    sim::Handles<ExampleObject::Ptr> handles{"Example.Object"};
};

SIM_PLUGIN(Plugin)
#include "stubsPlusPlus.cpp"
