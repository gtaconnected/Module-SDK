#pragma once

#ifndef DECLSPEC
#define DECLSPEC
#endif

#include <SDL.h>
#include "ScriptingSDK.h"

#define GAlloc(Size) SDL_malloc(Size)
#define GReAlloc(pMemory, NewSize) SDL_realloc(pMemory, NewSize)
#define GFree(pMemory) SDL_free(pMemory)

namespace Galactic3D
{
	class Referenceable
	{
	protected:
		virtual ~Referenceable(void) {};

	public:
		virtual size_t GetRefCount(void) = 0;
		virtual size_t AddRef(void) = 0;
		virtual size_t Release(void) = 0;

		virtual ReflectedClass* GetReflectedClass(void) = 0;

		void SetPrivate(void* pPrivate);
		void* GetPrivate();
	};

	class ReflectedClass
	{
	public:
		ReflectedProperty* GetProperty(const char* pszName);
		ReflectedFunction* GetFunction(const char* pszName);
		bool RegisterConstructor(const ScriptFunction* pFunction, void* pUser);
		bool RegisterClone(const ScriptFunction* pFunction, void* pUser);
		bool RegisterMove(const ScriptFunction* pFunction, void* pUser);
		bool RegisterToString(const ScriptFunction* pFunction, void* pUser);
		bool RegisterFunction(const ScriptFunction* pFunction, void* pUser);
		bool AddProperty(void* pUser, const char* pszName, unsigned char Type, const ScriptFunction* pGetter, const ScriptFunction* pSetter = nullptr);
	};

	class ReflectedNamespace
	{
	public:
		ReflectedProperty* GetProperty(const char* pszName);
		ReflectedFunction* GetFunction(const char* pszName);
		bool RegisterFunction(const ScriptFunction* pFunction, void* pUser);
		bool AddProperty(void* pUser, const char* pszName, unsigned char Type, const ScriptFunction* pGetter, const ScriptFunction* pSetter = nullptr);
		ReflectedClass* NewClass(ReflectedClassDecl* pClassDecl, ReflectedClass* pParent = nullptr);
	};

	namespace Reflection
	{
		// Use this function to get why any of the functions failed below
		const char* GetLastError(void);
		Referenceable* Clone(Referenceable* pReferenceable);
		bool Move(Referenceable* pReferenceable, Referenceable* pNewReferenceable);
		char* ToString(Referenceable* pReferenceable, size_t* pLength);
		ReflectedProperty* LookupProperty(Referenceable* pReferenceable, const char* pszName);
		ReflectedFunction* LookupFunction(Referenceable* pReferenceable, const char* pszName);
		bool GetProperty(Referenceable* pReferenceable, ReflectedProperty* pProperty, CReturnValue* pReturnValue);
		bool GetProperty(Referenceable* pReferenceable, const char* pszName, CReturnValue* pReturnValue);
		bool SetProperty(Referenceable* pReferenceable, ReflectedProperty* pProperty, CArguments* pArguments);
		bool SetProperty(Referenceable* pReferenceable, const char* pszName, CArguments* pArguments);
		bool Call(Referenceable* pThis, ReflectedFunction* pFunction, CArguments* pArguments, CReturnValue* pReturnValue);
		bool Call(Referenceable* pThis, const char* pszName, CArguments* pArguments, CReturnValue* pReturnValue);
		Referenceable* New(ReferenceableFinaliser Finaliser, ReflectedClass* pClass);
	}
}

#define REGISTER_MODULE(Name) extern "C" __declspec(dllexport) Uint32 MODULE_VERSION = 1; extern "C" __declspec(dllexport) const char* MODULE_NAME = Name;
