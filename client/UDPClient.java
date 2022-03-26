package client;

import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.TimeoutException;

import client.handler.*;

class UDPClient{
    private int idCounter;
    private int timeout = 2000;
    private DatagramSocket clientSocket;
    private InetAddress IP;
    private int port;
    private float failrate=0.0f;
    private int seminvo = 2; //semantic invocation, 1: At Most once, 2: At Least once
    private int maxTimeout = 0;
    public UDPClient(String ip, int port) throws SocketException,UnknownHostException{
        this.idCounter = 0;
        this.IP = InetAddress.getByName(ip);
        this.port = port;
        this.clientSocket = new DatagramSocket();

    }

    public int getID(){
        this.idCounter++;
        return this.idCounter;
    }

    public void setTimeout(int timeout) throws SocketException{
        clientSocket.setSoTimeout(timeout);
        this.timeout = timeout;
    }

    private byte[] SendAndReceive(byte[] packageByte) throws IOException, InterruptedException,TimeoutException{
        byte[] response = new byte[0];
        int timeoutCount = 0;
        do{
            try{
                this.Send(packageByte);
                response = this.Receive(false);
                break;
            }catch(SocketTimeoutException e){
            System.out.printf("Timeout!",++timeoutCount);
            //Check if the current timeout times exceed the maximum timeout allowed
            if (this.maxTimeout >0 && timeoutCount >=this.maxTimeout){
                throw new TimeoutException("Max timeout reached!");
            }
            }
            continue;
        } while(this.seminvo >=1);
        return response;
    }

    //@TODO Send and Receive functions
    private void Send(byte[] packageByte) throws IOException, InterruptedException{
        if (Math.random()<this.failrate){
            System.out.println("Simulated Failure");
            return;
        }
        byte[] header = new byte[Constants.INT_SIZE];
        Utils.marshal(packageByte.length,header,0);
        System.out.println(header);
        DatagramPacket headerPacket;
        // try{
        headerPacket = new DatagramPacket(header, header.length, this.IP, this.port);
        this.clientSocket.send(headerPacket);
        DatagramPacket sendPacket = new DatagramPacket(packageByte, packageByte.length, this.IP, this.port);
        this.clientSocket.send(sendPacket);
    }

    private byte[] Receive(boolean monitor) throws IOException, InterruptedException,TimeoutException{
        //Get header
        
        byte[] header = new byte[4];
        DatagramPacket headerPacket = new DatagramPacket(header, Constants.INT_SIZE);
        this.clientSocket.receive(headerPacket);
        header = headerPacket.getData();
        int length = Utils.unmarshalInteger(header,0);



        //Get body
        byte[] response = new byte[length];
        DatagramPacket responsePacket = new DatagramPacket(response, response.length);
        this.clientSocket.receive(responsePacket);
        response = responsePacket.getData();

        return response;

    }

    //@TODO settings
    //private void Settings()
    public static void main(String[] args) throws Exception{
        
        UDPClient udpclient = new UDPClient("10.27.248.51",8888);
        udpclient.failrate = Float.parseFloat(args[0]);
        udpclient.setTimeout(Integer.parseInt(args[1]));
        udpclient.maxTimeout = Integer.parseInt(args[2]);

        boolean exit = false;
        while(!exit){
        System.out.println(
            "Please enter selection:\n" +
            "1. Open New Account\n" +
            "2. Deposit\n" +
            "3. Withdraw\n" +
            "4. Transfer\n" +
            "5. Check Balance\n" +
            "6. Close Account\n" +
            "7. Exit");

        
        
        //Temp variables and buffers for passing parameters
        Scanner scanner = new Scanner(System.in);
        String msg = scanner.nextLine();
        int selection = Integer.parseInt(msg);
        byte[] packageByte = null;
        int currID = udpclient.getID();
        boolean send= true;
        Handler handler = null;

        switch (selection){
            
            case 1:
                handler = new OpenAccountHandler();
                break;
            case 2:
                handler = new DepositHandler();
                break;
            case 3:
                handler = new WithdrawHandler();
                break;
            case 4:
                handler = new TransferHandler();
                break;
            case 5:
                handler = new CheckBalanceHandler();
                break;
            case 6:
                handler = new CloseAccountHandler();
                break;
            case 7:
                System.out.println("Exit");
                exit = true;
                send = false;
                break;
            default:
                System.out.println("Invalid selection");
                send = false;
                break;
        }
        if (send){
            
            packageByte = handler.executeService(scanner, currID);
            byte[] receivedbytes = udpclient.SendAndReceive(packageByte);
            handler.handleResponse(receivedbytes);
        }
        

            

        }

    

    }
}
