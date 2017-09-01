 // -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2004-2011 New Dream Network
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 *
 */


#ifndef CEPH_THREAD_H
#define CEPH_THREAD_H

#include <pthread.h>
#include <sys/types.h>


//线程是Ceph的基础机制，搞清楚该机制有助于梳理Ceph的流程

class Thread {
 private:
  pthread_t thread_id;
  pid_t pid;
  int ioprio_class, ioprio_priority;
  int cpuid;
  const char *thread_name;

  void *entry_wrapper();

 public:
  Thread(const Thread&) = delete;
  Thread& operator=(const Thread&) = delete;

  Thread();
  virtual ~Thread();

 protected:
  virtual void *entry() = 0;	//由继承Thread的子类去实现，是所有线程执行入口。

 private:
  static void *_entry_func(void *arg);

 public:
  const pthread_t &get_thread_id() const;
  pid_t get_pid() const { return pid; }
  bool is_started() const;
  bool am_self() const;
  int kill(int signal);
  int try_create(size_t stacksize);
  void create(const char *name, size_t stacksize = 0);//创建线程
  int join(void **prval = 0);	//创建thread后，处于active状态的线程，退出前释放资源
  int detach();//创建thread后，处于unactive状态的线程，退出前释放资源
  int set_ioprio(int cls, int prio);//设置thread的调度优先级
  int set_affinity(int cpuid);//退出指定cpiid的线程
};

#endif
