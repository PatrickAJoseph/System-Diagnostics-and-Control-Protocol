
import multiprocessing as mp
import timing

def fun(q):

  while(1):
    print(q.get())
    timing.delay_ms(250)
    


if __name__ == '__main__':
  
  mp.set_start_method('spawn')
  q = mp.Queue()
  process = mp.Process( target = fun, args=(q,) )
  process.start()
  
  for i in range(0,10):
    print(q.put(i))
    timing.delay_ms(500)
    
  process.terminate();