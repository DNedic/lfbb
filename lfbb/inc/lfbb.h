/**************************************************************
 * @file lfbb.h
 * @brief A bipartite buffer implementation written in standard
 * c11 suitable for both low-end microcontrollers all the way
 * to HPC machines. Lock-free for single consumer single
 * producer scenarios.
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
 * Version:         1.3.5
 **************************************************************/

/************************** INCLUDE ***************************/
#ifndef LFBB_H
#define LFBB_H

#include <stdint.h>
#include <stdlib.h>
#ifndef __cplusplus
#include <stdalign.h>
#include <stdatomic.h>
#include <stdbool.h>
#else
#include <atomic>
#define atomic_size_t std::atomic_size_t
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/************************** DEFINE ****************************/

#ifndef LFBB_MULTICORE_HOSTED
#define LFBB_MULTICORE_HOSTED false
#endif

#ifndef LFBB_CACHELINE_LENGTH
#define LFBB_CACHELINE_LENGTH 64U
#endif

/*************************** TYPES ****************************/

typedef struct {
#if LFBB_MULTICORE_HOSTED
    alignas(LFBB_CACHELINE_LENGTH) atomic_size_t r; /**< Read index */
    alignas(LFBB_CACHELINE_LENGTH) atomic_size_t w; /**< Write index */
    alignas(LFBB_CACHELINE_LENGTH)
        atomic_size_t i; /**< Invalidated space index */
#else
    atomic_size_t r; /**< Read index */
    atomic_size_t w; /**< Write index */
    atomic_size_t i; /**< Invalidated space index */
#endif
    size_t size;        /**< Size of the data array */
    uint8_t *data;      /**< Pointer to the data array */
    bool write_wrapped; /**< Write wrapped flag, used only in the producer */
    bool read_wrapped;  /**< Read wrapped flag, used only in the consumer */
} LFBB_Inst_Type;

/******************** FUNCTION PROTOTYPES *********************/

/**
 * @brief Initializes a bipartite buffer instance
 * @param[in] Instance pointer
 * @param[in] Data array pointer
 * @param[in] Size of data array
 * @retval None
 */
void LFBB_Init(LFBB_Inst_Type *inst, uint8_t *data_array, size_t size);

/**
 * @brief Acquires a linear region in the bipartite buffer for writing
 * @param[in] Instance pointer
 * @param[in] Free linear space in the buffer required
 * @retval Pointer to the beginning of the linear space
 */
uint8_t *LFBB_WriteAcquire(LFBB_Inst_Type *inst, size_t free_required);

/**
 * @brief Releases the bipartite buffer after a write
 * @param[in] Instance pointer
 * @param[in] Bytes written to the linear space
 * @retval None
 */
void LFBB_WriteRelease(LFBB_Inst_Type *inst, size_t written);

/**
 * @brief Acquires a linear region in the bipartite buffer for reading
 * @param[in] Instance pointer
 * @param[out] Available linear data in the buffer
 * @retval Pointer to the beginning of the data
 */
uint8_t *LFBB_ReadAcquire(LFBB_Inst_Type *inst, size_t *available);

/**
 * @brief Releases the bipartite buffer after a read
 * @param[in] Instance pointer
 * @param[in] Bytes read from the linear region
 * @retval None
 */
void LFBB_ReadRelease(LFBB_Inst_Type *inst, size_t read);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LFBB_H */
