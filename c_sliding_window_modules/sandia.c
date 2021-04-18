#include <Python.h>
#include <string.h>
#include "slidingwindow.h"
#include <numpy/arrayobject.h>

// wrapper function for sliding_sum
// takes in arrar.array or numpy array and window size as args and returns a Python list
static PyObject * Sliding_Sum(PyObject *self, PyObject *args){
	PyObject * bufobj;
	long win_size;
	Py_buffer view;


	double * result;

	/* Get passed Python object */
	if (!PyArg_ParseTuple(args, "Ol", &bufobj, &win_size)){
		return NULL;
	}

	/* Attempt to extract buffer info from it */
	if (PyObject_GetBuffer(bufobj, &view, PyBUF_ANY_CONTIGUOUS | PyBUF_FORMAT) == -1){
		return NULL;
	}

	if (view.ndim != 1){
		PyErr_SetString(PyExc_TypeError, "Expected a 1-dimensional array");
		PyBuffer_Release(&view);
		return NULL;
	}

	if (strcmp(view.format, "d") != 0){
		PyErr_SetString(PyExc_TypeError, "Expected an array of doubles");
        PyBuffer_Release(&view);
        return NULL;
	}
	/* Pass the raw buffer, array size, and the window size to the function */
	long input_arr_size = (long)view.shape[0];
	if (input_arr_size < win_size){
		return NULL;
	}
	long size_output_arr = input_arr_size - win_size + 1;
	result = (double*)malloc(size_output_arr * sizeof(double));
	sliding_sum(result, view.buf, input_arr_size, win_size);
	/* Release the buffer */
	PyBuffer_Release(&view);

	// Return Numpy array
	npy_intp dims[1] = {size_output_arr};
	PyObject * ret = PyArray_SimpleNew(1, dims, NPY_DOUBLE);
	memcpy(PyArray_DATA(ret), result, size_output_arr*sizeof(double));
	free(result);
	return ret;
}

static PyObject * Sliding_T_Values(PyObject *self, PyObject *args){
	PyObject * bufobj;
	long win_size;
	Py_buffer view;

	double * result;

	/* Get passed Python object */
	if (!PyArg_ParseTuple(args, "Ol", &bufobj, &win_size)){
		return NULL;
	}

	/* Attempt to extract buffer info from it */
	if (PyObject_GetBuffer(bufobj, &view, PyBUF_ANY_CONTIGUOUS | PyBUF_FORMAT) == -1){
		return NULL;
	}

	if (view.ndim != 1){
		PyErr_SetString(PyExc_TypeError, "Expected a 1-dimensional array");
		PyBuffer_Release(&view);
		return NULL;
	}

	if (strcmp(view.format, "d") != 0){
		PyErr_SetString(PyExc_TypeError, "Expected an array of doubles");
        PyBuffer_Release(&view);
        return NULL;
	}
	/* Pass the raw buffer, array size, and the window size to the function */
	long input_arr_size = (long)view.shape[0];
	if (input_arr_size < win_size){
		return NULL;
	}
	long size_output_arr = input_arr_size - win_size + 1;
	result = (double*)malloc(size_output_arr * sizeof(double));
	sliding_ttest(result, view.buf, input_arr_size, win_size);
	/* Release the buffer */
	PyBuffer_Release(&view);

	// Return Numpy array
	npy_intp dims[1] = {size_output_arr};
	PyObject * ret = PyArray_SimpleNew(1, dims, NPY_DOUBLE);
	memcpy(PyArray_DATA(ret), result, size_output_arr*sizeof(double));
	free(result);
	return ret;
}

// module's function table
static PyMethodDef SandiaLib_FunctionsTable[] = {
	{
		"sliding_sum",
		Sliding_Sum,
		METH_VARARGS,
		"Calculates sliding sum from either array.array or numpy array"
	},
	{
		"sliding_tvalues",
		Sliding_T_Values,
		METH_VARARGS,
		"Calculates t_values using a sliding window from either an array.array or numpy array"
	},
	{
		NULL, NULL, 0, NULL
	}
};

// modules definition
static struct PyModuleDef SandiaLib_Module = {
	PyModuleDef_HEAD_INIT,
	"sandia",
	"Python wrapper for Sandia functions implemented in C.",
	-1,
	SandiaLib_FunctionsTable
};

PyMODINIT_FUNC PyInit_sandia(void){
	import_array();
	return PyModule_Create(&SandiaLib_Module);
}