///    Copyright (C) 2018 DG.C, DGCHOW, deguangchow
///        deguangchow@qq.com
///
///    \brief    chapter6: Designing lock-based concurrent data structures
///
///    \author   deguangchow
///    \version  1.0
///    \2018/12/11
#include "stdafx.h"
#include "lock_based_concurrent_data_structures.h"

namespace lock_based_conc_data {

//6.2 Lock-based concurrent data structures
//6.2.1 A thread-safe stack using locks
//Listing 6.1 A class definition for a thread-safe stack
thread_safe_stack<int>          g_threadSafeStack;
void lock_thread_safe_stack_write() {
    TICK();
    g_threadSafeStack.push(1);
    g_threadSafeStack.push(2);
    g_threadSafeStack.push(3);
}
void lock_thread_safe_stack_read() {
    TICK();
    shared_ptr<int> ptr1 = g_threadSafeStack.pop();
    shared_ptr<int> ptr2 = g_threadSafeStack.pop();
    shared_ptr<int> ptr3 = g_threadSafeStack.pop();
    INFO("ptr1=%d", *ptr1);
    INFO("ptr2=%d", *ptr2);
    INFO("ptr3=%d", *ptr3);
}
void test_lock_based_thread_safe_stack() {
    TICK();
    try {
        thread t1(lock_thread_safe_stack_write);
        thread t2(lock_thread_safe_stack_write);
        thread t3(lock_thread_safe_stack_write);

        thread t4(lock_thread_safe_stack_read);
        thread t5(lock_thread_safe_stack_read);
        thread t6(lock_thread_safe_stack_read);

        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
    } catch (empty_stack const &e) {
        INFO("empty_stack err:%s", e.what());
    } catch (...) {
        INFO("exception");
    }
}

//6.2.2 A thread-safe queue using locks and condition variables
//Listing 6.2 The full class definition for a thread-safe queue using condition variables
threadsafe_queue<unsigned>          g_threadSafeQueue;
void threadsafe_queue_write() {
    TICK();
    g_threadSafeQueue.push(1);
    g_threadSafeQueue.push(2);
    g_threadSafeQueue.push(3);
#if 0
    g_threadSafeQueue.push(4);
#endif
}
void threadsafe_queue_read() {
    TICK();
    shared_ptr<unsigned> ptr1 = g_threadSafeQueue.wait_and_pop();
    if (nullptr == ptr1) {
        WARN("wait_and_pop()=nullptr");
    } else {
        INFO("wait_and_pop()=%d", *ptr1);
    }

    shared_ptr<unsigned> ptr2 = g_threadSafeQueue.try_pop();
    if (nullptr == ptr2) {
        WARN("try_pop()=nullptr");
    } else {
        INFO("try_pop()=%d", *ptr2);
    }

    unsigned u3 = -1;
    g_threadSafeQueue.wait_and_pop(u3);
    INFO("wait_and_pop(%d)", u3);

    unsigned u4 = -1;
    g_threadSafeQueue.try_pop(u4);
    INFO("try_pop(%d)", u4);
}
void test_threadsafe_queue() {
    TICK();
    thread t1(threadsafe_queue_write);
    thread t2(threadsafe_queue_write);
    thread t3(threadsafe_queue_write);

    thread t4(threadsafe_queue_read);
    thread t5(threadsafe_queue_read);
    thread t6(threadsafe_queue_read);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
}

//Listing 6.3 A thread-safe queue holding shared_ptr<> instances
threadsafe_queue_shared_ptr<unsigned> g_threadSafeQueueSharedPtr;
void threadsafe_queue_shared_ptr_write() {
    TICK();
    g_threadSafeQueueSharedPtr.push(1);
    g_threadSafeQueueSharedPtr.push(2);
    g_threadSafeQueueSharedPtr.push(3);
#if 0
    g_threadSafeQueueSharedPtr.push(4);
#endif
}
void threadsafe_queue_shared_ptr_read() {
    TICK();
    unsigned u1 = -1;
    g_threadSafeQueueSharedPtr.wait_and_pop(u1);
    INFO("wait_and_pop(%d)", u1);

    unsigned u2 = -1;
    g_threadSafeQueueSharedPtr.try_pop(u2);
    INFO("try_pop(%d)", u2);

    shared_ptr<unsigned> ptr3 = g_threadSafeQueueSharedPtr.wait_and_pop();
    if (nullptr == ptr3) {
        WARN("wait_and_pop()=nullptr");
    } else {
        INFO("wait_and_pop()=%d", *ptr3);
    }

    shared_ptr<unsigned> ptr4 = g_threadSafeQueueSharedPtr.try_pop();
    if (nullptr == ptr4) {
        WARN("try_pop()=nullptr");
    } else {
        INFO("try_pop()=%d", *ptr4);
    }
}

void test_threadsafe_queue_shared_ptr() {
    TICK();
    thread t1(threadsafe_queue_shared_ptr_write);
    thread t2(threadsafe_queue_shared_ptr_write);
    thread t3(threadsafe_queue_shared_ptr_write);

    thread t4(threadsafe_queue_shared_ptr_read);
    thread t5(threadsafe_queue_shared_ptr_read);
    thread t6(threadsafe_queue_shared_ptr_read);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
}

//Listing 6.4 A simple single-threaded queue implementation
void test_queue() {
    TICK();
    queue<unsigned> q;

    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);

    shared_ptr<unsigned> ptr1 = q.try_pop();
    shared_ptr<unsigned> ptr2 = q.try_pop();
    shared_ptr<unsigned> ptr3 = q.try_pop();
    shared_ptr<unsigned> ptr4 = q.try_pop();
    shared_ptr<unsigned> ptr5 = q.try_pop();

    INFO("try_pop()=%d", *ptr1);
    INFO("try_pop()=%d", *ptr2);
    INFO("try_pop()=%d", *ptr3);
    INFO("try_pop()=%d", *ptr4);
    INFO("try_pop()=%d", *ptr5);//0 will be printed as the function 'try_pop' would return 0 if empty
}

//Listing 6.5 A simple queue with a dummy node
dummy_queue<unsigned>           g_dummyQueue;
void dummy_queue_write() {
    TICK();
    g_dummyQueue.push(1);
    g_dummyQueue.push(2);
    g_dummyQueue.push(3);
    g_dummyQueue.push(4);
}
void dummy_queue_read() {
    TICK();
    shared_ptr<unsigned> ptr1 = g_dummyQueue.try_pop();
    shared_ptr<unsigned> ptr2 = g_dummyQueue.try_pop();
    shared_ptr<unsigned> ptr3 = g_dummyQueue.try_pop();
    shared_ptr<unsigned> ptr4 = g_dummyQueue.try_pop();
    INFO("try_pop()=%d", *ptr1);
    INFO("try_pop()=%d", *ptr2);
    INFO("try_pop()=%d", *ptr3);
    INFO("try_pop()=%d", *ptr4);
}
void test_dummy_queue() {
    TICK();
    thread t1(dummy_queue_write);
    thread t2(dummy_queue_write);
    thread t3(dummy_queue_write);

    thread t4(dummy_queue_read);
    thread t5(dummy_queue_read);
    thread t6(dummy_queue_read);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
}

//Listing 6.6 A thread-safe queue with fine-grained locking
threadsafe_queue_fine_grained<unsigned> g_threadSafeQueueFineGrainedWrite;
void threadsafe_queue_fine_grained_write() {
    TICK();
    g_threadSafeQueueFineGrainedWrite.push(1);
    g_threadSafeQueueFineGrainedWrite.push(2);
    g_threadSafeQueueFineGrainedWrite.push(3);
    g_threadSafeQueueFineGrainedWrite.push(4);
}
void threadsafe_queue_fine_grained_read() {
    TICK();
    shared_ptr<unsigned> ptr1 = g_threadSafeQueueFineGrainedWrite.try_pop();
    shared_ptr<unsigned> ptr2 = g_threadSafeQueueFineGrainedWrite.try_pop();
    shared_ptr<unsigned> ptr3 = g_threadSafeQueueFineGrainedWrite.try_pop();
    shared_ptr<unsigned> ptr4 = g_threadSafeQueueFineGrainedWrite.try_pop();
    INFO("try_pop()=%d", ptr1 ? *ptr1 : -1);//-1 won`t be printed as the function 'try_pop' would return 0 if empty
    INFO("try_pop()=%d", ptr2 ? *ptr2 : -1);
    INFO("try_pop()=%d", ptr3 ? *ptr3 : -1);
    INFO("try_pop()=%d", ptr4 ? *ptr4 : -1);
}
void test_threadsafe_queue_fine_grained() {
    TICK();
    thread t1(threadsafe_queue_fine_grained_write);
    thread t2(threadsafe_queue_fine_grained_write);
    thread t3(threadsafe_queue_fine_grained_write);

    thread t4(threadsafe_queue_fine_grained_read);
    thread t5(threadsafe_queue_fine_grained_read);
    thread t6(threadsafe_queue_fine_grained_read);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
}

//Listing 6.7 A thread-safe queue with locking and waiting: internals and interface
threadsafe_waiting_queue<unsigned> g_threadSafeWaitingQueue;
void threadsafe_waiting_queue_write() {
    TICK();
    g_threadSafeWaitingQueue.push(1);
    g_threadSafeWaitingQueue.push(2);
    g_threadSafeWaitingQueue.push(3);
    g_threadSafeWaitingQueue.push(4);
}
void threadsafe_waiting_queue_read() {
    TICK();
#if 0//if empty print 0
    shared_ptr<unsigned> ptr1 = g_threadSafeWaitingQueue.try_pop();
    INFO("try_pop()=%d", *ptr1);

    shared_ptr<unsigned> ptr2 = g_threadSafeWaitingQueue.try_pop();
    INFO("try_pop()=%d", *ptr2);

    unsigned u3 = 0;
    g_threadSafeWaitingQueue.try_pop(u3);
    INFO("try_pop(%d)", u3);

    unsigned u4 = 0;
    g_threadSafeWaitingQueue.try_pop(u4);
    INFO("try_pop(%d)", u4);
#else//if empty wait cv
    shared_ptr<unsigned> ptr1 = g_threadSafeWaitingQueue.wait_and_pop();
    INFO("wait_and_pop()=%d", *ptr1);

    shared_ptr<unsigned> ptr2 = g_threadSafeWaitingQueue.wait_and_pop();
    INFO("wait_and_pop()=%d", *ptr2);

    unsigned u3 = 0;
    g_threadSafeWaitingQueue.wait_and_pop(u3);
    INFO("try_pop(%d)", u3);

    unsigned u4 = 0;
    g_threadSafeWaitingQueue.wait_and_pop(u4);
    INFO("try_pop(%d)", u4);
#endif
}
void threadsafe_waiting_queue_loop() {
    TICK();
    g_threadSafeWaitingQueue.loop();
}
void test_threadsafe_waiting_queue() {
    TICK();
    thread t(threadsafe_waiting_queue_loop);

    thread t1(threadsafe_waiting_queue_write);
    thread t2(threadsafe_waiting_queue_write);
    thread t3(threadsafe_waiting_queue_write);
    thread t4(threadsafe_waiting_queue_write);

    thread t5(threadsafe_waiting_queue_read);
    thread t6(threadsafe_waiting_queue_read);
    thread t7(threadsafe_waiting_queue_read);
    thread t8(threadsafe_waiting_queue_read);

    t.join();

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    t5.join();
    t6.join();
    t7.join();
    t8.join();
}

//6.3 Designing more complex lock-based data structures
//6.3.1 Writing a thread-safe lookup table using locks
//Listing 6.11 A thread-safe lookup table
typedef threadsafe_lookup_table<int, int> MAP_KEY_VALUE;
MAP_KEY_VALUE                   g_mapKeyValue;
const unsigned                  &KEY_NUMS = 5;
void threadsafe_lookup_table_add() {
    TICK();
    for (unsigned i = 1; i < KEY_NUMS; ++i) {
        DEBUG("%s(%d,%d)", g_mapKeyValue.insert(i, i * HUNDRED) ? "add" : "update", i, i * HUNDRED);
    }
}
void threadsafe_lookup_table_remove_all() {
    TICK();
    for (unsigned i = 1; i < KEY_NUMS; ++i) {
        WARN("remove(%d)=%s", i, g_mapKeyValue.remove(i) ? "true" : "false");
    }
}
void threadsafe_lookup_table_read() {
    TICK();
    for (unsigned i = 1; i < KEY_NUMS; ++i) {
        INFO("read(%d)=%d", i, g_mapKeyValue.get(i));
    }
}
void test_threadsafe_lookup_table() {
    TICK();
    thread t1(threadsafe_lookup_table_add);
    thread t2(threadsafe_lookup_table_add);
    thread t3(threadsafe_lookup_table_add);
    thread t4(threadsafe_lookup_table_add);

    thread t5(threadsafe_lookup_table_read);
    thread t6(threadsafe_lookup_table_read);
    thread t7(threadsafe_lookup_table_read);
    thread t8(threadsafe_lookup_table_read);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();

    auto const& mapRes = g_mapKeyValue.get_map();
    for (auto& pos = mapRes.begin(); pos != mapRes.end(); ++pos) {
        PRINT("[%d, %d], ", pos->first, pos->second);
    }

    thread t9(threadsafe_lookup_table_remove_all);
    t9.join();
}

//6.3.2 Writing a thread-safe list using locks
//Listing 6.13 A thread-safe list with iteration support
threadsafe_list<unsigned const>     g_threadSafeList;
void threadsafe_list_write(unsigned const& val) {
    TICK();
    for (unsigned i = 1; i < 5; ++i) {
        DEBUG("push front(%d).", i*val);
        g_threadSafeList.push_front(i*val);
    }
}

void threadsafe_list_remove() {
    TICK();
    unsigned count = 0;
    g_threadSafeList.remove_if([&count](unsigned const& item) {
        bool ret = item % 2 == 0;
        if (ret) {
            INFO("remove(%d).", item);
            ++count;
        }
        return ret;
     });
    INFO("remove total is %d.", count);
}
void threadsafe_list_read() {
    TICK();
    unsigned count = 0;
    g_threadSafeList.for_each([&count](unsigned const& item) {INFO("read(%d).", item); ++count; });
    INFO("read total is %d.", count);
}
void test_threadsafe_list() {
    TICK();
    thread t1(threadsafe_list_write, ONE);
    thread t2(threadsafe_list_write, TEN);
    thread t3(threadsafe_list_write, HUNDRED);
    thread t4(threadsafe_list_write, THOUSAND);
#if 1
    typedef function<bool(unsigned const&)> Func;
    thread remove(&threadsafe_list<unsigned const>::remove_if<Func>, &g_threadSafeList, [](unsigned const& item) {
        bool ret = item % 2 == 0;
        if (ret) {
            INFO("remove(%d).", item);
        }
        return ret;
    });
#else
    thread remove(threadsafe_list_remove);
#endif
#if 0
    typedef function<void(unsigned const&)> Func;
    thread read(&threadsafe_list<unsigned const>::for_each<Func>,
        &tsl, [](unsigned const& item) {INFO("%d", item); });
#else
    thread read(threadsafe_list_read);
#endif
    typedef function<bool(unsigned const&)> Func;
    thread find(&threadsafe_list<unsigned const>::find_first_if<Func>, &g_threadSafeList, [](unsigned const& item) {
        if (bool ret = item > 10000) {
            INFO("find(%d).", item);
            return ret;
        }
        WARN("not find.");
        return false;
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    remove.join();
    read.join();
    find.join();
}

}//namespace lock_based_conc_data

