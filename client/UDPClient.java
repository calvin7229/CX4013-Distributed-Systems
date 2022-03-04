package client;

import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.util.*;

import client.handler.*;

class UDPClient{
    private int idCounter;

    public UDPClient(String ip, int port) throws SocketException,UnknownHostException{
        this.idCounter = 0;


    }

    public int getID(){
        this.idCounter++;
        return this.idCounter;
    }
    private byte[] SendAndReceive(byte[] packageByte) {
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
    private void Send(byte[] packageByte) {
    }

    private byte[] Receive(boolean isBlocking) {
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

        byte[] packageByte;
        int currID = udpclient.getID();
        boolean send= true;
        Handler handler = null;

        switch (selection){
            
            case 1:
                handler = new OpenAccountHandler();
                packageByte = OpenAccountHandler.create(scanner,currID);
                break;
            case 2:
                handler = new DepositHandler();
                packageByte = DepositHandler.create(scanner,currID);
                break;
            case 3:
                handler = new WithdrawHandler();
                packageByte = WithdrawHandler.create(scanner,currID);
                break;
            case 4:
                handler = new TransferHandler();
                packageByte = TransferHandler.create(scanner,currID);
                break;
            case 5:
                handler = new CheckBalanceHandler();
                packageByte = CheckBalanceHandler.create(scanner,currID);
                break;
            case 6:
                handler = new CloseAccountHandler();
                packageByte = CloseAccountHandler.create(scanner,currID);
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
        }
        }

            

        }

    

    }
}
