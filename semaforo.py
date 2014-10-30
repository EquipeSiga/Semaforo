import serial
import time
import jenkins

INTERVALO = 60
PORTA = 'COM4'
UNFORTUNATELY_RED = 'R'
FINALLY_GREEN = 'G'
RED = 'r'
GREEN = 'g'
TOGGLE = 'b'
ERROR = 'e'

def get_status():
  status = GREEN

  try:
    for job in j.get_jobs():
      if job['color'] == 'red_anime' or job['color'] == 'aborted_anime' or job['color'] == 'blue_anime':
        status = TOGGLE
        break
      elif job['color'] == 'red':
        status = RED
        break
      
  except Exception:
    status = ERROR

  return status

j = jenkins.Jenkins('url', 'user', 'password')

ser = serial.Serial(PORTA, 9600)
ser.read(1)

print 'Ready!'

projects = dict()

while True:
  status = get_status()
  if status == TOGGLE:
    ser.write(RED)
    time.sleep(1)
    ser.write(GREEN)
    time.sleep(1)
  elif status == ERROR:
    ser.write(RED)
    time.sleep(2)
    ser.write(GREEN)
    time.sleep(2)
  else:
    ser.write(status)
    time.sleep(INTERVALO)
    
  if projects == dict():
    try:
      for job in j.get_jobs():
        projects[job['name']] = job['color'];
    except Exception:
      pass
  else:
    try:
      for job in j.get_jobs():
        if (('red' == job['color']) and ('blue' == projects[job['name']])):
          ser.write(UNFORTUNATELY_RED)
          time.sleep(2)
        elif (('blue' == job['color']) and ('red' == projects[job['name']])):
          ser.write(FINALLY_GREEN)
          time.sleep(2)
        projects[job['name']] = job['color'];
    except Exception:
      pass
  
  print "=================== ESTADO DOS PROJETOS ==================="
  print projects
  
ser.close()
