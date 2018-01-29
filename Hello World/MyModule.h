#pragma once

template<class T> class Class
{
public:
	ReflectedClass * m_pClass;

	Reference<Referenceable> Check(IScriptState* pScriptState, size_t Index, bool bNull)
	{
		Referenceable* pReferenceable;
		if (!pScriptState->CheckClass(m_pClass, Index, bNull, &pReferenceable))
			return nullptr;
		return pReferenceable;
	}

	Reference<Referenceable> Create(void)
	{
		Reference<Referenceable> pReferenceable = Reference<Referenceable>::New(Reflection::New([](Referenceable* pReferenceable) {
			// We delete the class here if it is still existing just incase the user forgets to.
			// This function is called after scripting has no reference to the variable anymore.
			// In some languages this won't be called for some time (until a garbage collection cycle).
			// It shouldn't be used for heavy cleanup generally.
			T* pRealClass = (T*)pReferenceable->GetPrivate();
			if (pRealClass != nullptr)
				delete pRealClass;
		}, m_pClass));

		pReferenceable->SetPrivate(new T);
		return pReferenceable;
	}
};

// Just a sample class to test...
class CTestClass
{
public:
};

class CModule
{
public:
	ReflectedClass* m_pTestClass;
};
