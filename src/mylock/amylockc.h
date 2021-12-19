#ifndef AMYLOCKC_H
#define AMYLOCKC_H

#include <mutex>

class AMyLockC
{
public:
    explicit AMyLockC(std::mutex &m) : m_mutex(m)
    {
        m_mutex.lock();
    }

    ~AMyLockC()
    {
        m_mutex.unlock();
    }

private:
    std::mutex &m_mutex;
};


#endif  // AMYLOCKC_H