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
===========================================================================*/
/*!
    \file portcfg.h

    \brief Mark3 Port Configuration

    This file is used to configure the kernel for your specific target CPU
    in order to provide the optimal set of features for a given use case.

    !! NOTE:  This file must ONLY be included from mark3cfg.h
*/

#ifndef __PORTCFG_H__
#define __PORTCFG_H__

/*!
    Define a macro indicating the CPU architecture for which this port belongs.

    This may also be set by the toolchain, but that's not guaranteed.
*/
#ifndef AVR
# define AVR                        (1)
#endif

/*!
    Define types that map to the CPU Architecture's default data-word and address
    size.
*/
#define K_WORD                      uint8_t     //!< Size of a data word
#define K_ADDR                      uint16_t    //!< Size of an address (pointer size)

/*!
    Set a base datatype used to represent each element of the scheduler's
    priority bitmap.

    PORT_PRIO_MAP_WORD_SIZE should map to the *size* of an element of type
    PORT_PROI_TYPE.
*/
#define PORT_PRIO_TYPE              uint8_t     //!< Type used for bitmap in the PriorityMap class
#define PORT_PRIO_MAP_WORD_SIZE     (1)         //!< size of PORT_PRIO_TYPE in bytes

/*!
    Define the running CPU frequency.  This may be an integer constant, or an alias
    for another variable which holds the CPU's current running frequency.
*/
#if !defined(PORT_SYSTEM_FREQ)
#define PORT_SYSTEM_FREQ            ((uint32_t)16000000)   //!< CPU Frequency in Hz
#endif

/*!
    Set the timer frequency.  If running in tickless mode, this is simply the frequency
    at which the free-running kernel timer increments.

    In tick-based mode, this is the frequency at which the fixed-frequency kernel tick
    interrupt occurs.
*/
#if KERNEL_TIMERS_TICKLESS
#define PORT_TIMER_FREQ             ((uint32_t)(PORT_SYSTEM_FREQ / 256))      //!< Timer resolution in ticks
#else
#define PORT_TIMER_FREQ             ((uint32_t)(PORT_SYSTEM_FREQ / 1000))     //!< Fixed timer interrupt frequency
#endif

/*!
    Define the default/minimum size of a thread stack
*/
#define PORT_KERNEL_DEFAULT_STACK_SIZE      ((K_ADDR)256)

/*!
    Define the size of the kernel-timer thread stack (if one is configured)
*/
#define PORT_KERNEL_TIMERS_THREAD_STACK     ((K_ADDR)256)

/*!
    Define the native type corresponding to the kernel timer hardware's counter register.
*/
#define PORT_TIMER_COUNT_TYPE       uint16_t    //!< Timer counter type

/*!
    Minimum number of timer ticks for any delay or sleep, required to ensure that a timer cannot
    be initialized to a negative value.
*/
#define PORT_MIN_TIMER_TICKS        (0)

#endif // __PORTCFG_H__
