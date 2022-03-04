package client;

import java.io.*;
import java.lang.*;
import java.util.*;

public class Utils {
    public static byte[] marshal(int n) {
        byte[] result = new byte[4];
        result[0] = (byte)(n >> 24);
        result[1] = (byte)(n >> 16);
        result[2] = (byte)(n >> 8);
        result[3] = (byte)(n);
        return result;
    }
    public static byte[] marshal(String s) {
        byte[] result = new byte[s.length()];
        for(int i = 0; i < s.length(); i++) {
            result[i] = (byte)s.charAt(i);
        }
        return result;
    }
    public static int unmarshalInteger(byte[] b, int start) {
        return b[start] << 24 | (b[start+1] & 0xFF) << 16 | (b[start+2] & 0xFF) << 8 | (b[start+3] & 0xFF);
    }
    public static String unmarshalString(byte[] b, int start, int end) {
        char[] c = new char[end - start];
        for(int i = start; i < end; i++) {
            c[i-start] = (char)(b[i]);
        }
        return new String(c);
    }
    
}
