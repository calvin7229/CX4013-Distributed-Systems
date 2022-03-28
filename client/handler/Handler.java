package client.handler;

import java.io.*;
import java.lang.*;
import java.util.*;

/**
 * Abstract handler class to be implemented by each individual service
 */
public abstract class Handler{
    /**
     * Handle response recieved from server
     * @param response
     * @throws IOException
     */
    public abstract void handleResponse(byte[] response) throws IOException; 
    /**
     * Generate request package to be sent to server
     * @param scanner
     * @param id
     * @return {@code byte[]} request package to be sent to server
     * @throws UnsupportedEncodingException
     */
    public abstract byte[] executeService(Scanner scanner, int id)throws UnsupportedEncodingException;

    /**
     * get the end time of the monitor duration
     * @return
     */
    public long getEndTime(){
        return 0;
    }
}
