import sys, tty, termios
import json
import urllib2
import serial
import time
import atexit, os

old_settings=None
user_id = []
symb_count = 0
ready_for_new_user = True
user_id_string = ''
# arduino = serial.Serial('/dev/tty.usbmodem1421', 9600)
arduino = serial.Serial('/dev/ttyACM0', 9600)

def init_anykey():
   global old_settings
   old_settings = termios.tcgetattr(sys.stdin)
   new_settings = termios.tcgetattr(sys.stdin)
   new_settings[3] = new_settings[3] & ~(termios.ECHO | termios.ICANON) # lflags
   new_settings[6][termios.VMIN] = 0  # cc
   new_settings[6][termios.VTIME] = 0 # cc
   termios.tcsetattr(sys.stdin, termios.TCSADRAIN, new_settings)

@atexit.register
def term_anykey():
   global old_settings
   if old_settings:
      termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)

def anykey():
   # ch_set = []
   ch = os.read(sys.stdin.fileno(), 1)
   # while ch != None and len(ch) > 0:
      # ch_set.append( ord(ch[0]) )
      # ch = os.read(sys.stdin.fileno(), 1)
   return ch;

def get_heart_info(user_id):
    try:
        print('https://sys.mail.ru/valentine/api/stats/{}/'.format(user_id_string))
        data = json.load(urllib2.urlopen('https://sys.mail.ru/valentine/api/stats/{}/?api_app=valentine_contraption&api_key=XXX'.format(user_id_string)))
    except:
        return None
    print(data)
    return data

def reset_all():
    global ready_for_new_user
    global symb_count
    global user_id
    ready_for_new_user = True
    symb_count = 0
    user_id = []
    print("Resetting everything")

def wait_for_arduino():
    print("Waiting for arduino")
    arduino_data = ''
    while not ready_for_new_user:
        getch = anykey()
        if getch == 'i':
            reset_all()
            return
        elif getch == 'x':
            print ("Exit requested. Quitting!")
            sys.exit(0)
        if arduino.inWaiting() >= 1:
            data_ser = arduino.read(arduino.inWaiting()).decode('ascii')
            arduino_data = arduino_data + data_ser.lower()
            print("Data from arduino: {}".format(data_ser))
            if 'done' in arduino_data.lower():
                reset_all()
                return



init_anykey()

while True:
    print("Ready for card")
    symb_count=0
    user_id=[]
    while ready_for_new_user and symb_count!=6:
        getch = anykey()
        if getch is None or getch == '':
            time.sleep(0.1)
        elif getch == 'x':
            print ("Exit requested. Quitting!")
            sys.exit(0)
        elif getch == 'i':
            break
        else:
            print(ord(getch))
            user_id.append(getch)
            symb_count+=1
    user_id_string = ''.join(user_id)
    data = get_heart_info(user_id_string)
    if data is None:
        print("Error receiving data")
        reset_all()
        continue
    else:
        data['match_count']=10
        print("Got the data! Match count: {}".format(data['match_count']))
    match_cnt = str(data['match_count'])+"\n"
    arduino.write(bytes(match_cnt))
    ready_for_new_user = False
    wait_for_arduino()
