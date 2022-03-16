import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.concurrent.TimeUnit;

public class Main {

    private static String ip="192.168.1.53";

    public static void main(String[] args) throws IOException, InterruptedException {
        System.out.println(isOK(setAuto(ip)));
        TimeUnit.SECONDS.sleep(5);
        System.out.println(isOK(setClosed(ip)));
        TimeUnit.SECONDS.sleep(5);
        System.out.println(isOK(setManuel(ip)));
        TimeUnit.SECONDS.sleep(5);
        System.out.println(isOK(setOpen(ip)));
        TimeUnit.SECONDS.sleep(5);
        System.out.println(isOK(setClosed(ip)));

        getHumidityA(ip);
        getTemperature(ip);
        getHumidityS(ip);
        getPression(ip);
    }

    public static String setOpen(String ip) throws IOException {
        URL url = new URL("http://"+ip+"/open");

        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");
        BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String inputLine;
        StringBuffer content = new StringBuffer();
        while ((inputLine = in.readLine()) != null) {
            content.append(inputLine);
        }
        in.close();

        System.out.println(content);
        return content.toString();
    }

    public static String setManuel(String ip) throws IOException {
        URL url = new URL("http://"+ip+"/manuel");

        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");
        BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String inputLine;
        StringBuffer content = new StringBuffer();
        while ((inputLine = in.readLine()) != null) {
            content.append(inputLine);
        }
        in.close();

        System.out.println(content);
        return content.toString();
    }

    public static String setClosed(String ip) throws IOException {
        URL url = new URL("http://"+ip+"/close");

        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");
        BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String inputLine;
        StringBuffer content = new StringBuffer();
        while ((inputLine = in.readLine()) != null) {
            content.append(inputLine);
        }
        in.close();

        System.out.println(content);
        return content.toString();
    }

    public static String setAuto(String ip) throws IOException {
        URL url = new URL("http://"+ip+"/auto");

        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");
        BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String inputLine;
        StringBuffer content = new StringBuffer();
        while ((inputLine = in.readLine()) != null) {
            content.append(inputLine);
        }
        in.close();

        System.out.println(content);
        return content.toString();
    }
    
    public static String getTemperature(String ip) throws IOException {
        URL url = new URL("http://"+ip+"/temperature");

        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");
        BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String inputLine;
        StringBuffer content = new StringBuffer();
        while ((inputLine = in.readLine()) != null) {
            content.append(inputLine);
        }
        in.close();

        System.out.println(content);
        return content.toString();
    }

    public static String getPression(String ip) throws IOException {
        URL url = new URL("http://"+ip+"/pression");

        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");
        BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String inputLine;
        StringBuffer content = new StringBuffer();
        while ((inputLine = in.readLine()) != null) {
            content.append(inputLine);
        }
        in.close();

        System.out.println(content);
        return content.toString();
    }

    public static String getHumidityS(String ip) throws IOException {
        URL url = new URL("http://"+ip+"/humidity/sol");

        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");
        BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String inputLine;
        StringBuffer content = new StringBuffer();
        while ((inputLine = in.readLine()) != null) {
            content.append(inputLine);
        }
        in.close();

        System.out.println(content);
        return content.toString();
    }

    public static String getHumidityA(String ip) throws IOException {
        URL url = new URL("http://"+ip+"/humidity/air");

        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");
        BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String inputLine;
        StringBuffer content = new StringBuffer();
        while ((inputLine = in.readLine()) != null) {
            content.append(inputLine);
        }
        in.close();

        System.out.println(content);
        return content.toString();
    }
    
    public static boolean isOK(String toTest) {

        if (toTest.contains("nok"))
            return false;

        return true;
    }
}

