package com.pi.glyph;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.MalformedURLException;
import java.util.List;

import javax.swing.JFrame;

import com.pi.axiscam.Camera;

public class GlyphCam extends JFrame {
    private static final long serialVersionUID = 1L;
    private static long bufferExpiry = 3000;

    public int threshold = 200;
    public int minLength = 25;
    public int maxMinorPairDistance = 25;
    public int maxMajorPairDistance = 50;
    public int maxLines = 25;
    public boolean isRunning = true;
    public GlyphPoseEstimation pose = new GlyphPoseEstimation();

    public double currentPixelWidth = -1;
    public double currentPixelHeight = -1;
    public boolean calibrating = false;

    private Camera camera;
    private GlyphConfig config;

    public GlyphCam() throws MalformedURLException, IOException {
	super("Glyph Camera");
	config = new GlyphConfig(this);
	setSize(640, 480);
	setLocation(0, 0);
	setVisible(true);
	setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
	camera = new Camera("http://10.19.83.11/jpg/1/image.jpg", "FRC", "FRC");

	new Thread(new Runnable() {
	    @Override
	    public void run() {
		while (isVisible()) {
		    if (isRunning) {
			camera.refresh();
		    } else {
			try {
			    Thread.sleep(10L);
			} catch (InterruptedException e) {
			}
		    }
		}
	    }
	}).start();

	new Thread(new Runnable() {
	    @Override
	    public void run() {
		while (isVisible()) {
		    if (isRunning) {
			try {
			    if (!paintToBuffer())
				try {
				    Thread.sleep(10L);
				} catch (InterruptedException e) {
				}
			    repaint();
			} catch (Exception e) {
			    e.printStackTrace();
			}
		    } else {
			try {
			    Thread.sleep(10L);
			} catch (InterruptedException e) {
			}
		    }
		}
	    }
	}).start();
	config.setVisible(true);

	WindowAdapter closer = new WindowAdapter() {
	    @Override
	    public void windowClosing(WindowEvent e) {
		config.dispose();
		dispose();
	    }
	};
	config.addWindowListener(closer);
	addWindowListener(closer);
    }

    BufferedImage backBuffer;
    Image errorBuffer = null;
    long backBufferTime = 0;
    BasicStroke lineStroke = new BasicStroke(3f);
    BasicStroke textStroke = new BasicStroke(1f);
    Font errorFont = new Font("Arial", Font.BOLD, 32);
    Font normalFont = new Font("Arial", Font.PLAIN, 14);
    int charHeight = -1;

    public boolean paintToBuffer() throws MalformedURLException, IOException {
	if (!camera.hasFreshImage())
	    return false;
	BufferedImage image = camera.getImage();// ImageIO.read(new
	// URL("http://10.19.83.11/jpg/1/image.jpg"));
	if (image == null)
	    return false;

	int height = image.getHeight();
	int width = image.getWidth();
	Graphics2D g = (Graphics2D) image.getGraphics();
	g.setFont(normalFont);
	List<Point[]> lines = VerticalLines.getVerticalLines(image, threshold,
		maxLines);
	lines = LineFilter.filterLines(lines, minLength, maxMinorPairDistance);
	g.setColor(Color.RED);
	for (Point[] p : lines) {
	    g.drawLine(p[0].x, p[0].y, p[1].x, p[1].y);
	}
	g.setStroke(lineStroke);
	g.setColor(Color.GREEN);
	Point[] pair = LinePairFinder.getTopPair(lines, maxMajorPairDistance);
	if (pair != null)
	    g.drawLine(pair[0].x, pair[0].y, pair[1].x, pair[1].y);
	g.setColor(new Color(0f, 0f, 0f, 0.5f));
	g.fillRect(0, height - 100, width, 100);
	g.setStroke(textStroke);
	g.setColor(Color.WHITE);
	if (charHeight == -1)
	    charHeight = g.getFontMetrics().getHeight();
	if (pair != null) {
	    if (calibrating) {
		currentPixelWidth = pose.getDistance(pair[0], pair[1]);
		currentPixelHeight = (pose.parallaxY - ((double) (pair[0].y + pair[1].y)) / 2d);
	    }
	    double depth = pose.getDepth(pair);
	    double yaw = pose.getYaw(pair, depth);
	    double basketDepth = pose.getBasketDepth(depth, yaw);
	    double basketYaw = Math.toDegrees(pose.getBasketYaw(basketDepth,
		    yaw));
	    double yawD = Math.toDegrees(yaw);
	    System.out.println("Depth: " + depth);
	    System.out.println("Yaw: " + yawD);
	    System.out.println("Basket Depth: " + basketDepth);
	    System.out.println("Basket Yaw: " + basketYaw);
	    g.drawString("Depth: " + (pair != null ? depth + " ft" : "N/A"), 0,
		    height - 100 + charHeight);
	    g.drawString("Yaw: " + (pair != null ? yawD + "°" : "N/A"), 0,
		    height - 100 + (charHeight * 2));
	    g.drawString("Basket Depth: "
		    + (pair != null ? basketDepth + " ft" : "N/A"), 0, height
		    - 100 + (charHeight * 4));
	    g.drawString("Basket Yaw: "
		    + (pair != null ? basketYaw + "°" : "N/A"), 0, height - 100
		    + (charHeight * 5));
	    backBuffer.getGraphics().drawImage(image, 0, 0, null);
	    backBufferTime = System.currentTimeMillis();
	} else if (System.currentTimeMillis() - backBufferTime > bufferExpiry) {
	    backBuffer = image;
	}
	return true;
    }

    Rectangle2D errorBounds = null;

    @Override
    public void paint(Graphics g) {
	// super.paint(g);
	try {
	    if (camera.hasIOError()) {
		if (errorBuffer == null) {
		    errorBuffer = this.createImage(getWidth(), getHeight());
		    Graphics eG = errorBuffer.getGraphics();
		    eG.setColor(Color.RED);
		    eG.fillRect(0, 0, getWidth(), getHeight());
		    eG.setColor(Color.BLACK);
		    eG.setFont(errorFont);
		    if (errorBounds == null)
			errorBounds = eG.getFontMetrics().getStringBounds(
				"I/O Error!", eG);
		    eG.drawString("I/O Error!", (getWidth() / 2)
			    - ((int) (errorBounds.getWidth() / 2d)),
			    (getHeight() / 2)
				    - ((int) (errorBounds.getHeight() / 2d)));
		}
		g.drawImage(errorBuffer, 0, 0, null);
	    } else if (backBuffer != null) {
		g.drawImage(backBuffer, 0, 0, null);
	    }
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }

    public static void main(String[] args) throws MalformedURLException,
	    IOException {
	new GlyphCam();
    }
}
