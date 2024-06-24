using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using stringUtils;
using System.IO;
using System.IO.Ports;
using System.Threading;
using System.Runtime.InteropServices;

namespace SDCP_API
{
    public struct SDCP_Parameter
    {
        public string parameter_name;
        public string frame_name;
        public int frame_ID;
        public int data_type;
        public int start_bit;
        public int bit_length;
        public float factor;
        public float offset;
        public float value;
    }

    public struct SDCP_Frame
    {
        public int frame_ID;
        public string frame_name;
    }

    public struct SDCP_Frame_Array_Record
    {
        public int frame_ID;
        public string frame_name;
        
        public UInt64 B0;              //  Most significant byte.
        public UInt64 B1;
        public UInt64 B2;
        public UInt64 B3;
        public UInt64 B4;
        public UInt64 B5;              //  Least significant byte.

        public UInt32 data;         //  32-bit integer representing the data byte.
        public UInt64 time_ms;      //  Last update time of frame.
        public int enabled;         //  Flag to check whether details of frame has been
                                    //  entered by the user or not.

        public int checksumStatus;  

        public int priority;        //  Priority of message frame.
    };

    public struct Transmit_List_Record
    {
        public SDCP_Frame_Array_Record frame;
        public uint transmitCount;
        public uint transmitInterval_ms;
        public bool cyclicModeEnable;
        public bool trigger;
        public bool checksumStatus;
    };

    public struct Receive_List_Record
    {
        public SDCP_Frame_Array_Record frame;
        public uint receiveCount;
        public double receiveInterval_ms;
        public bool checksumStatus;
        public double lastReceiveTime_ms;
    };


    public class SDCP_API
    {
        public string filePath;
        int numberOfFrames;
        int numberOfParameters;

        SerialPort _port;
        int _timeoutFlag;

        public List<SDCP_Parameter> parameterList;
        public List<SDCP_Frame> frameList;
        public List<SDCP_Frame_Array_Record> frames;
        public List<Transmit_List_Record> transmitList;

        /* Session configuration flags. */

        public static bool printParameterPollResults = true;

        public SDCP_API()
        {
            parameterList = new List<SDCP_Parameter>();
            frameList = new List<SDCP_Frame>();
            frames = new List<SDCP_Frame_Array_Record>();
            transmitList = new List<Transmit_List_Record>();

            _port = new SerialPort();
            _port.DataBits = 8;
            _port.DataBits = 8;
            _port.StopBits = StopBits.One;
            _port.Parity = Parity.None;
            _port.Handshake = Handshake.None;
            _port.ReadTimeout = 30000;
            _port.ReadTimeout = 30000;

            for (int i = 0; i < 128; i++)
            {
                SDCP_Frame_Array_Record record = new SDCP_Frame_Array_Record();

                record.frame_ID = i;
                record.frame_name = "";

                record.priority = 0;
                record.B0 = 0;
                record.B1 = 0;
                record.B2 = 0;
                record.B3 = 0;
                record.B4 = 0;
                record.B5 = 0;

                record.time_ms = 0;
                record.checksumStatus = 0;
                record.enabled = 0;

                frames.Add(record);
            }
        }

        public void loadParameterFile(string parameterFilePath)
        {
            string fileContents = File.ReadAllText(parameterFilePath);

            filePath = parameterFilePath;

            if( File.Exists(parameterFilePath) )
            {
                Console.WriteLine("\nParameter file path exists.\n");

                if (File.Exists("data.txt"))
                {

                }
                else
                {
                    File.Create("data.txt");
                }

                File.WriteAllText("data.txt", parameterFilePath);
            }
            else
            {
                Console.WriteLine("\nParameter file path does not exist\n");
                return;
            }

            //Console.WriteLine(fileContents);

            stringUtils.stringUtils stringHandler = new stringUtils.stringUtils();

            /* Calculate number of frames and number of parameters. */

            numberOfFrames = 0;

            while (stringHandler.getCharIndex(fileContents, '[', numberOfFrames + 1) != -1)
            {
                numberOfFrames++;
            }

            //Console.WriteLine($"Number of frames = {numberOfFrames}");

            numberOfParameters = 0;

            while (stringHandler.getCharIndex(fileContents, ':', numberOfParameters + 1) != -1)
            {
                numberOfParameters++;
            }

            //Console.WriteLine($"Number of parameters = {numberOfParameters}");

            /****   Obtain frame details.  ****/

            string tempString;
            string[] tempStringArr;
            int tempIndex = 0;
            int tempFrameID = 0;
            string tempFrameName = "";
            SDCP_Frame tempFrame;
            SDCP_Parameter tempParameter;

            int numberOfParameterPerFrame = 0;
            //int parameterCounter = 0;

            string tempParameterName = "";
            int tempParameterDataType = 0;
            int tempParameterStartBit = 0;
            int tempParameterBitLength = 0;
            float tempParameterFactor = 0.0f;
            float tempParameterOffset = 0.0f;

            /* Extract details for each frame. */

            for (int i = 0; i < numberOfFrames; i++)
            {
                tempFrame = new SDCP_Frame();

                tempString = fileContents.Substring(tempIndex = stringHandler.getCharIndex(fileContents, '{', i + 1));
                tempString = stringHandler.stringBetweenCharacters(tempString, '{', '}');
                tempStringArr = tempString.Split(',');
                tempFrameName = stringHandler.matchPattern_afterMatch(tempStringArr[0], "Frame_Name=");
                tempFrameID = int.Parse(stringHandler.matchPattern_afterMatch(tempStringArr[1], "Frame_ID="));
                tempFrame.frame_ID = tempFrameID;
                tempFrame.frame_name = tempFrameName;
                frameList.Add(tempFrame);

                tempString = fileContents.Substring(tempIndex = stringHandler.getCharIndex(fileContents, '{', i + 1));
                tempString = stringHandler.stringBetweenCharacters(tempString, '}', '[');

                SDCP_Frame_Array_Record frameInstance = frames[tempFrameID];

                frameInstance.enabled = 1;
                frameInstance.data = 0;
                frameInstance.frame_name = tempFrameName;
                frameInstance.priority = 0;
                frameInstance.time_ms = 0;

                frames[tempFrameID] = frameInstance;

                numberOfParameterPerFrame = 0;

                while (stringHandler.getCharIndex(tempString, ':', numberOfParameterPerFrame + 1) != -1)
                {
                    numberOfParameterPerFrame++;
                }

                /* Extract details of parameters under current frame. */

                for (int j = 0; j < numberOfParameterPerFrame; j++)
                {
                    string _temp = "";

                    if (j == 0)
                    {
                        _temp = stringHandler.stringBetweenCharacters(tempString, '\n', ':');
                    }
                    else
                    {
                        _temp = stringHandler.stringBetweenCharacters(tempString.Substring(stringHandler.getCharIndex(tempString, ')', j)), ')', ':');
                    }

                    _temp = _temp.Replace("\n\r", "");
                    _temp = _temp.Replace("\r\n", "");

                    tempParameterName = _temp;

                    /* Extract parameter specific parameter data. */

                    _temp = stringHandler.stringBetweenCharacters(tempString.Substring(stringHandler.getCharIndex(tempString, '(', j + 1)), '(', ')');

                    //Console.WriteLine(_temp);

                    tempStringArr = _temp.Split(',');

                    _temp = stringHandler.matchPattern_afterMatch(tempStringArr[0], "dataType=");

                    if (_temp == "unsigned")
                    {
                        tempParameterDataType = 0;
                    }
                    if (_temp == "signed")
                    {
                        tempParameterDataType = 1;
                    }
                    if (_temp == "float")
                    {
                        tempParameterDataType = 2;
                    }

                    _temp = stringHandler.matchPattern_afterMatch(tempStringArr[1], "startPosition=");
                    tempParameterStartBit = int.Parse(_temp);
                    _temp = stringHandler.matchPattern_afterMatch(tempStringArr[2], "length=");
                    tempParameterBitLength = int.Parse(_temp);
                    _temp = stringHandler.matchPattern_afterMatch(tempStringArr[3], "factor=");
                    tempParameterFactor = float.Parse(_temp);
                    _temp = stringHandler.matchPattern_afterMatch(tempStringArr[4], "offset=");
                    tempParameterOffset = float.Parse(_temp);

                    tempParameter = new SDCP_Parameter();

                    tempParameter.frame_name = tempFrameName;
                    tempParameter.parameter_name = tempParameterName;
                    tempParameter.data_type = tempParameterDataType;
                    tempParameter.start_bit = tempParameterStartBit;
                    tempParameter.bit_length = tempParameterBitLength;
                    tempParameter.factor = tempParameterFactor;
                    tempParameter.offset = tempParameterOffset;
                    tempParameter.frame_ID = tempFrameID;

                    parameterList.Add(tempParameter);
                }
            }

            /* Obtain parameter details. */


        }

        /* Print list of available frames. */

        public void printFrameList()
        {
            for (int i = 0; i < frameList.Count; i++)
            {
                Console.WriteLine($"Frame Name: {frameList[i].frame_name} \t Frame ID: {frameList[i].frame_ID}");
            }

            Console.WriteLine("\n");
        }

        /* Print list of available parameters */

        public void printParameterList()
        {
            for (int i = 0; i < parameterList.Count; i++)
            {
                Console.WriteLine(" ");
                Console.WriteLine($"Name of parameter: " + $"{parameterList[i].parameter_name}");
                Console.WriteLine($"Frame name: {parameterList[i].frame_name}");
                Console.WriteLine($"Frame ID: {parameterList[i].frame_ID}");
                Console.WriteLine($"Parameter data type: {parameterList[i].data_type}");
                Console.WriteLine($"Start bit: {parameterList[i].start_bit}");
                Console.WriteLine($"Bit Length: {parameterList[i].bit_length}");
                Console.WriteLine($"Factor: {parameterList[i].factor}");
                Console.WriteLine($"Offset: {parameterList[i].offset}");
                Console.WriteLine("_______________________________________________________");
            }
        }

        public void printParameterNames()
        {
            string output = "";

            for (int i = 0; i < parameterList.Count; i++)
            {
                output += $"Parameter [{i}] :\t";
                output += $"{parameterList[i].parameter_name}\n";
            }

            Console.Write("PARAMETER NAMES:\n\n");
            Console.Write(output);
            Console.Write("\n\n");
        }

        /* Function to print details about a given message frame. */

        public void printAvailableFrames()
        {
            string output = "";

            Console.WriteLine("\n\nAVAILABLE STANDARD MESSAGE FRAMES\n\n");

            for (int i = 0; i < frames.Count; i++)
            {
                if (frames[i].enabled == 1)
                {
                    output = $"\n Frame ID: {frames[i].frame_ID}";
                    output += $"\n Frame name: {frames[i].frame_name}";
                    output += $"\n Frame priority: {frames[i].priority}";
                    output += $"\n Checksum status: {frames[i].checksumStatus}";
                    output += $"\n Last update time (ms): {frames[i].time_ms}";
                    output += $"\n Raw frame bytes: \t {frames[i].B0}  {frames[i].B1}  {frames[i].B2}  {frames[i].B3}  {frames[i].B4}  {frames[i].B5}";

                    Console.WriteLine(output);
                    Console.WriteLine("________________________________________________________");
                }
            }
        }

        /* Function to print details about a given frame reffered to by frame ID. */

        public void printFrameDetails(int frame_ID)
        {
            string output = "";

            int i = frame_ID;

            Console.WriteLine("\nFRAME DETAILS");

            if (frames[frame_ID].enabled == 1)
            {
                output = $"\n Frame ID: {frames[i].frame_ID}";
                output += $"\n Frame name: {frames[i].frame_name}";
                output += $"\n Frame priority: {frames[i].priority}";
                output += $"\n Checksum status: {frames[i].checksumStatus}";
                output += $"\n Last update time (ms): {frames[i].time_ms}";
                output += $"\n Raw frame bytes: \t {frames[i].B0}  {frames[i].B1}  {frames[i].B2}  {frames[i].B3}  {frames[i].B4}  {frames[i].B5}";
                output += $"\n Data word:\t {frames[i].data}";

                Console.WriteLine(output);
                Console.WriteLine("\n");
            }
            else
            {
                Console.WriteLine("\nFrame not found.\n");
            }

        }


        /* Function to print details about a given frame referred to by frame name. */

        public void printFrameDetails(string frameName)
        {
            string output = "";

            int frame_ID = -1;

            int i = frame_ID;

            for (int k = 0; k < frames.Count; k++)
            {
                if (frames[k].frame_name == frameName)
                {
                    frame_ID = frames[k].frame_ID;
                    break;
                }
            }

            i = frame_ID;

            if ((frames[frame_ID].enabled == 1) && (frame_ID != -1))
            {
                Console.WriteLine("\nFRAME DETAILS");

                output = $"\n Frame ID: {frames[i].frame_ID}";
                output += $"\n Frame name: {frames[i].frame_name}";
                output += $"\n Frame priority: {frames[i].priority}";
                output += $"\n Checksum status: {frames[i].checksumStatus}";
                output += $"\n Last update time (ms): {frames[i].time_ms}";
                output += $"\n Raw frame bytes: \t {frames[i].B0}  {frames[i].B1}  {frames[i].B2}  {frames[i].B3}  {frames[i].B4}  {frames[i].B5}";
                output += $"\n Data word: \t {frames[i].data}";

                Console.WriteLine(output);
                Console.WriteLine("\n");
            }
            else
            {
                Console.WriteLine("\nFrame not found.\n");
            }

        }


        /* Function to print details about a parameter loaded from the parameter file (*.PARAM) . */

        public void printParameterDetails(int parameter_index)
        {
            string output = "";

            try
            {
                output += $"\nParameter name: \t {parameterList[parameter_index].parameter_name}";
                output += $"\nFrame name: \t {parameterList[parameter_index].frame_name}";
                output += $"\nFrame ID: \t {parameterList[parameter_index].frame_ID}";
                output += $"\nParameter data type: \t {parameterList[parameter_index].data_type}";
                output += $"\nParameter start bit: \t {parameterList[parameter_index].start_bit}";
                output += $"\nParameter bit length: \t {parameterList[parameter_index].bit_length}";
                output += $"\nParameter factor: \t {parameterList[parameter_index].factor}";
                output += $"\nParameter offset: \t {parameterList[parameter_index].offset}";
                output += $"\nParameter value: \t {parameterList[parameter_index].value}";
                output += "\n";

                Console.WriteLine(output);
            }
            catch (Exception e)
            {
                Console.WriteLine("Parameter not found");
            }
        }

        /*Function to print details about a given parameter.*/

        public void printParameterDetails(string parameterName)
        {
            int parameter_index = -1;

            for (int i = 0; i < parameterList.Count; i++)
            {
                if (parameterList[i].parameter_name == parameterName)
                {
                    parameter_index = i;
                }
            }

            if (parameter_index == -1)
            {
                Console.WriteLine("Paramenter not found\n");
                return;
            }

            printParameterDetails(parameter_index);
        }

        /* Function to write a parameter to a message frame. */

        public void writeParameterToFrame(float value, string parameterName)
        {
            int parameter_ID = -1;

            for (int i = 0; i < parameterList.Count; i++)
            {
                if (parameterList[i].parameter_name == parameterName)
                {
                    parameter_ID = i;
                    break;
                }
            }

            if (parameter_ID == -1)
            {
                Console.WriteLine("Parameter not found.");
                return;
            }

            /* Get structure of the required parameter. */

            SDCP_Parameter parameterInstance = parameterList[parameter_ID];
            SDCP_Frame_Array_Record frameInstance = frames[parameterList[parameter_ID].frame_ID];

            /* Extract data bytes. */

            UInt64 data = 0;

            //frameInstance.B0 = (UInt64)parameterInstance.frame_ID;

            data = (frameInstance.B1 << 24)
                 | (frameInstance.B2 << 16)
                 | (frameInstance.B3 << 8)
                 | (frameInstance.B4 << 0);

            UInt64 mask = ((UInt64)1 << parameterInstance.bit_length) - 1;

            data &= ~(mask << parameterInstance.start_bit);

            float processedValue = (value - parameterInstance.offset) / parameterInstance.factor;

            /* For unsigned data type. */

            UInt64 newData = 0;

            if (parameterInstance.data_type == 0)
            {
                newData = (UInt64)processedValue & mask;
            }
            else if (parameterInstance.data_type == 1)
            {
                Console.WriteLine(">>>>>");

                if (processedValue < 0.0f)
                {
                    newData = (((UInt64)1 << parameterInstance.bit_length) - ((UInt64)Math.Abs(processedValue) & mask));
                }
                else
                {
                    newData = (UInt64)processedValue & mask;
                }
            }
            else if (parameterInstance.data_type == 2)
            {
                byte[] tempArr = BitConverter.GetBytes(value);

                newData = (((UInt32)tempArr[0] << 24) |
                          ((UInt32)tempArr[1] << 16) |
                          ((UInt32)tempArr[2] << 8) |
                          ((UInt32)tempArr[3] << 0));
            }

            data |= (newData << parameterInstance.start_bit);

            frameInstance.B0 = (UInt64)parameterInstance.frame_ID + 128 * (UInt64)frameInstance.priority;
            frameInstance.B1 = ((data >> 24) & 255);
            frameInstance.B2 = ((data >> 16) & 255);
            frameInstance.B3 = ((data >> 8) & 255);
            frameInstance.B4 = ((data >> 0) & 255);

            UInt64 checksum = 0;

            checksum = 255 - ((frameInstance.B0 + frameInstance.B1 + frameInstance.B2 + frameInstance.B3 + frameInstance.B4) % 255);

            frameInstance.B5 = checksum;

            frames[parameterInstance.frame_ID] = frameInstance;
        }

        /* Function to read a parameter from the received message frames. */

        public float readParameterFromFrame(string parameterName)
        {
            float value = 0.0f;
            int parameter_ID = -1;

            for (int i = 0; i < parameterList.Count; i++)
            {
                if (parameterList[i].parameter_name == parameterName)
                {
                    parameter_ID = i;
                    break;
                }
            }

            if (parameter_ID == -1)
            {
                Console.WriteLine("Parameter not found.");
                return (value);
            }

            SDCP_Parameter parameterInstance = new SDCP_Parameter();
            SDCP_Frame_Array_Record frameInstance = new SDCP_Frame_Array_Record();

            parameterInstance = parameterList[parameter_ID];
            frameInstance = frames[parameterInstance.frame_ID];

            UInt64 data = ((UInt64)frameInstance.B1 << 24)
                        | ((UInt64)frameInstance.B2 << 16)
                        | ((UInt64)frameInstance.B3 << 8)
                        | ((UInt64)frameInstance.B4 << 0);

            UInt64 mask = ((UInt64)1 << parameterInstance.bit_length) - 1;

            data = ((data >> parameterInstance.start_bit) & mask);

            if (parameterInstance.data_type == 0)
            {
                value = (parameterInstance.factor * (float)data + parameterInstance.offset);
            }
            else if (parameterInstance.data_type == 1)
            {
                UInt64 maxAbsValue = ((UInt64)1 << (parameterInstance.bit_length - 1)) - 1;

                //Console.WriteLine($"Maximum absolute value: {maxAbsValue}");

                if (data > maxAbsValue)
                {
                    //Console.WriteLine("Greater than maximum absolute value.");
                }

                if (data > maxAbsValue)
                {
                    value = -(parameterInstance.factor * (float)((((UInt64)1 << parameterInstance.bit_length) - data) + parameterInstance.offset));
                }
                else
                {
                    value = (parameterInstance.factor * (float)data + parameterInstance.offset);
                }
            }
            else if (parameterInstance.data_type == 2)
            {
                value = parameterInstance.factor * BitConverter.UInt32BitsToSingle((UInt32)data) + parameterInstance.offset;
            }

            parameterInstance.value = value;

            parameterList[parameter_ID] = parameterInstance;

            return (value);
        }

        /******     SERIAL PORT HANDLING FUNCTIONS.  **********/

        /* Function to list all available serial ports. */

        public void listAvailablePorts()
        {
            string[] availableSerialPorts = SerialPort.GetPortNames();

            Console.WriteLine("\nAvailable serial ports:\n");

            for (int i = 0; i < availableSerialPorts.Length; i++)
            {
                Console.WriteLine(availableSerialPorts[i]);
            }

            Console.WriteLine("");
        }


        /* Function to get parameter name. */

        public string getParameterName(int pid)
        {
            try
            {
                return (parameterList[pid].parameter_name);
            }
            catch
            {
                return ("");
            }
        }

        /* Function to get parameter index. */

        public int getParameterIndex(string parameter_name)
        {
            int index = -1;

            for(int i = 0; i < parameterList.Count ; i++)
            {
                if (parameterList[i].parameter_name == parameter_name)
                {
                    index = i;
                }
            }

            return (index);
        }

        public int getParameterFID(string parameter_name)
        {
            int fid = 0;

            for (int i = 0; i < parameterList.Count; i++)
            {
                if (parameterList[i].parameter_name == parameter_name)
                {
                    fid = parameterList[i].frame_ID;
                    break;
                }
            }

            return (fid);
        }


        /* Function to open the serial port and auto-detect a COM Port with valid SDCP session. */
        public void openSerialPort()
        {
            string[] availableSerialPorts = SerialPort.GetPortNames();
            int[] baudRate = new int[10];

            baudRate[0] = 1200;
            baudRate[1] = 2400;
            baudRate[2] = 4800;
            baudRate[3] = 9600;
            baudRate[4] = 19200;
            baudRate[5] = 38400;
            baudRate[6] = 76800;
            baudRate[7] = 115200;
            baudRate[8] = 230400;
            baudRate[9] = 460800;

            /* Loop through available serial ports. */

            bool serialPortAvailable = false;

            List<string> validSerialPorts = new List< string > ();

            Console.WriteLine("\n\nSearching for available serial ports.....\n");

            for (int i = 0; i < availableSerialPorts.Length; i++)
            {
                if( _port.IsOpen )
                {
                    _port.Close();
                }

                _port.PortName = availableSerialPorts[i];
                _port.BaudRate = 9600;

                try
                {
                    _port.Open();

                    if(_port.IsOpen)
                    {
                        Console.WriteLine($"{availableSerialPorts[i]} available.");
                        serialPortAvailable = true;
                        validSerialPorts.Add(availableSerialPorts[i]);
                        //break;
                    }
                }
                catch(Exception e)
                {
                    Console.WriteLine($"{availableSerialPorts[i]} not available.");
                    serialPortAvailable = false;
                    //break;
                }
            }


            if(serialPortAvailable == false)
            {
                Console.WriteLine("\nNo serial ports available. Exiting search.");
                return;
            }

            /* If serial port is available iterate through available baud rates
               and try to sync with the device. If no sync is received, exit. */

            Console.WriteLine("\n\nAttempting to synchronize with device at standard baud rates .....\n");

            bool syncSuccessful = false;

            for (int k = 0; k < validSerialPorts.Count; k++)
            {
                Console.WriteLine($"\nAttemping to start SDCP sesstion at serial port:{validSerialPorts[k]}\n");

                for (int i = 0; i < baudRate.Length; i++)
                {

                    if (_port.IsOpen)
                    {
                        _port.Close();
                    }

                    _port.BaudRate = baudRate[i];
                    _port.PortName = validSerialPorts[k];

                    _port.WriteTimeout = 1000;
                    _port.ReadTimeout = 1000;
                    _port.Open();

                    if (sync(2000) == 0)
                    {
                        Console.WriteLine($"Timeout with baud rate of {_port.BaudRate} at serial port {_port.PortName}");
                    }
                    else
                    {
                        Console.WriteLine($"\nOpened SDCP session with baud rate of {_port.BaudRate} at serial port {_port.PortName}\n\n");
                        syncSuccessful = true;
                        break;
                        //return;
                    }
                }

                if(syncSuccessful)
                {
                    break;
                }
            }

            if (syncSuccessful == false)
            {
                Console.WriteLine($"\nNot able start SDCP session at any serial port.");
            }
        }


        public void openSerialPort(double timeout_ms)
        {
            string[] availableSerialPorts = SerialPort.GetPortNames();
            int[] baudRate = new int[10];

            baudRate[0] = 1200;
            baudRate[1] = 2400;
            baudRate[2] = 4800;
            baudRate[3] = 9600;
            baudRate[4] = 19200;
            baudRate[5] = 38400;
            baudRate[6] = 76800;
            baudRate[7] = 115200;
            baudRate[8] = 230400;
            baudRate[9] = 460800;

            /* Loop through available serial ports. */

            bool serialPortAvailable = false;

            List<string> validSerialPorts = new List<string>(); 

            Console.WriteLine("\n\nSearching for available serial ports.....\n");

            for (int i = 0; i < availableSerialPorts.Length; i++)
            {
                if(_port.IsOpen)
                {
                    _port.Close();
                }

                _port.PortName = availableSerialPorts[i];
                _port.BaudRate = 9600;

                try
                {
                    _port.Open();

                    if (_port.IsOpen)
                    {
                        Console.WriteLine($"{availableSerialPorts[i]} available.");
                        serialPortAvailable = true;
                        validSerialPorts.Add(availableSerialPorts[i]);
                        //break;
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine($"{availableSerialPorts[i]} not available.");
                    serialPortAvailable = false;
                    //break;
                }
            }


            if (serialPortAvailable == false)
            {
                Console.WriteLine("\nNo serial ports available. Exiting search.");
                return;
            }

            /* If serial port is available iterate through available baud rates
               and try to sync with the device. If no sync is received, exit. */

            Console.WriteLine("\n\nAttempting to synchronize with device at standard baud rates .....\n");

            bool syncSuccessful = false;

            for (int k = 0; k < validSerialPorts.Count; k++)
            {
                Console.WriteLine($"\nAttemping to start SDCP sesstion at serial port:{validSerialPorts[k]}\n");

                for (int i = 0; i < baudRate.Length; i++)
                {
                    if (_port.IsOpen)
                    {
                        _port.Close();
                    }

                    _port.PortName = validSerialPorts[k];
                    _port.BaudRate = baudRate[i];

                    _port.WriteTimeout = 1000;
                    _port.ReadTimeout = 1000;
                    _port.Open();

                    if (sync(timeout_ms) == 0)
                    {
                        Console.WriteLine($"Timeout with baud rate of {_port.BaudRate} at serial port {_port.PortName}");
                    }
                    else
                    {
                        Console.WriteLine($"\nOpened SDCP session with baud rate of {_port.BaudRate} at serial port {_port.PortName} \n\n");
                        syncSuccessful = true;
                        break;
                        //return;
                    }
                }

                if (syncSuccessful == false)
                {
                    Console.WriteLine($"\nNot able to start a SDCP session at serial port {_port.PortName}.");
                    //break;
                }
                else
                {
                    break;
                }
            }
        }


        /* Function to open the serial port. */

        public void openSerialPort(string portName, int baudRate)
        {
            int serialPortAvailable = 0;

            /* Check if serial port is available. */

            string[] availableSerialPorts = SerialPort.GetPortNames();

            for (int i = 0; i < availableSerialPorts.Length; i++)
            {
                if (availableSerialPorts[i] == portName)
                {
                    serialPortAvailable = 1;
                }
            }

            if (serialPortAvailable == 0)
            {
                Console.WriteLine($"{portName} not found.");
                return;
            }
            else
            {
                Console.WriteLine($"{portName} found.");
            }

            /***** Establish connection. *****/


            if (_port.IsOpen)
            {
                Console.WriteLine($"{portName} already open.");
                _port.Close();
            }

            _port.PortName = portName;
            _port.BaudRate = baudRate;
            //_port.ReadBufferSize = 4096;

            try
            {
                _port.Open();
            }
            catch (Exception e)
            {
                Console.WriteLine($"{e.Message}");
            }

            if (_port.IsOpen)
            {
                Console.WriteLine($"Opened serial connection @ {portName} with baud rate of {baudRate}");
            }
            else
            {
                Console.WriteLine($"Unable to open serial connection at serial port: {portName}");
            }
        }

        /* Function to close existing serial port. */

        public void closeSerialPort()
        {
            if (_port.IsOpen)
            {
                Console.WriteLine($"{_port.PortName} closed.");
            }
            else
            {
                Console.WriteLine($"{_port.PortName} already closed.");
            }
        }


        public int sync(double timeout_ms)
        {
            if (_port.IsOpen)
            {
                byte[] readBuffer = new byte[1];
                byte[] writeBuffer = new byte[6];

                writeBuffer[0] = 0;
                writeBuffer[1] = 0;
                writeBuffer[2] = 0;
                writeBuffer[3] = 0;
                writeBuffer[4] = 0;
                writeBuffer[5] = 0;


                _port.DiscardInBuffer();
                _port.DiscardOutBuffer();
                _port.Write(writeBuffer, 0, 6);

                byte currentByte = 0;
                byte previousByte = 255;
                int syncCounter = 0;
                int syncAttemps = 0;

                DateTime startTime = DateTime.Now;
                double elapsedTime = 0;

                //_port.WriteTimeout = 1000;
                _port.ReadTimeout = (int)timeout_ms;

                while ((syncCounter < 5) && ((elapsedTime = (DateTime.Now - startTime).TotalMilliseconds) < timeout_ms))
                {
                    try
                    {
                        currentByte = (byte)_port.ReadByte();

                        if ((currentByte == previousByte) && (currentByte == 0) && (previousByte == 0))
                        {
                            syncCounter++;
                        }
                        else
                        {
                            syncCounter = 0;
                            syncAttemps++;
                        }

                        previousByte = currentByte;
                    }
                    catch (Exception e)
                    {
                        //Console.WriteLine($"Timeout with baud rate of {_port.BaudRate} at serial port {_port.PortName}");
                        return (0);
                    }
                }

                //Console.WriteLine($"Synchronization attempts: {syncAttemps}");

                if (elapsedTime < timeout_ms)
                {
                    Console.WriteLine($"\nSynchronized successfully in {elapsedTime} ms.\n");
                    return (1);
                }
                else
                {
                    Console.WriteLine("\nSynchronization failed.\n");
                    return (0);
                }
            }
            else
            {
                Console.WriteLine("\nNo serial port opened !\n");
                return (0);
            }
            //return (0);
        }


        public int sync()
        {
            if (_port.IsOpen)
            {

                byte[] readBuffer = new byte[1];
                byte[] writeBuffer = new byte[6];

                writeBuffer[0] = 0;
                writeBuffer[1] = 0;
                writeBuffer[2] = 0;
                writeBuffer[3] = 0;
                writeBuffer[4] = 0;
                writeBuffer[5] = 0;


                _port.DiscardInBuffer();
                _port.DiscardOutBuffer();
                _port.Write(writeBuffer, 0, 6);

                byte currentByte = 0;
                byte previousByte = 255;
                int syncCounter = 0;
                int syncAttemps = 0;

                while ((syncCounter < 5) && (syncAttemps < 10000))
                {
                    currentByte = (byte)_port.ReadByte();

                    if ((currentByte == previousByte) && (currentByte == 0) && (previousByte == 0))
                    {
                        syncCounter++;
                    }
                    else
                    {
                        syncCounter = 0;
                        syncAttemps++;
                    }

                    previousByte = currentByte;
                }

                //Console.WriteLine($"Synchronization attempts: {syncAttemps}");

                if (syncAttemps < 10000)
                {
                    Console.WriteLine("Synchronized successfully.\n");
                    return (1);
                }
                else
                {
                    Console.WriteLine("Synchronization failed.\n");
                    return (0);
                }

            }
            else
            {
                Console.WriteLine("\nNo serial port opened.\n");
            }

            return (0);
        }


        /*********************  FUNCTIONS TO READ FRAMES FROM SERIAL PORT.    ******************/

        /* Poll for a given standard message frame / data frame until the message frame is 
           received or a timeout event occurs. */

        public int[] pollFrame(int frame_ID, UInt32 timeout_ms)
        {
            int[] frame = new int[6];

            DateTime startTime = DateTime.Now;

            _timeoutFlag = 0;

            /* Wait until the given standard message frame with required frame ID is 
               received or timeout occurs. */

            double elapsedTime = 0;

            while ( ( elapsedTime = ( DateTime.Now - startTime ).TotalMilliseconds ) < timeout_ms )
            {
                for (int i = 0; i < 6; i++)
                {
                    frame[i] = _port.ReadByte();
                }

                if ((frame[0] & 127) == frame_ID)
                {
                    break;
                }
            }

            /* If a timeout event has occured, return a empty frame. */

            if( elapsedTime >= timeout_ms )
            {
                return (frame);
            }

            /* Obtain current frame ID. */

            int currentFrameID = (frame[0] & 127);

            SDCP_Frame_Array_Record frameInstance = new SDCP_Frame_Array_Record();

            /* Select record corresponding to the received frame ID. */

            frameInstance = frames[currentFrameID];

            frameInstance.B0 = (UInt64)frame[0];
            frameInstance.B1 = (UInt64)frame[1];
            frameInstance.B2 = (UInt64)frame[2];
            frameInstance.B3 = (UInt64)frame[3];
            frameInstance.B4 = (UInt64)frame[4];
            frameInstance.B5 = (UInt64)frame[5];

            /* Verify checksum for received message frame. */

            bool checksumStatus = ((frame.Sum() % 255) == 0);

            /* Update checksum status and decode only if checksum is true. */

            if (checksumStatus == true)
            {
                frameInstance.checksumStatus = 1;
                frameInstance.data = (uint)((frame[1] << 24) | (frame[2] << 16) | (frame[3] << 8) | frame[4]);
                frames[frame_ID] = frameInstance;
            }
            else
            {
                frameInstance.checksumStatus = 0;
            }

            return (frame);
        }


        /* Poll for a given standard message frame / data frame until the message frame is 
           received or a timeout event occurs. */

        public int[] pollFrame(string frame_name, UInt32 timeout_ms)
        {
            int frame_ID = -1;
            int[] frameData = new int[6];

            for (int i = 0; i < frames.Count; i++)
            {
                if (frames[i].frame_name == frame_name)
                {
                    frame_ID = frames[i].frame_ID;
                    break;
                }
            }

            if (frame_ID == -1)
            {
                Console.WriteLine($"Frame with name {frame_name} not found.");
                return (frameData);
            }

            frameData = pollFrame(frame_ID, timeout_ms);

            return (frameData);
        }


        /* Function to poll for a given parameter and stop polling if the required parameter is
           received or when a timeout event occurs. */

        public float pollParameter(int parameter_index, UInt32 timeout_ms)
        {
            SDCP_Parameter parameterInstance = new SDCP_Parameter();
            parameterInstance = parameterList[parameter_index];

            int[] frameData = new int[6];

            frameData = pollFrame(parameterInstance.frame_ID, timeout_ms);

            float value = readParameterFromFrame(parameterInstance.parameter_name);

            //if (printParameterPollResults)
            //{
            //    Console.WriteLine($"Parameter value: {value}");
            //}

            return (value);
        }


        /* Function to poll for a certain value for a given parameter. */

        public double pollParameterForValue(int parameter_index, float value, UInt32 timeout_ms)
        {
            SDCP_Parameter parameterInstance = parameterList[parameter_index];

            float receivedValue = value + 0.1f;

            DateTime startTime = DateTime.Now;

            double elapsedTime = 0;

            while( ( ( elapsedTime = (DateTime.Now - startTime).TotalMilliseconds ) < timeout_ms )
                   && ( receivedValue != value ) )
            {
                receivedValue = readParameterFromFrame(parameterInstance.parameter_name);
            }

            return (elapsedTime);
        }

        public double pollParameterForValue(string parameter_name, float value, UInt32 timeout_ms)
        {
            int parameterIndex = -1;

            for( int i = 0; i < parameterList.Count; i++ )
            {
                if (parameterList[i].parameter_name == parameter_name)
                {
                    parameterIndex = i;
                    break;
                }
            }

            float receivedValue = value + 0.1f;
            double elapsedTime = 0;

            if (parameterIndex != -1)
            {
                SDCP_Parameter parameterInstance = parameterList[parameterIndex];

                DateTime startTime = DateTime.Now;

                while (((elapsedTime = (DateTime.Now - startTime).TotalMilliseconds) < timeout_ms)
                       && (receivedValue != value))
                {
                    receivedValue = readParameterFromFrame(parameterInstance.parameter_name);
                }
            }
            else
            {
                receivedValue = value;
            }

            return (elapsedTime);
        }

        /* Function to poll for a given parameter and stop polling if the required parameter is
           received or when a timeout event occurs. */

        public float pollParameter(string parameter_name, UInt32 timeout_ms)
        {
            int parameter_index = -1;

            float value = 0.0f;

            for (int i = 0; i < parameterList.Count; i++)
            {
                if (parameterList[i].parameter_name == parameter_name)
                {
                    parameter_index = i;
                }
            }

            if (parameter_index == -1)
            {
                Console.WriteLine("Paramenter name not found in parameter list.");
                return (0);
            }

            value = pollParameter(parameter_index, timeout_ms);

            return (value);
        }

        /* Function to generate delay for a required time. */

        public void delay(UInt32 delay_ms)
        {

            DateTime startTime = DateTime.Now;
            double elapsedTime = 0;
            while (( elapsedTime = (DateTime.Now - startTime).TotalMilliseconds) < delay_ms) ;
            //Console.WriteLine($"{elapsedTime}");
        }

        public static int liveStreamTimerElapsed = 0;


        /* Function to livestream a given parameter updated at specified interval and
           stop live streaming at specified time. */

        public void liveStreamParameter(string ParameterName, UInt32 update_interval_ms, UInt32 stop_time_ms)
        {
            DateTime startTime = DateTime.Now;
            DateTime displayUpdateTime = DateTime.Now;
            float value = 0;
            bool previousPrintConfig = printParameterPollResults;
            printParameterPollResults = false;
            string output = "";
            float updatePercentage = 0;
            int updateCount = 0;

            //_port.DiscardInBuffer();
            sync();

            Console.WriteLine("");

            while( (DateTime.Now - startTime).TotalMilliseconds < stop_time_ms )
            {
                value = pollParameter(ParameterName,update_interval_ms);

                if ((DateTime.Now - displayUpdateTime).TotalMilliseconds >= update_interval_ms)
                {
                    Console.SetCursorPosition(0, Console.CursorTop - 1);
                    Console.WriteLine("");
                    Console.SetCursorPosition(0, Console.CursorTop - 1);

                    updatePercentage = (float)(DateTime.Now - startTime).TotalMilliseconds / (float)stop_time_ms;
                    updateCount = (int)Math.Ceiling(20 * updatePercentage);

                    output = $"Parameter name: {ParameterName}";
                    output += $"    Value: {value}  ";
                    output += $"  Time: { (DateTime.Now - startTime).TotalMilliseconds} (ms)";
                    output += "\t  [";

                    for(int i = 0; i < 20; i++)
                    {
                        if(i<updateCount)
                        {
                            output += "#";
                        }
                        else
                        {
                            output += " ";
                        }
                    }

                    output += "]";

                    Console.WriteLine(output);
                    displayUpdateTime = DateTime.Now;  
                }                
            }

            printParameterPollResults = previousPrintConfig;
        }

        /* Function to transfer data from a frame upon request by client. */
        public void ftor(int[] frame_IDs)
        {
            int[] txBuffer = new int[6];
            byte[] __temp = new byte[42];

            /* First, write the FTOR2 register which contains the request FIDs 3-7. */

            if (frame_IDs.Length == 4)
            {
                txBuffer[0] = 249;
                txBuffer[1] = frame_IDs[3];
                txBuffer[2] = 0;
                txBuffer[3] = 0;
                txBuffer[4] = 0;
                txBuffer[5] = generate_checksum(txBuffer);
            }
            else if (frame_IDs.Length == 5)
            {
                txBuffer[0] = 249;
                txBuffer[1] = frame_IDs[3];
                txBuffer[2] = frame_IDs[4];
                txBuffer[3] = 0;
                txBuffer[4] = 0;
                txBuffer[5] = generate_checksum(txBuffer);
            }
            else if (frame_IDs.Length == 6)
            {
                txBuffer[0] = 249;
                txBuffer[1] = frame_IDs[3];
                txBuffer[2] = frame_IDs[4];
                txBuffer[3] = frame_IDs[5];
                txBuffer[4] = 0;
                txBuffer[5] = generate_checksum(txBuffer);
            }
            else if (frame_IDs.Length == 7)
            {
                txBuffer[0] = 249;
                txBuffer[1] = frame_IDs[3];
                txBuffer[2] = frame_IDs[4];
                txBuffer[3] = frame_IDs[5];
                txBuffer[4] = frame_IDs[6];
                txBuffer[5] = generate_checksum(txBuffer);
            }


            if(frame_IDs.Length>3)
            {
                __temp[0] = (byte)txBuffer[0];
                __temp[1] = (byte)txBuffer[1];
                __temp[2] = (byte)txBuffer[2];
                __temp[3] = (byte)txBuffer[3];
                __temp[4] = (byte)txBuffer[4];
                __temp[5] = (byte)txBuffer[5];   

                _port.Write(__temp, 0,6);

                Console.WriteLine($"Sending FTOR2 frame: [ {__temp[0]},{__temp[1]},{__temp[2]},{__temp[3]},{__temp[4]},{__temp[5]} ]");
            }

            /* Form the frame for FTOR1 register and load number of request frames
             * and set request bit in FTOR1 register. */

            if (frame_IDs.Length == 1)
            {
                txBuffer[0] = 248;
                txBuffer[1] = (1 << 6) | (frame_IDs.Length & 7);
                txBuffer[2] = frame_IDs[0];
                txBuffer[3] = 0;
                txBuffer[4] = 0;
                txBuffer[5] = generate_checksum(txBuffer);
            }
            else if (frame_IDs.Length == 2)
            {
                txBuffer[0] = 248;
                txBuffer[1] = (1 << 6) | (frame_IDs.Length & 7);
                txBuffer[2] = frame_IDs[0];
                txBuffer[3] = frame_IDs[1];
                txBuffer[4] = 0;
                txBuffer[5] = generate_checksum(txBuffer);
            }
            else if (frame_IDs.Length >= 3)
            {
                txBuffer[0] = 248;
                txBuffer[1] = (1 << 6) | (frame_IDs.Length & 7);
                txBuffer[2] = frame_IDs[0];
                txBuffer[3] = frame_IDs[1];
                txBuffer[4] = frame_IDs[2];
                txBuffer[5] = generate_checksum(txBuffer);
            }

            __temp[0] = (byte)txBuffer[0];
            __temp[1] = (byte)txBuffer[1];
            __temp[2] = (byte)txBuffer[2];
            __temp[3] = (byte)txBuffer[3];
            __temp[4] = (byte)txBuffer[4];
            __temp[5] = (byte)txBuffer[5];


            Console.WriteLine($"Sending FTOR1 frame: [ {__temp[0]},{__temp[1]},{__temp[2]},{__temp[3]},{__temp[4]},{__temp[5]} ]");

            /* Once FTOR1 has been sent by the client, the receiver must respond
               by sending the same FTOR1 frame with the ACK bit set. */

            int ackFlag = 0;

            DateTime startTime = DateTime.Now;
            double elapsedTime = 0;

            sync();

            _port.Write(__temp, 0, 6);

            do
            {
                for(int i = 0; i < 6; i++)
                {
                    __temp[i] = (byte)_port.ReadByte();
                }

                elapsedTime = (DateTime.Now - startTime).TotalMilliseconds;

                //Console.WriteLine($"Elapsed time (ms): {elapsedTime}");

                if(__temp[0]!=248)
                {
                    ackFlag = 0; 
                }
                else if(__temp[0] == 248)
                {
                    ackFlag = ((__temp[1] & (1 << 7)) >> 7);
                    Console.WriteLine($"Received FTOR1 frame: [ {__temp[0]},{__temp[1]},{__temp[2]},{__temp[3]},{__temp[4]},{__temp[5]} ]");
                }

                //Console.WriteLine(ackFlag);

            } while ((ackFlag == 0) && (elapsedTime <10000));


            if ((elapsedTime<10000) && (ackFlag==1))
            {
                Console.WriteLine("\nFTOR ACK received");
            }
            else
            {
                Console.WriteLine("\nFTOR ACK not received.");
                Console.WriteLine($"Elapsed time (ms): {elapsedTime}");
            }


            if (ackFlag == 1)
            {
                for(int i = 0; i < 6*frame_IDs.Length; i++)
                {
                    __temp[i] = (byte)_port.ReadByte();
                }
            }

            Console.WriteLine("\nReceived frames: \n");

            for (int i = 0; i < frame_IDs.Length; i++)
            {
                Console.WriteLine($"Frame {i}: [{__temp[6 * i + 0]},{__temp[6 * i + 1]},{__temp[6 * i + 2]},{__temp[6 * i + 3]},{__temp[6 * i + 4]},{__temp[6 * i + 5]}]");
            }

            Console.WriteLine("\n");
        }

        public void ftor(string[] frame_names,UInt32 timeout_ms)
        {
            int count = 0;

            for(int i = 0; i < frames.Count; i++)
            {
                for(int j = 0; j < frame_names.Length;j++)
                {
                    if (frames[i].frame_name == frame_names[j])
                    {
                        count++;
                    }
                }
            }

            int[] FIDs = new int[count];

            count = 0;

            for (int i = 0; i < frames.Count; i++)
            {
                for (int j = 0; j < frame_names.Length; j++)
                {
                    if (frames[i].frame_name == frame_names[j])
                    {
                        FIDs[count] = frames[i].frame_ID;
                        count++;
                    }
                }
            }

            ftor(FIDs,timeout_ms);
        }

        public void ftor(int[] frame_IDs,UInt32 timeout_ms)
        {
            int[] txBuffer = new int[6];
            byte[] __temp = new byte[42];

            /* First, write the FTOR2 register which contains the request FIDs 3-7. */

            if (frame_IDs.Length == 4)
            {
                txBuffer[0] = 249;
                txBuffer[1] = frame_IDs[3];
                txBuffer[2] = 0;
                txBuffer[3] = 0;
                txBuffer[4] = 0;
                txBuffer[5] = generate_checksum(txBuffer);
            }
            else if (frame_IDs.Length == 5)
            {
                txBuffer[0] = 249;
                txBuffer[1] = frame_IDs[3];
                txBuffer[2] = frame_IDs[4];
                txBuffer[3] = 0;
                txBuffer[4] = 0;
                txBuffer[5] = generate_checksum(txBuffer);
            }
            else if (frame_IDs.Length == 6)
            {
                txBuffer[0] = 249;
                txBuffer[1] = frame_IDs[3];
                txBuffer[2] = frame_IDs[4];
                txBuffer[3] = frame_IDs[5];
                txBuffer[4] = 0;
                txBuffer[5] = generate_checksum(txBuffer);
            }
            else if (frame_IDs.Length == 7)
            {
                txBuffer[0] = 249;
                txBuffer[1] = frame_IDs[3];
                txBuffer[2] = frame_IDs[4];
                txBuffer[3] = frame_IDs[5];
                txBuffer[4] = frame_IDs[6];
                txBuffer[5] = generate_checksum(txBuffer);
            }


            if (frame_IDs.Length > 3)
            {
                __temp[0] = (byte)txBuffer[0];
                __temp[1] = (byte)txBuffer[1];
                __temp[2] = (byte)txBuffer[2];
                __temp[3] = (byte)txBuffer[3];
                __temp[4] = (byte)txBuffer[4];
                __temp[5] = (byte)txBuffer[5];

                _port.Write(__temp, 0, 6);

                Console.WriteLine($"Sending FTOR2 frame: [ {__temp[0]},{__temp[1]},{__temp[2]},{__temp[3]},{__temp[4]},{__temp[5]} ]");
            }

            /* Form the frame for FTOR1 register and load number of request frames
             * and set request bit in FTOR1 register. */

            if (frame_IDs.Length == 1)
            {
                txBuffer[0] = 248;
                txBuffer[1] = (1 << 6) | (frame_IDs.Length & 7);
                txBuffer[2] = frame_IDs[0];
                txBuffer[3] = 0;
                txBuffer[4] = 0;
                txBuffer[5] = generate_checksum(txBuffer);
            }
            else if (frame_IDs.Length == 2)
            {
                txBuffer[0] = 248;
                txBuffer[1] = (1 << 6) | (frame_IDs.Length & 7);
                txBuffer[2] = frame_IDs[0];
                txBuffer[3] = frame_IDs[1];
                txBuffer[4] = 0;
                txBuffer[5] = generate_checksum(txBuffer);
            }
            else if (frame_IDs.Length >= 3)
            {
                txBuffer[0] = 248;
                txBuffer[1] = (1 << 6) | (frame_IDs.Length & 7);
                txBuffer[2] = frame_IDs[0];
                txBuffer[3] = frame_IDs[1];
                txBuffer[4] = frame_IDs[2];
                txBuffer[5] = generate_checksum(txBuffer);
            }

            __temp[0] = (byte)txBuffer[0];
            __temp[1] = (byte)txBuffer[1];
            __temp[2] = (byte)txBuffer[2];
            __temp[3] = (byte)txBuffer[3];
            __temp[4] = (byte)txBuffer[4];
            __temp[5] = (byte)txBuffer[5];


            Console.WriteLine($"Sending FTOR1 frame: [ {__temp[0]},{__temp[1]},{__temp[2]},{__temp[3]},{__temp[4]},{__temp[5]} ]");

            /* Once FTOR1 has been sent by the client, the receiver must respond
               by sending the same FTOR1 frame with the ACK bit set. */

            int ackFlag = 0;

            DateTime startTime = DateTime.Now;
            double elapsedTime = 0;

            sync();

            _port.Write(__temp, 0, 6);

            do
            {
                for (int i = 0; i < 6; i++)
                {
                    __temp[i] = (byte)_port.ReadByte();
                }

                elapsedTime = (DateTime.Now - startTime).TotalMilliseconds;

                //Console.WriteLine($"Elapsed time (ms): {elapsedTime}");

                if (__temp[0] != 248)
                {
                    ackFlag = 0;
                }
                else if (__temp[0] == 248)
                {
                    ackFlag = ((__temp[1] & (1 << 7)) >> 7);
                    Console.WriteLine($"Received FTOR1 frame: [ {__temp[0]},{__temp[1]},{__temp[2]},{__temp[3]},{__temp[4]},{__temp[5]} ]");
                }

                //Console.WriteLine(ackFlag);

            } while ((ackFlag == 0) && (elapsedTime < timeout_ms));


            if ((elapsedTime < 10000) && (ackFlag == 1))
            {
                Console.WriteLine("\nFTOR ACK received");
            }
            else
            {
                Console.WriteLine("\nFTOR ACK not received.");
                Console.WriteLine($"Elapsed time (ms): {elapsedTime}");
            }

            if (ackFlag == 1)
            {
                for (int i = 0; i < 6 * frame_IDs.Length; i++)
                {
                    __temp[i] = (byte)_port.ReadByte();
                }
            }

            Console.WriteLine("\nReceived frames: \n");

            for (int i = 0; i < frame_IDs.Length; i++)
            {
                Console.WriteLine($"Frame {i}: [{__temp[6 * i + 0]},{__temp[6 * i + 1]},{__temp[6 * i + 2]},{__temp[6 * i + 3]},{__temp[6 * i + 4]},{__temp[6 * i + 5]}]");
            }

            Console.WriteLine("\n");
        }

        static int generate_checksum(int[] array)
        {
            int checksum = 0;
            checksum += array[0];
            checksum += array[1];
            checksum += array[2];
            checksum += array[3];
            checksum += array[4];
            checksum = 255 - (checksum % 255);
            return (checksum);
        }

        /*
         *  Function to write a message frame to the device.
         *
         */

        public void writeFrame(int frame_ID,int priority)
        {
            SDCP_Frame_Array_Record frameInstance = frames[frame_ID];

            frameInstance.B0 = (UInt64)frame_ID + (UInt64)(128 * priority);
            frameInstance.B5 = 255 - ((frameInstance.B0 + frameInstance.B1 + frameInstance.B2 + frameInstance.B3 + frameInstance.B4) % 255);

            byte[] frameBytes = new byte[6];

            int _temp = 0;
            
            _temp = (int)frameInstance.B0;    frameBytes[0] = (byte)_temp;
            _temp = (int)frameInstance.B1;    frameBytes[1] = (byte)_temp;
            _temp = (int)frameInstance.B2;    frameBytes[2] = (byte)_temp;
            _temp = (int)frameInstance.B3;    frameBytes[3] = (byte)_temp;
            _temp = (int)frameInstance.B4;    frameBytes[4] = (byte)_temp;
            _temp = (int)frameInstance.B5;    frameBytes[5] = (byte)_temp;

            _port.Write(frameBytes, 0, 6);
        }


        public void writeFrame(string frame_name, int priority)
        {
            for(int i = 0; i < frames.Count; i++)
            {
                if (frames[i].frame_name == frame_name )
                {
                    writeFrame(frames[i].frame_ID, priority);
                    break;
                }
            }
        }


        /* Function to livestream a given set of parameters updated at specified interval and 
           stop updating at the specified stop time. */

        public void liveStreamParameters(string[] parameterNames,UInt32 update_time_ms, UInt32 stop_time_ms)
        {
            DateTime startTime = DateTime.Now;
            DateTime previousUpdateTime = DateTime.Now;
            DateTime displayTime = DateTime.Now;

            printParameterPollResults = false;

            _port.DiscardInBuffer();
            sync();

            startTime = DateTime.Now;
            displayTime = DateTime.Now;
            string output = "";

            int initialUpdate = 0;
            int updateCount = 0;
            float[] value = new float[parameterNames.Length];
            float updatePercentage;


            Console.WriteLine(" ");
            Console.WriteLine(" ");
            Console.WriteLine(" ");

            while ( ( DateTime.Now - startTime ).TotalMilliseconds < stop_time_ms )
            {
                for(int i = 0; i < parameterNames.Length; i++)
                {
                    value[i] = pollParameter(parameterNames[i], update_time_ms);
                }

                updatePercentage = (float)(DateTime.Now - startTime).TotalMilliseconds / (float)stop_time_ms;
                updateCount = (int)Math.Ceiling(50 * updatePercentage);

                if ((DateTime.Now - displayTime).TotalMilliseconds > update_time_ms )
                {
                    displayTime = DateTime.Now;

                    output = "";

                    for(int i = 0; i < parameterNames.Length; i++)
                    {
                        output += $"Parameter name: {parameterNames[i]}";
                        output += $"    Value: {value[i]}\n";
                    }


                    if(initialUpdate == 0)
                    {
                        initialUpdate = 1;
                    }

                    for(int i = 0; i < parameterNames.Length+5; i++)
                    {
                        Console.SetCursorPosition(0, Console.CursorTop - 1);
                        //Console.SetCursorPosition(0, Console.CursorTop - 1);
                        Console.Write(new string(' ', Console.WindowWidth));
                    }

                    output += $"\nTime: { (DateTime.Now - startTime).TotalMilliseconds} (ms)";
                    output += $"\t Update interval (ms): { (DateTime.Now - previousUpdateTime).TotalMilliseconds}\n";
                    output += "  \n[";

                    previousUpdateTime = DateTime.Now;

                    for (int i = 0; i < 50; i++)
                    {
                        if (i < updateCount)
                        {
                            output += "#";
                        }
                        else
                        {
                            output += " ";
                        }
                    }

                    output += "]";

                    Console.WriteLine(output);
                }
            }
        }


        public static Thread rxThread;
        private CancellationTokenSource rxCancelSource;


        public List<Receive_List_Record> receiveList;

        public void start_rx()
        {
            rxCancelSource = new CancellationTokenSource();

            Console.WriteLine("Rx thread started");

            new Thread(() =>
            {
                try
                {
                    rx_handler(rxCancelSource.Token);
                }
                catch (Exception e)
                {
                    Console.WriteLine("Rx Transmit task stopped");
                }
            }).Start();
        }



        public static int numberOfReceivedFrames = 0;

        public void rx_handler(CancellationToken token)
        {

            readLatestFrame(true);

            while (true)
            {
                readLatestFrame();
                token.ThrowIfCancellationRequested();
            }

        }

        public void update_frame_info_from_receive_list()
        {
            SDCP_Frame_Array_Record frameInstance;
            receive_frame_record receivedFrameInstance;

            for( int i = 0; i < receivedFrames.Count; i++ )
            {
                if (receivedFrames[i].received)
                {
                    receivedFrameInstance = receivedFrames[i];

                    frameInstance = frames[i];
                    frameInstance.B0 = (ulong)receivedFrameInstance.B0;
                    frameInstance.B1 = (ulong)receivedFrameInstance.B1;
                    frameInstance.B2 = (ulong)receivedFrameInstance.B2;
                    frameInstance.B3 = (ulong)receivedFrameInstance.B3;
                    frameInstance.B4 = (ulong)receivedFrameInstance.B4;
                    frameInstance.B5 = (ulong)receivedFrameInstance.B5;
                    
                    frames[i] = frameInstance;
                }
            }
        }

        public void printReceiveList()
        {
            Receive_List_Record record = new Receive_List_Record();

            string output = "";

            for(int i = 0; i < receiveList.Count; i++)
            {
                record = receiveList[i];

                output += $">>> {record.frame.frame_name} [{record.frame.frame_ID}]:  ";
                output += $"[ {record.frame.B0}, {record.frame.B1}, {record.frame.B2}, {record.frame.B3}, {record.frame.B4}, {record.frame.B5}]  ";
                output += $"  ( {record.receiveInterval_ms} ms, {record.receiveCount} )";
                output += "\n";
            }

            Console.WriteLine(output);
        }

        public void stop_rx()
        {
            rxCancelSource.Cancel();
            //rxThread.Join();

            Console.WriteLine($"Number of received frames = {numberOfReceivedFrames}");
        }


        public struct receive_frame_record
        {
            public bool received;
            public DateTime lastReceived;
            public double receive_interval_ms;
            public int B0;
            public int B1;
            public int B2;
            public int B3;
            public int B4;
            public int B5;
            public int count;
        }

        public static List<receive_frame_record> receivedFrames;

        public void readLatestFrame(bool sync_rqst)
        {
            receivedFrames = new List<receive_frame_record>();

            receive_frame_record tempRecord;

            DateTime tempDateTime = DateTime.Now;

            for(int i = 0; i <= 127; i++)
            {
                tempRecord = new receive_frame_record();
                
                tempRecord.receive_interval_ms = 0;
                tempRecord.lastReceived = DateTime.Now;
                tempRecord.B0 = 0; tempRecord.B1 = 0; tempRecord.B2 = 0;
                tempRecord.B3 = 0; tempRecord.B4 = 0; tempRecord.B5 = 0;
                tempRecord.received = false;
                tempRecord.count = 0;

                receivedFrames.Add(tempRecord);
            }

            if (sync_rqst)
            {
                sync();
            }

            int[] B = new int[6];

            for (int i = 0; i < 6; i++)
            {
                B[i] = _port.ReadByte();
            }

            Console.WriteLine($"Received frame: [ {B[0]}, {B[1]}, {B[2]}, {B[3]}, {B[4]}, {B[5]} ]");
        }


        public void readLatestFrame()
        {
            int[] B = new int[6];

            for (int i = 0; i < 6; i++)
            {
                B[i] = _port.ReadByte();
            }

            int index = B[0] & 127;

            receive_frame_record record = receivedFrames[index];

            record.B0 = B[0];
            record.B1 = B[1];
            record.B2 = B[2];
            record.B3 = B[3];
            record.B4 = B[4];
            record.B5 = B[5];

            record.count++;
            record.received = true;

            record.receive_interval_ms = (DateTime.Now - record.lastReceived).TotalMilliseconds;
            record.lastReceived = DateTime.Now;

            receivedFrames[index] = record;

            //Console.WriteLine($"Received frame: [ {record.B0}, {record.B1}, {record.B2}, {record.B3}, {record.B4}, {record.B5} ] \t count = {record.count} \t interval = {record.receive_interval_ms}");
        }

        public void readFrame()
        {
            int[] bytes = new int[6];

            sync();

            for(int i = 0; i < 6; i++)
            {
                bytes[i] = _port.ReadByte();
            }

            Console.WriteLine($"\nReceived frame: [ {bytes[0]}, {bytes[1]}, {bytes[2]}, {bytes[3]}, {bytes[4]}, {bytes[5]} ]\n");
        }

        public void readFrames(uint count)
        {
            int[] bytes = new int[6];

            sync();

            Console.WriteLine();

            for (int k = 0; k < count; k++)
            {

                for (int i = 0; i < 6; i++)
                {
                    bytes[i] = _port.ReadByte();
                }

                Console.WriteLine($"Received frame: [ {bytes[0]}, {bytes[1]}, {bytes[2]}, {bytes[3]}, {bytes[4]}, {bytes[5]} ]");
            }

            Console.WriteLine("\n");
        }


        public void print_receive_frame_info()
        {
            update_frame_info_from_receive_list();

            string output = "";

            output += "\n\n \t <<<<<    Receive List    >>>>> \t \n\n";

            for (int i = 0; i < receivedFrames.Count; i++)
            {
                if (receivedFrames[i].received)
                {
                    output += $"{frames[i].frame_name} [{frames[i].frame_ID}] :\t";
                    output += $"count = {receivedFrames[i].count}  interval = {receivedFrames[i].receive_interval_ms} (ms)";

                    output += "\n";
                }
            }

            Console.WriteLine(output);
        }


        public void print_detailed_receive_frame_info()
        {
            update_frame_info_from_receive_list();

            string output = "";

            output += "\n\n \t <<<<<    Receive List    >>>>> \t \n\n";

            for (int i = 0; i < receivedFrames.Count; i++)
            {
                if (receivedFrames[i].received)
                {
                    output += $"\n[  {frames[i].frame_name} [{frames[i].frame_ID}] :\t";
                    output += $"count = {receivedFrames[i].count}  interval = {receivedFrames[i].receive_interval_ms} (ms)  ]\n\n";

                    for(int j = 0; j < parameterList.Count; j++)
                    {
                        if (parameterList[j].frame_ID == frames[i].frame_ID )
                        {
                            readParameterFromFrame(parameterList[j].parameter_name);

                            output += $"{parameterList[j].parameter_name} :\t {parameterList[j].value}\n";
                        }
                    }

                    output += "______________________________________________________";

                    output += "\n";
                }
            }

            Console.WriteLine(output);
        }


        /********************  Functions to handle the transmission of data to device.   **********/

        public static Thread txThread;
        private CancellationTokenSource txCancelSource;

        /* Function to start the transmit thread. */

        public void start_tx()
        {
            txCancelSource = new CancellationTokenSource();

            Console.WriteLine("Tx Transmit task started");

            new Thread(()=>
            {
                try
                {
                    tx_handler(txCancelSource.Token);
                }
                catch(Exception e)
                {
                    Console.WriteLine("Tx Transmit task stopped");
                }
            }).Start();
        }

        public void tx_delay(uint delay_ms)
        {
            DateTime startTime = DateTime.Now;
            while ((DateTime.Now - startTime).TotalMilliseconds < delay_ms);
        }

        private static uint[] tx_delays;
        private static uint tx_min_delay;
        private static DateTime[] tx_update_time;
        private static byte[] tx_buffer;

        public void tx_handler(CancellationToken token)
        {
            Transmit_List_Record record= new Transmit_List_Record();
            uint tx_buffer_pointer = 0;


            while(true)
            {
                _port.DiscardOutBuffer();
                tx_buffer_pointer = 0;

                for(int i = 0; i < transmitList.Count; i++)
                {
                    record = transmitList[i];

                    if( ( DateTime.Now - tx_update_time[i] ).TotalMilliseconds > transmitList[i].transmitInterval_ms )
                    {
                        //Console.WriteLine("Update event");
                        
                        tx_update_time[i] = DateTime.Now;

                        if(transmitList[i].cyclicModeEnable || transmitList[i].trigger )
                        {
                            tx_buffer[tx_buffer_pointer + 0] = (byte)transmitList[i].frame.B0;
                            tx_buffer[tx_buffer_pointer + 1] = (byte)transmitList[i].frame.B1;
                            tx_buffer[tx_buffer_pointer + 2] = (byte)transmitList[i].frame.B2;
                            tx_buffer[tx_buffer_pointer + 3] = (byte)transmitList[i].frame.B3;
                            tx_buffer[tx_buffer_pointer + 4] = (byte)transmitList[i].frame.B4;
                            tx_buffer[tx_buffer_pointer + 5] = (byte)transmitList[i].frame.B5;

                            tx_buffer_pointer += 6;

                            record.trigger = false;
                            record.transmitCount++;
                        }

                        _port.Write(tx_buffer, 0, (int)tx_buffer_pointer);

                        transmitList[i] = record;
                    }
                }

                //xmt_delay(XMT_min_delay);
                token.ThrowIfCancellationRequested();
            }
        }

        public void stop_tx()
        {
            txCancelSource.Cancel();
        }

        public void tx_trigger(int frame_ID)
        {
            Transmit_List_Record record;

            for(int i = 0; i < transmitList.Count; i++)
            {
                if( transmitList[i].frame.frame_ID == frame_ID )
                {
                    record = transmitList[i];
                    record.trigger = true;
                    transmitList[i] = record;
                }
            }
        }

        public void change_tx_mode(int frame_ID,bool cyclicModeEnable)
        {
            try
            {
                Transmit_List_Record record;

                for (int i = 0; i < transmitList.Count; i++ )
                {
                    record = transmitList[i];

                    if( record.frame.frame_ID == frame_ID )
                    {
                        record.cyclicModeEnable = cyclicModeEnable;
                        transmitList[i] = record;
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Frame not present in transmit list.");
            }
        }

        public void change_tx_mode(string frame_name,bool cyclicModeEnable)
        {
            Transmit_List_Record record;

            for(int i = 0; i < transmitList.Count; i++)
            {
                if (transmitList[i].frame.frame_name == frame_name)
                {
                    record = transmitList[i];
                    record.cyclicModeEnable = cyclicModeEnable;
                    transmitList[i] = record;
                    break;
                }
            }
        }

        public void change_tx_interval(int frame_ID,int transmit_interval_ms)
        {
            Transmit_List_Record record;

            for (int i = 0; i < transmitList.Count; i++ )
            {
                if(transmitList[i].frame.frame_ID == frame_ID)
                {
                    record = transmitList[i];
                    record.transmitInterval_ms = (uint)transmit_interval_ms;
                    transmitList[i] = record;
                    break;
                }
            }
        }

        public void change_tx_interval(string frame_name, int transmit_interval_ms)
        {
            Transmit_List_Record record;

            for (int i = 0; i < transmitList.Count; i++)
            {
                if (transmitList[i].frame.frame_name == frame_name)
                {
                    record = transmitList[i];
                    record.transmitInterval_ms = (uint)transmit_interval_ms;
                    transmitList[i] = record;
                    break;
                }
            }
        }


        public void addFrameToTransmitList(int frame_ID,uint transmit_interval_ms)
        {
            Transmit_List_Record record = new Transmit_List_Record();

            record.frame = frames[frame_ID];
            record.transmitCount = 0;
            record.transmitInterval_ms = transmit_interval_ms;
            record.cyclicModeEnable = false;
            record.trigger = false;

            bool isPresent = false;

            for(int i = 0; i < transmitList.Count; i++)
            {
                if (transmitList[i].frame.frame_ID == frame_ID)
                {
                    isPresent = true;
                    break;
                }
            }

            if (isPresent == false)
            {
                transmitList.Add(record);
            }
        }

        public void addFrameToTransmitList(string frame_name, uint transmit_interval_ms)
        {
            for(int i = 0; i < frames.Count; i++)
            {
                if (frames[i].frame_name == frame_name)
                {
                    addFrameToTransmitList(frames[i].frame_ID, transmit_interval_ms);
                    break;
                }
            }
        }


        public void updateTransmitList()
        {
            Transmit_List_Record record = new Transmit_List_Record();
            SDCP_Frame_Array_Record frameInstance = new SDCP_Frame_Array_Record();

            tx_delays = new uint[transmitList.Count];
            tx_update_time = new DateTime[transmitList.Count];

            for(int i = 0; i < transmitList.Count; i++)
            {
                tx_update_time[i] = DateTime.Now;
                record = transmitList[i];
                frameInstance = frames[record.frame.frame_ID];
                record.frame = frameInstance;
                transmitList[i] = record;

                tx_delays[i] = transmitList[i].transmitInterval_ms;
            }

            tx_buffer = new byte[6*transmitList.Count];
            tx_min_delay = getMinimumTXdelay();
        }

        public uint getMinimumTXdelay()
        {
            uint minimumDelay = 0;
            uint isDivisibleCount = 0;
            uint gcd = 0;

            for (int k = 3000000; k >= 2; k--)
            {
                isDivisibleCount = 0;

                for (int i = 0; i < transmitList.Count; i++)
                {
                    if( ( transmitList[i].transmitInterval_ms % k ) == 0 )
                    {
                        isDivisibleCount++;
                    }
                }


                if( isDivisibleCount == transmitList.Count )
                {
                    minimumDelay = (uint)k;
                    break;
                }
            }

            return (minimumDelay);
        }

        public void updateTransmitFrameDetails(int frame_ID, UInt32 update_interval_ms)
        {
            Transmit_List_Record record = new Transmit_List_Record();

            int index = -1;

            for(int i = 0; i < transmitList.Count; i++)
            {
                if (transmitList[i].frame.frame_ID == frame_ID )
                {
                    index = i;
                }
            }

            if( index == -1 )
            {
                Console.WriteLine("\nFrame not present in transmit list.");
            }
            else
            {
                record = transmitList[index];
                record.transmitInterval_ms = update_interval_ms;
                transmitList[index] = record;
            }
        }

        public void updateTransmitFrameDetails(int frame_ID, int transmitMode)
        {
            Transmit_List_Record record = new Transmit_List_Record();

            int index = -1;

            for (int i = 0; i < transmitList.Count; i++)
            {
                if (transmitList[i].frame.frame_ID == frame_ID)
                {
                    index = i;
                }
            }

            if (index == -1)
            {
                Console.WriteLine("\nFrame not present in transmit list.");
            }
            else
            {
                record = transmitList[index];
                record.cyclicModeEnable = (transmitMode==1);
                transmitList[index] = record;
            }
        }

        public void updateTransmitFrameDetails(int frame_ID, UInt32 update_interval_ms, int transmitMode)
        {
            Transmit_List_Record record = new Transmit_List_Record();

            int index = -1;

            for (int i = 0; i < transmitList.Count; i++)
            {
                if (transmitList[i].frame.frame_ID == frame_ID)
                {
                    index = i;
                }
            }

            if (index == -1)
            {
                Console.WriteLine("\nFrame not present in transmit list.");
            }
            else
            {
                record = transmitList[index];
                record.transmitInterval_ms = update_interval_ms;
                record.cyclicModeEnable = (transmitMode == 1);
                transmitList[index] = record;
            }
        }

        public void triggerTransmitEvent(int frame_ID)
        {
            Transmit_List_Record record = new Transmit_List_Record();

            int index = -1;

            for (int i = 0; i < transmitList.Count; i++)
            {
                if (transmitList[i].frame.frame_ID == frame_ID)
                {
                    index = i;
                }
            }

            if (index == -1)
            {
                Console.WriteLine("\nFrame not present in transmit list.");
            }
            else
            {
                record = transmitList[index];
                record.trigger = true;
                transmitList[index] = record;
            }
        }

        public void updateTransmitFrameDetails(string frame_name, UInt32 update_interval_ms)
        {
            Transmit_List_Record record = new Transmit_List_Record();

            int index = -1;

            for (int i = 0; i < transmitList.Count; i++)
            {
                if (transmitList[i].frame.frame_name == frame_name)
                {
                    index = i;
                }
            }

            if (index == -1)
            {
                Console.WriteLine("\nFrame not present in transmit list.");
            }
            else
            {
                record = transmitList[index];
                record.transmitInterval_ms = update_interval_ms;
                transmitList[index] = record;
            }
        }

        public void updateTransmitFrameDetails(string frame_name, int transmitMode)
        {
            Transmit_List_Record record = new Transmit_List_Record();

            int index = -1;

            for (int i = 0; i < transmitList.Count; i++)
            {
                if (transmitList[i].frame.frame_name == frame_name)
                {
                    index = i;
                }
            }

            if (index == -1)
            {
                Console.WriteLine("\nFrame not present in transmit list.");
            }
            else
            {
                record = transmitList[index];
                record.cyclicModeEnable = (transmitMode == 1);
                transmitList[index] = record;
            }
        }

        public void updateTransmitFrameDetails(string frame_name, UInt32 update_interval_ms, int transmitMode)
        {
            Transmit_List_Record record = new Transmit_List_Record();

            int index = -1;

            for (int i = 0; i < transmitList.Count; i++)
            {
                if (transmitList[i].frame.frame_name == frame_name)
                {
                    index = i;
                }
            }

            if (index == -1)
            {
                Console.WriteLine("\nFrame not present in transmit list.");
            }
            else
            {
                record = transmitList[index];
                record.transmitInterval_ms = update_interval_ms;
                record.cyclicModeEnable = (transmitMode == 1);
                transmitList[index] = record;
            }
        }

        public void triggerTransmitEvent(string frame_name)
        {
            Transmit_List_Record record = new Transmit_List_Record();

            int index = -1;

            for (int i = 0; i < transmitList.Count; i++)
            {
                if (transmitList[i].frame.frame_name == frame_name)
                {
                    index = i;
                }
            }

            if (index == -1)
            {
                Console.WriteLine("\nFrame not present in transmit list.");
            }
            else
            {
                record = transmitList[index];
                record.trigger = true;
                transmitList[index] = record;
            }
        }

        public void printTransmitList()
        {
            Transmit_List_Record record = new Transmit_List_Record();
            SDCP_Frame_Array_Record frameInstance = new SDCP_Frame_Array_Record();
            string output = "";

            for(int i = 0; i < transmitList.Count; i++)
            {
                record = transmitList[i];

                frameInstance = frames[record.frame.frame_ID];

                record.frame = frameInstance;
                transmitList[i] = record;

                updateTransmitList();

                for(int k = 0; k < parameterList.Count; k++)
                {
                    if (parameterList[k].frame_ID == frameInstance.frame_ID)
                    {
                        readParameterFromFrame(parameterList[k].parameter_name);
                    }
                }

                output += "\n";
                output += $">>>> {record.frame.frame_name} [{record.frame.frame_ID}]:  ";
                output += $" [ {record.frame.B1}, {record.frame.B2}, {record.frame.B3}, {record.frame.B4} ]  ";
                output += $" Update interval (ms): {record.transmitInterval_ms}  ";
                output += $" Transmit count: {record.transmitCount}  ";

                string transmitModeString = "";

                if(record.cyclicModeEnable)
                {
                    transmitModeString = "Cyclic";
                }
                else
                {
                    transmitModeString = "Triggered";
                }

                output += $" Transmit mode: {transmitModeString}";

                output += "\n\n";

                for(int j = 0; j < parameterList.Count ; j++ )
                {
                    if(parameterList[j].frame_ID == record.frame.frame_ID)
                    {
                        output += $"{parameterList[j].parameter_name}: \t {parameterList[j].value}\n";
                    }
                }

                //output += "_____________________________________________________________________";
                //output += "_____________________________________________________________________\n";
            }

            Console.WriteLine(output);
        }
    }

}
