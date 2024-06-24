
import time

startTimes = [];
stopTimes = [];
intervals = [];

def delay_ms(interval):
  startTime = time.perf_counter();
  empty = 0;
  
  while( ( time.perf_counter() - startTime ) < (interval/1000) ):
    empty = empty;


def create_channels(numberOfChannels):

  for i in range(numberOfChannels):
    startTimes.append( time.perf_counter() );
    stopTimes.append( time.perf_counter() );
    intervals.append( 0 );
    
def start(channel):
  startTimes[channel] =  time.perf_counter();

def stop(channel):
  stopTimes[channel] = time.perf_counter();
  intervals[channel] = stopTimes[channel] - startTimes[channel];
  return( intervals[channel] );  
