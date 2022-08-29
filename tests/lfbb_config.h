/**************************************************************
 * @file lfbb_config_default.h
 * @brief This header defines configuration variables for the
 * library, please read carefully and chose the right ones.
 * @version	1.1.2
 * @date 29. August 2022
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
 * Version:         v1.1.2
 **************************************************************/

/************************** INCLUDE ***************************/
#ifndef LFBB_CONFIG_H
#define LFBB_CONFIG_H

/************************** DEFINE ****************************/

/* This define should be used when the library is used in a multicore
 * environment on a hosted system. Uncomment to enable lock-free operation in
 * such cases. What the define does under the hood is align all the indexes to
 * cacheline size, ensuring no two indexes can be present in a single cacheline.
 * Underlying hardware should have cache coherence. */
#define LFBB_MULTICORE_HOSTED

/* Common cacheline sizes to be used for LFBB_CACHELINE_LENGTH */
#define LFBB_CACHELINE_LENGTH_ARMV8 64U

#define LFBB_CACHELINE_LENGTH_X86_64 64U

#define LFBB_CACHELINE_LENGTH_APPLE_M 128U

/* This define is used to set the system cacheline size.
 * Values for typical systems are provided above. */
#define LFBB_CACHELINE_LENGTH LFBB_CACHELINE_LENGTH_X86_64

#endif /* LFBB_CONFIG_H */
