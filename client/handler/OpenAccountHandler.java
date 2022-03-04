package client.handler;

import java.io.*;
import java.lang.*;
import java.util.*;
import client.Constants;

public class OpenAccountHandler extends Handler {
    
    @Override
    public byte[] executeService(Scanner scanner, int id)throws UnsupportedEncodingException{
        System.out.println("Open Account:");

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

        // Enter Default Currency
        System.out.println("Please choose your account currency: ");
        for (int i = 1; i < Constants.CURRENCY_STR.length; i++){
            System.out.printf("%d. %s\n", i, Constants.CURRENCY_STR[i]);
        }
        int currency = 0;
        if(scanner.hasNextInt()){
            currency = scanner.nextInt();
        }
        while(currency < 1 || currency >= Constants.CURRENCY_STR.length){
            System.out.println("Invalid input. Please try again.");
            System.out.println("Please choose your account currency: ");
            if(scanner.hasNextInt()){
                currency = scanner.nextInt();
            }
        }
        
        // Enter Starting Balance
        System.out.print("Please enter account balance: ");
        Float balance = null;
        if(scanner.hasNextFloat()){
            balance = scanner.nextFloat();
        }
        while(balance == null){
            System.out.println("Invalid input. Please try again.");
            System.out.println("Please enter account balance: ");
            if(scanner.hasNextFloat()){
                balance = scanner.nextFloat();
            }
        }


        // parent class method to construct and send;

        return new byte[0];
    }

    public static byte[] create(Scanner scanner, int currID) {
        return null;
    }
    
}
