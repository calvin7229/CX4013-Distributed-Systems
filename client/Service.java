package client;

import java.io.*;
import java.lang.*;
import java.util.*;

public abstract class Service{
    public static void handleMessage() {
        ;
    }
    public static void handleResponse() {
        ;
    }
    public static boolean confirm(String name, String password, int currency, float balance,Scanner scanner){
        //to confirm entered detail
        //imo not really necessary lah 
        return true;
    }
    public abstract void executeService(Scanner scanner, int id)throws UnsupportedEncodingException;
}
