public class ListenerThread extends Thread {
    private final EchoClient client;

    public ListenerThread(EchoClient client) {
        this.client = client;
    }

    public void run() {
        while (true) {
            var received = client.receiveMessage();
            if (received == null) {
                break;
            }
            System.out.println("Receive: " + received);

            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
