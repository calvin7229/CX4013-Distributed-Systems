package client.handler;

import java.io.*;
import java.lang.*;
import java.util.*;
import client.Constants;
import client.Utils;


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

        // Enter Deposit Amount
        System.out.print("Please enter deposit amount: ");
        Float amount = null;
        if(scanner.hasNextFloat()){
            amount = scanner.nextFloat();
        }
        while(amount == null){
            System.out.println("Invalid input. Please try again.");
            System.out.println("Please enter deposit amount: ");
            if(scanner.hasNextFloat()){
                amount = scanner.nextFloat();
            }
        }


        int size = name.length() + password.length() + Constants.INT_SIZE*5 + Constants.FLOAT_SIZE;
        byte[] packageByte = new byte[size];
        int index = 0;
        Utils.marshal(id, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(3, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(name.length(), packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(name, packageByte, index);
        index += name.length();
        Utils.marshal(password.length(), packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(password, packageByte, index);
        index += password.length();
        Utils.marshal(account, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(amount, packageByte, index);
        index += Constants.FLOAT_SIZE;

        return packageByte;
    }
    @Override
    public void handleResponse(byte[] response) throws IOException{
        int index = 0;
        int id = Utils.unmarshalInteger(response, index);
        System.out.println(id);
        index += Constants.INT_SIZE;
        String status = Utils.unmarshalString(response, index,index+1);
        //System.out.println(index);
        index += 1;
        
        if(status.charAt(0) == '0'){
            System.out.println("Deposit Failed");
            int errorsize = Utils.unmarshalInteger(response, index);
            index += Constants.INT_SIZE;
            System.out.println(errorsize);
            String error = Utils.unmarshalString(response, index, index+errorsize); index+=errorsize;
            System.out.println(error);
        }
        else{
            System.out.println("Deposit Successful");
            //todo print acc balance
            int currsize = Utils.unmarshalInteger(response, index); index += Constants.INT_SIZE;
            int currency = Utils.unmarshalInteger(response, index); index += Constants.INT_SIZE;
            System.out.println("Currency: "+ currency);
            int balsize = Utils.unmarshalInteger(response, index);  index += Constants.INT_SIZE;
            Float balance = Utils.unmarshalFloat(response, index);
            System.out.println("Balance: "+ balance);
        }
    }
}
