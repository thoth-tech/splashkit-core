using SplashKitSDK;
using static SplashKitSDK.SplashKit;

const int Server1Port = 5000;
const int Server2Port = Server1Port + 1;

ServerSocket svr;
Connection lConA, lConB, lTmp;
Message msg;

void PauseTest()
{
    Console.WriteLine("Press any key to continue");
    Console.ReadLine();
}

void CheckMessages()
{
    Console.WriteLine("Checking for messages");
    CheckNetworkActivity();

    if (HasMessages())
    {
        Console.WriteLine("Reading messages received for client");
        while (HasMessages(lConA))
        {
            msg = ReadMessage(lConA);
            Console.WriteLine($" -> {MessageData(msg)} from {MessageHost(msg)}:{MessagePort(msg)}");
            CloseMessage(msg);
        }

        Console.WriteLine("Reading messages received for ToSvr client");
        while (HasMessages("ToSvr"))
        {
            msg = ReadMessage("ToSvr");
            Console.WriteLine($" -> {MessageData(msg)} from {MessageHost(msg)}:{MessagePort(msg)}");
            CloseMessage(msg);
        }

        for (int i = 0; i < ConnectionCount(svr); ++i)
        {
            lTmp = RetrieveConnection(svr, i);
            if (HasMessages(lTmp))
            {
                Console.WriteLine($" -> {ReadMessageData(lTmp)} from {ConnectionPort(lTmp)}");
            }
        }

        PauseTest();
    }
    else
    {
        Console.WriteLine("No messages received");
    }
}

Console.WriteLine("Starting");

svr = CreateServer("svr1", Server1Port);
CreateServer("svr2", Server2Port);

Console.WriteLine($"Listening on {Server1Port} and {Server2Port}.");
PauseTest();

Console.WriteLine($"Connecting to Port (x2)  {Server1Port}");
lConA = OpenConnection("lConA", "127.0.0.1", Server1Port);
OpenConnection("ToSvr", "127.0.0.1", Server1Port);

Console.WriteLine($"ToSvr is open:  {IsConnectionOpen("ToSvr")}");
Console.WriteLine("Fred is open? Expect warning/false:");
Console.WriteLine(IsConnectionOpen("Fred"));

CheckNetworkActivity();

Console.WriteLine($"Are there new connections?  {HasNewConnections()}");
Console.WriteLine($"New Connection to {Server1Port}:{ServerHasNewConnection(svr)}");
Console.WriteLine($" Number of connections:  {ConnectionCount(svr)}");

lConB = LastConnection(svr);

CheckNetworkActivity();
Console.WriteLine($"Are there new connections?  {HasNewConnections()}");
Console.WriteLine($"New Connection to {Server1Port}:{ServerHasNewConnection(svr)}");
Console.WriteLine($" Number of connections: {ConnectionCount(svr)}");

PauseTest();
Console.WriteLine("Checking for messages -- shouldn't be any");
CheckMessages();

Console.WriteLine("Sending messages");
SendMessageTo("To server --> from client", lConA);
SendMessageTo("How are you :)", lConA);
SendMessageTo("To server --> from named client", "ToSvr");
SendMessageTo(new string('7', 509 - 4), lConA);
SendMessageTo("1234567", lConA);
SendMessageTo("0987654", lConA);
SendMessageTo(new string('A', 876), lConA);

SendMessageTo(new string('7', 509 - 4), lConB);
SendMessageTo("Hello Client", lConB);
SendMessageTo(new string('A', 876), lConB);

PauseTest();
CheckMessages();
CheckMessages();
CheckMessages();

Console.WriteLine($"Closing client -  {CloseConnection(lConA)}");
PauseTest();

Console.WriteLine($"Client still:  {lConA}");
Console.WriteLine($"Test message send (to closed client):  {SendMessageTo(new string('A', 876), lConB)}");

PauseTest();
Console.WriteLine($"Test message send (expect false):  {SendMessageTo(new string('A', 876), lConB)}");

Console.WriteLine($"Server still connected to client:  {IsConnectionOpen(RetrieveConnection(svr, 0))}");

Console.WriteLine($"Closing server's client connection (should already by closed -- just testing):  {CloseConnection(lConB)}");
Console.WriteLine($"Server connections:  {ConnectionCount(svr)}");
PauseTest();

Console.WriteLine("Checking for activity");
CheckNetworkActivity();

Console.WriteLine("Opening a new connection");

lConA = OpenConnection("lConA1", "127.0.0.1", Server1Port);
Console.WriteLine($"Reconnected:  {lConA}");

CheckNetworkActivity();
lConB = LastConnection(svr);
Console.WriteLine($"Connections =  {ConnectionCount(svr)}");

SendMessageTo("New connection --> to server", lConA);
SendMessageTo("New connection --> to client", lConB);

CheckMessages();

Console.WriteLine($"Closing server:  {CloseServer(svr)}");
PauseTest();

Console.WriteLine("Can connect to old server? Expect error/false:");
Console.WriteLine(OpenConnection("oldsvr", "127.0.0.1", Server1Port) != null);
PauseTest();

Console.WriteLine("Restarting server...");
svr = CreateServer("svr1", Server1Port);

Reconnect(lConA);
Reconnect("ToSvr");

CheckNetworkActivity();
lConB = LastConnection(svr);

CheckNetworkActivity();

BroadcastMessage("Hello Everyone");
BroadcastMessage("Hello Everyone on svr", svr);

SendMessageTo("Another message --> to server", lConA);
SendMessageTo("Another message --> to client", lConB);
SendMessageTo("Another message --> from named client", "ToSvr");
SendMessageTo("Another message --> to named client", LastConnection(svr));

CheckMessages();
CheckMessages();

Console.WriteLine("Close all");
CloseAllConnections();
CloseAllServers();
