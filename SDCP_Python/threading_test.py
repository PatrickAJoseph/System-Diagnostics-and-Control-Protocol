import threading
import timing 
 
 
def TX_thread():
  while(1):    
    print("Cube: {}")

    global TX_stop
    if(TX_stop):
      break;    

 
if __name__ =="__main__":
    
    timing.create_channels(10);
    
    t1 = threading.Thread(target=TX_thread)
    TX_stop = False;
    t1.start()
    timing.start(5);
    timing.delay_ms(1);
    TX_stop = True;
    print( timing.stop(5) );
    t1.join()
 
    print("Done!")