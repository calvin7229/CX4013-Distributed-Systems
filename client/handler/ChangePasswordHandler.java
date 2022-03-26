package client.handler;

import java.io.*;
import java.lang.*;
import java.util.*;
import client.Constants;
import client.Utils;

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
        String oldpassword = scanner.nextLine();
        while(oldpassword.isEmpty()){
            System.out.println("Invalid input. Please try again.");
            System.out.print("Please enter old password: ");
            oldpassword = scanner.nextLine();
        }

        // Enter New Password
        System.out.print("Please enter new password: ");
        String newpassword = scanner.nextLine();
        while(newpassword.isEmpty()){
            System.out.println("Invalid input. Please try again.");
            System.out.print("Please enter new password: ");
            newpassword = scanner.nextLine();
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


        int size = name.length() + oldpassword.length() + newpassword.length() + Constants.INT_SIZE*6;
        byte[] packageByte = new byte[size];
        int index = 0;
        Utils.marshal(id, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(4, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(name.length(), packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(name, packageByte, index);
        index += name.length();
        Utils.marshal(oldpassword.length(), packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(oldpassword, packageByte, index);
        index += oldpassword.length();
        Utils.marshal(newpassword.length(), packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(newpassword, packageByte, index);
        index += newpassword.length();
        Utils.marshal(account, packageByte, index);
        index += Constants.INT_SIZE;

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
            System.out.println("Change Password Failed");
            int errorsize = Utils.unmarshalInteger(response, index);
            index += Constants.INT_SIZE;
            System.out.println(errorsize);
            String error = Utils.unmarshalString(response, index, index+errorsize);
            System.out.println(error);
        }
        else{
            System.out.println("Change Password Successful");
        }
    }
}