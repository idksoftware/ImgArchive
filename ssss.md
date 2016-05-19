
import java.io.BufferedReader; 



04 
import java.io.IOException; 



05 
import java.io.InputStream; 



06 
import java.io.InputStreamReader; 



07 
  



08 
public class ProcessBuilderExample { 



09 
    public static void main(String[] args) throws InterruptedException, 



10 
            IOException { 



11 
        ProcessBuilder pb = new ProcessBuilder("echo", "This is ProcessBuilder Example from JCG"); 
        System.out.println("Run echo command"); 
        Process process = pb.start(); 
        int errCode = process.waitFor(); 
        System.out.println("Echo command executed, any errors? " + (errCode == 0 ? "No" : "Yes")); 
        System.out.println("Echo Output:\n" + output(process.getInputStream()));     
    } 
    private static String output(InputStream inputStream) throws IOException { 
        StringBuilder sb = new StringBuilder(); 
        BufferedReader br = null; 
        try { 
            br = new BufferedReader(new InputStreamReader(inputStream)); 
            String line = null; 
            while ((line = br.readLine()) != null) { 
                sb.append(line + System.getProperty("line.separator")); 
            } 
        } finally { 
            br.close(); 
        } 
        return sb.toString(); 
    } 
} 
