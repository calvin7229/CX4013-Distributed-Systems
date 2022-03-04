package client;

import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.TimeoutException;

import client.handler.*;

class UDPClient{
    private int idCounter;
    private int timeout;
    private DatagramSocket clientSocket;
    
    public UDPClient(String ip, int port) throws SocketException,UnknownHostException{
        this.idCounter = 0;


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
                //@TODO: Handle timeout
            }
        } while(false);
        return response;
    }

    //@TODO Send and Receive functions
    private void Send(byte[] packageByte) throws IOException, InterruptedException{
    }

    private byte[] Receive(boolean isBlocking) throws IOException, InterruptedException,TimeoutException{
        byte[] header = new byte[4];
        DatagramPacket headerPacket = new DatagramPacket(new byte[0], 0);
        this.clientSocket.receive(headerPacket);
        return new byte[0];
    }

    public static void main(String[] args) throws Exception{

        UDPClient udpclient = new UDPClient("",0);

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
                packageByte = handler.executeService(scanner,currID);
                break;
            case 2:
                handler = new DepositHandler();
                packageByte = handler.executeService(scanner,currID);
                break;
            case 3:
                handler = new WithdrawHandler();
                packageByte = handler.executeService(scanner,currID);
                break;
            case 4:
                handler = new TransferHandler();
                packageByte = handler.executeService(scanner,currID);
                break;
            case 5:
                handler = new CheckBalanceHandler();
                packageByte = handler.executeService(scanner,currID);
                break;
            case 6:
                handler = new CloseAccountHandler();
                packageByte = handler.executeService(scanner,currID);
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
            byte[] receivedbytes = udpclient.SendAndReceive(packageByte);
            handler.executeService(scanner, currID);
        }
        

            

        }

    

    }
}
