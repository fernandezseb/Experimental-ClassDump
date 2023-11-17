import java.io.IOException;
import java.util.function.Supplier;
import java.beans.Transient;

@Deprecated
public class Main {

    // ConstantValue attribute
    private final int constantValue = 4512345;

    @Transient(value = false)
    public static void main(String[] args) throws IOException {
        Supplier<Integer> integerSupplier = () -> 42;
        throw new IOException("Something went wrong with IO");
    }

}