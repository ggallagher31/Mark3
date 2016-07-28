/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/

//---------------------------------------------------------------------------

#include "kerneltypes.h"
#include "kernel.h"
#include "../ut_platform.h"
#include "message.h"
#include "thread.h"

static Thread clMsgThread;

#define MSG_STACK_SIZE (192)
static K_WORD aucMsgStack[MSG_STACK_SIZE];

static MessageQueue clMsgQ;
static volatile uint8_t u8PassCount = 0;

//===========================================================================
// Local Defines
//===========================================================================

void MsgConsumer(void* unused_)
{
    Message* pclMsg;
    uint8_t i;

    for (i = 0; i < 20; i++) {
        pclMsg = clMsgQ.Receive();
        u8PassCount = 0;

        if (pclMsg) {
            u8PassCount++;
        } else {
            u8PassCount = 0;
            continue;
        }

        switch (i) {
            case 0:
                if (0 == pclMsg->GetCode()) {
                    u8PassCount++;
                }
                if (0 == pclMsg->GetData()) {
                    u8PassCount++;
                }
                break;
            case 1:
                if (1337 == (pclMsg->GetCode())) {
                    u8PassCount++;
                }
                if (7331 == (uint16_t)(pclMsg->GetData())) {
                    u8PassCount++;
                }

            case 2:
                if (0xA0A0 == (pclMsg->GetCode())) {
                    u8PassCount++;
                }
                if (0xC0C0 == (uint16_t)(pclMsg->GetData())) {
                    u8PassCount++;
                }

                break;

            default: break;
        }
        GlobalMessagePool::Push(pclMsg);
    }
}

//===========================================================================
// Define Test Cases Here
//===========================================================================
TEST(ut_message_tx_rx)
{
    // Test - verify that we can use a message queue object to send data
    // from one thread to another, and that the receiver can block on the
    // message queue.  This test also relies on priority scheduling working
    // as expected.

    Message* pclMsg;

    clMsgThread.Init(aucMsgStack, MSG_STACK_SIZE, 7, MsgConsumer, 0);

    clMsgQ.Init();

    clMsgThread.Start();

    // Get a message from the pool
    pclMsg = GlobalMessagePool::Pop();
    EXPECT_FAIL_FALSE(pclMsg);

    // Send the message to the consumer thread
    pclMsg->SetData(0);
    pclMsg->SetCode(0);

    clMsgQ.Send(pclMsg);

    EXPECT_EQUALS(u8PassCount, 3);

    pclMsg = GlobalMessagePool::Pop();
    EXPECT_FAIL_FALSE(pclMsg);

    // Send the message to the consumer thread
    pclMsg->SetCode(1337);
    pclMsg->SetData((void*)7331);

    clMsgQ.Send(pclMsg);

    EXPECT_EQUALS(u8PassCount, 3);

    pclMsg = GlobalMessagePool::Pop();
    EXPECT_FAIL_FALSE(pclMsg);

    // Send the message to the consumer thread
    pclMsg->SetCode(0xA0A0);
    pclMsg->SetData((void*)0xC0C0);

    clMsgQ.Send(pclMsg);

    EXPECT_EQUALS(u8PassCount, 3);

    clMsgThread.Exit();
}
TEST_END

//===========================================================================
TEST(ut_message_exhaust)
{
    // Test - exhaust the global message pool and ensure that we eventually
    // get "NULL" returned when the pool is depleted, and not some other
    // unexpected condition/system failure.
    for (int i = 0; i < GLOBAL_MESSAGE_POOL_SIZE; i++) {
        EXPECT_FAIL_FALSE(GlobalMessagePool::Pop());
    }
    EXPECT_FALSE(GlobalMessagePool::Pop());

    // Test is over - re-init the pool..
    GlobalMessagePool::Init();
}
TEST_END

static volatile bool bTimedOut = false;
//===========================================================================
void MsgTimed(void* unused)
{
    Message* pclRet;
    u8PassCount = 0;
    pclRet = clMsgQ.Receive(10);
    if (0 == pclRet) {
        u8PassCount++;
    } else {
        GlobalMessagePool::Push(pclRet);
    }

    pclRet = clMsgQ.Receive(1000);
    if (0 != pclRet) {
        u8PassCount++;
    } else {
        GlobalMessagePool::Push(pclRet);
    }

    while (1) {
        pclRet = clMsgQ.Receive();
        GlobalMessagePool::Push(pclRet);
    }
}

//===========================================================================
TEST(ut_message_timed_rx)
{
    Message* pclMsg;

    pclMsg = GlobalMessagePool::Pop();
    EXPECT_FAIL_FALSE(pclMsg);

    // Send the message to the consumer thread
    pclMsg->SetData(0);
    pclMsg->SetCode(0);

    // Test - Verify that the timed blocking in the message queues works
    clMsgThread.Init(aucMsgStack, MSG_STACK_SIZE, 7, MsgTimed, 0);
    clMsgThread.Start();

    // Just let the timeout expire
    Thread::Sleep(11);
    EXPECT_EQUALS(u8PassCount, 1);

    // other thread has a timeout set... Don't leave them waiting!
    clMsgQ.Send(pclMsg);

    EXPECT_EQUALS(u8PassCount, 2);

    clMsgQ.Send(pclMsg);

    clMsgThread.Exit();
}
TEST_END

//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
TEST_CASE(ut_message_tx_rx), TEST_CASE(ut_message_exhaust), TEST_CASE(ut_message_timed_rx), TEST_CASE_END
