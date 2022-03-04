package client.handler;

import java.io.*;
import java.lang.*;
import java.util.*;
import client.Constants;

public class ChangePasswordHandler extends Handler {
    
    @Override
    public byte[] executeService(Scanner scanner, int id)throws UnsupportedEncodingException{
        System.out.println("Change Account Password:");

        // Enter Name
        System.out.print("Please enter your name: ");
        String name = scanner.nextLine();
        while(name.isEmpty()){
            System.out.println("Invalid input. Please try again.");
            System.out.print("Please enter your name: ");
            name = scanner.nextLine();
        }

        // Enter Old Password
        System.out.print("Please enter old password: ");
        String oldPassword = scanner.nextLine();
        while(oldPassword.isEmpty()){
            System.out.println("Invalid input. Please try again.");
            System.out.print("Please enter old password: ");
            oldPassword = scanner.nextLine();
        }

        // Enter New Password
        System.out.print("Please enter new password: ");
        String newPassword = scanner.nextLine();
        while(newPassword.isEmpty()){
            System.out.println("Invalid input. Please try again.");
            System.out.print("Please enter new password: ");
            newPassword = scanner.nextLine();
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
    
}
