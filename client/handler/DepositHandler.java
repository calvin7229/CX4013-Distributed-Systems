package client.handler;

import java.io.*;
import java.lang.*;
import java.util.*;
import client.Constants;
import client.Utils;


public class DepositHandler extends Handler {
    

    /**
     * Get user input and and marshall into packet to be sent to server
     * @param scanner {@code Scanner} object to read input from user.
     * @param id {@code int} id of the request.
     * @return {@code byte[]} body package to be sent to server.
     * @throws UnsupportedEncodingException
     */
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
        while(account == null || account < 0){
            String temp = scanner.nextLine();
            try{
                account = Integer.parseInt(temp);
                if(account < 0){
                    throw new IllegalArgumentException();
                }
            }catch(IllegalArgumentException e){
                System.out.println("Invalid input. Please try again.");
                System.out.print("Please choose your account number: ");
            }
        }

        // Enter currency
        System.out.println("Please choose your account currency: ");
        for (int i = 1; i < Constants.CURRENCY_STR.length; i++){
            System.out.printf("%d. %s\n", i, Constants.CURRENCY_STR[i]);
        }
        int currency = 0;
        while(currency < 1 || currency >= Constants.CURRENCY_STR.length){
            String temp = scanner.nextLine();
            try{
                currency = Integer.parseInt(temp);
                if(currency < 1 || currency >= Constants.CURRENCY_STR.length){
                    throw new IllegalArgumentException();
                }
            }catch(IllegalArgumentException e){
                System.out.println("Invalid input. Please try again.");
                System.out.print("Please choose your account currency: ");
            }
        }

        // Enter Deposit Amount
        System.out.print("Please enter deposit amount: ");
        Float amount = null;
        while(amount == null || amount < 0){
            String temp = scanner.nextLine();
            try{
                amount = Float.parseFloat(temp);
                if(amount < 0){
                    throw new IllegalArgumentException();
                }
            }catch(IllegalArgumentException e){
                System.out.println("Invalid input. Please try again.");
                System.out.print("Please enter deposit amount: ");
            }
        }


        int size = name.length() + password.length() + Constants.INT_SIZE*6 + Constants.FLOAT_SIZE;
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
        Utils.marshal(currency-1, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(amount, packageByte, index);
        index += Constants.FLOAT_SIZE;

        return packageByte;
    }

    /**
     * Handle response recieved from server
     * @param response {@code byte[]} response packet recieved from server.
     * @throws IOException
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
            System.out.println("Deposit Failed");
            int errorsize = Utils.unmarshalInteger(response, index);
            index += Constants.INT_SIZE;
            //System.out.println(errorsize);
            String error = Utils.unmarshalString(response, index, index+errorsize); index+=errorsize;
            System.out.println(error);
        }
        else{
            System.out.println("Deposit Successful");
            //todo print acc balance
            int currency = Utils.unmarshalInteger(response, index); index += Constants.INT_SIZE;
            System.out.println("Currency: "+ currency);
            Float balance = Utils.unmarshalFloat(response, index);
            System.out.println("Balance: "+ balance);
        }
    }
}

