package com.pi.glyph.filter;

public class FastMath {
    //private static final int FLOAT_BIT_MASK = 2147483647;
    //private static final long DOUBLE_BIT_MASK = 9223372036854775807L;

    public static int abs(int i) {
	return (i ^ (i >> 31)) - (i >> 31);
    }

    public static long abs(long i) {
	return (i ^ (i >> 63)) - (i >> 63);
    }
    /*
     * public static double abs(double i) { return
     * Double.longBitsToDouble(Double.doubleToRawLongBits(i) & DOUBLE_BIT_MASK);
     * }
     * 
     * public static float abs(float i) { return Float
     * .intBitsToFloat(Float.floatToRawIntBits(i) & FLOAT_BIT_MASK); }
     */
}
