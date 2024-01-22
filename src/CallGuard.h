#ifndef STLREPAIR_CALLGUARD__H_
#define STLREPAIR_CALLGUARD__H_

/**
 * Simple RAII class for ensuring a given callable is invoked whenever the
 * instance of this class goes out of scope. Instances of this class
 * should be created with makeCallGuard().
 */
template<typename T>
class CallGuard
{
public:

    //! Constructor.
    explicit CallGuard(T callable) : m_callable(callable)
    {
    }

    //! Destructor
    ~CallGuard()
    {
        m_callable();
    }

private:

    T m_callable;
};

/**
 * Convenience function that performs type deduction so users
 * don't have to explicitly instantiate template types in their
 * code.
 */
template<typename T> CallGuard<T> makeCallGuard(T callable)
{
    return CallGuard<T>(callable);
}

#endif
