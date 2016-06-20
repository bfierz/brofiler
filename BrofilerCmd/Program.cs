using System;
using System.IO;
using System.Net;
using System.Threading;

using Profiler.Data;

namespace Profiler
{
    class Program
    {
        static void Main(string[] args)
        {
            IPAddress ip = null;
            IPAddress.TryParse("127.0.0.1", out ip);

            ProfilerClient.Get().IpAddress = ip;
            ProfilerClient.Get().Port = 31313;

            // Collected frames
            FrameCollection frames = new FrameCollection();

            var connectionEstablished = false;
            do
            {
                System.Console.WriteLine("Trying to connect...");

                connectionEstablished = ProfilerClient.Get().CheckConnection(100);

                if (connectionEstablished)
                {
                    System.Console.WriteLine("Connected sucessfully.");
                }
                else
                {
                    System.Threading.Thread.Sleep(500);
                }
            }
            while (!connectionEstablished);

            // Begin profiling
            if (ProfilerClient.Get().SendMessage(new StartMessage()))
            {
                System.Console.WriteLine("Begin capturing...");

                /// TODO: Periodically send a stop message in order to dump the frames
                while (true)
                {
                    DataResponse response = ProfilerClient.Get().RecieveMessage();
                    if (response != null)
                    {
                        // Handle the response
                        if (response.Version == NetworkProtocol.NETWORK_PROTOCOL_VERSION)
                        {
                            switch (response.ResponseType)
                            {
                                case DataResponse.Type.ReportProgress:
                                    Int32 length = response.Reader.ReadInt32();
                                    System.Console.WriteLine("Progress: " + new String(response.Reader.ReadChars(length)));
                                    break;

                                case DataResponse.Type.NullFrame:
                                    lock (frames)
                                    {
                                        frames.Flush();
                                    }
                                    break;

                                case DataResponse.Type.Handshake:
                                    break;

                                default:
                                    lock (frames)
                                    {
                                        frames.Add(response.ResponseType, response.Reader);
                                    }
                                    break;
                            }
                        }
                    }
                    else
                    {
                        // Lost connection -> exit
                        if (!ProfilerClient.Get().IsConnected)
                            break;

                        // Wait until a packet was done
                        Thread.Sleep(1000);
                    }                    
                }

                System.Console.WriteLine("Done capturing.");

                // Connection was closed, save the data
                FileStream stream = new FileStream("Test.prof", FileMode.Create);
                frames.Serialize(stream);
            }

            // Close the connection
            ProfilerClient.Get().Close();
        }
    }
}
