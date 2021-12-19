#ifndef AMYLOCK_H
#define AMYLOCK_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

/* 封装信号量的类 */
class ASem
{
public:
    /* 创建并初始化信号量 */
    ASem() 
    {
        if(sem_init(&m_sem, 0, 0) != 0)
        {
            /* 构造函数没有返回值,可以通过抛出异常来报告错误 */
            throw std::exception();
        }
    }

    /* 销毁信号量 */
    ~ASem() 
    {
        sem_destroy(&m_sem);
    }

    /* 等待信号量 */
    bool wait()
    {
        return 0 == sem_wait(&m_sem);
    }

    /* 增加信号量 */
    bool post()
    {
        return 0 == sem_post(&m_sem);
    }

private:
    sem_t m_sem;
};

/* 封装互斥锁的类 */
class ALocker
{
public:
    /* 创建并初始化互斥锁 */
    ALocker()
    {
        if(pthread_mutex_init(&m_mutex, NULL) != 0)
        {
            throw std::exception();
        }
    }

    /* 销毁互斥锁 */
    ~ALocker()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    /* 获取互斥锁 */
    bool lock()
    {
        return 0 == pthread_mutex_lock(&m_mutex);
    }

    /* 释放互斥锁 */
    bool unlock()
    {
        return 0 == pthread_mutex_unlock(&m_mutex);
    }

private:
    pthread_mutex_t m_mutex;
};

/* 封装条件变量的类 */
class ACond
{
public:
    /* 创建并初始化条件变量 */
    ACond()
    {
        if(pthread_mutex_init(&m_mutex, NULL) != 0)
        {
            throw std::exception();
        }

        if(pthread_cond_init(&m_cond, NULL) != 0)
        {
            pthread_mutex_destroy(&m_mutex);
            throw std::exception();
        }
    }

    /* 销毁条件变量 */
    ~ACond()
    {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_cond);
    }

    /* 等待条件变量 */
    bool wait()
    {
        int ret = 0;
        pthread_mutex_lock(&m_mutex);
        ret = pthread_cond_wait(&m_cond, &m_mutex);
        pthread_mutex_unlock(&m_mutex);
        return 0 == ret;
    }

    /* 唤醒等待条件变量的线程 */
    bool signal()
    {
        return 0 == pthread_cond_signal(&m_cond);
    }
private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
};

#endif  // AMYLOCK_H