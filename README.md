# CX4013-Distributed-Systems

To test the server codes, run:
```sh
g++ main.cpp account.cpp accountmanager.cpp handler.cpp transform.cpp udpserver.cpp monitor.cpp -o main
```

Then, the server can be started by running command:
```sh
./main <port> <mode> <failureRate>
```
port        : port number

mode        : modes for invocation semantics, 0 for at-least-once and 1 for at-most-once

failureRate : failure rate in percentage

The server must be running before starting client program. To compile the Java code, run command:
```sh
javac .\client\*.java .\client\handler\*.java
```

Then, the client can be started with command:
```sh
java client.UDPClient <ip> <port> <failurerate> <timeout> <max timeout> <mode>
```
