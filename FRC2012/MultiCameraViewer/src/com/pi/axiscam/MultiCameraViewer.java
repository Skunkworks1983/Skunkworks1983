package com.pi.axiscam;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferStrategy;
import java.awt.image.BufferedImage;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Authenticator;
import java.net.URL;
import java.util.Enumeration;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

import javax.imageio.ImageIO;
import javax.swing.JFrame;

import net.java.games.input.Controller;
import net.java.games.input.Controller.Type;
import net.java.games.input.ControllerEnvironment;
import net.java.games.input.Event;
import net.java.games.input.EventQueue;

public class MultiCameraViewer extends JFrame implements MouseListener {
	private static final long serialVersionUID = 1L;
	private URL[] urls;
	private BufferStrategy buffer;
	private int current = 0;
	Controller joy;
	int frames = 0;
	float fps = 0;
	long nextFrame = -1;
	private Rectangle2D[] rec;
	private Point[] start;
	private boolean disposing = false;
	private Thread t;

	public MultiCameraViewer(String[] ips) throws IOException {
		super("Multiple Camera Viewer");
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

		Authenticator.setDefault(new CameraAuthenticator("root", "1983"));

		Controller[] cont = ControllerEnvironment.getDefaultEnvironment()
				.getControllers();
		for (Controller c : cont) {
			if (c.getType() == Type.STICK) {
				joy = c;
				break;
			}
		}

		setIgnoreRepaint(true);
		setSize(640, 480);
		setLocation(0, 0);
		start = new Point[ips.length];
		rec = new Rectangle2D[ips.length];
		urls = new URL[ips.length];
		for (int i = 0; i < ips.length; i++) {
			urls[i] = new URL("http://" + ips[i] + "/jpg/1/image.jpg");
		}
		setVisible(true);
		addMouseListener(this);
		createBufferStrategy(2);
		buffer = getBufferStrategy();

		t = Thread.currentThread();
		while (isVisible() && !disposing) {
			render();
			if (joy != null) {
				joy.poll();
				EventQueue q = joy.getEventQueue();
				Event event = new Event();
				while (q.getNextEvent(event)) {
					if (event.getComponent() != null
							&& event.getComponent().getName().trim()
									.equalsIgnoreCase("button 1")
							&& event.getValue() == 1) {
						current++;
						if (current >= urls.length)
							current = 0;
						break;
					}
				}
			}
		}
		buffer.dispose();
	}

	BufferedImage img;
	int baseX = -1, baseY, widthScale, heightScale;

	public void render() {
		try {
			img = ImageIO.read(urls[current]);
			frames++;
		} catch (IOException e) {
		}
		Graphics g = buffer.getDrawGraphics();
		g.setColor(Color.BLACK);
		g.fillRect(0, 0, getWidth(), getHeight());
		if (img != null) {
			float aspect = ((float) img.getWidth()) / ((float) img.getHeight());
			int estHeight = (int) (getWidth() / aspect);
			int estWidth = (int) (getHeight() * aspect);
			if (estHeight > getHeight()) {
				g.drawImage(img, baseX = (getWidth() / 2) - (estWidth / 2),
						baseY = 0, widthScale = estWidth,
						heightScale = getHeight(), null);
			} else {
				g.drawImage(img, baseX = 0, baseY = (getHeight() / 2)
						- (estHeight / 2), widthScale = getWidth(),
						heightScale = estHeight, null);
			}
			g.setColor(Color.RED);
			if (rec[current] != null) {
				g.drawRect((int) ((double) rec[current].getX() * widthScale)
						+ baseX,
						(int) ((double) rec[current].getY() * heightScale)
								+ baseY,
						(int) (rec[current].getWidth() * widthScale),
						(int) (rec[current].getHeight() * heightScale));
			}
		}
		g.setColor(Color.WHITE);
		if (System.currentTimeMillis() >= nextFrame) {
			long millis = System.currentTimeMillis() - nextFrame + 1000;
			fps = (frames * 1000) / millis;
			frames = 0;
			nextFrame = System.currentTimeMillis() + 1000;
		}
		g.drawString("FPS: " + fps, 50, 50);
		g.drawString("Current camera: " + current, 50, 70);
		g.dispose();
		buffer.show();
	}

	public static void main(String[] args) throws IOException {
		String[] myPath = System.getProperty("java.class.path").split(";");
		for (int i = 0; i < myPath.length; i++) {
			myPath[i] = new File(myPath[i]).getAbsolutePath();
		}
		String libPath = myPath[0];
		System.out.println(libPath);
		if (libPath.endsWith(".jar")) {
			String dest = "";
			extractDLLs(libPath, dest = new File(libPath).getParent());
			libPath = dest;
			System.out.println(libPath);
		}
		System.setProperty("net.java.games.input.librarypath", libPath);
		new MultiCameraViewer(new String[] { "10.19.83.11", "10.19.83.12" });
	}

	public static void extractDLLs(String jPath, String dest)
			throws IOException {
		JarFile jar = new JarFile(jPath);
		Enumeration<JarEntry> en = jar.entries();
		byte[] buffer = new byte[2048];
		while (en.hasMoreElements()) {
			JarEntry entry = en.nextElement();
			if (entry.getName().endsWith("dll")) {
				String[] parts = entry.getName().split("/");
				String file = parts[parts.length - 1];
				File f = new File(dest, file);
				if (!f.exists()) {
					f.createNewFile();
					BufferedOutputStream fO = new BufferedOutputStream(
							new FileOutputStream(f));
					InputStream fIn = new BufferedInputStream(
							jar.getInputStream(entry));
					int nBytes = -1;
					while ((nBytes = fIn.read(buffer)) > 0) {
						fO.write(buffer, 0, nBytes);
					}
					fO.close();
					fIn.close();
					System.out.println("Extracted " + file);
				}
			}
		}
	}

	@SuppressWarnings("deprecation")
	public void dispose() {
		disposing = true;
		if (t != null && t.isAlive()) {
			try {
				t.join();
			} catch (InterruptedException e) {
				t.stop();
			}
		}
		super.dispose();
	}

	@Override
	public void mouseClicked(MouseEvent arg0) {
	}

	@Override
	public void mouseEntered(MouseEvent arg0) {
	}

	@Override
	public void mouseExited(MouseEvent arg0) {

	}

	@Override
	public void mousePressed(MouseEvent arg0) {
		if (start[current] == null)
			start[current] = arg0.getPoint();
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		double minX = Math.min(e.getX(), start[current].x);
		double minY = Math.min(e.getY(), start[current].y);
		double maxX = Math.max(e.getX(), start[current].x);
		double maxY = Math.max(e.getY(), start[current].y);
		start[current] = null;
		if (img != null && baseX != -1)
			rec[current] = new Rectangle2D.Double((minX / widthScale) - baseX,
					(minY / heightScale) - baseY, (maxX - minX) / widthScale,
					(maxY - minY) / heightScale);
	}
}
