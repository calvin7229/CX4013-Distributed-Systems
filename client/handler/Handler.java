package handler;

import java.io.*;
import java.lang.*;
import java.util.*;

public abstract class Handler{
    public static void handleMessage() {
        ;
    }
    public static void handleResponse() {
        ;
    }
    public abstract void executeService(Scanner scanner, int id)throws UnsupportedEncodingException;
}
