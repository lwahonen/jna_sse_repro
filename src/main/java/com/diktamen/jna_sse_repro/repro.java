package com.diktamen.jna_sse_repro;

import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.win32.StdCallLibrary;
import com.sun.jna.win32.W32APIOptions;

import java.util.Arrays;

public class repro {
    public interface SSETest extends StdCallLibrary {
        Pointer ReadSSERegisters();

        Pointer ShuffleSSERegisters();

        SSETest INSTANCE = Native.load("sse_repro.dll", SSETest.class, W32APIOptions.UNICODE_OPTIONS);
    }

    public static void main(String[] args) {
        // Read just the six volatile XMM registers (XMM0...XMM5)
        // Sixteen bytes per register
        int arraySize = 6 * 16;
        byte[] pre = SSETest.INSTANCE.ReadSSERegisters().getByteArray(0, arraySize);
        // Call a function that writes into the volatile registers, as per allowed in spec
        byte[] wrote = SSETest.INSTANCE.ShuffleSSERegisters().getByteArray(0, arraySize);
        // All of the registers are now clobbered
        byte[] post = SSETest.INSTANCE.ReadSSERegisters().getByteArray(0, arraySize);
        System.out.println("Before JNA shuffle " + Arrays.toString(pre));
        System.out.println("\nJNA calling code that clobbers registers with " + Arrays.toString(wrote));
        System.out.println("\nAfter JNA clobbering, registers are " + Arrays.toString(post));
    }
}