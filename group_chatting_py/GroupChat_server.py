import socket
from _thread import *#쓰레드
users=[] #user들의 정보
def thread(c,_id,_addr):#서버 쓰레드 함수 구현
	global users
	while True:
		data=c.recv(1024) # Receive message from Client
		data=data.decode() #Decoding bytecode...
		if not data: #메세지 안에 아무것도 없을경우
			print('Disconneted by'+addr[0],':',addr[1])
			for conn,users_id in users:
				if conn != c : conn.send((_id+' left the chat server').encode())
			users.remove([c,_id])
			break
		if data=='/exit': #클라이언트가 채팅방을 나갈경우
			for conn,users_id in users:
				if conn != c : conn.send((_id+' left the chat server').encode()) #나머지 유저들에게 나간 유저를 알림
				else: c.send(data.encode())
			users.remove([c,_id])#해당 클라이언트의 정보들을 삭제
			break
		for conn,users_id in users: #메세지 전송
			if conn != c : conn.send((_id+':'+data).encode())
	c.close()
def Main():
	host='127.0.0.1'
	port=8000 #포트8000사용
	s=socket.socket(socket.AF_INET,socket.SOCK_STREAM) #IPv4 소켓 생성
	s.bind((host,port)) 
	s.listen(5)
	print('<Multi Chat Server>')
	while True:
		id_dup_flag=False #ID중복확인 flag
		conn,addr=s.accept()
		user_id=conn.recv(1024).decode()
		for user in users:#중복ID 찾기
			if user_id in user:
				id_dup_flag=True
		if id_dup_flag:#해당 아이디가 중복일경우
			conn.send('The name is already in used'.encode())
			conn.close()
			continue
		else: conn.send('Welcome to my chat SERVER'.encode())
		for c,users_id in users:#유저 입장
			c.send((user_id+' enter the chat server').encode())
		users.append([conn,user_id])
		start_new_thread(thread,(conn,user_id,addr))
	s.close()
if __name__=='__main__':
	Main()
