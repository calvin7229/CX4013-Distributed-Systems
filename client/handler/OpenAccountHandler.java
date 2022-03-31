package client.handler;

import java.io.*;
import java.lang.*;
import java.util.*;
import client.Constants;
import client.Utils;

public class OpenAccountHandler extends Handler {
    
    @Override
    /**
     * Execute the service
     * @param scanner
     * @param id
     * @return
     * @throws UnsupportedEncodingException
     * 
     */
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
        
        // Enter Starting Balance
        System.out.print("Please enter account balance: ");
        Float balance = null;
        while(balance == null || balance < 0){
            String temp = scanner.nextLine();
            try{
                balance = Float.parseFloat(temp);
                if(balance < 0){
                    throw new IllegalArgumentException();
                }
            }catch(IllegalArgumentException e){
                System.out.println("Invalid input. Please try again.");
                System.out.print("Please enter account balance: ");
            }
        }

        int size = name.length() + password.length() + Constants.INT_SIZE*5 + Constants.FLOAT_SIZE;
        byte[] packageByte = new byte[size];
        int index = 0;
        Utils.marshal(id, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(1, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(name.length(), packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(name, packageByte, index);
        index += name.length();
        Utils.marshal(password.length(), packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(password, packageByte, index);
        index += password.length();
        Utils.marshal(currency-1, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(balance, packageByte, index);
        index += Constants.FLOAT_SIZE;

        return packageByte;
    }
    /**
     * Handle the response received by the server
     * @param response
     * @throws IOException
     * 
     */
    @Override
    public void handleResponse(byte[] response) throws IOException{
        int index = 0;
        int id = Utils.unmarshalInteger(response, index);
        //System.out.println("ID is ");
        //System.out.println(id);
        index += Constants.INT_SIZE;
        String status = Utils.unmarshalString(response, index,index+1);
        //System.out.println(index);
        index += 1;
        //System.out.println(response);
        
        if(status.charAt(0) == '0'){
            System.out.println("Open Account Failed");
            int errorsize = Utils.unmarshalInteger(response, index);
            index += Constants.INT_SIZE;
            System.out.println(errorsize);
            String error = Utils.unmarshalString(response, index, index+errorsize);
            System.out.println(error);
        }
        else{
            System.out.println("Open Account Successful");
            int accid = Utils.unmarshalInteger(response, index);
            System.out.print("ACC ID :");
            System.out.println(accid);
        }
    }
}
