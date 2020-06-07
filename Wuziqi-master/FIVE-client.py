import socket


class Fight_AI(object):
    def __init__(self):
        pass

    def start(self):
        print('这个就不再写了，python运行起来也会很慢'
              '可以用我之前用C写的（见Things仓库）')


"""棋盘相关"""


class chess(object):

    def __init__(self):
        self.map = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] for i in range(15)]

    def initial_map(self):
        self.map = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] for i in range(15)]

    def print_map(self):
        print('当前棋盘:')
        print('  \t0 1 2 3 4 5 6 7 8 9 a b c d e', end='')
        for i in range(15):  # 15行
            print('\n%x:\t' % i, end='')
            for j in self.map[i]:
                if j == 0:
                    print('_ ', end='')
                elif j == 1:
                    print('Q ', end='')
                else:
                    print('X ', end='')
        print('\n')

    def step(self):
        print('请输入要下的地方,如：8 a')
        list = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e']
        while True:
            the_step1 = input('行：')
            the_step2 = input('列：')
            if not (the_step1 in list and the_step2 in list):
                print('输入错误，请重新输入！')
                continue
            if self.map[int(the_step1, 16)][int(the_step2, 16)] == 0:
                return the_step1 + the_step2
            else:
                print('输入错误，请重新输入！')

    def change(self, step, num):
        row = int(step[0], 16)
        col = int(step[1], 16)
        self.map[row][col] = num
        self.print_map()

    def judge(self):
        for i in range(15):
            for j in range(11):
                if self.map[i][j] == 0: continue
                row = self.map[i][j] + self.map[i][j + 1] + self.map[i][j + 2] + self.map[i][j + 3] + \
                      self.map[i][j + 4]
                if row == 5: return 1
                elif row == 30: return 2

        for i in range(11):
            for j in  range(15):
                if self.map[i][j] == 0: continue
                col = self.map[i][j] + self.map[i + 1][j] + self.map[i + 2][j] + self.map[i + 3][j] + \
                      self.map[i + 4][j]
                if col == 5: return 1
                elif col ==30: return 2

        for i in range(11):
            for j in range(11):
                if self.map[i][j] == 0: continue
                xx1 = self.map[i][j] + self.map[i + 1][j + 1] + self.map[i + 2][j + 2]+self.map[i + 3][j + 3] + \
                      self.map[i + 4][j + 4]
                if xx1 == 5: return 1
                elif xx1 == 30: return 2

        for i in range(4,15):
            for j in range(11):
                if self.map[i][j] == 0: continue
                xx1 = self.map[i][j] + self.map[i - 1][j + 1] + self.map[i - 2][j + 2] + self.map[i - 3][j + 3] + \
                      self.map[i - 4][j + 4]
                if xx1 == 5: return 1
                elif xx1 == 30: return 2

        return 0


"""联机对战"""


class Fight_online(object):  # 有socket name roomid 三个属性值

    def __init__(self, ip='127.0.0.1', port=5555):
        self.socket = socket.socket()
        self.socket.connect((ip, port))
        print('已连接到服务器!')
        self.name = input('输入名字：')
        self.socket.send(self.name.encode())

    def option(self):
        while True:
            print('请输入数字选择：\n'
                  '1:创建新房间\n'
                  '2:加入房间\n'
                  '3:返回主菜单')
            the_command = input('命令:')
            if the_command == '1':
                self.createroom()  # 进入创建房间状态
                if self.wait0():  # 等待别人加入状态
                    self.game(True)
            elif the_command == '2':
                if self.join():  # 成功加入则进入game状态
                    self.game(False)
            elif the_command == '3':
                self.socket.close()
                print('连接关闭！')
                break
            else:
                print('输入错误！')

    def createroom(self):
        self.socket.send(('C\r\t' + self.name).encode())  # C\r\tname
        self.roomid = int(self.socket.recv(1024).decode().split('\r\t')[1])
        print('成功\n你的房间ID为:%d' % self.roomid)

    def wait0(self):
        print('等待别人加入房间...')
        data = self.socket.recv(1024).decode()
        print('收到来自服务器的信息：', data)
        print('%s 加入了房间' % data)
        print('游戏即将开始！!')

        return True

    def join(self):
        self.socket.send(('J\r\t' + self.name).encode())
        roommsg = self.socket.recv(1024).decode().split('\r\t')[1:-1]
        loc_roomid = []
        for i in roommsg:  # 输出房间信息
            i = i.split('\t')
            loc_roomid.append(i[0])
            print('房间信息：')
            print('房间id\t\t房间状态\t\t玩家1\t\t\t玩家2\t\t')
            print(i[0] + ' \t\t', end='')
            if i[1] == 'True':
                print('正在游戏' + '\t\t', end='')
            else:
                print('等待加入' + '\t\t', end='')
            print(('%-8s' % i[2]) + '\t\t', end='')
            if i[1] == 'True':
                print(i[3] + '\t\t')
            else:
                print(' \t\t')

        while True:
            command = input('输入命令：房间id/exit：')
            if command == 'exit':
                self.socket.send(b'exit')
                break
            elif command in loc_roomid:  # 房间ID输入正确
                self.socket.send(command.encode())  # 发送给服务器
                self.roomid = int(command)
                if (self.socket.recv(1024).decode() == 'SUC'):
                    print('加入成功')
                    return True

    def game(self, init):  # init=True为先手，False为后手
        self.socket.send(('G\r\t%s' % self.name).encode())  # 告知服务器进入game状态
        print('进入game状态', 'roomid:%d' % self.roomid, 'your name:%s' % self.name)
        the_chess = chess()
        the_chess.initial_map()
        the_chess.print_map()
        flag = 0  # 0继续游戏，1为先手胜，2为后手胜利。
        i = 1
        if init:  # 为真，先手为1
            num = 1
            other = 6
            print('你先手')
            step = the_chess.step()
            self.socket.send(step.encode())
            the_chess.change(step, num)
        else:
            num = 6
            other = 1
            print('对方先手')
        while True:
            print('等待...')
            step = self.socket.recv(1024).decode()
            print('debug:收到对方的步:', step)
            the_chess.change(step, other)
            flag = the_chess.judge()
            if flag:
                break
            step = the_chess.step()
            self.socket.send(step.encode())
            the_chess.change(step, num)
            flag = the_chess.judge()
            if flag:
                break
        print('游戏结束！')
        if (init and flag == 1) or (not init and flag != 1):
            print('你赢了')
        else: print('对方胜利')

        self.socket.send(b'over')



"""主菜单"""


def Main_menu_msg():
    print('***主菜单***')
    print('请输入数字选择：\n'
          '1:与AI对战\n'
          '2:联机对战\n'
          '3:退出')


def Main_menu():
    Main_menu_msg()
    while True:
        the_command = input('命令:')

        if the_command == '1':
            fight_ai = Fight_AI()
            fight_ai.start()

            # 结束AI对战，返回主菜单
            Main_menu_msg()

        elif the_command == '2':
            fight_online = Fight_online()
            fight_online.option()

            # 结束联机对战，返回主菜单
            Main_menu_msg()

        elif the_command == '3':
            break

        else:
            print('输入错误！')


if __name__ == '__main__':
    print('**************************')
    print('***********WELCOME********')
    print('**************************')

    Main_menu()
