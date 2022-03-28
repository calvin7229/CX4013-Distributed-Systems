package client.handler;

import java.io.*;
import java.lang.*;
import java.time.Instant;
import java.util.*;
import client.Constants;
import client.Utils;

/**
 * MonitorUpdateHandler.java
 * Generates a monitor update request and sends it to the server.
 * 
 * 
 */
public class MonitorUpdateHandler extends Handler{
    private long endtime;
    private long duration;
    /**
     * Get input from user and mashall it into a byte array.
     * @param scanner {@code Scanner} object to read input from user.
     * @param id {@code int} id of the request.
     * @return {@code byte[]} body package to be sent to server.
     * @throws UnsupportedEncodingException
     * 
     */
    @Override
    public byte[] executeService(Scanner scanner, int id)throws UnsupportedEncodingException{
        System.out.println("Monitor Subsciption");

        
        // Enter Duration in Seconds
        System.out.println("Please enter the monitor duration in seconds: ");
        Integer duration = null;
        if(scanner.hasNextInt()){
            duration = scanner.nextInt();
        }
        while(duration == null || duration < 0){
            System.out.println("Invalid input. Please try again.");
            System.out.println("Please enter the monitor duration: ");
            if(scanner.hasNextInt()){
                duration = scanner.nextInt();
            }
        }
        
        int size = Constants.INT_SIZE*3;
        byte[] packageByte = new byte[size];
        int index = 0;
        Utils.marshal(id, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(7, packageByte, index);
        index += Constants.INT_SIZE;
        Utils.marshal(duration, packageByte, index);
        index += Constants.INT_SIZE;
        this.duration = duration;
        return packageByte;
    }
    /**
     * Receive acknowledgement from server and
     * updates current {@code endtime} to the end time of monitor duration
     * @param response {@code byte[]} response from server.
     * @throws IOException
     * 
     */
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
            System.out.println("Listening for updates....");
            //Upon success, set endtime for termination of subscription
            this.endtime = Instant.now().getEpochSecond() + this.duration;
        }
    }
    @Override
    /**
     * Get the endtime of the monitor subscription.
     * @return {@code long} endtime of the monitor subscription.
     */
    public long getEndTime(){
        return this.endtime;
    }
}