#pragma once

#include <QtPlugin>

class i_base
{
public:
	virtual ~i_base() {}

	virtual int add_ref() = 0;
	virtual int release() = 0;
};
Q_DECLARE_INTERFACE(i_base, "launcher.i_base/1.0")

template<class T>
class WSPtr
{
	T*		p;
public:
	inline WSPtr(T* _p = 0) : p(_p) {}

	WSPtr(const WSPtr<T>& _p)
	{
		p = _p.p;
		if (p != NULL)
		{
			p->add_ref();
		}
	}

	template<class X>
	explicit WSPtr(const WSPtr<X>& _p)
	{
		p = const_cast<X*>(_p.data());
		if (p != NULL)
		{
			p->add_ref();
		}
	}

	WSPtr<T>& operator = (const WSPtr<T>& _p)
	{
		if (_p.p == p)
			return *this;
		if (p != 0)
		{
			p->release();
		}
		p = _p.p;
		if (p != NULL)
		{
			p->add_ref();
		}
		return *this;
	}
	WSPtr<T>& operator = (T* _p)
	{
		if (p != 0)
		{
			p->release();
		}
		p = _p;
		return *this;
	}
	inline ~WSPtr()
	{
		if (p != 0)
			p->release();
	}
	inline T* operator->() { return p; }
	inline const T* operator->()const { return p; }
	inline T& operator*() { return *p; }
	inline operator T*() { return p; }
	inline operator const T*()const { return p; }
	inline operator T**() { return &p; }
	inline bool operator == (T* _p)	const { return p == _p; }
	inline bool operator != (T* _p)	const { return p != _p; }
	inline T* data() { return p; }
	inline const T* data() const { return p; }

	template<class X>
	inline WSPtr<X> dynamic_cast_to()
	{
		if (p == NULL)
			return NULL;
		X* xp = dynamic_cast<X*>(p);
		if (xp != NULL)
		{
			xp->add_ref();
		}
		return WSPtr<X>(xp);
	}
};

#include <QAtomicInt>

class SmartPointer
{
public:
	SmartPointer() : refCount(1) {}
	bool ref() { return refCount.ref(); }
	bool deref() { return refCount.deref(); }
	operator int() const { return refCount; }

private:
	QAtomicInt		refCount;
	SmartPointer(const SmartPointer&);
	SmartPointer& operator = (const SmartPointer&);
};

#define DECLARE_SMARTPOINTER						\
private:											\
    SmartPointer			refCount;               \
public:												\
    int add_ref();									\
    int release();									\

#define IMPLEMENT_SMARTPOINTER( class_name )		\
int class_name::add_ref()							\
{													\
    refCount.ref();                                 \
    return refCount;								\
};													\
int class_name::release()							\
{													\
    if( !refCount.deref() )                         \
    {												\
        delete this;								\
        return 0;									\
    }												\
    return refCount;								\
}