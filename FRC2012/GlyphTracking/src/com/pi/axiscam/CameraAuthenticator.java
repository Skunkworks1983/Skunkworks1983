package com.pi.axiscam;

import java.net.Authenticator;
import java.net.PasswordAuthentication;

public class CameraAuthenticator extends Authenticator {

    private PasswordAuthentication passAuth;

    public CameraAuthenticator(String username, String password) {
	passAuth = new PasswordAuthentication(username, password.toCharArray());
    }

    @Override
    public PasswordAuthentication getPasswordAuthentication() {
	return passAuth;
    }
}
