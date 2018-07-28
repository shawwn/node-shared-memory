#include <v8.h>
#include <node.h>
#include <nan.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>

#include "shmem.h"

#define JS_STR(...) Nan::New<v8::String>(__VA_ARGS__).ToLocalChecked()
#define JS_INT(val) Nan::New<v8::Integer>(val)
#define JS_INT64(val) ((val)->IntegerValue())
#define JS_NUM(val) Nan::New<v8::Number>(val)
#define JS_FLOAT(val) Nan::New<v8::Number>(val)
#define JS_BOOL(val) Nan::New<v8::Boolean>(val)



using namespace v8;
using namespace std;

/* const int MEM = 800000; /1* Two hundred thousand 32-bit integers *1/ */

void shm_open(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    v8::String::Utf8Value name(args[0]);
    uint64_t size = JS_INT64(args[1]);
    uint32_t flags = args[2]->Uint32Value();
    if (size <= 0) {
        flags |= SHMEM_MUST_NOT_CREATE;
    }
    //cout << "shmemOpen(" << " name=" << *name << " size=" << size << "flags=" << flags << ")\n";
    shmem* mem = shmemOpen( size, flags, "%s", *name );
    //cout << "shmemResult(" << " name=" << *name << " size=" << size << "flags=" << flags << " mem=" << (size_t)mem << " ptr=" << (size_t)shmemGetPtr(mem) << ")\n";
    void* data = shmemGetPtr( mem );

    if( !data ) {
        cout << "shmem failed." << endl;
        if (size <= 0) {
            cout << "you requested zero bytes." << endl;
        }
        shmemClose( mem );
        return;
    }

    size = shmemGetSize(mem);
    //cout << "shm_open got data " << (size_t)data << " of size " << size << "\n";

    /* Create a ArrayBuffer */
    Local<ArrayBuffer> buffer = ArrayBuffer::New(isolate, data, size);

    /* Return buffer */
    args.GetReturnValue().Set(buffer);
}

void init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "open", shm_open);
}

NODE_MODULE(shm_addon, init)
