package client.handler;

import java.io.*;
import java.lang.*;
import java.util.*;
import client.Constants;


public class DepositHandler extends Handler {
    
    @Override
    public byte[] executeService(Scanner scanner, int id)throws UnsupportedEncodingException{
        System.out.println("Deposit Money to Account:");

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

        // Enter Starting Balance
        System.out.print("Please enter deposit amount: ");
        Float balance = null;
        if(scanner.hasNextFloat()){
            balance = scanner.nextFloat();
        }
        while(balance == null){
            System.out.println("Invalid input. Please try again.");
            System.out.println("Please enter deposit amount: ");
            if(scanner.hasNextFloat()){
                balance = scanner.nextFloat();
            }
        }


        // parent class method to construct and send;

        return new byte[0];
    }

    
}