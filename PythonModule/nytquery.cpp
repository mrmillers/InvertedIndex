#include "nytquery.h"
#include "NewYorkTimeLib/NewYorkTime.h"
#include <iostream>
//#include <iostream>
//using namespace std;

/*static PyObject * init(PyObject *self){
	cout << "Init";
	return Py_BuildValue("i", 5);
	}*/
using namespace NewYorkTime;
using namespace std;


static InvertedList* list[20] = { 0 };
static PyObject *nytError;

static PyMethodDef nytMethods[] = {
	{ "query", NewYorkTime::query, METH_VARARGS, "query(int key,int topk,string query)." },
	{ "init", NewYorkTime::init, METH_VARARGS, "init(int key,string path)" },
	{ "close", NewYorkTime::close, METH_VARARGS, "close(int key)" },
	{ NULL, NULL, 0, NULL }        /* Sentinel */
};

PyMODINIT_FUNC NewYorkTime::initnytquery(void)
{
	PyObject *m;
	m = Py_InitModule("nytquery", nytMethods);
	if (m == NULL)
		return;

	nytError = PyErr_NewException("nyt.error", NULL, NULL);
	Py_INCREF(nytError);
	PyModule_AddObject(m, "error", nytError);
}


static PyObject * NewYorkTime::query(PyObject *self, PyObject *args){
	int key = 0, k = 0;
	const char * query;
	string ans;
	PyArg_ParseTuple(args, "iis", &key, &k, &query);
	if (list[key] != NULL){
		//do query
		ans = list[key]->query(k, query);
	}
	return Py_BuildValue("s", ans.c_str());
}

static PyObject * NewYorkTime::init(PyObject *self, PyObject * args){
	int key = 0;
	const char * path;
	PyArg_ParseTuple(args, "is", &key, &path);
	if (list[key] == NULL){
		//init here
		list[key] = new InvertedList(const_cast<char*>(path));
		cerr << "Start loading...\nMay take some time." << endl;
		cerr << "File:" << path << endl;
		list[key]->load();
		cerr << path << " successfully loaded." << endl;
	}
	//cout << key << path;
	return Py_BuildValue("i", 1);
}

static PyObject * NewYorkTime::close(PyObject *self, PyObject * args){
	int key = 0;
	PyArg_ParseTuple(args, "i", &key);
	if (list[key] != NULL){
		delete list[key];
		list[key] = NULL;
	}
	return Py_BuildValue("i", 1);
}