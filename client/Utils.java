package client;


import java.nio.ByteBuffer;


/**
 * Utility function for marshalling and unmarshalling data
 */
public class Utils {
    public static void marshal(int n,  byte[] data, int index) {
        data[index] = (byte)(n >> 24);
        data[index+1] = (byte)(n >> 16);
        data[index+2] = (byte)(n >> 8);
        data[index+3] = (byte)(n);
        return;
    }
    public static int getDecimal(int binary){  
        int decimal = 0;  
        int n = 0;  
        while(true){  
          if(binary == 0){  
            break;  
          } else {  
              int temp = binary%10;  
              decimal += temp*Math.pow(2, n);  
              binary = binary/10;  
              n++;  
           }  
        }  
        return decimal;  
    }  

    public static void marshal(float f, byte[] data, int index) {
        byte[] b = ByteBuffer.allocate(Constants.FLOAT_SIZE).putFloat(f).array();
        for (int i=0;i<4;i++){
            data[index+i] = b[i];
        }
        return;
    }
    public static void marshal(String s,byte[] data, int index) {
        for(int i = 0; i < s.length(); i++) {
            data[index+i] = (byte)s.charAt(i);
        }
        return;
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
    public static float unmarshalFloat(byte[] b, int start) {
        int n = b[start] << 24 | (b[start+1] & 0xFF) << 16 | (b[start+2] & 0xFF) << 8 | (b[start+3] & 0xFF);
        return Float.intBitsToFloat(n);
    }

}
