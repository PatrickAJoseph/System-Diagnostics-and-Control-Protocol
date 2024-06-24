
using SDCP_API;
using stringUtils;
using CLI_Interface;

namespace Application
{
    class MainClass
    {
        static void Main(string[] args)
        {
            //SDCP_API.SDCP_API sdcp = new SDCP_API.SDCP_API();
            CLI_Interface.CLI_Interface CLI = new CLI_Interface.CLI_Interface();

            bool proceed = true;
            string CLI_string = "";

            //sdcp.openSerialPort(500);
            //sdcp.closeSerialPort();

            while (proceed)
            {
                Console.Write("\nSDCP >>>  ");
                CLI_string = Console.ReadLine();
                //Console.WriteLine(CLI_string.Length);
                CLI.extract(CLI_string);
                proceed = CLI.interpret();
            }

            //CLI.extract("sdcp opensession -port COM7 -baudrate 115200");

            //sdcp.loadParameterFile("E:\\Projects\\System Diagnostics and Control Protocol\\SDCP_LabVIEW\\Parameter Files\\Motion_Processing.param");

            //sdcp.openSerialPort();
            //sdcp.sync();

            /*

            sdcp.listAvailablePorts();
            sdcp.openSerialPort("COM7", 115200);
            sdcp.sync();

            */

             /*      

            string[] parameters = { 
                                    "IMU_XACCEL_Processed", 
                                    "IMU_YACCEL_Processed", 
                                    "IMU_ZACCEL_Processed",
                                    "XACCEL_raw",
                                    "YACCEL_raw",
                                    "ZACCEL_raw"
                                  };

            sdcp.liveStreamParameters(parameters,200,25000);

            int[] ftorFIDs = { 7,8,9 };

            sdcp.ftor(ftorFIDs);

            */

            /*

            sdcp.writeParameterToFrame(1, "LED_OVRRD");
            sdcp.writeParameterToFrame(0, "XLED_state");
            sdcp.writeParameterToFrame(0, "YLED_state");
            sdcp.writeParameterToFrame(1, "ZLED_state");
            sdcp.writeFrame(2, 0);
            sdcp.delay(1000);

            for (int i = 0; i < 100; i++)
            {
                //int duty = 10 * (i + 1);
                sdcp.writeParameterToFrame(1, "LED_OVRRD");
                sdcp.writeParameterToFrame(1, "XLED_PWM_Mode");
                sdcp.writeParameterToFrame(1, "YLED_PWM_Mode");
                sdcp.writeParameterToFrame(1, "ZLED_PWM_Mode");
                sdcp.writeParameterToFrame(i, "XLED_Brightness");
                sdcp.writeParameterToFrame((i+33)%100, "YLED_Brightness");
                sdcp.writeParameterToFrame((i+66)%100, "ZLED_Brightness");
                sdcp.writeFrame(2, 0);
                sdcp.delay(100);
            }

            sdcp.closeSerialPort();

            //sdcp.start_xmt();
            //sdcp.delay(5000);
            //sdcp.stop_xmt();


            /*

            sdcp.addFrameToTransmitList(2, 500);
            //sdcp.addFrameToTransmitList(3, 500);
            //sdcp.addFrameToTransmitList(4, 250);
            //sdcp.addFrameToTransmitList(5, 375);
            sdcp.change_tx_mode(2, true);
            sdcp.change_tx_mode(3, true);
            sdcp.change_tx_mode(4, true);
            sdcp.writeParameterToFrame(1, "LED_OVRRD");
            sdcp.updateTransmitList();

            sdcp.start_tx();

            for (int i = 0; i < 8; i++)
            {
                sdcp.writeParameterToFrame((i & ( 1 << 0)) >> 0, "XLED_state");
                sdcp.writeParameterToFrame((i & (1 << 1)) >> 1, "YLED_state");
                sdcp.writeParameterToFrame((i & (1 << 2)) >> 2, "ZLED_state");
                sdcp.updateTransmitList();
                sdcp.delay(1000);

                if( i == 4 )
                {
                    sdcp.tx_trigger(5);
                }
            }
            
            sdcp.stop_tx();

            */

            /*

            sdcp.start_rx();
            sdcp.delay(2500);
            sdcp.print_detailed_receive_frame_info();
            sdcp.delay(2500);
            sdcp.print_detailed_receive_frame_info();
            sdcp.delay(2500);
            sdcp.print_detailed_receive_frame_info();
            sdcp.stop_rx();

            */
        }
    }
}