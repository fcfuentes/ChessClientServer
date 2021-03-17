import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        var client = new EchoClient();
        client.startConnection("127.0.0.1", 1234);

        // Using Scanner for Getting Input from User
        var in = new Scanner(System.in);

        var t = new ListenerThread(client);
        t.start();

        while (true) {
            var s = in.nextLine();
            client.sendMessage(s);
        }
    }
}
