#include "crud.h"


void test_call_py()
{
    PyObject *pName, *pModule, *pDict, *pFunc;

    // Initialize the Python Interpreter
    Py_Initialize();

    // Build the name object
    pName = PyUnicode_FromString("py_function");

    // Load the module object
    pModule = PyImport_Import(pName);

    // pDict is a borrowed reference 
    pDict = PyModule_GetDict(pModule);

    // pFunc is also a borrowed reference 
    pFunc = PyDict_GetItemString(pDict, "multiply");

    if (PyCallable_Check(pFunc)) 
    {
        PyObject_CallObject(pFunc, NULL);
    } else 
    {
        PyErr_Print();
    }

    // Clean up
    Py_DECREF(pModule);
    Py_DECREF(pName);

    // Finish the Python Interpreter
    Py_Finalize();

}