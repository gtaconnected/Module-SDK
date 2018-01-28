#pragma once

#ifndef GALACTIC3D_INLINE
#define GALACTIC3D_INLINE inline
#endif

namespace Galactic3D
{
	class IScriptState;
	class Referenceable;
	class CResource;
	class CScripting;
	class CArgument;
	class CArguments;
	class CReturnValue;
	class ReflectedClass;
	class ReflectedNamespace;
	class CFunction;

	namespace Math
	{
		class Vector2D;
		class Vector3D;
		class Matrix4x4;
	};

	struct ReflectedProperty;
	struct ReflectedFunction;

	typedef bool(*ScriptFunctionCallback) (IScriptState* pState, Sint32 argc, void* pUser);

	// Mask:
	// b - bool
	// i - int32
	// f - float
	// s - string
	// c - function/callback
	// x - class/instance
	// z - vector2D
	// v - vector3D
	// t - thiscall
	// | - optional arguments
	// * - infinite arguments
	// . - any type
	struct ScriptFunction
	{
		const char* m_pszName;
		const char* m_pszMask;
		ScriptFunctionCallback m_Callback;

		unsigned int CalcArgumentCount(void) const;
	};

	class IScriptState
	{
	public:
		virtual bool Error(const char* pszFormat, ...)=0;
		virtual CScripting* GetScripting(void)=0;
		virtual CResource* GetResource(void)=0;
		virtual const char* GetFunctionName(void)=0;

		virtual const CArgument* GetArgument(size_t Index)=0;
		virtual bool CheckBoolean(size_t Index, bool* pbValue)=0;
		virtual bool CheckNumber(size_t Index, Sint8* pValue) = 0;
		virtual bool CheckNumber(size_t Index, Uint8* pValue) = 0;
		virtual bool CheckNumber(size_t Index, Sint16* pValue) = 0;
		virtual bool CheckNumber(size_t Index, Uint16* pValue) = 0;
		virtual bool CheckNumber(size_t Index, Sint32* pValue) = 0;
		virtual bool CheckNumber(size_t Index, Uint32* pValue) = 0;
		virtual bool CheckNumber(size_t Index, Sint64* pValue) = 0;
		virtual bool CheckNumber(size_t Index, Uint64* pValue) = 0;
		virtual bool CheckNumber(size_t Index, float* pValue) = 0;
		virtual bool CheckNumber(size_t Index, double* pValue) = 0;
		virtual const char* CheckString(size_t Index, size_t* pLength = nullptr)=0;
		virtual bool CheckVector2D(size_t Index, Math::Vector2D& vec)=0;
		virtual bool CheckVector3D(size_t Index, Math::Vector3D& vec) = 0;
		virtual bool CheckMatrix4x4(size_t Index, Math::Matrix4x4& mat) = 0;
		virtual bool CheckClass(ReflectedClass* pClass, size_t Index, bool bNull, Referenceable** ppReferenceable)=0;
		template<class T> bool CheckClass(ReflectedClass* pClass, size_t Index, bool bNull, T** ppReferenceable)
		{
			Referenceable* pReferenceable;
			if (!CheckClass(pClass,Index,bNull,&pReferenceable))
				return false;
			*ppReferenceable = static_cast<T*>(pReferenceable);
			return true;
		}
		virtual CFunction* CheckFunction(size_t Index) = 0;
		//virtual Stream* CheckStream(size_t Index) = 0;
		virtual bool GetThis(ReflectedClass* pClass, Referenceable** ppReferenceable)=0;
		template<class T> bool GetThis(ReflectedClass* pClass, T** ppReferenceable)
		{
			Referenceable* pReferenceable;
			if (!GetThis(pClass,&pReferenceable))
				return false;
			*ppReferenceable = static_cast<T*>(pReferenceable);
			return true;
		}

		virtual void Return(CArgument* pArgument)=0;
		virtual void Return(const CArgument* pArgument)=0;
		virtual void ReturnNull(void) = 0;
		virtual void ReturnUndefined(void)=0;
		virtual void ReturnBoolean(bool b)=0;
		virtual void ReturnString(const char* psz, size_t Length)=0;
		virtual void ReturnString(const char* psz)=0;
		virtual void ReturnNumber(Sint8 Value) = 0;
		virtual void ReturnNumber(Uint8 Value) = 0;
		virtual void ReturnNumber(Sint16 Value) = 0;
		virtual void ReturnNumber(Uint16 Value) = 0;
		virtual void ReturnNumber(Sint32 Value) = 0;
		virtual void ReturnNumber(Uint32 Value) = 0;
		virtual void ReturnNumber(Sint64 Value) = 0;
		virtual void ReturnNumber(Uint64 Value) = 0;
		virtual void ReturnFloat(float f)=0;
		virtual void ReturnDouble(double f)=0;
		virtual void ReturnVector2D(const Math::Vector2D& vec)=0;
		virtual void ReturnVector3D(const Math::Vector3D& vec)=0;
		virtual void ReturnMatrix4x4(const Math::Matrix4x4& mat)=0;
		virtual void ReturnReferenceable(Referenceable* pReferenceable)=0;
	};

	class ReflectedClassDecl
	{
	public:
		ReflectedClassDecl(const char* pszName) :
			m_pszName(pszName)
		{
		}

		const char* m_pszName;
		char m_Buffer[128];
	};

	typedef void(*ReferenceableFinaliser)(Referenceable* pReferenceable);

	typedef struct {
		void*(*Alloc)(size_t Size);
		void*(*ReAlloc)(void* pMemory, size_t NewSize);
		void(*Free)(void* pMemory);

		bool(*SetLastError)(const char* fmt, ...); // Unconditionally returns false
		const char*(*GetLastError)(); // If anything returns false or null, this will tell you why

		void(*SetPrivate)(Referenceable* pReferenceable, void* pPrivate);
		void*(*GetPrivate)(Referenceable* pReferenceable);

		Referenceable*(*Clone)(Referenceable* pReferenceable); // Must release
		bool(*Move)(Referenceable* pReferenceable, Referenceable* pNewReferenceable); // Invalidates this class
		char*(*ToString)(Referenceable* pReferenceable, size_t* pLength); // Must free the string

		ReflectedProperty*(*LookupProperty)(Referenceable* pReferenceable, const char* pszName);
		ReflectedFunction*(*LookupFunction)(Referenceable* pReferenceable, const char* pszName);

		ReflectedProperty*(*LookupClassProperty)(ReflectedClass* pClass, const char* pszName);
		ReflectedFunction*(*LookupClassFunction)(ReflectedClass* pClass, const char* pszName);

		ReflectedProperty*(*LookupNamespaceProperty)(ReflectedNamespace* pNamespace, const char* pszName);
		ReflectedFunction*(*LookupNamespaceFunction)(ReflectedNamespace* pNamespace, const char* pszName);

		bool(*GetPropertyPtr)(Referenceable* pReferenceable, ReflectedProperty* pProperty, CReturnValue* pReturnValue);
		bool(*GetPropertyStr)(Referenceable* pReferenceable, const char* pszName, CReturnValue* pReturnValue);

		bool(*SetPropertyPtr)(Referenceable* pReferenceable, ReflectedProperty* pProperty, CArguments* pArguments);
		bool(*SetPropertyStr)(Referenceable* pReferenceable, const char* pszName, CArguments* pArguments);

		bool(*Call)(Referenceable* pThis, ReflectedFunction* pFunction, CArguments* pArguments, CReturnValue* pReturnValue);
		bool(*CallMemb)(Referenceable* pThis, const char* pszName, CArguments* pArguments, CReturnValue* pReturnValue);

		Referenceable*(*New)(ReferenceableFinaliser Finaliser, ReflectedClass* pClass);

		bool(*RegisterClassConstructor)(ReflectedClass* pClass, const ScriptFunction* pFunction, void* pUser);
		bool(*RegisterClassClone)(ReflectedClass* pClass, const ScriptFunction* pFunction, void* pUser);
		bool(*RegisterClassMove)(ReflectedClass* pClass, const ScriptFunction* pFunction, void* pUser);
		bool(*RegisterClassToString)(ReflectedClass* pClass, const ScriptFunction* pFunction, void* pUser);
		bool(*RegisterClassFunction)(ReflectedClass* pClass, const ScriptFunction* pFunction, void* pUser);
		bool(*AddClassProperty)(ReflectedClass* pClass, void* pUser, const char* pszName, unsigned char Type, const ScriptFunction* pGetter, const ScriptFunction* pSetter);

		bool(*RegisterNamespaceFunction)(ReflectedNamespace* pNamespace, const ScriptFunction* pFunction, void* pUser);
		bool(*AddNamespaceProperty)(ReflectedNamespace* pNamespace, void* pUser, const char* pszName, unsigned char Type, const ScriptFunction* pGetter, const ScriptFunction* pSetter);
		ReflectedClass*(*NewClass)(ReflectedNamespace* pNamespace, ReflectedClassDecl* pClassDecl, ReflectedClass* pParent);
	} ModuleAPI;
};
