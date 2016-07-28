using System;
using System.Net.Sockets;
using System.Net;
using System.IO;
using System.Windows;

namespace Profiler
{
    public class ProfilerClient
    {
        private Object criticalSection = new Object();
        private static ProfilerClient profilerClient = new ProfilerClient();

        public IPAddress IpAddress
        {
            get { return ipAddress; }
            set { ipAddress = value; }
        }

        public int Port
        {
            get { return port; }
            set { port = value; }
        }

        public static ProfilerClient Get() { return profilerClient; }

        TcpClient client = new TcpClient();

        #region SocketWork

        public bool IsConnected
        {
            get { return client.Connected; }
        }

        public DataResponse RecieveMessage()
        {
            try
            {
                NetworkStream stream = null;

                lock (criticalSection)
                {
                    if (!client.Connected)
                        return null;

                    stream = client.GetStream();
                }

                return DataResponse.Create(stream);
            }
            catch (System.IO.IOException ex)
            {
                if (MessageBox.Show(ex.Message) == MessageBoxResult.OK)
                {
                    lock (criticalSection)
                    {
                        client = new TcpClient();
                    }
                }
            }

            return null;
        }

        /// <summary>
        /// IP address of the server
        /// </summary>
        private IPAddress ipAddress;

        /// <summary>
        /// Port to which the client connects
        /// </summary>
        private int port = -1;

        /// <summary>
        /// Range of ports to test
        /// </summary>
        const int PORT_RANGE = 8;

        /// <summary>
        /// Check connection to server, if not connected, try to establish connection
        /// </summary>
        /// <param name="timeout">Timeout for connection building</param>
        /// <returns>true, if the connection is already or could be established.</returns>
        public bool CheckConnection(int timeout)
        {
            lock (criticalSection)
            {
                if (!client.Connected)
                {
                    // Test a range of ports to connect to the server
                    for (int currentPort = port; currentPort < port + PORT_RANGE; ++currentPort)
                    {
                        try
                        {
                            if (timeout > 0)
                            {
                                var result = client.BeginConnect(ipAddress, currentPort, null, null);
                                var success = result.AsyncWaitHandle.WaitOne(timeout);

                                client.EndConnect(result);
                                if (!success)
                                {
                                    client.Close();
                                }
                                return success;
                            }
                            else
                            {
                                client.Connect(new IPEndPoint(ipAddress, currentPort));
                                return true;
                            }
                        }
                        catch (SocketException)
                        {
                            // Failed to connect
                        }
                    }
                }
            }

            return false;
        }
        
        public bool SendMessage(Message message)
        {
            try
            {
                CheckConnection(0);

                lock (criticalSection)
                {
                    MemoryStream buffer = new MemoryStream();
                    message.Write(new BinaryWriter(buffer));
                    buffer.Flush();

                    UInt32 length = (UInt32)buffer.Length;

                    NetworkStream stream = client.GetStream();

                    BinaryWriter writer = new BinaryWriter(stream);
                    writer.Write(Message.MESSAGE_MARK);
                    writer.Write(length);

                    buffer.WriteTo(stream);
                    stream.Flush();
                }

                return true;
            }
            catch (Exception ex)
            {
                if (MessageBox.Show(ex.Message) == MessageBoxResult.OK)
                {
                    lock (criticalSection)
                    {
                        //client.Client.Shutdown(SocketShutdown.Both);
                        client = new TcpClient();
                    }
                }
            }

            return false;
        }

        public void Close()
        {
            lock (criticalSection)
            {
                if (client != null)
                {
                    client.Close();
                    client = null;
                }
            }
        }
        
        #endregion
    }
}
