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
import java.awt.image.BufferStrategy;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.MalformedURLException;
import java.util.List;

import javax.swing.JFrame;

import com.pi.axiscam.Camera;
import com.pi.glyph.filter.GlyphPoseEstimation;
import com.pi.glyph.filter.LineFilter;
import com.pi.glyph.filter.LinePairFinder;
import com.pi.glyph.filter.VerticalLines;
import com.pi.glyph.net.GlyphClient;

public class GlyphCam extends JFrame {
    private static final long serialVersionUID = 1L;
    private static long bufferExpiry = 3000;
    private final ThreadGroup tGroup;
    private Thread procThread;

    public int threshold = 200;
    public int minLength = 25;
    public int maxMinorPairDistance = 25;
    public int maxMajorPairDistance = 50;
    public int maxLines = 25;
    public GlyphPoseEstimation pose = new GlyphPoseEstimation();

    public double currentPixelWidth = -1;
    public double currentPixelHeight = -1;
    public boolean calibrating = false;

    private boolean isPaused = false;
    private Camera camera;
    private GlyphConfig config;
    private GlyphClient client;
    private BufferStrategy buffer;

    public GlyphCam() throws MalformedURLException, IOException {
	super("Glyph Camera");
	tGroup = new ThreadGroup("GlyphCamera");
	config = new GlyphConfig(this);
	setSize(660, 500);
	setLocation(0, 0);
	setVisible(true);
	setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
	createBufferStrategy(2);
	buffer = getBufferStrategy();
	camera = new Camera(tGroup, "http://10.19.83.11/jpg/1/image.jpg",
		"FRC", "FRC");

	client = new GlyphClient(this, "10.19.83.2", 1180);
	(procThread = new Thread(new Runnable() {

	    @Override
	    public void run() {
		while (isVisible()) {
		    if (!isPaused) {
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
	})).start();
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
	BufferedImage image = camera.getImage();
	if (image == null)
	    return false;

	int height = image.getHeight();
	int width = image.getWidth();
	Graphics2D g = (Graphics2D) image.getGraphics();
	g.setFont(normalFont);
	Point[] lines = VerticalLines.getVerticalLines(image, threshold,
		maxLines);
	List<Point[]> linesR = LineFilter.filterLines(lines, minLength, maxMinorPairDistance);
	g.setColor(Color.RED);
	for (Point[] p : linesR) {
	    g.drawLine(p[0].x, p[0].y, p[1].x, p[1].y);
	}
	g.setStroke(lineStroke);
	g.setColor(Color.GREEN);
	Point[] pair = LinePairFinder.getTopPair(linesR, maxMajorPairDistance);
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
	    double yaw = pose.getRobotYaw(pair, depth);
	    double basketDepth = pose.getBasketDepth(depth, yaw);
	    double basketYaw = pose.getBasketYaw(pair);
	    g.drawString("Depth: " + depth + " ft", 0, height - 100
		    + charHeight);
	    g.drawString("Robot Yaw: " + Math.toDegrees(yaw) + "°", 0, height
		    - 100 + (charHeight * 2));
	    g.drawString("Basket Depth: " + basketDepth + " ft", 0, height
		    - 100 + (charHeight * 4));
	    g.drawString("Basket Yaw: " + basketYaw + "°", 0, height - 100
		    + (charHeight * 5));
	    Graphics bb = buffer.getDrawGraphics();
	    bb.drawImage(image, 20, 20, null);
	    g.dispose();
	    bb.dispose();
	    backBufferTime = System.currentTimeMillis();
	    client.send(basketDepth, basketYaw);
	} else if (System.currentTimeMillis() - backBufferTime > bufferExpiry) {
	    client.send(-1, -1);
	    Graphics bb = buffer.getDrawGraphics();
	    bb.drawImage(image, 20, 20, null);
	    g.dispose();
	    bb.dispose();
	}
	return true;
    }

    Rectangle2D errorBounds = null;

    @Override
    public void paint(Graphics g) {
	super.paint(g);
	try {
	    if (camera != null) {
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
			eG.drawString(
				"I/O Error!",
				(getWidth() / 2)
					- ((int) (errorBounds.getWidth() / 2d)),
				(getHeight() / 2)
					- ((int) (errorBounds.getHeight() / 2d)));
		    }
		    Graphics draw = buffer.getDrawGraphics();
		    draw.drawImage(errorBuffer, 20, 20, null);
		    draw.dispose();
		}
		buffer.show();
	    }
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }

    public static void main(String[] args) throws MalformedURLException,
	    IOException {
	new GlyphCam();
    }

    @Override
    public void dispose() {
	setVisible(false);
	try {
	    procThread.join();
	} catch (InterruptedException e) {
	}
	if (camera != null)
	    camera.dispose();
	if (client != null)
	    client.dispose();
	super.dispose();
    }

    public void pause(boolean state) {
	this.isPaused = state;
    }

    public boolean isPaused() {
	return isPaused;
    }

    public ThreadGroup getThreadGroup() {
	return tGroup;
    }
}
