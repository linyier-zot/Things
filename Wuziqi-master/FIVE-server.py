import socket
import threading
import random

class gameserver(object):
    room = {0: ['Bool', 'name', 'name'], }
    all_socket = {}
    def __init__(self, ip='127.0.0.1', port=5555):
        self.server = socket.socket()
        self.server.bind((ip, port))

    def start(self):
        self.server.listen()
        print('开始监听')
        threading.Thread(target=self.accept).start()

    def accept(self):
        while True:
            the_s, addr = self.server.accept()
            print('accept, create a new threading!')
            threading.Thread(target=self.the_recv, args=(the_s,)).start()   #为连接创建线程

    def the_recv(self, recv_socket):
        name = recv_socket.recv(1024).decode()
        self.all_socket[name] = recv_socket #存入名字与socket
        while True:
            global roomiddd
            data = recv_socket.recv(1024)
            if not data:
                break
            data = data.decode()
            command = data.split('\r\t')
            print('debug:收到命令：', command)
            """根据发来的byte流判断进入对应方法"""
            if command[0] == 'C':           #创建房间
                self.createroom(command[1], recv_socket) #传入名字及socket
            elif command[0] == 'J':
                roomiddd = self.joinroom(name, recv_socket)        #传入名字及socket
            elif command[0] == 'G':           #进入游戏状态
                self.game(roomiddd, recv_socket, name)

    def createroom(self, name, sock):
        id = 0
        while id in self.room:
            id = random.randint(1, 10000)   #生成房间ID
        self.room[id]=[False, name, None]   #False表示房间未满，另外两个元素记录名字，key为房间ID
        print('创建房间 %d' % id)
        sock.send(('ACK\r\t%d' % id).encode())

    def deleteroom(self):   #待开发，在发生异常（如客户端直接断开连接），要删除对应房间
        pass

    def joinroom(self, name, sock):
        #向客户端发送房间信息
        pack = ''       #打包房间信息
        for i in self.room:
            pack = pack + str(i) + '\t' + str(self.room[i][0])+ '\t' + str(self.room[i][1])+ '\t'\
                   + str(self.room[i][2]) + '\r\t'
        sock.send(pack.encode())

        #等待客户端下一步请求
        command = sock.recv(1024).decode()
        if command == 'exit':
            pass
        else:                                     #command为房间号
            self.room[int(command)][2] = name      #加入房间信息
            """这里应该考虑接受到加入A用户加入房间时，该房间已经被其他用户加入了"""
            sock.send(b'SUC')
            fightwith = self.room[int(command)][1]
            print(fightwith, type(fightwith))
            sendsock = self.all_socket[fightwith]
            sendsock.send(name.encode()) #告诉处在wait0状态的人，有人加入，告知其进入游戏状态
            self.room[int(command)][0] = True
            return int(command)

    """收到信息，转发给房间的另一个人"""
    def game(self, roomid, own_sock, name):   #init 决定先手，以及后续的发送信息先后
        for fight_with in [self.room[roomid][1], self.room[roomid][2]]:
            if fight_with != name:
                break
        while True:
            fight_sock = self.all_socket[fight_with]
            step = own_sock.recv(1024).decode()
            print('receive ', step)
            if step == 'over':
                break
            fight_sock.send(step.encode())   #转发
        self.room[roomid] = 0
        self.room.pop(roomid)











def debugmsg():
    print('1:房间dict')

if __name__ == '__main__':
    main_server = gameserver()
    main_server.start()
    while True:
    #    debugmsg()
        a = input()
        if a =='1':
            print(main_server.room)
