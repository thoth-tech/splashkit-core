using SplashKitSDK;
using static SplashKitSDK.SplashKit;

const int ListenPortB = 5000;

void PauseUdpTest()
{
    Console.WriteLine("Press any key to continue");
    Console.ReadLine();
}

Console.WriteLine($"Listening for UDP connections on port {ListenPortB}");
ServerSocket server = CreateServer("MyServer", ListenPortB, ConnectionType.UDP);
PauseUdpTest();

Console.WriteLine("Creating connection to send data to server");
Connection toServer = OpenConnection("to_server", "127.0.0.1", ListenPortB, ConnectionType.UDP);
PauseUdpTest();

Console.WriteLine("Sending message to server");
SendMessageTo("Hello UDP", toServer);
Delay(1000);

Console.WriteLine("Checking activity");
CheckNetworkActivity();
Console.WriteLine($"Server got message: {HasMessages(server)}");
Message msg = ReadMessage(server);
Console.WriteLine($"Message: {MessageData(msg)}");
PauseUdpTest();

Console.WriteLine($"Sending message to client {MessageHost(msg)}:{MessagePort(msg)}");
Connection toClient = OpenConnection("to_client", MessageHost(msg), MessagePort(msg), ConnectionType.UDP);
CloseMessage(msg);

Console.WriteLine("Connection created");
SendMessageTo("Hello Client", toClient);
Delay(100);
CheckNetworkActivity();

Console.WriteLine($"Client got message: {HasMessages("to_server")}");
Console.WriteLine($"Message: {ReadMessageData(toServer)}");

CloseConnection(toServer);

Console.WriteLine($"Closing UDP socket on port {ListenPortB}");
CloseServer(server);

Console.WriteLine("Close all");
CloseAllConnections();
CloseAllServers();
