#include <Python.h>
//#include <iostream>
//using namespace std;

/*static PyObject * init(PyObject *self){
	cout << "Init";
	return Py_BuildValue("i", 5);
}*/


void initnytquery(){
}

static PyObject * 
pyquery(PyObject *self,PyObject *args){
	const char * query;
	PyArg_ParseTuple(args, "s", &query);
	const char * ans = query;
	return Py_BuildValue("s", ans);
}
