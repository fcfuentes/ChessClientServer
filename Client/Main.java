//
//  Main class
//  It connects to localhost in port 1234 and start a listener thread and waits for user input.
//
//  Created by Fabiola Castellanos Fuentes on 2021-03-15.
//

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        var client = new EchoClient();
        client.startConnection("127.0.0.1", 1234);

        var in = new Scanner(System.in);

        var t = new ListenerThread(client);
        t.start();

        while (true) {
            var s = in.nextLine();
            client.sendMessage(s);
        }
    }
}
