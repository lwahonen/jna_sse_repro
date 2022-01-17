package com.diktamen.jna_sse_repro;

import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.win32.StdCallLibrary;
import com.sun.jna.win32.W32APIOptions;

public class repro {
    public interface SSETest extends StdCallLibrary {
        Pointer ReadSSERegisters();

        Pointer ShuffleSSERegisters();

        SSETest INSTANCE = Native.load("sse_repro.dll", SSETest.class, W32APIOptions.UNICODE_OPTIONS);
    }


    static String IntArrayToHexStringArray(int[] printMe) {
        String ret = "[";
        for (int i : printMe) {
            ret += Integer.toHexString(i) + ", ";
        }
        return ret.substring(0,ret.length() - 2) + "]";
    }

    public static void main(String[] args) {
        Pointer prePoint = SSETest.INSTANCE.ReadSSERegisters();
        // Read all eight XMM registers
        int[] pre = prePoint.getIntArray(0, 8 * 4);
        // Call a function that has __m128 paramers and a __vectorcall calling convention
        int[] wrote = SSETest.INSTANCE.ShuffleSSERegisters().getIntArray(0, 8 * 4);
        // All of the registers are now clobbered
        int[] post = SSETest.INSTANCE.ReadSSERegisters().getIntArray(0, 8 * 4);
        System.out.println("Before JNA shuffle "+ IntArrayToHexStringArray(pre));
        System.out.println("\nJNA calling code that clobbers registers with "+ IntArrayToHexStringArray(wrote));
        System.out.println("\nAfter JNA clobbering, registers are "+ IntArrayToHexStringArray(post));
    }
}