
#include "StdInc.h"
#include "MyModule.h"

using namespace Galactic3D;

// The table name, so Module.HelloWorld.anything would be where the functions are.
REGISTER_MODULE("HelloWorld");

// Returns a new class instance of TestClass.
static bool FunctionCreateTestClass(IScriptState* pState, Sint32 argc, void* pUser)
{
	CModule* pModule = (CModule*)pUser;

	Referenceable* pReferenceable = Reflection::New(nullptr, pModule->m_pTestClass);

	pState->ReturnReferenceable(pReferenceable);

	pReferenceable->Release();

	return true;
}

// Just returns true.
static bool FunctionHelloWorld(IScriptState* pState, Sint32 argc, void* pUser)
{
	CModule* pModule = (CModule*)pUser;

	pState->ReturnBoolean(true);

	return true;
}

void* SDLCALL MyRegisterModule(ReflectedNamespace* pNamespace)
{
	CModule* pModule = new CModule;

	static ReflectedClassDecl TestClass("TestClass");
	pModule->m_pTestClass = pNamespace->NewClass(&TestClass);

	static ScriptFunction SFCreateTestClass = { "createTestClass","",FunctionCreateTestClass };
	pNamespace->RegisterFunction(&SFCreateTestClass, pModule);

	static ScriptFunction SFHelloWorld = { "helloWorld","",FunctionHelloWorld };
	pNamespace->RegisterFunction(&SFHelloWorld, pModule);

	return pModule;
}

void SDLCALL MyUnregisterModule(void* pUser)
{
	delete (CModule*)pUser;
}
