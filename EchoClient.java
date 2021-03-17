//
//  EchoClient.java
//  Definition of the Client. Starts a connection, send and receive messages and stop a connection.
//
//  Created by Fabiola Castellanos Fuentes on 2021-03-15.
//

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class EchoClient {
    private Socket clientSocket;
    private PrintWriter out;
    private BufferedReader in;

    //Start the conecction. Open a new socket and listen
    public void startConnection(String ip, int port) {
        try {
            clientSocket = new Socket(ip, port);
            out = new PrintWriter(clientSocket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
        } catch (IOException e) {
        }

    }

    public String receiveMessage() {
        try {
            return in.readLine();
        } catch (IOException e) {
            return null;
        }
    }
 
    public void sendMessage(String msg) {
        try {
            out.println(msg);
        } catch (Exception e) {
        }
    }

    public void stopConnection() {
        try {
            in.close();
            out.close();
            clientSocket.close();
        } catch (IOException e) {
        }

    }
}
