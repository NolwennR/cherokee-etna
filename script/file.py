
def get():
    return [200, '{"data": "Call get"}']

def post():
    return [200, '{"data": "Call post"}']

def put():
    return [200, '{"data": "Call put with custom response code"}']

def delete():
    return [200, '{"data": "Call delete"}']
    
def head():
    return [200, '{"data": "Call head"}']