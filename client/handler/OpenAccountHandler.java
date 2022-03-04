package handler;

import java.io.*;
import java.lang.*;
import java.util.*;
import client.Constants;

//construct  message & create message & response(?) can have a separate class that handle all these
//scanner?
public class OpenAccountHandler extends Handler {
    
    @Override
    public void executeService(Scanner scanner, int id)throws UnsupportedEncodingException{
        System.out.println("Blablabla open acc");

        // Enter Name
        //System.out.print(Constants.OPEN_NAME_MSG);
        String name = scanner.nextLine();
        while(name.isEmpty()){
            System.out.println("Invalid input. Please try again.");
            //System.out.print(Constants.OPEN_NAME_MSG);
            name = scanner.nextLine();
        }

        // Enter Password
        //System.out.print(Constants.OPEN_PASSWORD_MSG);
        String password = scanner.nextLine();
        while(name.isEmpty()){
            //System.out.println(Constants.ERR_PASSWORD_INPUT);
            //System.out.print(Constants.OPEN_PASSWORD_MSG);
            password = scanner.nextLine();
        }

        // Enter Default Currency
        //System.out.println(Constants.OPEN_SELECT_CURRENCY_MSG);
        for (int i = 1; i < Constants.CURRENCY_STR.length; i++){
            System.out.printf("%d. %s\n", i, Constants.CURRENCY_STR[i]);
        }
        //System.out.print(Constants.OPEN_CURRENCY_MSG);
        int currency = 0;
        if(scanner.hasNextInt()){
            currency = scanner.nextInt();
        }
        while(currency < 1 || currency >= Constants.CURRENCY_STR.length){
            System.out.println("Invalid input. Please try again.");
            //System.out.println(Constants.OPEN_SELECT_CURRENCY_MSG);
            if(scanner.hasNextInt()){
                currency = scanner.nextInt();
            }
        }
        

        // Enter Starting Balance
        // TODO: Handle error???????
        //System.out.print(Constants.OPEN_BALANCE_MSG);
        
        String balanceStr = scanner.nextLine();
        float balance = Float.parseFloat(balanceStr);

        System.out.println();
        boolean confirm = OpenAccountHandler.confirm(name, password, currency, balance, scanner);
        System.out.println();

        if (confirm){
            // message class to construct and send;
        }

        return;
    }
    
}
