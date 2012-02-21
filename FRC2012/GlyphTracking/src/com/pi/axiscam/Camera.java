package com.pi.axiscam;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.Authenticator;
import java.net.MalformedURLException;
import java.net.URL;

import javax.imageio.ImageIO;

public class Camera {
    private BufferedImage[] camBuffer = new BufferedImage[2];
    private byte activeBuffer = 0;
    private CameraAuthenticator authenticator;
    private URL cameraURL;
    private int lastRead = 0;
    private boolean ioError = false;

    public Camera(String url, String username, String password) throws MalformedURLException {
	this.authenticator = new CameraAuthenticator(username, password);
	Authenticator.setDefault(authenticator);
	cameraURL = new URL(url);
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
	    camBuffer[Math.abs(activeBuffer - 1)] = ImageIO.read(cameraURL);
	    activeBuffer = (byte) Math.abs(activeBuffer - 1);
	    ioError = false;
	} catch (IOException e) {
	    ioError = true;
	}
    }
    
    public boolean hasIOError() {
	return ioError;
    }
}
