package client.handler;

import java.io.*;
import java.lang.*;
import java.util.*;

public abstract class Handler{
    public static void handleMessage() {
        ;
    }
    public void handleResponse(byte[] response) throws IOException {
        ;
    }
    public static void create() {
        ;
    }
    public static boolean confirm(String name, String password, int currency, float balance,Scanner scanner){
        //to confirm entered detail
        //imo not really necessary lah 
        return true;
    }
    public abstract byte[] executeService(Scanner scanner, int id)throws UnsupportedEncodingException;
}
