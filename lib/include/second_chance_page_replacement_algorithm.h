//
//  second_chance_page_replacement_algorithm.h
//  project3
//
//  Created by Betsalel "Saul" Williamson on 7/16/16.
//  Copyright (c) 2016 Betsalel "Saul" Williamson. All rights reserved.
//

/*
 * MIT License
 *
 * Copyright (c) 2016 Betsalel "Saul" Williamson
 *
 * contact: saul.williamson@pitt.edu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __project3__second_chance_page_replacement_algorithm__
#define __project3__second_chance_page_replacement_algorithm__

#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

#include "model.h"
#include "view.h"
#include "controller.h"

void second_chance_page_replacement_algorithm();

void *second_chance_refresh_rate_algorithm(void *ptr);

#endif /* defined(__project3__second_chance_page_replacement_algorithm__) */
