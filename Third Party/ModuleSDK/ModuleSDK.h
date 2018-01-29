#pragma once

#ifndef DECLSPEC
#define DECLSPEC
#endif

#include <SDL.h>
#include "GalacticReference.h"
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

	// TODO: Implement these
	class Stream;
	class Pool;

	enum eArgumentType
	{
		ARGUMENT_NIL,
		ARGUMENT_BOOLEAN,
		ARGUMENT_STRING,
		ARGUMENT_TABLE,//NEVER USED
		ARGUMENT_INTEGER,
		ARGUMENT_FLOAT,
		ARGUMENT_ELEMENT,//NETWORK ONLY!
		ARGUMENT_REFERENCEABLE,
		ARGUMENT_VECTOR2D,
		ARGUMENT_VECTOR3D,
		ARGUMENT_VECTOR4D,
		ARGUMENT_MATRIX4X4,
		ARGUMENT_UNDEFINED,
		ARGUMENT_LUAREF,//SPECIAL; GETS SWALLOWED!
		ARGUMENT_JSVALUE,//SPECIAL; GETS SWALLOWED!
		ARGUMENT_LONGINTEGER,//64-bit
		ARGUMENT_DOUBLE,//64-bit
		ARGUMENT_ARRAY,
		ARGUMENT_MAP,//NEVER USED BUT RESERVED
		ARGUMENT_FUNCTION,//GETS SWALLOWED!
		ARGUMENT_SQREF,//SPECIAL; GETS SWALLOWED!
	};

	class CArgument
	{
	public:
		virtual ~CArgument(void) {}

		virtual bool Read(Stream* pStream) = 0;
		virtual bool GetSize(size_t& Size) = 0;
		virtual bool Write(Stream* pStream) = 0;

		virtual CArgument* Clone(void) const = 0;
		virtual CArgument* Clone(Pool* pPool) const = 0;

		unsigned char GetType(void) const;
		const char* GetTypeName(void) const;

		bool IsNull(void) const;
		bool IsBoolean(void) const;
		bool IsString(void) const;
		bool IsReferenceable(void) const;
		bool IsVector2D(void) const;
		bool IsVector3D(void) const;
		bool IsVector4D(void) const;
		bool IsMatrix4x4(void) const;
		bool IsNumber(void) const;
		bool IsArray(void) const;
		bool IsFunction(void) const;

		bool ToBoolean(void) const;
		const char* ToString(size_t* pLength = nullptr) const;
		Referenceable* ToReferenceable(ReflectedClass* pClass) const;
		Referenceable* ToReferenceable(void) const;
		void ToVector2D(Math::Vector2D& vec) const;
		void ToVector3D(Math::Vector3D& vec) const;
		void ToVector4D(Math::Vector4D& vec) const;
		void ToMatrix4x4(Math::Matrix4x4& mat) const;
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
