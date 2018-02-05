
/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2018 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file   condvar.cpp

    \brief  Condition Variable implementation
*/

#include "mark3cfg.h"
#include "condvar.h"

#define _CAN_HAS_DEBUG
//--[Autogenerated - Do Not Modify]------------------------------------------
#include "dbg_file_list.h"
#include "buffalogger.h"
#if defined(DBG_FILE)
# error "Debug logging file token already defined!  Bailing."
#else
# define DBG_FILE _DBG___KERNEL_CONDVAR_CPP
#endif
//--[End Autogenerated content]----------------------------------------------

namespace Mark3 {

void ConditionVariable::Init()
{
    m_clMutex.Init();
    m_clSemaphore.Init(0, 255);
}

void ConditionVariable::Wait(Mutex* pclMutex_)
{
    m_clMutex.Claim();

    pclMutex_->Release();
    m_u8Waiters++;

    m_clMutex.Release();

    m_clSemaphore.Pend();
    pclMutex_->Claim();
}

#if KERNEL_USE_TIMEOUTS
bool ConditionVariable::Wait(Mutex* pclMutex_, uint32_t u32WaitTimeMS_)
{
    m_clMutex.Claim();

    pclMutex_->Release();
    m_u8Waiters++;

    m_clMutex.Release();

    auto rc = m_clSemaphore.Pend(u32WaitTimeMS_);
    if (!rc) {
        return false;
    }
    return pclMutex_->Claim(u32WaitTimeMS_);
}
#endif

void ConditionVariable::Signal()
{
    m_clMutex.Claim();
    if (m_u8Waiters) {
        m_u8Waiters--;
        m_clSemaphore.Post();
    }
    m_clMutex.Release();
}

void ConditionVariable::Broadcast()
{
    m_clMutex.Claim();

    while (m_u8Waiters > 0) {
        m_u8Waiters--;
        m_clSemaphore.Post();
    }

    m_clMutex.Release();
}

} // namespace Mark3
