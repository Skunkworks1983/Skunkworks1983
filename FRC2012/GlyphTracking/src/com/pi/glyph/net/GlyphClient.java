package com.pi.glyph.net;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.util.LinkedList;

import com.pi.glyph.GlyphCam;

public class GlyphClient extends Thread {
    private Socket sock;
    private boolean isRunning = true;
    private LinkedList<byte[]> writeQueue = new LinkedList<byte[]>();
    private GlyphCam glyphCam;
    private OutputStream out;

    public GlyphClient(GlyphCam glyphCam, String ip, int port)
	    throws IOException {
	super(glyphCam.getThreadGroup(), "NetSelector");
	this.glyphCam = glyphCam;
	sock = new Socket(ip, port);
	out = sock.getOutputStream();
	start();
    }

    public void send(double basketDepth, double basketYaw) {
	try {
	    synchronized (this.writeQueue) {
		writeQueue.add((basketDepth + "," + basketYaw + String
			.valueOf((char) 0)).getBytes());
	    }
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }

    @Override
    public void run() {
	while (isRunning) {
	    try {
		if (!glyphCam.isPaused()) {
		    if (writeQueue.size() > 0) {
			out.write(writeQueue.removeFirst());
			out.flush();
		    }
		}
	    } catch (Exception e) {
		e.printStackTrace();
	    }
	}
	System.out.println("Thread ended");
    }

    @SuppressWarnings("deprecation")
    public void dispose() {
	System.out.println("Stopping");
	isRunning = false;
	try {
	    join();
	    try {
		out.close();
	    } catch (IOException e1) {
		e1.printStackTrace();
	    }
	    try {
		sock.close();
	    } catch (IOException e) {
		e.printStackTrace();
	    }
	} catch (InterruptedException e) {
	    stop();
	}
    }
}