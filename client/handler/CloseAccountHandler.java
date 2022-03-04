package client.handler;

<<<<<<< HEAD
import java.io.UnsupportedEncodingException;
import java.util.Scanner;

public class CloseAccountHandler extends Handler {
=======
import java.io.*;
import java.lang.*;
import java.util.*;

public class CloseAccountHandler extends Handler{
    @Override
    public byte[] executeService(Scanner scanner, int id)throws UnsupportedEncodingException{
        System.out.println("Close Account:");
>>>>>>> bb37956502da48507a8bca5302ebb02ac5070cae

        // Enter Name
        System.out.print("Please enter your name: ");
        String name = scanner.nextLine();
        while(name.isEmpty()){
            System.out.println("Invalid input. Please try again.");
            System.out.print("Please enter your name: ");
            name = scanner.nextLine();
        }

        // Enter Password
        System.out.print("Please enter password: ");
        String password = scanner.nextLine();
        while(password.isEmpty()){
            System.out.println("Invalid input. Please try again.");
            System.out.print("Please enter password: ");
            password = scanner.nextLine();
        }

        // Enter Account Number
        System.out.println("Please enter your account number: ");
        Integer account = null;
        if(scanner.hasNextInt()){
            account = scanner.nextInt();
        }
        while(account == null){
            System.out.println("Invalid input. Please try again.");
            System.out.println("Please enter your account number: ");
            if(scanner.hasNextInt()){
                account = scanner.nextInt();
            }
        }

        // parent class method to construct and send;

        return new byte[0];
    }
<<<<<<< HEAD

    @Override
    public void executeService(Scanner scanner, int id) throws UnsupportedEncodingException {
        // TODO Auto-generated method stub
        
    }
    ;
=======
>>>>>>> bb37956502da48507a8bca5302ebb02ac5070cae
}
