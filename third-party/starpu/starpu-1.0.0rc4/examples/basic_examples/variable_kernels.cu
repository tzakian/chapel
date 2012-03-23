/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010  Université de Bordeaux 1
 * Copyright (C) 2010  Centre National de la Recherche Scientifique
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#include <starpu.h>
#include <starpu_cuda.h>

static __global__ void cuda_variable(float * tab)
{
	*tab += 1.0f;
	return;
}

extern "C" void cuda_codelet(void *descr[], STARPU_ATTRIBUTE_UNUSED void *_args)
{
	float *val = (float *)STARPU_VARIABLE_GET_PTR(descr[0]);

	cuda_variable<<<1,1, 0, starpu_cuda_get_local_stream()>>>(val);
	cudaStreamSynchronize(starpu_cuda_get_local_stream());
}