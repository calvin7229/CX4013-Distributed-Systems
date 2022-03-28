package client.handler;

import java.io.*;
import java.lang.*;
import java.util.*;
import client.Constants;
import client.Utils;

public class MonitorUpdateHandler extends Handler{
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

        // Enter Duration
        System.out.println("Please enter the monitor duration: ");
        Integer duration = null;
        if(scanner.hasNextInt()){
            duration = scanner.nextInt();
        }
        // which time unit is most suitable for backend?
        while(duration == null || duration < 0){
            System.out.println("Invalid input. Please try again.");
            System.out.println("Please enter the monitor duration: ");
            if(scanner.hasNextInt()){
                duration = scanner.nextInt();
            }
        }

        int size = name.length() + password.length() + Constants.INT_SIZE*6;
        byte[] packageByte = new byte[size];
        int index = 0;
        Utils.marshal(id, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(7, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(name.length(), packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(name, packageByte, index);
        index += name.length();
        Utils.marshal(password.length(), packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(password, packageByte, index);
        index += password.length();
        Utils.marshal(duration, packageByte, index);
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
            System.out.println("Subscription failed");
            int errorsize = Utils.unmarshalInteger(response, index);
            index += Constants.INT_SIZE;
            System.out.println(errorsize);
            String error = Utils.unmarshalString(response, index, index+errorsize);
            System.out.println(error);
        }
        else{
            System.out.println("Subscription success");
        }
    }
}