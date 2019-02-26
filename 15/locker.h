#pragma once
#include<exception>
#include<pthread.h>
#include<semaphore.h>
using namespace std ;
//封装信号的类
class sem
{
public:
    sem() 
    {
        if( sem_init( &m_sem, 0, 0 ) != 0 )
        {
            throw exception() ;
        }
    }
    ~sem() 
    {
        sem_destroy( &m_sem ) ;
    }
    
    //等待信号量
    bool wait()
    {
        return  sem_wait( &m_sem ) == 0 ;
    }

    //添加信号量
    bool post()
    {
        return sem_post( &m_sem ) == 0 ;
    }

private:
    sem_t m_sem ;
};

//添加互斥锁的类
class locker
{
public:
    locker() 
    {
        if( pthread_mutex_init( &_mutex, NULL ) != 0 ){
            throw exception() ;
        }   
    }
    //销毁互斥锁
    ~locker()
    {
        pthread_mutex_destroy( &_mutex ) ;
    }

    bool lock()
    {
        return pthread_mutex_lock( &_mutex ) == 0 ;
    }

    bool unlock()
    {
        return pthread_mutex_unlock( &_mutex ) ;
    }

private:
    pthread_mutex_t _mutex ;
};

//封装共享变量的类
class cond
{
public:
    cond()
    {
        pthread_cond_init( &_cond, NULL ) ;
    }
    
    ~cond()
    {
        pthread_cond_destroy( &_cond ) ;
        pthread_mutex_destroy( &mutex_ ) ;
    }

    bool wait()
    {
        int ret = 0 ;
        pthread_mutex_lock( &mutex_ ) ;
        ret = pthread_cond_wait( &_cond, &mutex_ ) ;
        pthread_mutex_unlock( &mutex_ ) ;
        return ret == 0 ;
    }
    
    bool notify()
    {
            return pthread_cond_signal( &_cond ) == 0 ;
    }

private:
    pthread_cond_t _cond ;
    pthread_mutex_t mutex_ ;

};
