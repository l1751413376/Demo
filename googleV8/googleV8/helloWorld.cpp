/**
 *  Google v8 shared library for Windows x86 & x64 under Microsoft Visual Studio 2013
 *  
 *  Compiled by march511@gmail.com
 *  2014-04-08 01:29
 *
 *  To compile helloWorld.cpp :
 *  e.g. cl.exe helloWorld.cpp -I.\Include .\x86\v8.lib
 *
 */
/**
 *  Code from https://developers.google.com/v8/get_started
 */
#include <v8.h>
using namespace v8;

int main(int argc, char* argv[]) {
  // Get the default Isolate created at startup.
  Isolate* isolate = Isolate::GetCurrent();

  // Create a stack-allocated handle scope.
  HandleScope handle_scope(isolate);

  // Create a new context.
  Handle<Context> context = Context::New(isolate);

  // Enter the context for compiling and running the hello world script.
  Context::Scope context_scope(context);

  // Create a string containing the JavaScript source code.
  Handle<String> source = String::NewFromUtf8(isolate, "'Hello' + ', World!'");
  
  // Compile the source code.
  Handle<Script> script = Script::Compile(source);
  
  // Run the script to get the result.
  Handle<Value> result = script->Run();
  
  // Convert the result to an UTF8 string and print it.
  String::Utf8Value utf8(result);
  printf("%s\n", *utf8);
  return 0;
}
