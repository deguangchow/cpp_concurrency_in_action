///    Copyright (C) 2018 DG.C, DGCHOW, deguangchow
///        deguangchow@qq.com
///
///    \brief    stdafx.h
///
///    \author   deguangchow
///    \version  1.0
///    \2018/11/23
#pragma once
#ifndef STDAFX_H
#define STDAFX_H

//for the usage of 'rand_s'
#define _CRT_RAND_S

//+ system`s head file.
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <mutex>
#include <deque>
#include <stack>
#include <map>
#include <queue>
#include <memory>
#include <condition_variable>
#include <future>
#include <utility>
#include <set>


//using std::
using std::thread;
using std::mutex;
using std::condition_variable;
using std::condition_variable_any;
using std::cv_status;

using std::lock;
using std::lock_guard;
using std::unique_lock;
using std::defer_lock;
using std::adopt_lock;

using std::future;
using std::shared_future;
using std::future_status;
using std::promise;
using std::packaged_task;
using std::this_thread::get_id;
using std::this_thread::sleep_for;
using std::this_thread::sleep_until;
using std::this_thread::yield;

using std::async;
using std::launch;

using std::atomic;
using std::atomic_bool;
using std::atomic_flag;
using std::memory_order;

using std::once_flag;
using std::call_once;

using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::steady_clock;

using std::function;

using std::move;
using std::forward;
using std::result_of;
using std::ref;
using std::bind;
using std::placeholders::_1;
using std::placeholders::_2;

using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;

using std::string;
using std::map;
using std::set;
using std::queue;
using std::vector;
using std::list;
using std::array;
using std::deque;
using std::list;
using std::stack;

using std::pair;
using std::make_pair;

using std::hash;

using std::exception;
using std::current_exception;
using std::out_of_range;
using std::logic_error;

using std::cin;
using std::cout;
using std::endl;

using std::max;
using std::min;


//+ user`s head file.
#include "targetver.h"
#include "common_def.h"
#include "console_log.h"
#include "common_fun.h"



#endif  //STDAFX_H

