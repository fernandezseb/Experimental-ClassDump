
public class Unicode {
    public static void main(String[] args) {
        String twoByte = "Start\u00DFEnd";
        String threeByte = "Start\u6771End";
        String supplementary = "Start\uD840\uDC00End";
        String supplementary2 = "Start\uD83D\uDE0AEnd";
    }

}