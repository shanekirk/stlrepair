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
    explicit CallGuard(T callable) : m_callable(callable),
        m_dismiss(false)
    {
    }

    //! Destructor
    ~CallGuard()
    {
        if (!m_dismiss)
            m_callable();
    }

    //! Dismisses the callable from being invoked.
    void dismiss()
    {
        m_dismiss = true;
    }

private:

    T m_callable;
    bool m_dismiss;
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
