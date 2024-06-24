
import stringUtils
from enum import Enum
import numpy as np
import serial
import time
import threading
import multiprocessing as mp
import timing


class sdcp:

  frameList = [];
  parameterList = [];
  serialPort = serial.Serial();                

  def __init__(self):                         # API INITIALIZATION.
  
    frameList = [];
    parameterList = [];  
    serialPort = serial.Serial();
  
  class frame:                                # ENTRY IN FRAME LIST. 

    frame_ID = 0;                             # ID number for a standard message frame. Its value must be from 0 to 127.   
    frame_name = "";                          # Name of the standard message frame / data frame.     
    priority = 0;                             # Priority of standard message frame ( 0 --> LOW, 1 --> HIGH ).
    transmitMode = "triggered";               # Transmit mode of the data frame. It can be set to cyclic or triggered mode.
                                              # In trigger mode, the application must send a trigger via SDCP.triggerTX
                                              # function to trigger the transmission of the data frame to device.
                                              # In case of cyclic mode, the data frame is transmitted at an interval mentioned
                                              # in frameListRecord.cyclicTransmitInterval parameter.                                            
    rxCount = 0;                              # Number of times the data frame has been received/read from device.
    txCount = 0;                              # Number of times the data frame has been transmitted by host to device.
    ctxEnable = False;                        # Enables continuous transfer of data from host to device.  
    crxEnable = False;                        # Enables continuous reception of data from device by the host.
    errorCount = 0;                           # The error count is incremented when the checksum of frame received from device
                                              # does not equal to zero (i.e) checksum failure.
    txInterval = 0;                           # Interval of transmission of standard message frame/data frame from host to device
                                              # in milli-seconds. 
    rxInterval = 0;                           # Interval of reception of standard message frame/data frame from device
                                              # in milli-seconds.
    ctxInterval = 0;                          # Cyclic transmit interval in milli-seconds. The data frame will be transmitted at 
                                              # the specified interval when cyclic transmission mode is enabled.   
    rxTime = 0;                               # Last time the message frame was received from device since session start in milli-seconds.
    txTime = 0;                               # Last time the message frame was transmitted by the host to device since session start in milli-seconds.
    triggerTX = False;                        # Enable/trigger transmission of message frame to device.
    packet = [];                              # Packet associated with frame.
    checksumStatus = False;                   # Status of the checksum of data packet received from device.             
    
    def __init__(self,frame_ID,frame_name,priority):       # Constructor for frame list record.
    
      # Loading of input arguments.
    
      self.frame_ID = frame_ID;                           
      self.frame_name = frame_name;                       
      self.priority = priority;

      # Initialization of internal variables.
    
      self.transmitMode = "triggered";
      self.rxCount = 0;
      self.txCount = 0;
      self.ctxEnable = False;
      self.crxEnable = False;
      self.errorCounter = 0;
      self.txInterval = 0;
      self.rxInterval = 0;
      self.txTime = 0;
      self.rxTime = 0;
      self.triggerTX = False;
      self.packet = [ 0, 0, 0, 0, 0, 0 ]
      self.checksumStatus = False;

  class parameterType(Enum):      # Data type of parameter.
    UNSIGNED = 0
    SIGNED = 1
    FLOAT = 2

  class parameter:                # Entry in parameter list.

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
      self.packet = [0, 0, 0, 0, 0, 0]
  
  #  
  #
  # @brief:     Loads frame list and parameter from "*.param" file.
  # @params:    Location of file to be loaded.
  # @details:   Load details of frame and parameters into the frame list and parameter list in API.
  #             When no file path is passed, it takes the *.param file which was loaded the previous
  #             time. 
  #
  
  def load_parameter_file( self, filePath ):
  
    parameterFile = open(filePath,"r");
    parameterFileContents = parameterFile.read();
  
    # Get number of frames and number of parameters.
  
    tempIndex = 0;
    tempCounter = 1;

    while( tempIndex != -1 ):
      tempIndex = stringUtils.getCharIndex(parameterFileContents,'{',tempCounter);
      tempCounter = tempCounter + 1;

    nFrames = tempCounter - 2;
  
    
    tempIndex = 0;
    tempCounter = 1;

    while( tempIndex != -1 ):
      tempIndex = stringUtils.getCharIndex(parameterFileContents,'(',tempCounter);
      tempCounter = tempCounter + 1;
  
    nParameters = tempCounter;
   
    ###########   Load frame records  ##########
  
    tempString = "";
    tempFrameName = "";
    tempFrameID = 0;
  
    for i in range(0,nFrames):
    
      #A frame descriptor has the format "{Frame_Name=[frame name],Frame_ID=[frame ID]}"
    
      tempString = stringUtils.getSubstring( parameterFileContents, '{', i+1, '}', i+1 );
    
      tempFrameName = stringUtils.getStringBeforeChar(stringUtils.getStringAfterPatternMatch(tempString,"Frame_Name="),',',1);
      tempFrameID = int(stringUtils.getStringAfterPatternMatch(tempString,"Frame_ID="));

      self.frameList.append( self.frame(tempFrameID,tempFrameName,0) );
    
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
       
        self.parameterList.append(self.parameter(tempParameterName,tempDataType,tempStartPosition,tempBitLength,tempFactor,tempOffset,tempFrameName,tempFrameID));
       
        #print(tempParameterName)  
        #print(tempDataType);
        #print(tempStartPosition);
        #print(tempBitLength);
        #print(tempFactor);
        #print(tempOffset);      
            
      parameterFile.close();
    
    
  #
  # @brief:       Prints the list of available frames with its names and and frame ID.
  # @parameters:  None. 
  # @Return:      None
  #
    
  def print_frame_list(self):
      
    print("\nFrame List:\n");
      
    for frame in self.frameList:
        
      print( "{_fname} [{_fid}]".format(_fname = frame.frame_name, _fid = frame.frame_ID) );
        
  #
  # @brief:       Prints the list of available parameters with its name, owning frame name and frame ID.
  # @parameters:  None. 
  # @Return:      None
  #
    
  def print_parameter_list(self):
      
    print("\nParameter List:\n");
      
    for parameter in self.parameterList:
        
      print( parameter.parameterName );

  #
  # @brief:         Returns the index of required message frame in the frame list.
  # @parameters:    Name of standard message frame / data frame.
  # @Return:        Index of the required message frame in the frame list.
  # @Details:       This function returns the index of the target message frame from the frame list.
  #

  def get_frame_index(self,frameName):
    
    index = -1;
    counter = 0;
    
    for frame in self.frameList:
      
      if( frame.frame_name == frameName ):
        index = counter;
      counter = counter + 1    

    return(index);

  #
  # @brief:         Returns the frame ID of the frame referenced by name.  
  # @parameters:    Name of the standard message frame.  
  # @Return:        Frame ID.
  # @Details:       Returns the ID of the frame referenced by its name.
  #  

  def get_frame_ID(self,frame_name):
  
    return( (self.frameList[ self.get_frame_index( frame_name ) ]).frame_ID );
        
  #
  # @brief:         Prints details about required frame.
  # @parameters:    Name of standard message frame /data frame.
  # @Return:        None.
  # @Details:       Prints details of a frame such as error count, transmit and receive count,
  #                 transmit interval, receive interval, frame name, frame ID and data packet
  #                 and checksum status.

  def print_frame_details(self,frameName):
    
    frame = self.frameList[ self.get_frame_index(frameName) ];
    
    print("\n>>>> {_fname} <<<<\n".format( _fname = frame.frame_name ));    
    
    print( "Frame name: {_fname}".format( _fname = frame.frame_name) );
    print( "Frame ID: {_fid}".format( _fid = frame.frame_ID ) );
    print( "Packet: {_packet}".format( _packet = frame.packet ) );
    print( "TX count: {_txcount}".format( _txcount = frame.txCount ) );
    print( "RX count: {_rxcount}".format( _rxcount = frame.rxCount ) );
    print( "ERR count: {_errcount}".format( _errcount = frame.errorCount ) );
    print( "TX interval: {_txInterval} (ms)".format( _txInterval = frame.txInterval ) );
    print( "RX interval: {_rxInterval} (ms)".format( _rxInterval = frame.rxInterval ) );
    print( "Last TX time: {_txTime} (ms)".format( _txTime = frame.txTime ) );
    print( "Last RX time: {_rxTime} (ms)".format( _rxTime = frame.rxTime ) );
    
    if( frame.checksumStatus == True ):
      print( "Checksum status: PASSED " );
    else:
      print( "Checksum status: FAILED " );

    print("\n");

  #
  # @brief:         Returns the index of required in the parameter list..
  # @parameters:    Name of target parameter.
  # @Return:        Index of parameter from the parameter list.
  # @Details:       Returns the index of the parameter addressed by its name from the parameter list.
  #
  
  def get_parameter_index(self,parameterName):
  
    index = -1;
    counter = 0;

    for parameter in self.parameterList:
      if( parameter.parameterName == parameterName ):
        index = counter;      
      counter = counter + 1;  
      
    return(index)

  #
  # @brief:         Prints details of parameter.
  # @parameters:    Name of the parameter whose details have to be printed.
  # @Return:        None.
  # @Details:       Prints details about a parameter such as the owning frame name, frame ID
  #                 start bit, bit length, data type and value
  
  def print_parameter_details(self,parameterName):
  
    parameter = self.parameterList[ self.get_parameter_index(parameterName) ];
    frame     = self.frameList[ self.get_frame_index(parameter.owningFrameName) ];
    
    print( "\n>>>> {_pname} <<<<\n".format( _pname = parameter.parameterName ) );
    print( "Parameter name: {_pname}".format( _pname = parameter.parameterName ) );
    print( "Owning frame: {_fname} [{_fid}]".format( _fname = frame.frame_name, _fid = frame.frame_ID ) );
    print( "Start bit: {_sbit}".format( _sbit = parameter.startPosition ) );
    print( "Bit length: {_blen}".format( _blen = parameter.bitLength ) );
    print( "Value: {_value}".format( _value = parameter.value) );
    print("\n");

  #
  # @brief:         Updates parameter values for a given frame.
  # @parameters:    Name of frame which has to be updated.
  # @Return:        None.
  # @Details:       Updates the values of all parameters related to a particular  
  #                 frame.

  def update_frame_parameters(self,frame_name):

    frameParameters = [];
    
    for parameter in self.parameterList:
      
      if( parameter.owningFrameName == frame_name ):
        self.read_parameter_from_frame(parameter.parameterName);
        
    
  #
  # @brief:         Prints frame details along with parameters associated with it.
  # @parameters:    Name of the parameter whose details have to be printed.
  # @Return:        None.
  # @Details:       Prints details about a frame such as frame name and frame ID etc.
  #                 and also prints the name of parameters associated with the frame and its values.
  
  def print_frame_and_parameter_details(self,frameName):
  
    frame = self.frameList[ self.get_frame_index( frameName ) ];
    parameters = [];
    
    for parameter in self.parameterList:
    
      if( parameter.owningFrameName == frameName ):
        parameters.append(parameter);      
        
    print("\n>>>> {_fname} <<<<\n".format( _fname = frame.frame_name ));    
    
    print( "Frame name: {_fname}".format( _fname = frame.frame_name) );
    print( "Frame ID: {_fid}".format( _fid = frame.frame_ID ) );
    print( "Packet: {_packet}".format( _packet = frame.packet ) );
    print( "TX count: {_txcount}".format( _txcount = frame.txCount ) );
    print( "RX count: {_rxcount}".format( _rxcount = frame.rxCount ) );
    print( "ERR count: {_errcount}".format( _errcount = frame.errorCount ) );
    print( "TX interval: {_txInterval} (ms)".format( _txInterval = frame.txInterval ) );
    print( "RX interval: {_rxInterval} (ms)".format( _rxInterval = frame.rxInterval ) );
    print( "Last TX time: {_txTime} (ms)".format( _txTime = frame.txTime ) );
    print( "Last RX time: {_rxTime} (ms)".format( _rxTime = frame.rxTime ) );
    
    if( frame.checksumStatus == True ):
      print( "Checksum status: PASSED " );
    else:
      print( "Checksum status: FAILED " );

    print("\n");
    
    for parameter in parameters:
      self.read_parameter_from_frame(parameter.parameterName);
      print("{_pname} = {_pvalue} ".format( _pname = parameter.parameterName , _pvalue = parameter.value ) );

  #
  #  @brief:            Function to obtain checksum of data packet.
  #  @parameters:       Array of bytes for which checksum has to be calculated. 
  #  @Return:           Checksum of array of bytes.  
  #  @Description:      Calculates the checksum of the array of bytes by taking the 1's   
  #                     complement of 255 th modulus of sum of lements of byte array. 
  #

  def obtainChecksum(self,_bytes):

    _checksum = 0;

    _checksum = _bytes[0] + _bytes[1] + _bytes[2] + _bytes[3] + _bytes[4];
    _checksum = 255 - (_checksum % 255);
  
    return(_checksum)  

  #
  # @brief:             Function to verify checksum of received data packet.
  # @parameters:        Checksum status of array of bytes.
  # @Return:            Status of checksum.
  # @Details:           Computes checksum of given byte array and returns verification 
  #                     status of checksum.
  #
    
  def verifyChecksum(self,_bytes):

    _checksum = 0;

    _checksum = (_bytes[0] + _bytes[1] + _bytes[2] + _bytes[3] + _bytes[4] + _bytes[5]) % 255;
  
    return( _checksum == 0 );
       
  #
  # @brief:         Function to write parameters to a standard message frame.
  # @parameters:    Name of parameter and its value.
  # @Return:        The packet corresponding to the owning frame of the parameter.
  # @Details:       Encodes the parameter value and writes it to its owning message frame.
  # 

  def write_parameter_to_frame(self,pName,pValue):
  
    index = self.get_parameter_index(pName);

    if( index > -1 ):

      _parameter = self.parameterList[index]; 
    
      frameIndex = -1;
      _counter = 0;
    
      #Search for the frame strucutre from frame list.
    
      for frame in self.frameList:
        if( frame.frame_ID == _parameter.owningFrameID ):
          frameIndex = _counter;
        _counter = _counter + 1;  
    
      _frame = self.frameList[frameIndex];
    
      _value = ( ( pValue - _parameter.offset ) / _parameter.factor );
      _bytes =  _frame.packet;
      
      #print(_bytes);
    
      #Obtain frame bytes and the data field for frame corresponding to parameter.
    
      _bytes[0] = _parameter.owningFrameID + ( 128 * _frame.priority );
      _data = ( ( _bytes[1] << 24 ) + ( _bytes[2] << 16 ) + ( _bytes[3] << 8 ) + ( _bytes[4] << 0 ) );
     
      #print(_data);      

      #Obtain parameter details.

      _sbit   = _parameter.startPosition;
      _blen   = _parameter.bitLength;
      _dtype  = _parameter.dataType;    
      _factor = _parameter.factor;
      _offset = _parameter.offset;
      _newData = 0;
    
      _mask = ( ( 1 << _blen ) - 1 );
      _data = _data & (~(_mask << _sbit));
    
      #print("Parameter name:");
      #print(_parameter.parameterName);
      #print("Start position:")
      #print(_parameter.startPosition);
      #print("Bit Length:");
      #print(_parameter.bitLength);
    
      #print(_data);
    
      # Write parameter to frame.
    
      if( _dtype == "unsigned" ):
        _newData = ( (int(_value) & ( ( 1 << _blen ) - 1 )) );
 
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
    
      _data = ( _data | ( _newData << _sbit ) );
    
      _bytes[1] = ( _data & ( 255 << 24 ) ) >> 24;
      _bytes[2] = ( _data & ( 255 << 16 ) ) >> 16;
      _bytes[3] = ( _data & ( 255 << 8 ) ) >> 8;
      _bytes[4] = ( _data & ( 255 << 0 ) ) >> 0;
      _bytes[5] = self.obtainChecksum(_bytes);
 
      _frame.packet = _bytes;
      
      self.frameList[ self.get_frame_index(_parameter.owningFrameName ) ] = _frame;      
    
      #print(_bytes);
      return(_bytes);
    
    else:
      print("Parameter not found !");
      return([0,0,0,0,0,0]);


  #
  # @brief:         Function to write parameters to a standard message frame.
  # @parameter:     Name of the parameter which has to be read. 
  # @Returns:       Value of parameter.
  # @Details:       Reads the parameter value from its owning message frame. 
  #  

  def read_parameter_from_frame(self,pName):
  
    index = -1;
    _counter = 0;

    for parameter in self.parameterList:
      if( parameter.parameterName == pName ):
        index = _counter;
      _counter = _counter + 1;


    if( index > -1 ):

      _parameter = self.parameterList[index]; 
    
      frameIndex = -1;
      _counter = 0;
    
      #Search for the frame strucutre from frame list.
    
      for frame in self.frameList:
        if( frame.frame_ID == _parameter.owningFrameID ):
          frameIndex = _counter;
        _counter = _counter + 1;  
    
      _frame = self.frameList[frameIndex];
      _bytes =  _frame.packet;
    
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
    
      _data = (_data & ( (_mask) << _sbit )) >> _sbit;
    
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
    
      self.parameterList[ self.get_parameter_index(pName) ] = _parameter;
      self.frameList[ self.get_frame_index( _parameter.owningFrameName ) ] = _frame;
    
      #print(_parameter.value);
    
    else:
      print("Parameter not found !");

    return(_parameter.value);
  
  #
  # @brief:             Function to send a SYNC frame to the device.
  # @parameters:        None.
  # @Return:            Synchronization pass/fail status.  
  # @Description:       Sends a synchronization frame to the device and waits for an acknowledge    
  #                     frame to be sent by the device.
  
  def sync(self):
    
    packet = bytearray();
    
    packet.append(0x00);
    packet.append(0x00);
    packet.append(0x00);
    packet.append(0x00);
    packet.append(0x00);
    packet.append(0x00);
  
    self.serialPort.flush();
    self.serialPort.reset_input_buffer();
    self.serialPort.reset_output_buffer();
    self.serialPort.write(packet);
  
    syncAttempts = 0;
    currentByte = 0;
    previousByte = 255;
    syncCounter = 0;
  
    while ((syncCounter < 5) and (syncAttempts < 10000)):

      currentByte = int.from_bytes(self.serialPort.read(1),"big");
      #print(currentByte);

      if ((currentByte == previousByte) and (currentByte == 0) and (previousByte == 0)):
        syncCounter = syncCounter + 1;
      else:
        syncCounter = 0;
        syncAttempts = syncAttempts + 1;

      previousByte = currentByte;

  #
  # @brief:             Open sessions a SDCP session with device a device with specified 
  #                     serial port and baud rate.
  # @parameters:        Serial port, baud rate    
  # @Return:            Status of session open request.
  # @Description:       This function opens a serial port connection at the specified COM
  #                     port with specified baud rate and then sends a SYNC frame to 
  #                     synchronization of frame reception.
  
  def open_session(self,port_name,baud_rate):
  
    self.serialPort.port = port_name;
    self.serialPort.baudrate = baud_rate;
    self.serialPort.open();
    
    self.sync();
    
    timing.delay_ms(100);

  #  
  # @brief:             Closes an existing serial session.
  # @parameters:        None 
  # @Return:            None
  # @Description:       Closes an existing serial port session only if the COM port is open.
  #

  def close_session(self):

    if( self.serialPort.is_open ):
      self.serialPort.close();

  #  
  # @brief:             Writes a parameter to a message frame and sends frame to device.
  # @parameters:        parameter name and parameter value. 
  # @Return:            None
  # @Description:       Writes a parameter to the message frame in the frame list and
  #                     transmits the same message frame to device via serial port.
  #
  
  def write_parameter_to_device(self,parameter_name,parameter_value):

    self.write_parameter_to_frame(parameter_name,parameter_value);

    # Get parameter object and the frame object corresponding to the parameter's owning frame. 

    _parameter  = self.parameterList[ self.get_parameter_index( parameter_name ) ];
    _frame      = self.frameList[ self.get_frame_index( _parameter.owningFrameName ) ];     
    _frameIndex = self.get_frame_index( _parameter.owningFrameName ); 

    if( self.serialPort.is_open ):
      self.serialPort.write( _frame.packet );

  #  
  # @brief:             Send message frame to device.
  # @parameters:        Frame name. 
  # @Return:            None
  # @Description:       Writes the packet corresponding to mentioned frame to device.
  #

  def write_frame_to_device(self,frame_name):
  
    _frame = self.frameList[ self.get_frame_index( frame_name ) ];

    self.serialPort.write( _frame.packet );

  def print_frame_packet(self,frame_name):
    
    for frame in self.frameList:
      if( frame.frame_name == frame_name ):
        print("{_fname} [{_fid}] : {_packet}".format(_fname=frame.frame_name,_fid=frame.frame_ID,_packet=frame.packet));

  #
  # @brief:             Request a frame from the device.
  # @parameters:        Name of the frame to be requested.
  # @Return:            Acknowlegmenet status for request.
  # @Description:       Sends a request for a target frame. In certain applicationns, frames
  #                     have to be communicated on a request basis.
  # Aliases:            ftor

  def request_frames(self,frame_names):

    _length = len(frame_names);
     
    if( len(frame_names) <= 3 ):                            #   If number of request frames <= 3.
      
      packet = [ 248, 0, 0, 0, 0, 0 ];
      
      if( _length >= 7 ):
        _length = 7;
      
      packet[1] = _length + ( 1 << 6 );                     #   Byte 1 of FTOR frame holds number of request frames.
                                                            #   Set request bit (BIT 6) in BYTE 1.
                                                            
      if( _length == 1 ):                                                      
        packet[2] = self.get_frame_ID(frame_names[0]);           
      
      if( _length == 2 ):                                                      
        packet[2] = self.get_frame_ID(frame_names[0]);           
        packet[3] = self.get_frame_ID(frame_names[1]);           
            
      if( _length == 3 ):                                                      
        packet[2] = self.get_frame_ID(frame_names[0]);           
        packet[3] = self.get_frame_ID(frame_names[1]);           
        packet[4] = self.get_frame_ID(frame_names[2]);           
    
      packet[5] = self.obtainChecksum(packet);

      self.sync();      #   Send synchronization frame to clear input and output buffers.
      
      self.serialPort.write(packet);    #   Send synchronization frame to device.
      
      _ack = 0;
      
      # Wait until the device sends an acknowledge frame back to host.
      
      rxPacket = [0,0,0,0,0,0];
      startTime = time.perf_counter();
      
      while( (_ack == 0) and ( ( time.perf_counter() - startTime ) < 10000 ) ):
        
        rxPacket[0] = int.from_bytes(self.serialPort.read(1),"big");
        rxPacket[1] = int.from_bytes(self.serialPort.read(1),"big");
        rxPacket[2] = int.from_bytes(self.serialPort.read(1),"big");
        rxPacket[3] = int.from_bytes(self.serialPort.read(1),"big");
        rxPacket[4] = int.from_bytes(self.serialPort.read(1),"big");
        rxPacket[5] = int.from_bytes(self.serialPort.read(1),"big");
        
        #Check if acknowledgement bit is set.
        
        if( self.verifyChecksum(rxPacket) and ( rxPacket[0] == 248 ) ):
          _ack = ( ( ( rxPacket[1] & ( 1 << 7 ) ) >> 7 ) == 1 );
          print("FTOR: ACK received from device on frame request");
      
      # If acknowledgement bit is set in the FTOR1 register, only then read the requested data frames.

      if( _ack ):    
 
        # Iterate through requested frames.   
 
        for i in range(0,_length):
            
          # Read 6 bytes from the host serial buffer corresponding to required frame ID.
            
          rxFID = -1;
            
          while( rxFID != self.get_frame_ID( frame_names[i] ) ):
            
            for k in range(0,6):        
              rxPacket[k] = int.from_bytes(self.serialPort.read(1),"big");
                
            rxFID = (rxPacket[0] & 127);  
            
            if( rxFID == self.get_frame_ID(frame_names[i]) ):
              print("Index: {_index}".format(_index = self.get_frame_index(frame_names[i]) ))
              self.frameList[ self.get_frame_index(frame_names[i]) ].packet = rxPacket;
              print("Frame packet: {_packet}".format( _packet =  self.frameList[self.get_frame_index(frame_names[i])].packet  ))
                    
            
    
