import java.io.IOException;
import java.util.function.Supplier;

public class Main {

    // ConstantValue attribute
    private final int constantValue = 4512345;

    public static void main(String[] args) throws IOException {
        Supplier<Integer> integerSupplier = () -> 42;
        throw new IOException("Something went wrong with IO");
    }

}