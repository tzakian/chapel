/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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
#include "../helper.h"

#ifdef STARPU_USE_CUDA
#include <starpu_cuda.h>
#endif
#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
#endif

static starpu_data_handle_t handle;

/*
 *	Reduction methods
 */

#ifdef STARPU_USE_CUDA
static void redux_cuda_kernel(void *descr[], void *arg)
{
	unsigned *dst = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[0]);
	unsigned *src = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[1]);

	unsigned host_dst, host_src;

	/* This is a dummy technique of course */
	cudaMemcpy(&host_src, src, sizeof(unsigned), cudaMemcpyDeviceToHost);
	cudaMemcpy(&host_dst, dst, sizeof(unsigned), cudaMemcpyDeviceToHost);
	cudaThreadSynchronize();

	host_dst += host_src;

	cudaMemcpy(dst, &host_dst, sizeof(unsigned), cudaMemcpyHostToDevice);
	cudaThreadSynchronize();
}

static void neutral_cuda_kernel(void *descr[], void *arg)
{
	unsigned *dst = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[0]);

	/* This is a dummy technique of course */
	unsigned host_dst = 0;
	cudaMemcpy(dst, &host_dst, sizeof(unsigned), cudaMemcpyHostToDevice);
	cudaThreadSynchronize();
}
#endif

#ifdef STARPU_USE_OPENCL
static void redux_opencl_kernel(void *descr[], void *arg)
{
	unsigned h_dst, h_src;

	cl_mem d_dst = (cl_mem)STARPU_VARIABLE_GET_PTR(descr[0]);
	cl_mem d_src = (cl_mem)STARPU_VARIABLE_GET_PTR(descr[1]);

	cl_command_queue queue;
	starpu_opencl_get_current_queue(&queue);

	/* This is a dummy technique of course */
	clEnqueueReadBuffer(queue, d_dst, CL_TRUE, 0, sizeof(unsigned), (void *)&h_dst, 0, NULL, NULL);
	clEnqueueReadBuffer(queue, d_src, CL_TRUE, 0, sizeof(unsigned), (void *)&h_src, 0, NULL, NULL);

	h_dst += h_src;

	clEnqueueWriteBuffer(queue, d_dst, CL_TRUE, 0, sizeof(unsigned), (void *)&h_dst, 0, NULL, NULL);
	clFinish(queue);
}

static void neutral_opencl_kernel(void *descr[], void *arg)
{
	unsigned h_dst = 0;
	cl_mem d_dst = (cl_mem)STARPU_VARIABLE_GET_PTR(descr[0]);

	cl_command_queue queue;
	starpu_opencl_get_current_queue(&queue);

	clEnqueueWriteBuffer(queue, d_dst, CL_TRUE, 0, sizeof(unsigned), (void *)&h_dst, 0, NULL, NULL);
	clFinish(queue);
}
#endif



static void redux_cpu_kernel(void *descr[], void *arg)
{
	unsigned *dst = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[0]);
	unsigned *src = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[1]);
	*dst = *dst + *src;
}

static void neutral_cpu_kernel(void *descr[], void *arg)
{
	unsigned *dst = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[0]);
	*dst = 0;
}

static struct starpu_codelet redux_cl =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {redux_cuda_kernel, NULL},
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_funcs = {redux_opencl_kernel, NULL},
#endif
	.cpu_funcs = {redux_cpu_kernel, NULL},
	.nbuffers = 2
};

static struct starpu_codelet neutral_cl =
{
	.where = STARPU_CPU|STARPU_CUDA,
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {neutral_cuda_kernel, NULL},
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_funcs = {neutral_opencl_kernel, NULL},
#endif
	.cpu_funcs = {neutral_cpu_kernel, NULL},
	.nbuffers = 1
};

/*
 *	Increment codelet
 */

#ifdef STARPU_USE_OPENCL
/* dummy OpenCL implementation */
static void increment_opencl_kernel(void *descr[], void *cl_arg __attribute__((unused)))
{
	cl_mem d_token = (cl_mem)STARPU_VARIABLE_GET_PTR(descr[0]);
	unsigned h_token;

	cl_command_queue queue;
	starpu_opencl_get_current_queue(&queue);

	clEnqueueReadBuffer(queue, d_token, CL_TRUE, 0, sizeof(unsigned), (void *)&h_token, 0, NULL, NULL);
	h_token++;
	clEnqueueWriteBuffer(queue, d_token, CL_TRUE, 0, sizeof(unsigned), (void *)&h_token, 0, NULL, NULL);
	clFinish(queue);
}
#endif


#ifdef STARPU_USE_CUDA
static void increment_cuda_kernel(void *descr[], void *arg)
{
	unsigned *tokenptr = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[0]);
	unsigned host_token;

	/* This is a dummy technique of course */
	cudaMemcpy(&host_token, tokenptr, sizeof(unsigned), cudaMemcpyDeviceToHost);
	cudaThreadSynchronize();

	host_token++;

	cudaMemcpy(tokenptr, &host_token, sizeof(unsigned), cudaMemcpyHostToDevice);
	cudaThreadSynchronize();
}
#endif

static void increment_cpu_kernel(void *descr[], void *arg)
{
	unsigned *tokenptr = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[0]);
	*tokenptr = *tokenptr + 1;
}

static struct starpu_codelet increment_cl =
{
	.where = STARPU_CPU|STARPU_CUDA|STARPU_OPENCL,
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {increment_cuda_kernel, NULL},
#endif
#ifdef STARPU_USE_OPENCL
	.opencl_funcs = {increment_opencl_kernel, NULL},
#endif
	.cpu_funcs = {increment_cpu_kernel, NULL},
	.nbuffers = 1,
	.modes = {STARPU_REDUX}
};

int main(int argc, char **argv)
{
	int ret;
	unsigned *var;

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	starpu_variable_data_register(&handle, -1, (uintptr_t)NULL, sizeof(unsigned));

	starpu_data_set_reduction_methods(handle, &redux_cl, &neutral_cl);

	unsigned ntasks = 1024;
	unsigned nloops = 16;

	unsigned loop;
	unsigned t;

	for (loop = 0; loop < nloops; loop++)
	{
		for (t = 0; t < ntasks; t++)
		{
			struct starpu_task *task = starpu_task_create();

			task->cl = &increment_cl;
			task->handles[0] = handle;

			int ret = starpu_task_submit(task);
			if (ret == -ENODEV) goto enodev;
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
		}

		ret = starpu_data_acquire(handle, STARPU_R);
		STARPU_CHECK_RETURN_VALUE(ret, "starpu_data_acquire");
		var = (unsigned*) starpu_variable_get_local_ptr(handle);
		starpu_data_release(handle);

		if (*var != ntasks*(loop + 1))
		{
			ret = EXIT_FAILURE;
			FPRINTF(stderr, "[end of loop] Value %u != Expected value %u\n", *var, ntasks * (loop+1));
			goto err;
		}
	}

	ret = starpu_data_acquire(handle, STARPU_R);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_data_acquire");
	var = (unsigned*) starpu_variable_get_local_ptr(handle);
	starpu_data_release(handle);
	starpu_data_unregister(handle);

	if (*var != ntasks*nloops)
	{
		ret = EXIT_FAILURE;
		FPRINTF(stderr, "Value %u != Expected value %u\n", *var, ntasks * (loop+1));
		goto err;
	}

err:
	starpu_shutdown();
	STARPU_RETURN(ret);

enodev:
	starpu_data_unregister(handle);
	fprintf(stderr, "WARNING: No one can execute this task\n");
	/* yes, we do not perform the computation but we did detect that no one
 	 * could perform the kernel, so this is not an error from StarPU */
	starpu_shutdown();
	STARPU_RETURN(STARPU_TEST_SKIPPED);
}