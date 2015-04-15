#ifndef NYTQUERY
#define NYTQUERY
#include <Python.h>

namespace NewYorkTime{

	PyMODINIT_FUNC initnytquery(void);
	static PyObject * query(PyObject *self, PyObject * args);
	static PyObject * init(PyObject *self, PyObject * args);
}



#endif //NYTQUERY