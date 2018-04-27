import server_pb2
import sensor_pb2
import status_pb2
import emulator_pb2
import motor_pb2
import sys
import time
from google.protobuf.internal.decoder import _DecodeVarint
from google.protobuf.internal.encoder import _VarintBytes
import socket


def read_java_varint_delimited_stream(sock):
    buf = []
    sock.setblocking(0)
    begin = time.time()
    timeout = 1
    size = 0
    position = 0
    while (1):
        if (buf and time.time()-begin >timeout):
            break
        elif time.time()-begin >timeout*2:
            break
        try:
            data = sock.recv(50)
            #print (data)
            #print ("data:" +data.SerializeToString())
            if data:
                if '~~~' in data:
                    index1 = data.index('~~~')
                    # index2 = data.index('}}}')
                    data = data[index1+3:]#index2]
                    rCount = len(data)
                    #print("count" + str(rCount))
                    (size, position) = _DecodeVarint(data, 0)
                    # size += size1
                    # position += position1
                    buf.append(data)
                    while rCount < size+1:
                        data = sock.recv(size+1-rCount)
                        rCount += len(data)
                        buf.append(data)
                    if(rCount > size+1):
                        buf = buf[:size+1]
            else:
                time.sleep(0.1)
        except:
            pass
    if(len(buf) > 1):
        buf.pop(0)
    #print buf
    return b''.join(buf), size, position

def readMessage(sock):
        data, size, position = read_java_varint_delimited_stream(sock)
        #print(data)
        #print('stuck?')
        try: 
            msg = server_pb2.server()
            msg.ParseFromString(data[position:position + size])
            #print('helo')
            return msg
        except ValueError:
            pass


ip = '192.168.1.100'
port = 2000
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((ip,port))
#s.setblocking(0)
while 1:
    #time.sleep(0.1)
    data= readMessage(s)
    #print (data)
    #print 'gg'
    if (data != None and data.sequence_number != 0):
        print ("received msg: " + str(data.sequence_number))
        # data.sequence_number+=1
        # if (data.sequence_number > 127):
        #     data.sequence_number =2
        size = data.ByteSize()
        z = "~~~" + _VarintBytes(size) + data.SerializeToString() + "}}}"
        s.sendall(z)

        #time.sleep(0.2)
        #print (i)
    else:
        print('msg failed')


s.close()
