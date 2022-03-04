import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.*;

import handler.*;

class UDPClient{

    // public UDPClient(String ip, int port) throws SocketException,UnknownHostException{

    // }
    public static void main(String[] args) throws Exception{
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
