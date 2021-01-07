import socket
import time

def try_connect(s,addr,port):
    try:
       r = s.connect_ex((addr,port))
       if r == 0:
           return
    except socket.error as err:
        print(err)
    finally:
        time.sleep(1)
        try_connect(s,addr,port)


s = socket.socket(family=socket.AF_INET, type=socket.SOCK_STREAM, proto=0, fileno=None)

s.connect(("10.10.22.140",9527))


while True:
    data = s.recv(1024)
    print('接收数据', repr(data))