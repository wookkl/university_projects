import socket
from _thread import *
exit_flag=True
def send(c,_id):#send_Thread
	while True:
		msg=input()
		if msg=='/exit':
			c.send('/exit'.encode())
			break
		else: c.send(msg.encode())
	c.close()
def recv(c,_id):#recv_Thread
	global exit_flag
	while True:
		data=c.recv(1024).decode()
		if data=='/exit':break
		print(data)
	c.close()
	exit_flag=False
def Main():
	host='127.0.0.1'
	port=8000
	c=socket.socket(socket.AF_INET,socket.SOCK_STREAM)#IPv4 소켓생성
	c.connect((host,port))
	my_id=input('Enter your ID:')
	c.send(my_id.encode())
	msg=c.recv(1024).decode()
	if msg=='The name is already in used': #ID가 중복되었다!/종료
		print(msg)
		return
	else: print(msg)
	start_new_thread(send,(c,my_id))
	start_new_thread(recv,(c,my_id))
	while exit_flag:pass
if __name__=='__main__':
	Main()
