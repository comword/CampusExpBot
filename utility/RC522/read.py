import sys
sys.path.append('./')
import MFRC522
MIFAREReader = MFRC522.MFRC522()
if __name__ == '__main__':
  import signal
  continue_reading = True
  def end_read(signal, frame):
    global continue_reading
    continue_reading = False
    print "Ctrl+C captured, ending read."
    MIFAREReader.GPIO_CLEEN()
  
  signal.signal(signal.SIGINT, end_read)
  while continue_reading:
    (status,TagType) = MIFAREReader.MFRC522_Request(MIFAREReader.PICC_REQIDL)
    if status == MIFAREReader.MI_OK:
      print "Card detected"
    (status,backData) = MIFAREReader.MFRC522_Anticoll()
    if status == MIFAREReader.MI_OK:
      print "Card read UID: "+str(backData[0])+","+str(backData[1])+","+str(backData[2])+","+str(backData[3])+","+str(backData[4])
def read_once():
  MIFAREReader.GPIO_CLEEN()
  (status,TagType) = MIFAREReader.MFRC522_Request(MIFAREReader.PICC_REQIDL)
  if status == MIFAREReader.MI_OK:
    (status,backData) = MIFAREReader.MFRC522_Anticoll()
    if status == MIFAREReader.MI_OK:
      return backData
  backData[0]=0
  backData[1]=0
  backData[2]=0
  backData[3]=0
  backData[4]=0
  print "fail"
  return backData
def end_call():
  MIFAREReader.GPIO_CLEEN()
