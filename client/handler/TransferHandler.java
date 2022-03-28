package client.handler;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.Scanner;

import client.Constants;
import client.Utils;
/**
 * Handler class to handle transfer service
 */
public class TransferHandler extends Handler{
    /**
     * executeService method to handle get user input and package it into a byte array to be sent to server
     * @param scanner {@code Scanner} Scanner object for user input.
     * @param id {@code int} Id of the client.
     * @return packageByte {@code byte[]} Byte array of the package with user input marshalled.
     * @throws UnsupportedEncodingException
     */
    @Override
    public byte[] executeService(Scanner scanner, int currID)throws UnsupportedEncodingException{
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
        while(account == null || account < 0){
            System.out.println("Invalid input. Please try again.");
            System.out.println("Please enter your account number: ");
            if(scanner.hasNextInt()){
                account = scanner.nextInt();
            }
        }

        

        //Enter Target Name
        System.out.print("Please enter target name: ");
        String targetName = scanner.nextLine();
        while(targetName.isEmpty()){
            System.out.println("Invalid input. Please try again.");
            System.out.print("Please enter target name: ");
            targetName = scanner.nextLine();
        }


        // Enter Target Account Number
        System.out.println("Please enter target account number: ");
        Integer targetAccount = null;
        if(scanner.hasNextInt()){
            targetAccount = scanner.nextInt();
        }
        while(account == null || account < 0){
            System.out.println("Invalid input. Please try again.");
            System.out.println("Please enter target account number: ");
            if(scanner.hasNextInt()){
                targetAccount = scanner.nextInt();
            }
        }

        // Enter Transfer Amount
        System.out.print("Please enter transfer amount: ");
        Float amount = null;
        if(scanner.hasNextFloat()){
            amount = scanner.nextFloat();
        }
        while(amount == null || amount < 0){
            System.out.println("Invalid input. Please try again.");
            System.out.println("Please enter transfer amount: ");
            if(scanner.hasNextFloat()){
                amount = scanner.nextFloat();
            }
        }

        int size = name.length() + password.length() + targetName.length() + Constants.INT_SIZE*7 + Constants.FLOAT_SIZE;
        byte[] packageByte = new byte[size];
        int index = 0;
        Utils.marshal(currID, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(5, packageByte, index);
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
        Utils.marshal(targetName.length(), packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(targetName, packageByte, index);
        index += targetName.length();
        Utils.marshal(targetAccount, packageByte, index);
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
            System.out.println("Transfer Failed");
            int errorsize = Utils.unmarshalInteger(response, index);
            index += Constants.INT_SIZE;
            //System.out.println(errorsize);
            String error = Utils.unmarshalString(response, index, index+errorsize); index += errorsize;
            System.out.println(error);
        }
        else{
            System.out.println("Transfer Successful");
            int currency = Utils.unmarshalInteger(response, index); index += Constants.INT_SIZE;
            System.out.println("Currency: "+ currency);
            Float balance = Utils.unmarshalFloat(response, index);
            System.out.println("Balance: "+ balance);
        }
    }


}
