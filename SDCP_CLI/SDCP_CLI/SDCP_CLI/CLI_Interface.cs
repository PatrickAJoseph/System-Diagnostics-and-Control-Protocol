using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Text;
using System.Threading.Tasks;

using SDCP_API;

namespace CLI_Interface
{
    public class CLI_Interface
    {

        public static string command;
        public static string[] parameter;
        public static string[] value;

        private static SDCP_API.SDCP_API sdcp;

        /* Initialization (constructor). */

        public CLI_Interface() 
        { 
          sdcp = new SDCP_API.SDCP_API();
        }


        /* Command exception list. */

        public static bool checkIfCommandInExceptionList(string CLI_string)
        {
            string _command = command;
            bool isException = false;

            int startIndex = 0;
            int endIndex = 0;
            uint[] indices;

            string tempString = "";

            /* Exception for load parameter from file. */

            if( _command == "lpf" )
            {
                indices = getCharacterIndices('"', CLI_string);

                startIndex = (int)indices[0];
                endIndex = (int)indices[1];

                parameter[0] = "path";
                value[0] = CLI_string.Substring(startIndex + 1, endIndex - startIndex - 1);

                isException = true;
            }

            return (isException);
        }

        /* CLI interpretation function to obtain the command and parameters
           associate with it. */

        public void extract(string CLI_string)
        {
            if (false)
            {
                uint[] indices = getCharacterIndices(' ', CLI_string);
                uint numberOfBlanks = getCharacterCount(' ', CLI_string);

                uint numberOfParameters = numberOfBlanks / 2;
                uint stringLength = (uint)CLI_string.Length;

                parameter = new string[numberOfParameters];
                value = new string[numberOfParameters];
                try
                {
                    if (CLI_string.Substring(0, 4) != "SDCP")
                    {
                        Console.WriteLine("\nError: Prefix all commands with SDCP\n");
                        return;
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine("\nError: Enter valid command by prefixing command with SDCP.\n");
                    return;
                }

                if (numberOfParameters == 0)
                {
                    command = CLI_string.Substring((int)indices[0] + 1, (int)stringLength - (int)indices[0] - 1);
                    //Console.WriteLine(command);
                }
                else
                {
                    command = CLI_string.Substring((int)indices[0] + 1, (int)(indices[1] - indices[0] - 1));
                    //Console.WriteLine(command);

                    /* Get parameter value set. */

                    string tempString = "";
                    int startIndex = 0;
                    int endIndex = 0;
                    int length = 0;
                    string tempString2 = "";

                    //Console.WriteLine($"\nNumber of parameters = {numberOfParameters}");

                    if (checkIfCommandInExceptionList(CLI_string) == false)
                    {

                        for (int i = 0; i < numberOfParameters; i++)
                        {
                            /* If not the last parameter. */

                            if (i != (numberOfParameters - 1))
                            {
                                startIndex = (int)indices[2 * i + 1] + 1;
                                endIndex = (int)indices[2 * i + 3] - 1;
                                length = endIndex - startIndex + 1;
                            }
                            else /* if the last parameter. */
                            {
                                startIndex = (int)indices[2 * i + 1] + 1;
                                endIndex = (int)CLI_string.Length - 1;
                                length = endIndex - startIndex + 1;
                            }

                            tempString = CLI_string.Substring(startIndex, length);

                            //Console.WriteLine(tempString);

                            /* Each parameter and value subsection has the format
                             *
                             *   -[parameter] [value]
                             */

                            tempString2 = tempString;

                            //Console.WriteLine(tempString);

                            startIndex = getCharacterIndex('-', tempString);
                            endIndex = getCharacterIndex(' ', tempString);
                            parameter[i] = tempString.Substring(startIndex + 1, endIndex - startIndex - 1);

                            startIndex = getCharacterIndex(' ', tempString);

                            endIndex = tempString.Length;

                            value[i] = tempString.Substring(startIndex + 1, endIndex - startIndex - 1);

                            //Console.WriteLine($"{parameter[i]},{value[i]}");
                        }
                    }
                }
            }

            if (true)
            {
                uint[] indices = getCharacterIndices(' ', CLI_string);
                uint numberOfBlanks = getCharacterCount(' ', CLI_string);

                uint numberOfParameters = numberOfBlanks/2;
                uint stringLength = (uint)CLI_string.Length;

                parameter = new string[numberOfParameters];
                value = new string[numberOfParameters];

                if (numberOfParameters == 0)
                {
                    command = CLI_string;
                    //Console.WriteLine(command);
                }
                else
                {
                    command = CLI_string.Substring(0, (int)indices[0]);
                    //Console.WriteLine(command);

                    /* Get parameter value set. */

                    string tempString = "";
                    int startIndex = 0;
                    int endIndex = 0;
                    int length = 0;
                    string tempString2 = "";

                    //Console.WriteLine($"\nNumber of parameters = {numberOfParameters}");

                    if (checkIfCommandInExceptionList(CLI_string) == false)
                    {

                        for (int i = 0; i < numberOfParameters; i++)
                        {
                            /* If not the last parameter. */

                            if (i != (numberOfParameters - 1))
                            {
                                startIndex = (int)indices[2 * i + 0] + 1;
                                endIndex = (int)indices[2 * i + 2] - 1;
                                length = endIndex - startIndex + 1;
                            }
                            else /* if the last parameter. */
                            {
                                startIndex = (int)indices[2 * i + 0] + 1;
                                endIndex = (int)CLI_string.Length - 1;
                                length = endIndex - startIndex + 1;
                            }

                            tempString = CLI_string.Substring(startIndex, length);

                            //Console.WriteLine(tempString);

                            /* Each parameter and value subsection has the format
                             *
                             *   -[parameter] [value]
                             */

                            tempString2 = tempString;

                            //Console.WriteLine(tempString);

                            startIndex = getCharacterIndex('-', tempString);
                            endIndex = getCharacterIndex(' ', tempString);
                            parameter[i] = tempString.Substring(startIndex + 1, endIndex - startIndex - 1);

                            startIndex = getCharacterIndex(' ', tempString);

                            endIndex = tempString.Length;

                            value[i] = tempString.Substring(startIndex + 1, endIndex - startIndex - 1);

                            //Console.WriteLine($"{parameter[i]},{value[i]}");
                        }
                    }
                }
            }

        }

        /* Function to get index of character. */

        private static int getCharacterIndex(char c,string inString)
        {
            int index = -1;

            for(int i = 0; i < inString.Length ; i++)
            {
                if (inString[i] == c )
                {
                    index = i;
                    break;
                }
            }

            return (index);
        }

        /* Function to get number of occurances of a character. */

        private static uint getCharacterCount(char c,string inputString)
        {
            uint count = 0;

            for( int i = 0; i < inputString.Length; i++ )
            {
                if (inputString[i] == c )
                {
                    count++;
                }
            }

            return (count);
        }

        /* Function to get indices of parameters in a string. */

        private static uint[] getCharacterIndices(char c,string inputString)
        {
            uint count = getCharacterCount(c,inputString);

            uint[] indices = new uint[count];

            uint counter = 0;

            for(int i = 0; i < inputString.Length; i++)
            {
                if(c == inputString[i])
                {
                    indices[counter] = (uint)i;
                    counter++;
                }
            }

            return(indices);
        }

        /* Interpreter function. This function provides an interface between the
           command line and the SDCP API. */

        public bool interpret()
        {
            bool proceed = true;


            /****************************  SERIAL PORT HANDLING. **************************/

            /* Case for closing session. */

            if (command == "exit")
            {
                proceed = false;
            }

            /* Fuction to list all serial ports available on host. */

            if(command == "lasp")
            {
              sdcp.listAvailablePorts();
            }

            /* Function to open a serial port session. */

            //Console.WriteLine(command.Length);

            if(command == "opensession")
            {
                int numberOfParameters = parameter.Length;

                /* Format 1: SDCP opensession */

                if( numberOfParameters == 0 )
                {
                    sdcp.openSerialPort();
                }

                /* Format 2: SDCP opensession -timeout [timeout] */

                if ( ( numberOfParameters == 1 ) )
                {
                    if (parameter[0] == "timeout")
                    {
                        Console.WriteLine(value[0]);
                        sdcp.openSerialPort(int.Parse(value[0]));
                    }
                    else
                    {
                        Console.WriteLine("\nError: Use format SDCP opensession -timeout [Timeout in milli-seconds].\n");
                    }
                }

                if( (numberOfParameters == 2) )
                {
                    /* Format 3: SDCP opensession -port [COM Port] -baudrate [baud rate]  */

                    if ((parameter[0] == "port") && (parameter[1]=="baudrate"))
                    {
                        sdcp.openSerialPort(value[0], int.Parse(value[1]));

                        if (sdcp.sync(1000) == 1)
                        {
                            Console.WriteLine($"\nOpened SDCP session successfully at serial port: {value[0]}.");
                        }
                        else
                        {
                            Console.WriteLine($"\nNot able to open SDCP session at serial port: {value[0]}");
                        }
                    }

                    /* Format 4: SDCP opensession -baudrate [baudrate] -port [COM Port] */

                    if ((parameter[0] == "baudrate") && (parameter[1] == "port"))
                    {
                        sdcp.openSerialPort(value[1], int.Parse(value[0]));
                        
                        if( sdcp.sync(1000) == 1 )
                        {
                            Console.WriteLine($"\nOpened SDCP session successfully at serial port: {value[1]}.");
                        }
                        else
                        {
                            Console.WriteLine($"\nNot able to open SDCP session at serial port: {value[1]}");
                        }
                    }
                }
            }

            /* Function to close current serial port sesison. */

            if (command == "closesession")
            {
                sdcp.closeSerialPort();    
            }

            /**************   Functions to handle frames and parameters ***********************/

            /* Function to load parameter file. */

            if(command == "lpf")
            {
                int numberOfParameters = parameter.Length;

                if(numberOfParameters == 0)
                {
                    Console.WriteLine("\nNo file path entered.\n");
                }
                else
                {
                    if (parameter[0] == "path")
                    {
                        if (File.Exists(value[0]))
                        {
                            sdcp.loadParameterFile(value[0]);
                        }
                        else
                        {
                            Console.WriteLine("\nSelect valid parameter file path !\n");
                        }
                    }
                    else
                    {
                        Console.WriteLine("\nEnter valid command !");
                    }
                }
            }

            /* Function to load previously/last loaded parameter file. */

            if(command == "llpf")
            {
                sdcp.filePath = File.ReadAllText("data.txt");

                if (File.Exists(sdcp.filePath))
                {
                    sdcp.loadParameterFile(sdcp.filePath);
                }
                else
                {
                    Console.WriteLine("\nError: Parameter file does not exist.\n");
                }
            }

            /* Function to display available frames from the parameter file. */

            if(command == "pfl")
            {
                /* Command syntax: SDCP pfl */

                int numberOfParameters = parameter.Length;

                sdcp.printFrameList();
            }

            /* Function to display available parameters from the parameter file. */

            if (command == "ppl")
            {
                /* Command syntax: SDCP ppl */

                int numberOfParameters = parameter.Length;

                sdcp.printParameterList();
                Console.WriteLine("\n\n");
            }

            /* Function to display available parameter names from the parameter file. */

            if( command == "ppn" )
            {
                sdcp.printParameterNames();
                //Console.WriteLine("\n\n");
            }

            /* Function to print details about a given frame. */

            if( command == "pfd" )
            {
                int numberOfParameters = parameter.Length;

                if(numberOfParameters == 0)
                {
                    Console.WriteLine("\nError: Enter -fname or -fid to select frame name of frame ID\n");
                }

                if(numberOfParameters == 1)
                {
                    /* Format 1: SDCP pfd -fname [Frame name] */

                    if (parameter[0] == "fname")
                    {
                        sdcp.printFrameDetails(value[0]);
                    }

                    /* Format 2: SDCP pfd -fid [Frame ID] */

                    if (parameter[0] == "fid")
                    {
                        sdcp.printFrameDetails(int.Parse(value[0]));
                    }
                }
            }


            /* Function to print details about a given parameter. */

            if (command == "ppd")
            {
                int numberOfParameters = parameter.Length;

                if (numberOfParameters == 0)
                {
                    Console.WriteLine("\nError: Enter -pname or -pid to select parameter name of parameter ID\n");
                }

                if (numberOfParameters == 1)
                {
                    /* Format 1: SDCP ppd -pname [Parameter name] */

                    if (parameter[0] == "pname")
                    {
                        sdcp.printParameterDetails(value[0]);
                    }

                    /* Format 2: SDCP ppd -pid [Parameter ID] */

                    if (parameter[0] == "pid")
                    {
                        sdcp.printParameterDetails(int.Parse(value[0]));
                    }
                }
            }


            /*******************    READ COMMANDS   ************************/

            /* Function to sync with device. */

            if(command=="sync")
            {
                int numberOfParameters = parameter.Length;

                /* Format 1: SDCP sync */

                if( ( numberOfParameters == 0 ) )
                {
                    sdcp.sync();
                }

                /* Format 2: SDCP sync -timeout [timeout in milli-seconds] */

                if( ( numberOfParameters == 1 ) )
                {
                    if (parameter[0] == "timeout")
                    {
                        int _timeout = 0;

                        if( int.TryParse(value[0],out _timeout) )
                        {
                            sdcp.sync(_timeout);
                        }
                        else
                        {
                            Console.WriteLine("\nError: Enter integer timeout.");
                        }
                    }
                    else
                    {
                        Console.WriteLine("\nError: Use format : SDCP sync - timeout [timeout in milli-seconds]\n");
                    }
                }
            }

            /* Function to read the lastest standard message frame (SMF) after sending sync. */

            if( command == "rlf" )
            {
                int numberOfParameters = parameter.Length;

                /* Format 1: SDCP rlf */

                if( numberOfParameters == 0 )
                {
                    sdcp.readFrame();
                }

                /* Format 2: SDCP rlf -nframes [number of frames] */

                if( numberOfParameters == 1 )
                {
                    if (parameter[0] == "nframes")
                    {
                        int _temp;

                        if(int.TryParse(value[0],out _temp) )
                        {
                            sdcp.readFrames(uint.Parse(value[0]));
                        }
                        else
                        {
                            Console.WriteLine("\nError: Enter valid number of frames to receive.\n");
                        }
                    }
                    else
                    {
                        Console.WriteLine("\nError: Use format SDCP rlf -nframes [number of frames]\n");
                    }
                }
            }

            /* Function to poll for frame with specified frame ID or frame name. */

            if( command == "pf" )
            {
                int numberOfParameters = parameter.Length;

                if(numberOfParameters != 2)
                {
                    Console.WriteLine("\nError: Invalid command format. Use syntax: SDCP pf -fid [Frame ID] -timeout [Timeout in milli-seconds]\n");
                }

                if(numberOfParameters == 2 )
                {
                    if (parameter[0]=="fid")
                    {
                        int _temp = 0;

                        bool isValidTimeout = int.TryParse(value[1], out _temp);
                        bool isValidFID = int.TryParse(value[0], out _temp);
                        isValidFID = ((_temp >= 0) && (_temp <= 127));

                        int fid;
                        int timeout;

                        if(isValidFID && isValidTimeout)
                        {
                            fid = int.Parse(value[0]);
                            timeout = int.Parse(value[1]);

                            //Console.WriteLine($">>\t {fid}");
                            //Console.WriteLine($">>\t {timeout}");

                            DateTime startTime = DateTime.Now;  

                            int[] B = sdcp.pollFrame(fid,(uint)timeout);

                            double elapsedTime = (DateTime.Now - startTime).TotalMilliseconds;

                            Console.WriteLine($"Received frame:\t [ {B[0]}, {B[1]}, {B[2]}, {B[3]}, {B[4]}, {B[5]} ] \t Time: {(uint)Math.Ceiling(elapsedTime)}\n");
                        }
                        else
                        {
                            if(isValidFID == false)
                                Console.WriteLine("\nError: Entered frame ID is not valid ( 0 <= frame ID <= 127 ).\n");
                            if (isValidTimeout == false)
                                Console.WriteLine("\nError: Entered timeout is not an integer.\n");
                        }
                    }

                    if (parameter[0] == "fname")
                    {
                        int timeout;
                        bool isValidTimeout = int.TryParse(value[1], out timeout);
                        
                        if(isValidTimeout==true)
                        {
                            DateTime startTime = DateTime.Now;

                            int[] B = sdcp.pollFrame(value[0], (uint)timeout);
                            Console.WriteLine();

                            double elapsedTime = (DateTime.Now - startTime).TotalMilliseconds;

                            Console.WriteLine($"Received frame:\t [ {B[0]}, {B[1]}, {B[2]}, {B[3]}, {B[4]}, {B[5]} ] \t Time: {(uint)Math.Ceiling(elapsedTime)}\n");
                        }
                        else
                        {
                            Console.WriteLine("\nError: Enter valid timeout value !");
                        }
                    }
                }
            }

            /* Function to poll for a given parameter with given parameter ID or parameter name. */

            if( command == "pp" )
            {
                int numberOfParameters = parameter.Length;

                sdcp.sync();

                /* Format 1: SDCP pp -pid [Parameter ID] -timeout [Timeout in milli-seconds] */

                if (parameter[0]=="pid")
                {

                    if (numberOfParameters != 2)
                    {
                        Console.WriteLine("\nError (Incorrect syntax): Use syntax SDCP pp -pid [Parameter ID] -timeout [Timeout in milli-seconds] \n");
                    }
                    else
                    {
                        int pid;
                        int timeout;
                        bool isValidPID = int.TryParse(value[0], out pid);
                        bool isValidTimeout = int.TryParse(value[1], out timeout);

                        if (isValidPID == false)
                        {
                            Console.WriteLine("\nError (Invalid parameter): PID entered should be an integer.\n");
                        }

                        if (isValidTimeout == false)
                        {
                            Console.WriteLine("\nError (Invalid parameter): Timeout should be an integer.");
                        }

                        if ((isValidPID & isValidTimeout) == false)
                        {
                            return (true);
                        }
                        else
                        {
                            DateTime startTime = DateTime.Now;
                            double elapsedTime = 0;

                            float value = sdcp.pollParameter(pid, (uint)timeout);

                            elapsedTime = (DateTime.Now - startTime).TotalMilliseconds;
                            string parameterName = sdcp.getParameterName(pid);

                            Console.WriteLine($"\n{parameterName} [{pid}] = {value} \t time (ms): {(uint)elapsedTime}\n");
                        }
                    }
                }

                if (parameter[0] == "pname")
                {
                    if (numberOfParameters != 2)
                    {
                        Console.WriteLine("\nError (Incorrect syntax): Use syntax SDCP pp -pname [Parameter name] -timeout [Timeout in milli-seconds] \n");
                    }
                    else
                    {
                        int timeout;
                        bool isValidTimeout = int.TryParse(value[1], out timeout);
                        string parameter_name = value[0];

                        if (isValidTimeout == false)
                        {
                            Console.WriteLine("\nError (Invalid parameter): Timeout should be an integer.");
                        }

                        if ( (isValidTimeout) == false)
                        {
                            return (true);
                        }
                        else
                        {
                            DateTime startTime = DateTime.Now;
                            double elapsedTime = 0;
                            int pid = sdcp.getParameterIndex(parameter_name);

                            float value = sdcp.pollParameter(parameter_name, (uint)timeout);

                            elapsedTime = (DateTime.Now - startTime).TotalMilliseconds;
                            string parameterName = sdcp.getParameterName(pid);

                            Console.WriteLine($"\n{parameterName} [{pid}] = {value} \t time (ms): {(uint)elapsedTime}\n");
                        }
                    }
                }

            }

            /* Function to livestream the value of given set of parameters. */

            if( command == "lsparams" )
            {
                /* Format lsparams -ui [update interval in ms] -st [stop time] -p0 [parameter 0 name] .... -pn [parameter n name]  */

                int numberOfParameters = parameter.Length;

                if(numberOfParameters<=2)
                {
                    Console.WriteLine("\nError (Parameter requirement): This command requires atleat 3 parameters\n");
                }
                else
                {
                    int updateInterval;
                    int stopTime;

                    bool isValidUpdateInterval = int.TryParse(value[0], out updateInterval);
                    bool isValidStopTime = int.TryParse(value[1], out stopTime);

                    if(isValidUpdateInterval == false)
                    {
                        Console.WriteLine("\nError (Parameter value): Update interval is not an integer.\n");
                        return (true);
                    }

                    if (isValidStopTime == false)
                    {
                        Console.WriteLine("\nError (Parameter value): Stop time is not an integer.\n");
                        return (true);
                    }

                    string[] liveStreamParameters = new string[numberOfParameters - 2];

                    for(int i = 0; i < (numberOfParameters - 2); i++)
                    {
                        liveStreamParameters[i] = value[i + 2];
                    }

                    sdcp.liveStreamParameters(liveStreamParameters, (uint)updateInterval, (uint)stopTime);
                }
            }

            /* Start receive data thread. */

            if( command == "startrx" )
            {
                sdcp.start_rx();
            }

            /* Print received data. */

            if( command == "prx")
            {
                sdcp.print_detailed_receive_frame_info();
            }

            /* Stop receive data thread. */

            if( command == "stoprx" )
            {
                sdcp.stop_rx();
            }


            /************************* WRITE COMMANDS ************************/

            /* Function to write parameter to its corresponding message frame. */

            if(command == "wp")
            {
                int numberOfParameters = parameter.Length;

                if( numberOfParameters != 2 )
                {
                    Console.WriteLine("\nError (Parameter Requirement): This command takes two arguments (-pid / -pname) and -value.\n");
                    return (proceed);
                }

                /* Format 1: wp -pname [parameter name] -value [parameter value] */

                if ((parameter[0] == "pname") && (parameter[1]=="value"))
                {
                    float pvalue;
                    bool isValidValue = float.TryParse(value[1], out pvalue);

                    if( isValidValue == false )
                    {
                        Console.WriteLine("\nError (Parameter Value): Parameter value must be a number.\n");
                        return (proceed);
                    }

                    if( isValidValue )
                    {
                        sdcp.writeParameterToFrame(pvalue, value[0]);
                    }
                }
                else
                {
                    Console.WriteLine("\nSyntax error: Use syntax wp -pname [parameter name] -value [parameter value]\n");
                }
            }


            /* Function to write parameter to its corresponding message frame and send message. */

            if (command == "wpsf")
            {
                int numberOfParameters = parameter.Length;

                if (numberOfParameters != 2)
                {
                    Console.WriteLine("\nError (Parameter Requirement): This command takes two arguments (-pid / -pname) and -value.\n");
                    return (proceed);
                }

                /* Format 1: wpsf -pname [parameter name] -value [parameter value] */

                if ((parameter[0] == "pname") && (parameter[1] == "value"))
                {
                    float pvalue;
                    bool isValidValue = float.TryParse(value[1], out pvalue);

                    if (isValidValue == false)
                    {
                        Console.WriteLine("\nError (Parameter Value): Parameter value must be a number.\n");
                        return (proceed);
                    }

                    if (isValidValue)
                    {
                        sdcp.writeParameterToFrame(pvalue, value[0]);
                        sdcp.writeFrame(sdcp.getParameterFID(value[0]), 0);
                    }
                }
                else
                {
                    Console.WriteLine("\nSyntax error: Use syntax wpsf -pname [parameter name] -value [parameter value]\n");
                }
            }


            /* Function to write frame data to the device via serial interface. */

            if (command == "wf")
            {
                int numberOfParameters = parameter.Length;

                if(numberOfParameters != 2)
                {
                    Console.WriteLine("\nError (Parameter requirement): This command requires two parameters.\n");
                }
                else
                {
                    /* Format 1: wf -fid [Frame ID] -priority [Frame priority] */

                    if ((parameter[0]=="fid") && (parameter[1]=="priority"))
                    {
                        int fid;
                        int priority;

                        bool isValidFID = int.TryParse(value[0], out fid);
                        bool isValidPriority = int.TryParse(value[1], out priority);

                        if(isValidFID == false)
                        {
                            Console.WriteLine("\nError (Parameter value): FID value must be an integer and between 0 to 127.");
                            return (proceed);
                        }

                        if(isValidPriority == false)
                        {
                            Console.WriteLine("\nError (Parameter value): Priority must take values 0 to high and 1 for low.\n");
                            return (proceed);
                        }

                        if( isValidFID & isValidPriority )
                        {
                            sdcp.writeFrame(fid, priority);
                        }
                    }

                    /* Format 2: wf -fname [Frame name] -priority [Frame priority] */

                    else if ((parameter[0] == "fname") && (parameter[1] == "priority"))
                    {
                        int priority;

                        bool isValidPriority = int.TryParse(value[1], out priority);

                        if (isValidPriority == false)
                        {
                            Console.WriteLine("\nError (Parameter value): Priority must take values 0 to high and 1 for low.\n");
                            return (proceed);
                        }

                        if (isValidPriority)
                        {
                            sdcp.writeFrame(value[0], priority);
                        }
                    }
                    else
                    {
                        Console.WriteLine("\nError (Incorrect syntax):Use syntax wf -fid [frame id] -priority [frame priority]");
                        Console.WriteLine("\nError (Incorrect syntax):Use syntax wf -fname [frame name] -priority [frame priority]\n");
                    }
                }
            }

            /* Function to add a paramter to the transmit list. */

            if( command == "aftxl" )
            {
                int numberOfParameters = parameter.Length;

                if(numberOfParameters != 2)
                {
                    Console.WriteLine("\nError (Parameter Requirement): Number of parameters for this command must be 2.\n");
                    return (proceed);
                }

                /* Format 1: aftxl -fid [frame ID] -txi [transmit interval] */

                if ((parameter[0] == "fid") & (parameter[1] == "txi"))
                {
                    int fid;
                    int txi;
                    bool isValidFID = int.TryParse(value[0], out fid);
                    bool isValidTXI  = int.TryParse(value[1], out txi);

                    if( !isValidFID )
                    {
                        Console.WriteLine("\nError (Parameter Value): FID must be an integer and take values from 0 to 127.\n");
                        return (proceed);
                    }

                    if (!isValidTXI)
                    {
                        Console.WriteLine("\nError (Parameter Value): Transmit interval must be an integer.\n");
                        return (proceed);
                    }

                    if(isValidFID & isValidTXI)
                    {
                        sdcp.addFrameToTransmitList(fid, (uint)txi);
                    }
                }
                else if ((parameter[0] == "fname") & (parameter[1] == "txi"))
                {
                    int txi;
                    bool isValidTXI = int.TryParse(value[1], out txi);

                    if (!isValidTXI)
                    {
                        Console.WriteLine("\nError (Parameter Value): Transmit interval must be an integer.\n");
                        return (proceed);
                    }

                    if (isValidTXI)
                    {
                        sdcp.addFrameToTransmitList(value[0], (uint)txi);
                    }
                }
                else
                {
                    Console.WriteLine("\nError (Incorrect syntax): Use one of these syntax:");
                    Console.WriteLine("aftxl -fid [frame ID] -txi [transmit interval]");
                    Console.WriteLine("aftxl -fname [frame name] -txi [transmit interval]\n");
                }

            }


            /* Function to add a paramter to the transmit list and update transmit list. */

            if (command == "aftxlu")
            {
                int numberOfParameters = parameter.Length;

                if (numberOfParameters != 2)
                {
                    Console.WriteLine("\nError (Parameter Requirement): Number of parameters for this command must be 1.\n");
                    return (proceed);
                }

                /* Format 1: aftxlu -fid [frame ID] -txi [transmit interval] */

                if ((parameter[0] == "fid") & (parameter[1] == "txi"))
                {
                    int fid;
                    int txi;
                    bool isValidFID = int.TryParse(value[0], out fid);
                    bool isValidTXI = int.TryParse(value[1], out txi);

                    if (!isValidFID)
                    {
                        Console.WriteLine("\nError (Parameter Value): FID must be an integer and take values from 0 to 127.\n");
                        return (proceed);
                    }

                    if (!isValidTXI)
                    {
                        Console.WriteLine("\nError (Parameter Value): Transmit interval must be an integer.\n");
                        return (proceed);
                    }

                    if (isValidFID & isValidTXI)
                    {
                        sdcp.addFrameToTransmitList(fid, (uint)txi);
                        sdcp.updateTransmitList();
                    }
                }
                else if ((parameter[0] == "fname") & (parameter[1] == "txi"))
                {
                    int txi;
                    bool isValidTXI = int.TryParse(value[1], out txi);

                    if (!isValidTXI)
                    {
                        Console.WriteLine("\nError (Parameter Value): Transmit interval must be an integer.\n");
                        return (proceed);
                    }

                    if (isValidTXI)
                    {
                        sdcp.addFrameToTransmitList(value[0], (uint)txi);
                        sdcp.updateTransmitList();
                    }
                }
                else
                {
                    Console.WriteLine("\nError (Incorrect syntax): Use one of these syntax:");
                    Console.WriteLine("aftxlu -fid [frame ID] -txi [transmit interval]");
                    Console.WriteLine("aftxlu -fname [frame name] -txi [transmit interval]\n");
                }

            }

            /* Function to change transmit mode. */

            if( command == "ctxm" )
            {
                int nParameters = parameter.Length;

                if(nParameters != 2)
                {
                    Console.WriteLine("\nError (Parameter Requirement): This command requires 2 parameters.\n");
                    return (proceed);
                }

                /* Format 1: ctxm -fid [frame ID] -tm [ 0 for triggered / 1 for cyclic ] */

                if ( (parameter[0] == "fid") && (parameter[1] == "tm") )
                {
                    int fid;
                    int tm;
                    bool isValidFID = int.TryParse(value[0], out fid);
                    bool isValidTM = int.TryParse(value[1], out tm);

                    if( !isValidFID )
                    {
                        Console.WriteLine("\nError (Parameter Value): FID must take integer value from 0 to 127.\n");
                        return (proceed);
                    }
                    if (!isValidTM)
                    {
                        Console.WriteLine("\nError (Parameter Value): Transmit mode must take values from 0 to 1, 0 for trigger and 1 for cyclic.\n");
                        return (proceed);
                    }

                    if(isValidFID & isValidTM)
                    {
                        sdcp.change_tx_mode(fid, tm == 1);
                    }
                }
                else if ((parameter[0] == "fname") && (parameter[1] == "tm"))
                {
                    int tm;
                    bool isValidTM = int.TryParse(value[1], out tm);

                    if (!isValidTM)
                    {
                        Console.WriteLine("\nError (Parameter Value): Transmit mode must take values from 0 to 1, 0 for trigger and 1 for cyclic.\n");
                        return (proceed);
                    }

                    if (isValidTM)
                    {
                        sdcp.change_tx_mode(value[0], tm == 1);
                    }
                }
                else
                {
                    Console.WriteLine("\nError (Incorrect syntax): Use one of the two syntax:");
                    Console.WriteLine("ctxm -fid [frame id] -tm [tranmit mode]");
                    Console.WriteLine("ctxm -fname [frame name] -tim [transmit mode]");
                    Console.WriteLine("\nNOTE: transmit mode = 0 --> triggered, 1 --> cyclic\n");
                    return (proceed);
                }
            }


            /* Function to change transmit interval. */

            if (command == "ctxi")
            {
                int nParameters = parameter.Length;

                if (nParameters != 2)
                {
                    Console.WriteLine("\nError (Parameter Requirement): This command requires 2 parameters.\n");
                    return (proceed);
                }

                /* Format 1: ctxm -fid [frame ID] -tm [ 0 for triggered / 1 for cyclic ] */

                if ((parameter[0] == "fid") && (parameter[1] == "txi"))
                {
                    int fid;
                    int txi;
                    bool isValidFID = int.TryParse(value[0], out fid);
                    bool isValidTXI = int.TryParse(value[1], out txi);

                    if (!isValidFID)
                    {
                        Console.WriteLine("\nError (Parameter Value): FID must take integer value from 0 to 127.\n");
                        return (proceed);
                    }
                    if (!isValidTXI)
                    {
                        Console.WriteLine("\nError (Parameter Value): Transmit interval must be an integer.\n");
                        return (proceed);
                    }

                    if (isValidFID & isValidTXI)
                    {
                        sdcp.change_tx_interval(fid, txi);
                    }
                }
                else if ((parameter[0] == "fname") && (parameter[1] == "txi"))
                {
                    int txi;
                    bool isValidTXI = int.TryParse(value[1], out txi);

                    if (!isValidTXI)
                    {
                        Console.WriteLine("\nError (Parameter Value): Transmit interval must be an integer.\n");
                        return (proceed);
                    }

                    if (isValidTXI)
                    {
                        sdcp.change_tx_interval(value[0], txi);
                    }
                }
                else
                {
                    Console.WriteLine("\nError (Incorrect syntax): Use one of the two syntax:");
                    Console.WriteLine("ctxm -fid [frame id] -txi [tranmit interval in ms]");
                    Console.WriteLine("ctxm -fname [frame name] -txi [transmit interval in ms]");
                    return (proceed);
                }
            }

            /* Function to write parameter to its corresponding message frame and update transmit list. */

            if (command == "wputxl")
            {
                int numberOfParameters = parameter.Length;

                if (numberOfParameters != 2)
                {
                    Console.WriteLine("\nError (Parameter Requirement): This command takes two arguments (-pid / -pname) and -value.\n");
                    return (proceed);
                }

                /* Format 1: wpsf -pname [parameter name] -value [parameter value] */

                if ((parameter[0] == "pname") && (parameter[1] == "value"))
                {
                    float pvalue;
                    bool isValidValue = float.TryParse(value[1], out pvalue);

                    if (isValidValue == false)
                    {
                        Console.WriteLine("\nError (Parameter Value): Parameter value must be a number.\n");
                        return (proceed);
                    }

                    if (isValidValue)
                    {
                        sdcp.writeParameterToFrame(pvalue, value[0]);
                        //sdcp.writeFrame(sdcp.getParameterFID(value[0]), 0);
                        sdcp.updateTransmitList();
                    }
                }
                else
                {
                    Console.WriteLine("\nSyntax error: Use syntax wpsf -pname [parameter name] -value [parameter value]\n");
                }
            }

            /* Function to trigger a transmit event. */

            if ( command == "txtrig" )
            {
                if (parameter[0] == "fid")
                {
                    sdcp.triggerTransmitEvent(int.Parse(value[0]));
                }
                else if (parameter[0] == "fname")
                {
                    sdcp.triggerTransmitEvent(value[0]);
                }
                else
                {
                    Console.WriteLine("\nError (Syntax Error): Use one of the following syntax:\n");
                    Console.WriteLine("txtrig -fid [frame id]");
                    Console.WriteLine("txtrig -fname [frane name]\n");
                }
            }

            /* Function to print transmit list. */

            if( command == "ptxl" )
            {
                sdcp.printTransmitList();
            }

            /* Function to update transmit list. */

            if ( command == "utxl" )
            {
                sdcp.updateTransmitList();
            }

            /* Function to start transmit thread. */

            if( command == "starttx" )
            {
                sdcp.start_tx();
            }

            /* Function to stop transmit thread. */

            if( command == "stoptx" )
            {
                sdcp.stop_tx();
            }

            /***************************  FRAME TRANSFER ON REQUEST   ***************************/

            if( command == "ftor" )
            {
                int nParameters = parameter.Length;

                if(nParameters<=1)
                {
                    Console.WriteLine("\nError (Parameter Requirement): Expected atleast 2 parameters.\n");
                    return (proceed);
                }

                if((parameter[0]=="timeout") && ( parameter[1] == "fid0" ))
                {
                    int timeout = 0;

                    if(int.TryParse(value[0],out timeout) == false)
                    {
                        Console.WriteLine("\nError (Parameter Value): Timeout value must be an integer.\n");
                        return (proceed);
                    }

                    int[] fids = new int[nParameters-1];

                    for(int i = 0; i < (nParameters-1); i++)
                    {
                        if( int.TryParse(value[i+1], out fids[i]) == false )
                        {
                            Console.WriteLine("\nError (Parameter value): FIDs must be integers.\n");
                            return (proceed);
                        }
                    }

                    sdcp.ftor(fids,(uint)timeout);
                }
                else if (parameter[1] == "fname0" )
                {
                    int timeout = 0;

                    if (int.TryParse(value[0], out timeout) == false)
                    {
                        Console.WriteLine("\nError (Parameter Value): Timeout value must be an integer.\n");
                        return (proceed);
                    }

                    string[] fnames = new string[nParameters - 1];

                    for (int i = 0; i < (nParameters - 1); i++)
                    {
                        fnames[i] = value[i + 1];
                        //Console.WriteLine(fnames[i]);
                    }

                    sdcp.ftor(fnames, (uint)timeout);
                }
                else
                {
                    Console.WriteLine("\nError (Syntax): Use one of the two formats:");
                    Console.WriteLine("ftor -timeout [timeout in ms] -fid0 [frame id 0] .... -fidn [frame id n]");
                    Console.WriteLine("ftor -timeout [timeout in ms] -fname0 [frame name 0] .... -fnamen [frame name n]\n");
                }
            }

            /* Clear received command. */

            command = "";

            return (proceed);
        }
    }
}
