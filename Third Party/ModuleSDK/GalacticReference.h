#pragma once

namespace Galactic3D
{
	// Always use WeakReference especially when using Pool
	template<class T> class WeakReference
	{
	public:
		WeakReference(void)
		{
			m_pRef = nullptr;
		}

		WeakReference(T* pReferenceable)
		{
			m_pRef = pReferenceable;
		}

		template<class Y>
		WeakReference(const WeakReference<Y>& r)
		{
			m_pRef = static_cast<T*>(r.GetPointer());
		}

		WeakReference(const WeakReference<T>& r)
		{
			m_pRef = r.m_pRef;
		}

		~WeakReference(void)
		{
		}

	protected:
		T* m_pRef;

	public:
		template<class Y>
		WeakReference& operator=(Y* r)
		{
			if (m_pRef != static_cast<T*>(r))
			{
				m_pRef = static_cast<T*>(r);
			}
			return *this;
		}

		template<class Y>
		WeakReference& operator=(const WeakReference<Y>& r)
		{
			if (m_pRef != static_cast<T*>(r.GetPointer()))
			{
				m_pRef = static_cast<T*>(r.GetPointer());
			}
			return *this;
		}

		WeakReference& operator=(const WeakReference<T>& r)
		{
			if (m_pRef != r.GetPointer())
			{
				m_pRef = r.GetPointer();
			}
			return *this;
		}

		template<class Y>
		inline WeakReference<Y> StaticCast(void) const
		{
			if (m_pRef != NULL)
			{
				return WeakReference<Y>(static_cast<Y*>(m_pRef));
			}
			return WeakReference<Y>();
		}

		inline operator T*() const { return m_pRef; }
		inline T& operator*() const { SDL_assert(m_pRef); return *m_pRef; }
		inline T* operator->() const { SDL_assert(m_pRef); return m_pRef; }
		inline T* GetPointer(void) const { return m_pRef; }
		inline bool IsNull(void) const { return m_pRef == nullptr; }
		inline void SetNull(void) {
			m_pRef = nullptr;
		}
	};

	template<class T, class U> inline bool operator==(WeakReference<T> const& a, WeakReference<U> const& b)
	{
		return a.GetPointer() == b.GetPointer();
	}

	template<class T, class U> inline bool operator!=(WeakReference<T> const& a, WeakReference<U> const& b)
	{
		return a.GetPointer() != b.GetPointer();
	}

	// Strong reference
	template<class T> class Reference
	{
	public:
		Reference(void)
		{
			m_pRef = nullptr;
		}

		Reference(T* pReferenceable)
		{
			m_pRef = pReferenceable;
			if (m_pRef != NULL)
				m_pRef->AddRef();
		}

		template<class Y>
		Reference(const Reference<Y>& r)
		{
			m_pRef = static_cast<T*>(r.GetPointer());
			if (m_pRef != NULL)
				m_pRef->AddRef();
		}

		Reference(const Reference<T>& r)
		{
			m_pRef = r.m_pRef;
			if (m_pRef != NULL)
				m_pRef->AddRef();
		}

		~Reference(void)
		{
			if (m_pRef != NULL)
				m_pRef->Release();
		}

	protected:
		T* m_pRef;

	public:
		static Reference New(T* r)
		{
			Reference<T> Ref;
			Ref.m_pRef = r;
			return Ref;
		}

		template<class Y>
		static Reference New(Y* r)
		{
			Reference<T> Ref;
			Ref.m_pRef = static_cast<T*>(r);
			return Ref;
		}

		template<class Y>
		Reference& operator=(Y* r)
		{
			if (m_pRef != static_cast<T*>(r))
			{
				if (m_pRef != NULL)
					m_pRef->Release();
				m_pRef = static_cast<T*>(r);
				if (m_pRef != NULL)
					m_pRef->AddRef();
			}
			return *this;
		}

		template<class Y>
		Reference& operator=(const Reference<Y>& r)
		{
			if (m_pRef != static_cast<T*>(r.GetPointer()))
			{
				if (m_pRef != NULL)
					m_pRef->Release();
				m_pRef = static_cast<T*>(r.GetPointer());
				if (m_pRef != NULL)
					m_pRef->AddRef();
			}
			return *this;
		}

		Reference& operator=(const Reference<T>& r)
		{
			if (m_pRef != r.GetPointer())
			{
				if (m_pRef != NULL)
					m_pRef->Release();
				m_pRef = r.GetPointer();
				if (m_pRef != NULL)
					m_pRef->AddRef();
			}
			return *this;
		}

		template<class Y>
		inline Reference<Y> StaticCast(void) const
		{
			if (m_pRef != NULL)
			{
				m_pRef->AddRef();
				return Reference<Y>(static_cast<Y*>(m_pRef));
			}
			return Reference<Y>();
		}

		inline operator T*() const { return m_pRef; }
		inline T& operator*() const { SDL_assert(m_pRef); return *m_pRef; }
		inline T* operator->() const { SDL_assert(m_pRef); return m_pRef; }
		inline T* GetPointer(void) const { return m_pRef; }
		inline bool IsNull(void) const { return m_pRef == NULL; }
		inline void SetNull(void) {
			if (m_pRef != NULL)
				m_pRef->Release();
			m_pRef = NULL;
		}
	};

	template<class T, class U> inline bool operator==(Reference<T> const& a, Reference<U> const& b)
	{
		return a.GetPointer() == b.GetPointer();
	}

	template<class T, class U> inline bool operator!=(Reference<T> const& a, Reference<U> const& b)
	{
		return a.GetPointer() != b.GetPointer();
	}
};
