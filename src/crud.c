#include "crud.h"

return_value_t* call_py_function(method_conf_t* method_conf)
{
    configuration_t* config = get_configuration();
    PyObject *pModule, *pDict, *pFunc;
    return_value_t *return_value = NULL;

    Py_Initialize();

    PySys_SetPath(Py_DecodeLocale(config->python_folder, NULL));

    pModule = PyImport_ImportModule(method_conf->file_name);

    if(pModule == NULL){
        PyErr_Print();
    }

    pDict = PyModule_GetDict(pModule);
    pFunc = PyDict_GetItemString(pDict, method_conf->function_name);

    if (PyCallable_Check(pFunc)) 
    {
        return_value = malloc(sizeof(return_value_t));

        PyObject* values = PyObject_CallObject(pFunc, NULL);
        return_value->code = PyLong_AsLong(PyList_GetItem(values, 0));
        return_value->body = PyUnicode_AsUTF8(PyList_GetItem(values, 1));

    } else {
        PyErr_Print();
    }

    // Clean up
    Py_DECREF(pModule);

    // Finish the Python Interpreter
    Py_Finalize();

    return return_value;
}

void handle_py_call(response_t* response, connection_instance_t* connection, method_conf_t* method_conf) {
  return_value_t *return_value = call_py_function(method_conf);

  if (return_value != NULL) {
    if(return_value->code == 200) {
      response->body = malloc(strlen(return_value->body) * sizeof(char) + 1); 
      strcpy(response->body, return_value->body);
      ok(response, connection);
    }

    free(return_value);
  } else if (return_value->code == 200) {
    internal_server_error(response, connection);
  }
}