package client.handler;

import java.io.*;
import java.lang.*;
import java.util.*;
import client.Constants;
import client.Utils;

public class CheckBalanceHandler extends Handler{
    /**
     * Execute service and return package
     * @param scanner
     * @param id
     * @return {@code byte[]} request package to be sent to server
     * @throws UnsupportedEncodingException
     */
    @Override
    public byte[] executeService(Scanner scanner, int id)throws UnsupportedEncodingException{
        System.out.println("Check Account Balance:");

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
        System.out.print("Please enter your account number: ");
        Integer account = null;
        while(account == null || account < 0){
            String temp = scanner.nextLine();
            try{
                account = Integer.parseInt(temp);
                if(account < 0){
                    throw new IllegalArgumentException();
                }
            }catch(IllegalArgumentException e){
                System.out.println("Invalid input. Please try again.");
                System.out.print("Please enter your account number: ");
            }
        }

        int size = name.length() + password.length() + Constants.INT_SIZE*6;
        byte[] packageByte = new byte[size];
        int index = 0;
        Utils.marshal(id, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(6, packageByte, index);
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

        return packageByte;
    }
    /**
     * Handle the response from the server
     * @param response from the server
     * @throws IOException
     * 
     * 
     */
    @Override
    public void handleResponse(byte[] response) throws IOException{
        int index = 0;
        int id = Utils.unmarshalInteger(response, index);
        //System.out.println(id);
        index += Constants.INT_SIZE;
        String status = Utils.unmarshalString(response, index,index+1);
        //System.out.println(index);
        index += 1;
        
        if(status.charAt(0) == '0'){
            System.out.println("Failed to get account balance");
            int errorsize = Utils.unmarshalInteger(response, index);
            index += Constants.INT_SIZE;
            //System.out.println(errorsize);
            String error = Utils.unmarshalString(response, index, index+errorsize);
            System.out.println(error);
        }
        else{
            System.out.println("Account Status:");
            int currency = Utils.unmarshalInteger(response, index); index += Constants.INT_SIZE;
            System.out.println("Currency: "+ currency);
            Float balance = Utils.unmarshalFloat(response, index);
            System.out.println("Balance: "+ balance);
        }
    }
}