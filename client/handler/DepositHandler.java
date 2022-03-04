package client.handler;
import java.util.Scanner;

public class DepositHandler{
    public static byte[] create(Scanner scanner, int currID) throws Exception{
        System.out.println("Enter account ID:");
        String msg = scanner.nextLine();
        int accountID = Integer.parseInt(msg);
        System.out.println("Enter amount:");
        msg = scanner.nextLine();
        double amount = Double.parseDouble(msg);
        byte[] packageByte = new byte[1024];
        packageByte[0] = (byte)currID;
        packageByte[1] = (byte)1;
        packageByte[2] = (byte)accountID;
        packageByte[3] = (byte)amount;
        return packageByte;
    }
}