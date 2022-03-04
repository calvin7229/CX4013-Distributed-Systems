import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.*;

import handler.*;

class UDPClient{
    private int idCounter;

    public UDPClient(String ip, int port) throws SocketException,UnknownHostException{
        this.idCounter = 0;


    }

    public int getID(){
        this.idCounter++;
        return this.idCounter;
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

        switch (selection){
            case 1:
                packageByte = OpenAccountHandler.create(scanner,currID);
                break;
            case 2:
                packageByte = DepositHandler.create(scanner,currID);
                break;
            case 3:
                packageByte = WithdrawHandler.create(scanner,currID);
                break;
            case 4:
                packageByte = TransferHandler.create(scanner,currID);
                break;
            case 5:
                packageByte = CheckBalanceHandler.create(scanner,currID);
                break;
            case 6:
                packageByte = CloseAccountHandler.create(scanner,currID);
                break;
            case 7:
                System.out.println("Exit");
                exit = true;
                break;
            default:
                System.out.println("Invalid selection");
                break;
            }

        }

    }
}
