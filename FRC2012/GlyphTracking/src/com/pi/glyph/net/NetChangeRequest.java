package com.pi.glyph.net;


public class NetChangeRequest {
    public static final int REGISTER = 1;
    public static final int CHANGEOPS = 2;

    public int type;
    public int ops;

    public NetChangeRequest(int type, int ops) {
	this.type = type;
	this.ops = ops;
    }
}
