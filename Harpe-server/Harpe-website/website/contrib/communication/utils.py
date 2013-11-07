# -*- coding: utf-8 -*-
import socket as csocket
from struct import pack,unpack

def enum(**enums):
    return type('Enum', (), enums)

class Socket:
    Dommaine = enum(IP=csocket.AF_INET,LOCAL=csocket.AF_UNIX)
    Type = enum(TCP=csocket.SOCK_STREAM, UDP=csocket.SOCK_DGRAM)
    Down = enum(SEND=0,RECIVE=1,BOTH=2)

    NTW_WELCOM_MSG = "hello!\0"
    NTW_ERROR_NO = 0

    def __init__ (self,dommaine,type,protocole=0):
        self.sock = csocket.socket(dommaine,type,protocole)
        self.buffer = ""

    def connect(self,host,port):
        self.sock.connect((host,port))

    def verify_connexion(self):
        code = 404
        if self.receive() > 0:
            msg = self._unpack_str()
            code = unpack("!i",self.buffer)[0]
            if msg == self.NTW_WELCOM_MSG and code == self.NTW_ERROR_NO:
                print "verify_connexion <%d : %s>" % (code,msg)
            else:
                print "verify_connexion <%d : %s>" % (code,msg)
        self.clear()
        return code
    
    def _unpack_str(self):
        i = 0
        while self.buffer[i]!= '\0':
            i+=1
        i+=1
        res = self.buffer[:i]
        self.buffer = self.buffer[i:]
        return res

    def send(self):
        size = len(self.buffer)
        _size = pack('!h',size)
        data = _size + self.buffer
        sent = self.sock.send(data)
        if sent == 0:
            print "Connexion lost"
            return False
        return True

            
    def receive(self):
        size = b''
        size = self.sock.recv(2)
        if size == b'':
            print "Connexion lost"
            return None
        size = unpack('!h',size)[0]
        self.buffer = self.sock.recv(size)
        return size

#Format C Type 	            Python type 	    Standard size
#x  	pad byte 	        no value 	  	 
#c  	char 	            string of length    1
#b  	signed char 	    integer 	        1
#B  	unsigned char 	    integer 	        1
#?  	_Bool 	            bool 	            1
#h  	short 	            integer 	        2
#H  	unsigned short 	    integer 	        2
#i  	int 	            integer 	        4
#I  	unsigned int 	    integer 	        4
#l  	long 	            integer 	        4
#L  	unsigned long 	    integer 	        4
#q  	long long 	        integer 	        8
#Q  	unsigned long long 	integer 	        8
#f  	float 	            float 	            4 
#d  	double 	            float 	            8
#s  	char[] 	            string 	  	 
#p  	char[] 	            string 	  	 
#P  	void * 	            integer
    def add(self,data,type="s"):
        self.buffer +=pack('!'+type,data)

    def clear(self):
        self.buffer = ""

    def call(self,ret_type,func_id,types="",*args):
        if len(types) != len(args):
            print "Wrong number of args/type"
            return None

        self.clear()
        self.add(func_id,"i")
        if types:
            for i,val in enumerate(args):
                self.add(args[i],types[i])
        self.send()
        size = self.receive()
        if size:
           return unpack("!"+ret_type,self.buffer)[0]
        return None


def create_socket():
    sock = Socket(Socket.Dommaine.IP,Socket.Type.TCP)
    sock.connect("127.0.0.1",3987)
    if sock.verify_connexion() != sock.NTW_ERROR_NO:
        print "An error accur"
        return None
    return sock


def send_AnalyseMgf_to_calc(analyseMfg):
    sock = create_socket()
    if not sock:
        return False
    sock.call("i",3,"i",analyseMfg.pk)
