package com.pi.axiscam;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.Authenticator;
import java.net.MalformedURLException;
import java.net.URL;

import javax.imageio.ImageIO;

import com.pi.glyph.filter.FastMath;

public class Camera extends Thread {
    private BufferedImage[] camBuffer = new BufferedImage[2];
    private byte activeBuffer = 0;
    private CameraAuthenticator authenticator;
    private URL cameraURL;
    private int lastRead = 0;
    private boolean ioError = false;
    private boolean isRunning = true;
    private boolean isPaused = false;

    public Camera(ThreadGroup tGroup, String url, String username,
	    String password) throws MalformedURLException {
	super(tGroup, "Camera Thread");
	this.authenticator = new CameraAuthenticator(username, password);
	Authenticator.setDefault(authenticator);
	cameraURL = new URL(url);
	start();
    }

    @Override
    public void run() {
	while (isRunning) {
	    if (!isPaused) {
		refresh();
	    } else {
		try {
		    Thread.sleep(10L);
		} catch (InterruptedException e) {
		}
	    }
	}
    }
    
    public boolean isPaused(){
	return isPaused;
    }
    
    public void pause(boolean state){
	this.isPaused = state;
    }

    public BufferedImage getImage() {
	lastRead = activeBuffer;
	return camBuffer[activeBuffer];
    }

    public boolean hasFreshImage() {
	return activeBuffer != lastRead;
    }

    public void refresh() {
	try {
	    byte newActive = (byte) FastMath.abs(activeBuffer - 1);
	    if (camBuffer[newActive] != null)
		camBuffer[newActive].flush();
	    camBuffer[newActive] = ImageIO.read(cameraURL);
	    activeBuffer = newActive;
	    ioError = false;
	} catch (IOException e) {
	    ioError = true;
	}
    }

    public boolean hasIOError() {
	return ioError;
    }
    
    @SuppressWarnings("deprecation")
    public void dispose(){
	this.isRunning = false;
	try {
	    join();
	} catch (InterruptedException e) {
	    stop();
	}
    }
}
