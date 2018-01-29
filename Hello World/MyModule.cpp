
#include "StdInc.h"
#include "MyModule.h"

// The table name, so Module.HelloWorld.anything would be where the functions are.
REGISTER_MODULE("HelloWorld");

// Returns a new class instance of TestClass.
static bool FunctionTestClassConstructor(IScriptState* pState, Sint32 argc, void* pUser)
{
	CModule* pModule = (CModule*)pUser;

	auto pReferenceable = Reference<Referenceable>::New(Reflection::New([](Referenceable* pReferenceable) {
		// We delete the class here if it is still existing just incase the user forgets to.
		// This function is called after scripting has no reference to the variable anymore.
		// In some languages this won't be called for some time (until a garbage collection cycle).
		// It shouldn't be used for heavy cleanup generally.
		CTestClass* pRealClass = (CTestClass*)pReferenceable->GetPrivate();
		if (pRealClass != nullptr)
			delete pRealClass;
	}, pModule->m_pTestClass));

	pReferenceable->SetPrivate(new CTestClass);

	pState->ReturnReferenceable(pReferenceable);

	return true;
}

// Destroys the CTestClass.
static bool FunctionDestroyTestClass(IScriptState* pState, Sint32 argc, void* pUser)
{
	CModule* pModule = (CModule*)pUser;

	Referenceable* pReferenceable;
	if (!pState->CheckClass(pModule->m_pTestClass, 0, false, &pReferenceable))
		return false;

	CTestClass* pTestClass = (CTestClass*)pReferenceable->GetPrivate();
	if (!pTestClass)
		return pState->Error("TestClass already destroyed");

	delete pTestClass;
	pReferenceable->SetPrivate(nullptr); // Set private to nullptr, so when we check it for null we know it's destroyed already!

	return true;
}

// Sample function using TestClass...
static bool FunctionSomethingTestClass(IScriptState* pState, Sint32 argc, void* pUser)
{
	CModule* pModule = (CModule*)pUser;

	Referenceable* pReferenceable;
	if (!pState->CheckClass(pModule->m_pTestClass, 0, false, &pReferenceable))
		return false;

	CTestClass* pTestClass = (CTestClass*)pReferenceable->GetPrivate();
	if (!pTestClass)
		return pState->Error("TestClass destroyed");

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

	{
		static ReflectedClassDecl TestClass("TestClass");
		pModule->m_pTestClass = pNamespace->NewClass(&TestClass);

		static ScriptFunction SFTestClassConstructor = { "TestClass","t",FunctionTestClassConstructor };
		pModule->m_pTestClass->RegisterConstructor(&SFTestClassConstructor, pModule);
	}

	static ScriptFunction SFDestroyTestClass = { "destroyTestClass","x",FunctionDestroyTestClass };
	pNamespace->RegisterFunction(&SFDestroyTestClass, pModule);

	static ScriptFunction SFSomethingTestClass = { "somethingTestClass","x",FunctionSomethingTestClass };
	pNamespace->RegisterFunction(&SFSomethingTestClass, pModule);

	static ScriptFunction SFHelloWorld = { "helloWorld","",FunctionHelloWorld };
	pNamespace->RegisterFunction(&SFHelloWorld, pModule);

	return pModule;
}

void SDLCALL MyUnregisterModule(void* pUser)
{
	delete (CModule*)pUser;
}
