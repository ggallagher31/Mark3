/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
=========================================================================== */
/*!
    \file   autoalloc.h

    \brief  Automatic memory allocation for kernel objects.
*/

#ifndef __AUTO_ALLOC_H__
#define __AUTO_ALLOC_H__

#include <stdint.h>
#include <stdbool.h>
#include "mark3cfg.h"

#if KERNEL_USE_AUTO_ALLOC
//---------------------------------------------------------------------------
// Define function pointer types used for interfacing with an external heap.

//---------------------------------------------------------------------------
typedef enum {
//-- Kernel object types
    AUTO_ALLOC_TYPE_EVENTFLAG,
    AUTO_ALLOC_TYPE_MAILBOX,
    AUTO_ALLOC_TYPE_MESSAGE,
    AUTO_ALLOC_TYPE_MESSAGEQUEUE,
    AUTO_ALLOC_TYPE_MUTEX,
    AUTO_ALLOC_TYPE_NOTIFY,
    AUTO_ALLOC_TYPE_SEMAPHORE,
    AUTO_ALLOC_TYPE_THREAD,
    AUTO_ALLOC_TYPE_TIMER,
//-- Allow for users to define their own object types beginning with AUTO_ALLOC_TYPE_USER
    AUTO_ALLOC_TYPE_USER,
//--
    AUTO_ALLOC_TYPE_RAW = 0xFF
} AutoAllocType_t;

//---------------------------------------------------------------------------
typedef void* (*AutoAllocAllocator_t)(AutoAllocType_t eType_, size_t sSize_);
typedef void* (*AutoAllocFree_t)(AutoAllocType_t eType_, void* pvObj_);

//---------------------------------------------------------------------------
// Forward declaration of kernel objects that can be auotomatically allocated.
#if KERNEL_USE_EVENTFLAG
class EventFlag;
#endif

#if KERNEL_USE_MAILBOX
class Mailbox;
#endif

#if KERNEL_USE_MESSAGE
class Message;
class MessageQueue;
#endif

#if KERNEL_USE_MUTEX
class Mutex;
#endif

#if KERNEL_USE_NOTIFY
class Notify;
#endif

#if KERNEL_USE_SEMAPHORE
class Semaphore;
#endif

class Thread;

#if KERNEL_USE_TIMERS
class Timer;
#endif

class AutoAlloc
{
public:
    /*!
     * \brief Init
     *
     * Initialize the AutoAllocator before use.  Called by Kernel::Init().
     *
     */
    static void Init(void);

    /*!
     * \brief SetAllocatorFunctions
     * Set the functions used by this class to allocate/free memory used in the kernel.
     * \param pfAllocator_ Function to allocate an object based on its type and/or size
     * \param pfFree_ Function to free a previously-allocated object
     */
    static void SetAllocatorFunctions(AutoAllocAllocator_t pfAllocator_, AutoAllocFree_t pfFree_)
        {m_pfAllocator = pfAllocator_; m_pfFree = pfFree_;}

#if KERNEL_USE_SEMAPHORE
    /*!
     * \brief NewSemaphore
     * Allocate and create a new Mutex object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Semaphore* NewSemaphore();

    /*!
     * \brief DestroySemaphore
     * Destroy and deallocate a previously-allocated Semaphore object from within the system.
     * \param pclSemaphore_ Pointer to the object to destroy.
     */
    static void DestroySemaphore(Semaphore* pclSemaphore_);
#endif
#if KERNEL_USE_MUTEX
    /*!
     * \brief NewMutex
     * Allocate and create a new Mutex object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Mutex* NewMutex();

    /*!
     * \brief DestroyMutex
     * Destroy and deallocate a previously-allocated Mutex object from within the system.
     * \param pclMutex_ Pointer to the object to destroy.
     */
    static void DestroyMutex(Mutex* pclMutex_);
#endif
#if KERNEL_USE_EVENTFLAG
    /*!
     * \brief NewEventFlag
     * Allocate and create a new EventFlag object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static EventFlag* NewEventFlag();

    /*!
     * \brief DestroyEventFlag
     * Destroy and deallocate a previously-allocated EventFlag object from within the system.
     * \param pclEventFlag_ Pointer to the object to destroy.
     */
    static void DestroyEventFlag(EventFlag* pclEventFlag_);
#endif
#if KERNEL_USE_TIMERS
    /*!
     * \brief NewTimer
     * Allocate and create a new Timer object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Timer* NewTimer();

    /*!
     * \brief DestroyTimer
     * Destroy and deallocate a previously-allocated Timer object from within the system.
     * \param pclTimer_ Pointer to the object to destroy.
     */
    static void DestroyTimer(Timer* pclTimer_);
#endif
#if KERNEL_USE_NOTIFY
    /*!
     * \brief NewNotify
     * Allocate and construct a new Thread object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Notify* NewNotify();

    /*!
     * \brief DestroyNotify
     * Destroy and deallocate a previously-allocated Notify object from within the system.
     * \param pclNotify_ Pointer to the object to destroy.
     */
    static void DestroyNotify(Notify* pclNotify_);
#endif
#if KERNEL_USE_MAILBOX
    /*!
     * \brief NewMailbox
     * Allocate and construct a new Mailbox object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Mailbox* NewMailbox();

    /*!
     * \brief DestroyMailbox
     * Destroy and deallocate a previously-allocated Mailbox object from within the system.
     * \param pclMailbox_ Pointer to the object to destroy.
     */
    static void DestroyMailbox(Mailbox* pclMailbox_);
#endif
#if KERNEL_USE_MESSAGE
    /*!
     * \brief NewMessage
     * Allocate and construct a new Message object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Message* NewMessage();

    /*!
     * \brief NewMessageQueue
     * Allocate and construct a new MessageQueue object for use within the system.
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static MessageQueue* NewMessageQueue();

    /*!
     * \brief DestroyMessage
     * Destroy and deallocate a previously-allocated Message object from within the system.
     * \param pclMessage_ Pointer to the object to destroy.
     */
    static void DestroyMessage(Message* pclMessage_);

    /*!
     * \brief DestroyMessageQueue
     * Destroy and deallocate a previously-allocated MessageQueue object from within the system.
     * \param pclMessageQ_ Pointer to the object to destroy.
     */
    static void DestroyMessageQueue(MessageQueue* pclMessageQ_);
#endif

    /*!
     * \brief NewThread
     * Allocate and construct a new Thread object for use within the system
     * \return Pointer to newly created object, or 0 on allocation failure.
     */
    static Thread* NewThread();

    /*!
     * \brief DestroyThread
     * Destroy and deallocate a Thread from within the process.
     * \param pclThread_ Pointer to the object to destroy.
     */
    static void DestroyThread(Thread* pclThread_);

    /*!
     * \brief NewUserTypeAllocation
     * Attempt to allocate a user-defined object type from the heap.
     * \param eUserType_ User defined object type, interpreted by the allocator function
     * \return pointer to a newly-created object, or NULL on error.
     */
    static void* NewUserTypeAllocation(uint8_t eUserType_);

    /*!
     * \brief DestroyUserTypeAllocation
     * Free a previously allocated user-defined object.
     * \param pvObj_ Pointer to previously-allocated object, allocated through NewUserTypeAllocation  ()
     * \param eUserType_ User defined object type, interpreted by the allocator function
     */
    static void DestroyUserTypeAllocation(uint8_t eUserType_, void* pvObj_);

    /*!
     * \brief NewRawData
     * Attempt to allocate a blob of raw data from the heap
     * \param sSize_ Size of the data blob (in bytes)
     * \return pointer to newly-allocated blob of data, or NULL on error.
     */
    static void* NewRawData(size_t sSize_);

    /*!
     * \brief DestroyRawData
     * Free a previously allocated blob of data allocated via NewRawData()
     * \param pvData_ pointer to previously-created data object
     */
    static void DestroyRawData(void* pvData_);

private:

    static void* Allocate(AutoAllocType_t eType_, size_t sSize_);
    static void Free(AutoAllocType_t eType_, void* pvObj_);

    static AutoAllocAllocator_t m_pfAllocator;    //!< Function used to allocate objects
    static AutoAllocFree_t      m_pfFree;         //!< Funciton used to free objects
};

#endif

#endif
