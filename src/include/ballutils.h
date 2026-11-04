// Ball Utils
// A collection of 8ball utils

#pragma once

#include "PoolTask.h"
#include "ThreadQueue.h"

typedef ThreadQueue<std::unique_ptr<PoolTask>> WorkQueue;