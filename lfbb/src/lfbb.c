/**************************************************************
 * @file lfbb.c
 * @brief A bipartite buffer implementation written in standard
 * c11 suitable for both low-end microcontrollers all the way
 * to HPC machines. Lock-free for single consumer single
 * producer scenarios.
 * @version	1.0.0
 * @date 21. August 2022
 * @author Djordje Nedic
 **************************************************************/

/**************************************************************
 * Copyright (c) 2022 Djordje Nedic
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
 * KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of LFBB - Lock Free Bipartite Buffer
 *
 * Author:          Djordje Nedic <nedic.djordje2@gmail.com>
 * Version:         v1.0.0
 **************************************************************/

/************************** INCLUDE ***************************/

#include "lfbb.h"

#include <assert.h>

/*************************** MACRO ****************************/

#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

/******************** FUNCTION PROTOTYPES *********************/

static size_t CalcFree(size_t w, size_t r, size_t size);
static size_t CalcAvailable(size_t w, size_t i, size_t r, size_t size);

/******************** EXPORTED FUNCTIONS **********************/

void LFBB_Init(LFBB_Inst_Type *inst, uint8_t *data_array, const size_t size) {
  assert(inst != NULL);
  assert(data_array != NULL);
  assert(size != 0U);

  inst->data = data_array;
  inst->size = size;
  inst->r = 0U;
  inst->w = 0U;
  inst->i = 0U;
}

uint8_t *LFBB_WriteAcquire(LFBB_Inst_Type *inst, const size_t free_required) {
  assert(inst != NULL);
  assert(inst->data != NULL);

  /* Preload variables with adequate memory ordering */
  const size_t w = atomic_load_explicit(&inst->w, memory_order_relaxed);
  const size_t r = atomic_load_explicit(&inst->r, memory_order_acquire);
  const size_t size = inst->size;

  /* Early return if there is not enough total free space */
  const size_t free = CalcFree(w, r, size);
  if (free_required > free) {
    return NULL;
  }

  /* Try to find enough linear space until the end of the buffer */
  const size_t linear_space = size - r;
  const size_t linear_free = MIN(free, linear_space);
  if (free_required <= linear_free) {
    return &inst->data[w];
  }

  /* If that doesn't work try from the beginning of the buffer */
  if (free_required <= free - linear_free) {
    return &inst->data[0];
  }

  /* Could not find free linear space with required size */
  return NULL;
}

void LFBB_WriteRelease(LFBB_Inst_Type *inst, const size_t written) {
  assert(inst != NULL);
  assert(inst->data != NULL);

  /* Preload variables with adequate memory ordering */
  size_t w = atomic_load_explicit(&inst->w, memory_order_relaxed);
  size_t i = atomic_load_explicit(&inst->i, memory_order_relaxed);

  /* If the write wrapped set the invalidate index and reset write index*/
  if (w + written >= inst->size) {
    i = w;
    w = 0U;
  }

  /* Increment the write index and wrap to 0 if needed */
  w += written;
  if (w == inst->size) {
    w = 0U;
  }

  /* If we wrote over invalidated parts of the buffer move the invalidate index
   */
  if (w > i) {
    i = w;
  }

  /* Store the indexes with adequate memory ordering */
  atomic_store_explicit(&inst->i, i, memory_order_release);
  atomic_store_explicit(&inst->w, w, memory_order_release);
}

uint8_t *LFBB_ReadAcquire(const LFBB_Inst_Type *inst, size_t *available) {
  assert(inst != NULL);
  assert(inst->data != NULL);
  assert(available != NULL);

  /* Preload variables with adequate memory ordering */
  const size_t w = atomic_load_explicit(&inst->w, memory_order_acquire);
  const size_t i = atomic_load_explicit(&inst->i, memory_order_acquire);
  const size_t r = atomic_load_explicit(&inst->r, memory_order_relaxed);
  const size_t size = inst->size;

  /* Read overflows */
  if (r == i) {
    *available = w;
    return &inst->data[0];
  }

  *available = CalcAvailable(w, i, r, size);
  return &inst->data[r];
}

void LFBB_ReadRelease(LFBB_Inst_Type *inst, const size_t read) {
  assert(inst != NULL);
  assert(inst->data != NULL);

  /* Preload variables with adequate memory ordering */
  size_t r = atomic_load_explicit(&inst->r, memory_order_relaxed);

  /* Increment the read index and wrap to 0 if needed */
  r += read;
  if (r == inst->size) {
    r = 0U;
  }

  /* Store the indexes with adequate memory ordering */
  atomic_store_explicit(&inst->r, r, memory_order_release);
}

/********************* PRIVATE FUNCTIONS **********************/

static size_t CalcFree(const size_t w, const size_t r, const size_t size) {
  if (r > w) {
    return (r - w) - 1U;
  } else {
    return (size - (w - r)) - 1U;
  }
}

static size_t CalcAvailable(const size_t w, const size_t i, const size_t r,
                            const size_t size) {
  const size_t avail_end = MIN(w, i);
  if (avail_end >= r) {
    return avail_end - r;
  } else {
    return size - (r - avail_end);
  }
}
