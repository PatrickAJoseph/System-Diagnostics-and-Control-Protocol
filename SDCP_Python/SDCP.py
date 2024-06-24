
import stringUtils
from enum import Enum
import numpy as np
import serial
import time
import threading
import multiprocessing as mp
import timing

##########  Type definitions  #########

class frameListRecord:

  frame_ID = 0;
  frame_name = "";
  priority = 0;
  transmitMode = 0;
  rxCounter = 0;
  txCounter = 0;
  txEnable = False;
  rxEnable = False;
  errorCounter = 0;
  transmitInterval = 0;
  receiveInterval = 0;
  transmitInterval = 0;
  lastReceivedTime = 0;
  lastTransmitTime = 0;
  cyclicTransmitInterval = 0;
  trigger = False;
  frameBytes = [];
  checksum = 0;
  txInterval = 0;
  rxInterval = 0;

  def __init__(self,frame_ID,frame_name,priority):
    self.frame_ID = frame_ID;
    self.frame_name = frame_name;
    self.priority = priority;
    self.transmitMode = 0;
    self.transmitInterval = 1000;
    self.trigger = 0;
    self.rxCounter = 0;
    self.txCounter = 0;
    self.errorCounter = 0;
    self.frameBytes = [0,0,0,0,0,0];
    self.checksum = 0;
    self.txEnable = False;
    self.rxEnable = False;
    self.lastReceivedTime = 0;
    self.lastTransmitTime = 0;

class parameterType(Enum):
  UNSIGNED = 0
  SIGNED = 1
  FLOAT = 2

class parameterListRecord:

  owningFrameID = 0;
  owningFrameName = "";
  parameterName = "";
  dataType = "";
  startPosition = 0;
  bitLength = 0;
  factor = 1.0;
  offset = 0.0;
  value = 0;

  def __init__(self,parameterName,dataType,startPosition,bitLength,Factor,Offset,frameName,frameID):
    self.owningFrameID = frameID;
    self.owningFrameName = frameName;
    self.parameterName = parameterName;
    self.dataType = dataType;
    self.startPosition = startPosition;
    self.bitLength = bitLength;
    self.factor = Factor;
    self.offset = Offset;
    self.value = 0;

###################### Global variables ##############################
  

frameList = [];
parameterList = [];
txFrameList = [];
rxFrameList = [];
_serialPort = serial.Serial();
_txStartTime = 0;
_rxStartTime = 0;
_sessionStartTime = 0;

######################  Function definitions ##########################

def loadParameterFile(filePath):

  parameterFile = open(filePath,"r");
  parameterFileContents = parameterFile.read();
  
  # Get number of frames and number of parameters.
  
  tempIndex = 0;
  tempCounter = 1;

  while( tempIndex != -1 ):
    tempIndex = stringUtils.getCharIndex(parameterFileContents,'{',tempCounter);
    tempCounter = tempCounter + 1;

  nFrames = tempCounter - 2;
  
  #print(nFrames);
    
  tempIndex = 0;
  tempCounter = 1;

  while( tempIndex != -1 ):
    tempIndex = stringUtils.getCharIndex(parameterFileContents,'(',tempCounter);
    tempCounter = tempCounter + 1;
  
  nParameters = tempCounter;
  
  #print(nParameters);
  
  ###########   Load frame records  ##########
  
  tempString = "";
  tempFrameName = "";
  tempFrameID = 0;
  
  for i in range(0,nFrames):
    
    #A frame descriptor has the format "{Frame_Name=[frame name],Frame_ID=[frame ID]}"
    
    tempString = stringUtils.getSubstring( parameterFileContents, '{', i+1, '}', i+1 );
    
    tempFrameName = stringUtils.getStringBeforeChar(stringUtils.getStringAfterPatternMatch(tempString,"Frame_Name="),',',1);
    tempFrameID = int(stringUtils.getStringAfterPatternMatch(tempString,"Frame_ID="));

    frameList.append( frameListRecord(tempFrameID,tempFrameName,0) );
    
    #print(tempString);
  
  ############   Load parameter record     #########
  
  tempString = "";
  numberOfParametersPerFrame = 0;
  tempString = "";
  tempFrameName = "";
  tempFrameID = 0;
  tempString_1 = "";
  tempDataType = "";
  tempParameterName = "";
  tempStartPosition = 0;
  tempBitLength = 0;
  tempFactor = 0.0;
  tempOffset = 0.0;
  
  for i in range (0,nFrames):

    tempString_1 = stringUtils.getSubstring( parameterFileContents, '{', i+1, '}', i+1 );
    tempFrameName = stringUtils.getStringBeforeChar(stringUtils.getStringAfterPatternMatch(tempString_1,"Frame_Name="),',',1);
    tempFrameID = int(stringUtils.getStringAfterPatternMatch(tempString_1,"Frame_ID="));

    tempString_2 = stringUtils.getSubstring( parameterFileContents,'{',i+1,'[',i+1);    
    numberOfParametersPerFrame = stringUtils.getNumberOfMatches(tempString_2,'(');
    
    for j in range(0,numberOfParametersPerFrame):
      
      tempString = stringUtils.getSubstring( tempString_2, '(', j + 1 , ')' , j + 1 );
      tempString = tempString + ")";
            
      tempDataType = stringUtils.getStringBeforeChar(stringUtils.getStringAfterPatternMatch(tempString,"dataType="),',',1);
      tempStartPosition = int(stringUtils.getStringBeforeChar(stringUtils.getStringAfterPatternMatch(tempString,"startPosition="),',',1));
      tempBitLength = int(stringUtils.getStringBeforeChar(stringUtils.getStringAfterPatternMatch(tempString,"length="),',',1));
      tempFactor = float(stringUtils.getStringBeforeChar(stringUtils.getStringAfterPatternMatch(tempString,"factor="),',',1));
      tempOffset = float(stringUtils.getStringBeforeChar(stringUtils.getStringAfterPatternMatch(tempString,"offset="),')',1));
            
      if( j == 0 ):
        #print( stringUtils.getSubstring( tempString_2, '}', j+1, ':', j+1 ).replace('\n','') );
        tempParameterName = stringUtils.getSubstring( tempString_2, '}', j+1, ':', j+1 ).replace('\n','');
      else:
        tempParameterName = stringUtils.getSubstring( tempString_2, ')', j, ':', j+1 ).replace('\n','');
       
      parameterList.append(parameterListRecord(tempParameterName,tempDataType,tempStartPosition,tempBitLength,tempFactor,tempOffset,tempFrameName,tempFrameID));
       
      #print(tempParameterName)  
      #print(tempDataType);
      #print(tempStartPosition);
      #print(tempBitLength);
      #print(tempFactor);
      #print(tempOffset);      
      
  txFrameList = frameList;
  rxFrameList = frameList;  
      
  parameterFile.close();
  

#
# @brief: Function to get the list of available frames.
#
  
def printFrameList():
  for frame in frameList:
    print("Frame name: {frame_name}  Frame ID: {frame_ID}".format( frame_name = frame.frame_name, frame_ID = frame.frame_ID ) );

#
# @brief: Function to get the name of a frame of a certain frame ID.
#

def getFrameName(frameID):

  frameName = "";

  for frame in frameList:
    if( frame.frame_ID == frameID ):
      frameName = frame.frame_name;      
  return(frameName)

#
# @brief: Function to get the frame ID of a frame by name.
#
 
def getFrameID(frameName):

  frameID = -1;

  for frame in frameList:
    if( frame.frame_name == frameName ):
      frameID = frame.frame_ID;      
  return(frameID)

#
# @brief: Returns index of frame structure in frame list.
#

def getFrameIndex(frameName):

  frameIndex = -1;
  counter = 0;
  
  for frame in frameList:
    if( frame.frame_name == frameName ):
      frameID = frame.frame_ID;
      frameIndex = counter;
    counter = counter + 1;      
  
  return(frameIndex)
  

#
# @brief: Function to get the list of available parameters.
#

def getParameterList():
  
  for parameter in parameterList:
    print( parameter.parameterName );

#
# @brief: Function to get the index of a parameter of given name.
#
  
def getParameterIndex(parameterName):

  parameterCounter = 0;

  for parameter in parameterList:
    if( parameter.parameterName == parameterName ):
      #print(parameter.parameterName);
      break;
    parameterCounter = parameterCounter + 1;      

  return(parameterCounter);          

#
# @brief: Function to get name of a parameter.
#
  
def getParameterName(parameterIndex):
  return( parameterList[parameterIndex].parameterName );  

#
# @brief: Function to print details of a frame
#

def printFrameDetailsByFID(frame_ID):

  index = -1;

  for frame in frameList:
    if( frame_ID == frame.frame_ID ):
      break;
  index = index + 1;

  if( index < len(frameList) ):
    print( "\n<<<< Frame Name: {name}  Frame ID: {_id} >>>>\n".format( name = frame.frame_name, _id = frame.frame_ID) );  
    _B0 = frame.frameBytes[0];
    _B1 = frame.frameBytes[1];
    _B2 = frame.frameBytes[2];
    _B3 = frame.frameBytes[3];
    _B4 = frame.frameBytes[4];
    _B5 = frame.frameBytes[5];
    print( "Data bytes --> [ {B0}, {B1}, {B2}, {B3}, {B4}, {B5} ]".format( B0 = _B0, B1 = _B1, B2 = _B2, B3 = _B3, B4 = _B4, B5 = _B5 ));
    print( "TX count: {tx_count}".format( tx_count = frame.txCounter ) );
    print( "RX count: {rx_count}".format( rx_count = frame.rxCounter ) );
    print( "ERR count: {error_count}".format( error_count = frame.errorCounter ) );
    
    _checksum = "FAIL";
    
    if( frame.checksum == 1 ):    
      _checksum = "PASS";
    
    print( "Checksum status: {checksum}".format( checksum = _checksum ) );

    transmitMode = "Triggered";
    
    if( frame.transmitMode == True ):
      transmitMode = "Cyclic";
    
    print( "Transmit mode: {_transmitMode}".format( _transmitMode = transmitMode ) );
    print( "TX interval: {_txInterval} (ms)".format( _txInterval = frame.txInterval) );
    print( "Last TX time: {txTime} (ms)".format( txTime = frame.lastTransmitTime ));
    print( "RX interval: {_rxInterval} (ms)".format( _rxInterval = frame.rxInterval) );
    print( "Last RX time: {rxTime} (ms)".format( rxTime = frame.lastReceivedTime ));

#
# @brief: Function to print parameter details.
#

def printFrameDetailsByName(frameName):
  
  frameID = -1;
  
  for frame in frameList:
    if( frame.frame_name == frameName ):
      frameID = frame.frame_ID;
  
  if( frameID > -1 ):
    printFrameDetailsByFID(frameID);
  else:
    print("\nFrame not found.");  


#
#  @brief: Function to obtain checksum of data packet.
#

def obtainChecksum(_bytes):

  _checksum = 0;

  _checksum = _bytes[0] + _bytes[1] + _bytes[2] + _bytes[3] + _bytes[4];
  _checksum = 255 - (_checksum % 255);
  
  return(_checksum)  

#
# @brief: Function to verify checksum of received data packet.
#
    
def verifyChecksum(_bytes):

  _checksum = 0;

  _checksum = (_bytes[0] + _bytes[1] + _bytes[2] + _bytes[3] + _bytes[4] + _bytes[5]) % 255;
  
  return( _checksum == 0 );
       
#
# @brief: Function to write parameters to a standard message frame.
#

def writeParameterToFrame(pName,pValue):
  
  index = -1;
  _counter = 0;

  for parameter in parameterList:
    if( parameter.parameterName == pName ):
      index = _counter;
    _counter = _counter + 1;


  if( index > -1 ):

    _parameter = parameterList[index]; 
    
    frameIndex = -1;
    _counter = 0;
    
    #Search for the frame strucutre from frame list.
    
    for frame in frameList:
      if( frame.frame_ID == _parameter.owningFrameID ):
        frameIndex = _counter;
      _counter = _counter + 1;  
    
    _frame = frameList[frameIndex];
    
    _value = ( ( pValue - _parameter.offset ) / _parameter.factor );
    _bytes =  _frame.frameBytes;
    
    #Obtain frame bytes and the data field for frame corresponding to parameter.
    
    _bytes[0] = _parameter.owningFrameID + ( 128 * _frame.priority );     
    _data = ( ( _bytes[1] << 24 ) + ( _bytes[2] << 16 ) + ( _bytes[3] << 8 ) + ( _bytes[4] << 0 ) );

    #Obtain parameter details.

    _sbit   = _parameter.startPosition;
    _blen   = _parameter.bitLength;
    _dtype  = _parameter.dataType;    
    _factor = _parameter.factor;
    _offset = _parameter.offset;
    _newData = 0;
    
    _mask = ( ( 1 << _blen ) - 1 );
    _data = _data & (~_mask);
    
    # Write parameter to frame.
    
    if( _dtype == "unsigned" ):
      _newData = ( (int(_value) & ( ( 1 << _blen ) - 1 )) << _sbit );
 
    if( _dtype == "signed" ):
      
      sign = 0;
      
      if( _value < 0 ):
        sign = 1;
      
      # If sign of number is negative.
      
      if(sign == 1):
        
        _newData = ( ( 1 << _blen ) - ( int( abs(_value) ) & _mask ) );
    
    if( _dtype == "float" ):
      
      _newData = int(struct.unpack('I',struct.pack('<f',_value))[0]);          
    
    # Merge new data bytes with old data bytes.
    
    _data = _data | _newData;
    
    _bytes[1] = ( _data & ( 255 << 24 ) ) >> 24;
    _bytes[2] = ( _data & ( 255 << 16 ) ) >> 16;
    _bytes[3] = ( _data & ( 255 << 8 ) ) >> 8;
    _bytes[4] = ( _data & ( 255 << 0 ) ) >> 0;
    _bytes[5] = obtainChecksum(_bytes);
 
    #print(_bytes);
    return(_bytes);
    
  else:
    print("Parameter not found !");
    return([0,0,0,0,0,0]);


#
# @brief: Function to write parameters to a standard message frame.
#

def readParameterFromFrame(pName):
  
  index = -1;
  _counter = 0;

  for parameter in parameterList:
    if( parameter.parameterName == pName ):
      index = _counter;
    _counter = _counter + 1;


  if( index > -1 ):

    _parameter = parameterList[index]; 
    
    frameIndex = -1;
    _counter = 0;
    
    #Search for the frame strucutre from frame list.
    
    for frame in frameList:
      if( frame.frame_ID == _parameter.owningFrameID ):
        frameIndex = _counter;
      _counter = _counter + 1;  
    
    _frame = frameList[frameIndex];
    _bytes =  _frame.frameBytes;
    
    #Obtain frame bytes and the data field for frame corresponding to parameter.
    
    _bytes[0] = _parameter.owningFrameID + ( 128 * _frame.priority );     
    _data = ( ( _bytes[1] << 24 ) + ( _bytes[2] << 16 ) + ( _bytes[3] << 8 ) + ( _bytes[4] << 0 ) );

    #Obtain parameter details.

    _sbit   = _parameter.startPosition;
    _blen   = _parameter.bitLength;
    _dtype  = _parameter.dataType;    
    _factor = _parameter.factor;
    _offset = _parameter.offset;
    _newData = 0;
    
    _mask = ( ( 1 << _blen ) - 1 );
    
    # Extract data corresponding to parameter from the frame.
    
    _data = (_data & _mask) >> _sbit;
    
    if( _parameter.dataType == "unsigned" ):
      _parameter.value = float(_data) * _factor + _offset;

    if( _parameter.dataType == "signed" ):
      _maxAbsValue = ( ( 1 << ( _blen - 1 ) ) - 1 );

      if( _data > _maxAbsValue ):
        _parameter.value = ( _offset - _factor * (float)( ( 1 << _blen ) - _data ) );
      else:
        _parameter.value = _factor * float(_data) + _offset;      
    
    if(_parameter.dataType == "float"):
        _parameter.value = _factor * (struct.unpack('<f',struct.pack('<I',num))[0]) + _offset;
    
    #print(_parameter.value);
    
  else:
    print("Parameter not found !");

  return(_parameter.value);

######################  Serial Port Functions.  ################


def sync():
  
  packet = bytearray();
    
  packet.append(0x00);
  packet.append(0x00);
  packet.append(0x00);
  packet.append(0x00);
  packet.append(0x00);
  packet.append(0x00);
  
  _serialPort.flush();
  _serialPort.reset_input_buffer();
  _serialPort.reset_output_buffer();
  _serialPort.write(packet);
  
  syncAttempts = 0;
  currentByte = 0;
  previousByte = 255;
  syncCounter = 0;
  
  while ((syncCounter < 5) and (syncAttempts < 10000)):

    currentByte = int.from_bytes(_serialPort.read(1),"big");
    #print(currentByte);

    if ((currentByte == previousByte) and (currentByte == 0) and (previousByte == 0)):
      syncCounter = syncCounter + 1;
    else:
      syncCounter = 0;
      syncAttempts = syncAttempts + 1;

    previousByte = currentByte;
    
  #if( syncAttempts > 9999 ):
    #print("\nSession not opened");
    #print("Number of tries : {_attempts}".format( _attempts = syncAttempts ));
  #else:
    #print("\nSession opened")
    #print("Number of tries : {_attempts}".format( _attempts = syncAttempts ));

def openSession(portName,baudRate):

  _serialPort.baudrate = baudRate;
  _serialPort.port = portName;
  _serialPort.open();
  _sessionStartTime = time.perf_counter();
  
  sync();
  
def closeSession():

  if( _serialPort.is_open ):
    _serialPort.close();      

#Function to write a parameter to a frame to send message to device.

def wpsf(parameterName,parameterValue):
  
  if( _serialPort.is_open ):
    _serialPort.write(writeParameterToFrame(parameterName,parameterValue));
    _parameter = parameterList[ getParameterIndex(parameterName) ];
    
    index = -1;
    counter = 0;
    
    for frame in frameList:
      if( frame.frame_ID == _parameter.owningFrameID ):
        index = counter;
        break;
      counter = counter + 1;

    _frame = frameList[index];

    #print("Frame ID: {_fid}".format( _fid = _frame.frame_ID ) ); 

    _frame.txCounter = _frame.txCounter + 1;
    _frame.transmitInterval = 1000*time.perf_counter() - _frame.lastTransmitTime;
    _frame.txInterval = 1000*time.perf_counter() - _frame.lastTransmitTime;    
    _frame.lastTransmitTime = 1000*( time.perf_counter() - _sessionStartTime );
    
    if(verifyChecksum(_frame.frameBytes)):
      _frame.checksum = 1;
    else:
      _frame.checksum = 0;
    
    frameList[index] = _frame;    

#Function to read a standard message frame from the device after sending
#a SYNC frame.

def rfdws_fid(frame_ID):
  
  _counter = 0;
  index = -1;
  
  #Search for object for frame from frame list.
  
  for frame in frameList:
    if( frame.frame_ID == frame_ID ):
      index = _counter;
    _counter = _counter + 1;  
    
  #Proceed only if frame is found.  
   
  if( index > -1 ):
    
    _frame = frameList[index];     
    
    #Proceed only if serial port is open.
    
    if( _serialPort.is_open ):
      
      _packet = [0,0,0,0,0,0];
   
      sync();
   
      startTime = time.perf_counter();
      
      _found = False;
      
      # Continue with attempts until the required message frame is found or timeout event is occured. 
      
      while( ( _found == 0 ) and ( (time.perf_counter() - startTime) < 10.0 ) ):

        # Read 6 bytes from device via serial port.

        for i in range(0,6):
          _packet[i] = int.from_bytes(_serialPort.read(1),"big");
   
        #verify checksum.
   
        if( verifyChecksum(_packet) ):
          
          #Extract PID from byte 0 (most significant byte) and check if current frame IDs match.
          
          if( (_packet[0] & 127) == frame_ID ):
            _found = True;
      
      #If required frame is found, load the details of the required frame.
      
      if(_found == True):
        _frame.frameBytes[0] = _packet[0];
        _frame.frameBytes[1] = _packet[1];
        _frame.frameBytes[2] = _packet[2];
        _frame.frameBytes[3] = _packet[3];
        _frame.frameBytes[4] = _packet[4];
        _frame.frameBytes[5] = _packet[5];
        _frame.rxCounter = _frame.rxCounter + 1;        

        _frame.rxInterval = 1000*time.perf_counter() - _frame.lastReceivedTime;
        _frame.lastReceivedTime = 1000*(time.perf_counter() - _sessionStartTime);        
        
        if( verifyChecksum(_frame.frameBytes) == False ):
          _frame.errorCounter = _frame.errorCounter + 1;
        else:
          _frame.checksum = 1;
        
        frameList[index] = _frame;        

#Function to read a standard message frame from the device without sending
#a SYNC frame.

def rfd_fid(frame_ID):
  
  _counter = 0;
  index = -1;
  
  #Search for object for frame from frame list.
  
  for frame in frameList:
    if( frame.frame_ID == frame_ID ):
      index = _counter;
    _counter = _counter + 1;  
    
  #Proceed only if frame is found.  
   
  if( index > -1 ):
    
    _frame = frameList[index];     
    
    #Proceed only if serial port is open.
    
    if( _serialPort.is_open ):
      
      _packet = [0,0,0,0,0,0];
      
      startTime = time.perf_counter();
      
      _found = False;
      
      # Continue with attempts until the required message frame is found or timeout event is occured. 
      
      while( ( _found == 0 ) and ( (time.perf_counter() - startTime) < 10.0 ) ):

        # Read 6 bytes from device via serial port.

        for i in range(0,6):
          _packet[i] = int.from_bytes(_serialPort.read(1),"big");
   
        #verify checksum.
   
        if( verifyChecksum(_packet) ):
          
          #Extract PID from byte 0 (most significant byte) and check if current frame IDs match.
          
          if( (_packet[0] & 127) == frame_ID ):
            _found = True;
      
      #If required frame is found, load the details of the required frame.
      
      if(_found == True):
        _frame.frameBytes[0] = _packet[0];
        _frame.frameBytes[1] = _packet[1];
        _frame.frameBytes[2] = _packet[2];
        _frame.frameBytes[3] = _packet[3];
        _frame.frameBytes[4] = _packet[4];
        _frame.frameBytes[5] = _packet[5];
        _frame.rxCounter = _frame.rxCounter + 1;        

        _frame.rxInterval = 1000*time.perf_counter() - _frame.lastReceivedTime;
        _frame.lastReceivedTime = 1000*(time.perf_counter() - _sessionStartTime);        

        #print("PERF counter: {_perf}".format(_perf=time.perf_counter()));
        #print("Last received time: {_rxTime}".format(_rxTime=_frame.lastReceivedTime));
        #print("Receive interval: {_rxInterval}".format(_rxInterval=_frame.rxInterval));

        
        if( verifyChecksum(_frame.frameBytes) == False ):
          _frame.errorCounter = _frame.errorCounter + 1;
          _frame.checksum = 0;
        else:
          _frame.checksum = 1;
        
        frameList[index] = _frame;        
 
#
# @brief: Read standard message frame from device (frame address by its name) after sending
#         SYNC frame.
#
 
def rfdws_name(frameName):
  rfdws_fid( getFrameID(frameName) );

#
# @brief: Read standard message frame from device (frame address by its name) without sending
#         SYNC frame.
#
 
def rfd_name(frameName):
  rfd_fid( getFrameID(frameName) );

#
# @breif: Function to read a certain parameter after sending SYNC.
#

def rpdws(parameterName):

  index = -1;
  counter = 0;
  value = 0;
  
  for parameter in parameterList:
    if( parameter.parameterName == parameterName ):
      index = counter;
    counter = counter + 1;  
 
  if( index > -1 ):
    _parameter = parameterList[index];    
    rfdws_fid(_parameter.owningFrameID);
    value = readParameterFromFrame(parameterName);
        
  return(value);

#
# @breif: Function to read a certain parameter without sending SYNC.
#

def rpd(parameterName):

  index = -1;
  counter = 0;
  value = 0;
  
  for parameter in parameterList:
    if( parameter.parameterName == parameterName ):
      index = counter;
    counter = counter + 1;  
 
  if( index > -1 ):
    _parameter = parameterList[index];    
    rfd_fid(_parameter.owningFrameID);
    value = readParameterFromFrame(parameterName);
        
  return(value);

#
# @brief: Transmit process.
#

def ctx(frameList):

  _txStartTime = time.perf_counter();
        
  counter = 0;
     
  _txBytes = [];
    
  #Loop through available frames.
    
  for frame in frameList:
            
    if( frame.txEnable == True ):
      
      #If transmit mode is triggered mode.
        
      if( frame.transmitMode == False ):
        
        #If trigger bit is set, push message to buffer.
         
        if( frame.trigger == True ):
          
          for i in range(0,6):
            
            _txBytes.append( frame.frameBytes[i] );
              
          frame.txCounter = frame.txCounter + 1;
          frame.txInterval = 1000*time.perf_counter() - frame.lastTransmitTime;
          frame.lastTransmitTime = 1000*(time.perf_counter() - _txStartTime);
          frameList[counter] = frame;

      #If transmit mode is cyclic.

      if( frame.transmitMode == True ):
        
        #print( "Time instance: {_instance}, Last tx time: {_txTime}".format(_instance = time.perf_counter(), _txTime = frame.lastTransmitTime ) );
        #print(1000*time.perf_counter() - frame.lastTransmitTime);

        #print("Cyclic transmit interval: {_interval} (ms)".format(_interval = frame.cyclicTransmitInterval ));

        
        if( (1000*time.perf_counter() - frame.lastTransmitTime) > frame.cyclicTransmitInterval ):
          
          #print("Transmitted");
          
          #frame.transmitInterval = 1000*time.perf_counter() - frame.lastTransmitTime;
          frame.lastTransmitTime = 1000*time.perf_counter();              

          for i in range(0,6):
            
            _txBytes.append( frame.frameBytes[i] );
                
                
          print(frame.txCounter);
          
          frame.txCounter = frame.txCounter + 1;
          frameList[counter] = frame;              
                                
    counter = counter + 1;    

  return(frameList)
  
  
############ Functions for continuous transmission of standard message frames.

#
# @brief: Enables continuous transmission of message frame.
#

def enableContinuousTransmission(frameName):
  
  frameIndex = getFrameIndex(frameName);
  _frame = frameList[frameIndex];
  _frame.txEnable = True;
  frameList[frameIndex] = _frame;
      
#
# @brief: Set Tx frame cyclic mode .
#

def setCyclicModeParams(frameName,isCyclicMode,cyclicTransmitInterval):

  frameIndex = getFrameIndex(frameName);
  
  _frame = frameList[frameIndex];
  _frame.transmitMode = isCyclicMode;
  _frame.cyclicTransmitInterval = cyclicTransmitInterval;
  _frame.trigger = False;
  
  frameList[frameIndex] = _frame;
    
  
  
  
  

