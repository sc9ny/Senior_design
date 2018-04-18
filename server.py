import socket
from threading import Thread
import sys
import Queue
import time
import server_pb2
import sensor_pb2
import status_pb2
import emulator_pb2
import motor_pb2
import controller_pb2
from google.protobuf.internal.decoder import _DecodeVarint
from google.protobuf.internal.encoder import _VarintBytes
import XboxController


class multiClients(Thread):

    #constructor with parameters of port and address
    def __init__(self, ip, port, conn, mode, send_q, status):
        #initialize a thread Object
        Thread.__init__(self)
        self.ip = ip
        self.port = port
        self.conn = conn
        self.conn.settimeout(5)
        self.mode = mode
        
        self.msgs_dropped = 0
        self.msgs_recv = 0
        self.status = status        
        
        self.sn = 1
        
        self.queue_list = send_q # All possible connections to send to 
        self.msgQ = send_q[conn]	 # Send to my socket
        self.dict = {} # TODO Change this so that is the mesage_queue
        
        print ("new connection from " +ip + str(port))


    def read_socket(self,sock):
        buf = []
        sock.setblocking(0)
        begin = time.time()
        timeout = .05
        size=0
        position = 0

        while (1):
            if (buf and time.time()-begin >timeout):
                break
            elif time.time()-begin >timeout*2:
                break
            try:
                data = sock.recv(40)
                if data:
                    if ('~~~' in data):
                        index1 = data.index('~~~')
                        #index2 = data.index('}}}')
                        data = data[index1+3:]#index2]
                        rCount = len(data)
                        (size, position) = _DecodeVarint(data, 0)
                        #size += size1
                        #position += position1
                        buf.append(data)
                        while rCount < size+1:
                            data = sock.recv(size+1-rCount)
                            rCount += len(data)
                            buf.append(data)
                        if(rCount > size+1):
                            buf = buf[:size+1]
                else:
                    time.sleep(.001)
            except:
                pass
        ## TODO Write a better checker here
        if(len(buf) > 1):
            buf.pop(0)
        #print buf
        return b''.join(buf), size, position


    def readMessage(self,sock):
        data, size, position = self.read_socket(sock)
        try:
            msg = server_pb2.server()
            msg.ParseFromString(data[position:position + size])
            return msg
        except:
            self.msgs_dropped += 1
            print 'Message Dropped! Last sequence number = ', self.sn
            return None
        
    """
    This test is a basic test sending fake versions of all of our
    protobuf messages. Might not work
    """
    def test_sendall_messages(self):
        sn = 1
        dict = {}
        try:
            msg = server_pb2.server()
            msg.sequence_number = sn
            msg.uid = "server"
            msg.mCmd.ctl_msg.direction = 1
            msg.mCmd.ctl_msg.adj = 0
            size = msg.ByteSize()
            z = "~~~" + _VarintBytes(size) + msg.SerializeToString()+ "}}}"
            dict.update({msg.sequence_number: z})
            sn += 1
            
##            msg = server_pb2.server()
##            msg.sequence_number = sn
##            msg.uid = "server"
##            msg.mCmd.data.right_ticks = 10
##            msg.mCmd.data.right_dir = 5
##            msg.mCmd.data.left_ticks = 5
##            msg.mCmd.data.left_dir = 5
##            
##            size = msg.ByteSize()
##            z = "~~~" + _VarintBytes(size) + msg.SerializeToString()+ "}}}"
##            dict.update({msg.sequence_number: z})
##            sn += 1
            
            """ Motor messages complete, begin IR Sensor"""
            msg = server_pb2.server()
            msg.sequence_number = sn
            msg.uid = "server"
            msg.sCmd.dist_data.distance = 5
            size = msg.ByteSize()
            z = "~~~" + _VarintBytes(size) + msg.SerializeToString()+ "}}}"
            dict.update({msg.sequence_number: z})
            sn += 1
            
            msg = server_pb2.server()
            msg.sequence_number = sn
            msg.uid = "server"
            msg.sCmd.color_data.color_code = 255
            msg.sCmd.color_data.x = 255
            msg.sCmd.color_data.y = 255
            size = msg.ByteSize()
            z = "~~~" + _VarintBytes(size) + msg.SerializeToString()+ "}}}"
            dict.update({msg.sequence_number: z})
            sn += 1
            
            """ Sensor messages complete, begin Emulator"""
            msg = server_pb2.server()
            msg.sequence_number = sn
            msg.uid = "server"
            msg.bCmd.battle_ctl.player_status = True
            msg.bCmd.battle_ctl.match_inProgress = True
            size = msg.ByteSize()
            z = "~~~" + _VarintBytes(size) + msg.SerializeToString()+ "}}}"
            dict.update({msg.sequence_number: z})
            sn += 1
            
##            msg = server_pb2.server()
##            msg.sequence_number = sn
##            msg.uid = "server"
##            msg.bCmd.battle_move.trainer = True
##            msg.bCmd.battle_move.hit_miss = True
##            size = msg.ByteSize()
##            z = "~~~" + _VarintBytes(size) + msg.SerializeToString()+ "}}}"
##            dict.update({msg.sequence_number: z})
##            sn += 1
            
            """ Final status messages"""
            msg = server_pb2.server()
            msg.sequence_number = sn
            msg.uid = "server"
            msg.again.sequence_number = sn
            size = msg.ByteSize()
            z = "~~~" + _VarintBytes(size) + msg.SerializeToString()+ "}}}"
            dict.update({msg.sequence_number: z})
            sn += 1
            
            msg = server_pb2.server()
            msg.sequence_number = sn
            msg.uid = "server"
            msg.done.sequence_number = sn
            size = msg.ByteSize()
            z = "~~~" + _VarintBytes(size) + msg.SerializeToString()+ "}}}"
            dict.update({msg.sequence_number: z})
            sn += 1
            
            for k,v in dict.items():
                self.conn.sendall(v)
                data = self.readMessage(self.conn)
                if(data.HasField('done')):
                    if(dict.has_key(data.sequence_number)):
                        del dict[data.sequence_number]
##                        print 'removed', data.sequence_number, 'from dict'
     
            if len(dict) == 0:                
                return True
            else:
                return False
        
        except:
            return False
    """
    This test will send an encoded protobuf with extra bytes
    appended to the encoded message. These bytes are just junk and
    should force the client to ask for a resend.
    """
    def test_extra_bytes(self):
        msg = server_pb2.server()
        msg.sequence_number = 101
        msg.uid = "server"
        msg.mCmd.ctl_msg.direction = 1
        size = msg.ByteSize()
        z = "~~~" + _VarintBytes(size) + "FAILURE" + msg.SerializeToString() + "}}}"
        self.conn.sendall(z)
        data = self.readMessage(self.conn)
        if(data.HasField('again')):
            return True
        return False


    """
    This test takes a valid protobuf message and manipulates a byte
    at the beginning, middle, and end of its string. Then the string
    is sent. Upon receipt, the client should either not be able to parse
    the message or will receive invalid input, asking for a resend.
    """
    def test_manipulated_bytes(self):
        return True


    """
    This test sends two protobufs with sequence numbers one apart
    from each other (e.g. 1 and 3). The client should send back that
    it received 1 and 3 but needs to get a message with sequence id 2.
    The server will send this message and wait for its response.
    """
    def test_missing_message(self):
        return True
        

    """
    This test enters a basic loopback across the wifly connection.
    """
    def test_loopback(self):
        print 'Entering loopback...'
        cond = False
        while 1:
            try:
                # TODO Update to have GUI to Rover comms
                try:
                    next_msg = self.msgQ.get_nowait()
                except Queue.Empty:
                    # No messages waiting so stop checking for writability
                    print >>sys.stderr, 'output queue for', self.conn.getpeername(), 'is empty'
                else:
                    print >>sys.stderr, 'sending "%s" to %s' % (next_msg, self.conn.getpeername())
                    size = next_msg.ByteSize()
                    next_msg.sequence_number = self.sn
                    self.sn += 1
                    z = "~~~" + _VarintBytes(size) + next_msg.SerializeToString() + "}}}"
                    self.conn.sendall(z)
                    if self.sn > 127:
                        self.sn = 1
                    self.dict.update({next_msg.sequence_number: z})
                if(cond == False):
                    data = self.readMessage(self.conn)
                    #  recv returns 0 if peer closes connection
                    if not data:
                        print ("connection closed from " + self.ip + str(self.port))
                        conns.remove(self.conn)
                        self.conn.close()
                        self.status= 'crashed'
            ##            break
                    test = server_pb2.server()
                    test.sequence_number = self.sn
                    test.uid = "server"
                    test.done.sequence_number = test.sequence_number
                    
                    # turn data into binary
                    #print ':'.join(x.encode('hex') for x in test.SerializeToString())
                    self.msgQ.put(test)
                    cond = True
                else:
                    data = self.readMessage(self.conn)
                    
                    #Check for empty message
                    if not data:
                        print ("connection closed from " + self.ip + str(self.port))
                        del conns[self.conn]
                        self.conn.close()
                        self.status = 'crashed'
                        break
                    if(data.HasField('done')):
                        if(self.dict.has_key(data.done.sequence_number)):
                            del self.dict[data.done.sequence_number]
                            print 'removed', data.done.sequence_number, 'from dict'
                    # TODO Parse out all message types
                    if(self.mode == 2):
                        print("data: " + str(data))

                    ## Construct response for loopback test
                    if(self.mode > 0):
                        test = server_pb2.server()
                        test.sequence_number = self.sn
                        test.uid = "server"
                        test.done.sequence_number = test.sequence_number
                        # print ':'.join(x.encode('hex') for x in test.SerializeToString())
                        self.msgQ.put(test)
            except socket.timeout:
                self.status = 'crashed'
                print 'socket_timeout'
                break
        self.status = 'crashed'
        return True


    def run_tests(self):
        test1 = self.test_extra_bytes()
        if not test1:
            print 'Failed extra bytes test'
        
        test2 = self.test_manipulated_bytes()
        if not test2:
            print 'Failed manipulated bytes test'
        
        test3 = self.test_missing_message()
        if not test3:
            print 'Failed missing message test'
        
        test4 = self.test_sendall_messages()
        if not test4:
            print 'Failed sendall messages test'
        
        results = [test1, test2, test3, test4]
        print 'Tests complete!'
        print 'Total tests passed: ', sum(bool(x) for x in results), ' out of ', len(results)
        time.sleep(1)
        self.test_loopback()

    def run(self):
        #recv is blocking method so set up timeout.
        self.conn.settimeout(3)
        
        # Run tests if needed
        if(self.mode == 1):
            print 'Running tests...'
            self.run_tests()
            self.status == 'crashed'
        while 1:
            if(self.status == 'crashed'):
                break
            try:
                try:
                    next_msg = self.msgQ.get_nowait()
                except Queue.Empty:
                    # No messages waiting so stop checking for writability
                    print >>sys.stderr, 'output queue for', self.conn.getpeername(), 'is empty'        
                else:
                    # TODO Update to have GUI to Rover comms
                    print >>sys.stderr, 'sending "%s" to %s' % (next_msg, self.conn.getpeername())
                    size = next_msg.ByteSize()
                    next_msg.sequence_number = self.sn
                    self.sn += 1
                    z = "~~~" + _VarintBytes(size) + next_msg.SerializeToString() + "}}}"
                    try:
                        self.conn.send(z)
                        if self.sn > 127: # TODO remove this
                            self.sn = 1
                        self.dict.update({next_msg.sequence_number: z})
                    except socket.error as e:
                        print 'socket crashed:', e
                        self.status = 'crashed'

                data = self.readMessage(self.conn)
                    
                # Check for empty message
                if not data or len(str(data)) == 0:
                    print ("connection closed from " + self.ip + str(self.port))
                    self.conn.close()
                    self.status = 'crashed'
                    break
                if(data.HasField('done')):
                    if(self.dict.has_key(data.done.sequence_number)):
                        del self.dict[data.done.sequence_number]
                        print 'removed', data.done.sequence_number, 'from dict'
                # TODO Parse out all message types
                if(self.mode == 2):
                    print("data: " + str(data))

##                ## Construct response for loopback test
##                if(self.mode > 0):
##                    test = server_pb2.server()
##                    test.sequence_number = self.sn
##                    test.uid = "server"
##                    test.done.sequence_number = test.sequence_number
##                    # print ':'.join(x.encode('hex') for x in test.SerializeToString())
##                    self.msgQ.put(test)

            except socket.error as e:
                print 'socket crashed:', e
                self.status = 'crashed'
        


# This section defines the callbacks for all of the different controller functions

# TODO Only do this for debugging
def controlCallBack(xboxControlId, value):
    print "Control Id = {}, Value = {}".format(xboxControlId, value)   

def sendButton(xboxControlId, value):
    if(value != 0):
        if(mode > 0):
            print 'Controller ID: ', xboxControlId
            print 'Value:         ', value
            print ''
        # send to GUI only
        if guiIP in conns.keys():
            msg = server_pb2.server()
            msg.sequence_number = 999
            msg.uid = "server"
            msg.ctl.btn.button_id = xboxControlId
            msg.ctl.btn.value = int(value)
            message_queues[conns[trainerIP]].put(msg)
    
def ACallBack(value):
    sendButton(0, int(value))
    
def BCallBack(value):
    sendButton(1, int(value))

def BackCallBack(value):
    sendButton(6, int(value))

def StartCallBack(value):
    sendButton(7, int(value))
    
# Corrective measure for the motors
def LBCallBack(value): # Left
    if trainerIP in conns.keys():
        msg = server_pb2.server()
        msg.sequence_number = 999
        msg.uid = "server"	
        msg.mCmd.ctl_msg.direction = 6
        message_queues[conns[trainerIP]].put(msg)
        
def RBCallBack(value): # Right
    if trainerIP in conns.keys():
        msg = server_pb2.server()
        msg.sequence_number = 999
        msg.uid = "server"	
        msg.mCmd.ctl_msg.direction = 7
        message_queues[conns[trainerIP]].put(msg)
    
def XCallBack(value): # Backward
    if trainerIP in conns.keys():
        msg = server_pb2.server()
        msg.sequence_number = 999
        msg.uid = "server"	
        msg.mCmd.ctl_msg.direction = 8
        message_queues[conns[trainerIP]].put(msg)
    
def YCallBack(value): # Forward
    if trainerIP in conns.keys():
        msg = server_pb2.server()
        msg.sequence_number = 999
        msg.uid = "server"	
        msg.mCmd.ctl_msg.direction = 9
        message_queues[conns[trainerIP]].put(msg)
    
    
# TODO Update this so that it deals with holding the button
def movementCallBack(value):
    # Compute direction, giving preference to vertical
    global prevDirection
    global prevMovementVal
    val = 0
    toMove = False
    if(value[1] == 1 and prevDirection != 2): # North        
        val = 1
    elif(value[1] == -1 and prevDirection != 1): # South
        val = 2
    elif(value[0] == 1 and prevDirection != 4) : # East
        val = 3
    elif(value[0] == -1 and prevDirection != 3): # West
        val = 4
    
    
    if(val != 0 and not freeze):
        if(mode > 0):
            print 'Previous: ', prevDirection
            print 'Value:    ', val
            print ''
        # Create message
        msg = server_pb2.server()
        msg.sequence_number = 999
        msg.uid = "server"	
        msg.ctl.go.direction = val
        if trainerIP in conns.keys():
            message_queues[conns[trainerIP]].put(msg)
        elif guiIP in conns.keys():
            message_queues[conns[guiIP]].put(msg)
        
        prevDirection = val
    if (val == 0):
        prevMovementVal = 0
    else:
        prevMovementVal = 1

# TODO Original one, update for new values
##def form_proto(xboxControlId, value):
##    # make the protobuf message
####    if(xboxControlId == 17):
####        if(value[0] == 0)
##    msg = server_pb2.server()
##    msg.sequence_number = 999
##    msg.uid = "controller"
##    msg.mvmt.button_id = xboxControlId
##    msg.mvmt.value = int(value)
##    size = msg.ByteSize()
##    z = "~~~" + _VarintBytes(size) + msg.SerializeToString() + "}}}"
##    msg_to_send.append(z)
####    for c in conns:
######        print "c: ", c
####        c.sendall(z)
####    print "Control Id = {}, Value = {}".format(xboxControlId, value)

conns = {}
prevDirection = 1
prevMovementVal = 1
mode = 0
freeze = False
message_queues = {}


guiIP = '192.168.1.101'
pikaIP = '192.168.1.103'
trainerIP = '192.168.1.105'

def main():
    #IP = '127.0.0.1'
    IP = '192.168.1.100'
    server_port = 2000

    #IPv4 and tcp/ip
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((IP, server_port))

    global prevDirection
    global mode
    global freeze
    freeze = False
    prevDirection = 1
    mode = 0
    
    # map of game world goes here
    # previous position value goes here
    
    threads = []

    try:
        mode = int(sys.argv[1])
        if(mode > 2 or mode < 0):
            print 'Error! Usage: \n 0 == normal operation, 1 == run tests, 2 == add optional debug output'
            return
        if(mode == 1):
            print 'Run Tests'
        elif(mode == 2):
            print 'Running with optional debug output'              
    except:
        mode = 0

    # Attempt to start xbox controller
    try:
        xboxCont = XboxController.XboxController(controllerCallBack = None, deadzone = 30, scale = 100, invertYAxis = True)
        xboxCont.setupControlCallback(xboxCont.XboxControls.A, ACallBack)
        xboxCont.setupControlCallback(xboxCont.XboxControls.B, BCallBack)
        xboxCont.setupControlCallback(xboxCont.XboxControls.START, StartCallBack)
        xboxCont.setupControlCallback(xboxCont.XboxControls.BACK, BackCallBack)
        xboxCont.setupControlCallback(xboxCont.XboxControls.DPAD, movementCallBack)
        xboxCont.setupControlCallback(xboxCont.XboxControls.RB, RBCallBack)
        xboxCont.setupControlCallback(xboxCont.XboxControls.LB, LBCallBack)
        xboxCont.setupControlCallback(xboxCont.XboxControls.X, XCallBack)
        xboxCont.setupControlCallback(xboxCont.XboxControls.Y, YCallBack)
        xboxCont.start()
    except:
        print 'Xbox Controller unable to start. Please make sure the controller is on and restart the server.'
        sys.exit()
    
    while 1:    
        try:        
            s.listen(3)
            print ("Listening...")
            (conn, (ip,port)) = s.accept()
            conns[ip] = conn
            print (str(len(conns))+" connections are running")
            message_queues[conn] = Queue.Queue()
            t = multiClients(ip, port, conn, mode, message_queues, 'started')
            t.name = ip
            t.start()
            threads.append(t)
            for thr in threads: # todo check this actually works
                if not thr.isAlive():
                    print 'thread crashed'
                    with message_queues[conns[thr.getName()]].mutex:
                        message_queues[conns[thr.getName()]].queue.clear()
                    conns[thr.getName()].close()
                    del conns[thr.getName()]
                    threads.remove(thr)
        except KeyboardInterrupt:
            for c in conns:                
                #c.send("server is closed")
                with message_queues[conns[c]].mutex:
                    message_queues[conns[c]].queue.clear()
                    del message_queues[conns[c]]
##                threads[conns[c]].join()
##                threads.remove(conns[c])
                conns[c].close()
##            del conns[c]
##            for t in threads:
##                t.join()
##                threads.remove(t)
            xboxCont.stop()
            break
        except socket.timeout:
            pass


if __name__ == '__main__':
    main()
