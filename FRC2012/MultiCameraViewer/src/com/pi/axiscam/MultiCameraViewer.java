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
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.net.Authenticator;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
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
	private Thread renderThread;
	private Thread joyPoller;
	private Thread imageFetcher;
	private Object imageSemaphore = new Object();

	public static int getPriority(float prior) {
		if (prior > 0) {
			float range = Thread.MAX_PRIORITY - Thread.NORM_PRIORITY;
			return (int) (range * prior) + Thread.NORM_PRIORITY;
		} else {
			float range = Thread.MIN_PRIORITY - Thread.NORM_PRIORITY;
			return (int) (range * prior) + Thread.NORM_PRIORITY;
		}
	}

	public void loadRectangles() {
		File f = new File("rectangles.txt");
		if (f.exists()) {
			try {
				BufferedReader read = new BufferedReader(new FileReader(f));
				while (read.ready()) {
					String line = read.readLine();
					String[] data = line.split(",");
					if (data.length == 5) {
						try {
							int id = Integer.valueOf(data[0]);
							float x = Float.valueOf(data[1]);
							float y = Float.valueOf(data[2]);
							float w = Float.valueOf(data[3]);
							float h = Float.valueOf(data[4]);
							if (id >= rec.length || id < 0) {
								System.out.println("Bad id: " + id);
							} else {
								rec[id] = new java.awt.geom.Rectangle2D.Float(
										x, y, w, h);
							}
							continue;
						} catch (NumberFormatException e) {
							System.out.println("Badly formatted number: "
									+ line);
						}
					} else {
						System.out.println("Badly formatted line: " + line);
					}
				}
				read.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public void saveRectangles() {
		File f = new File("rectangles.txt");
		try {
			if (!f.exists())
				f.createNewFile();
			if (f.exists()) {
				BufferedWriter write = new BufferedWriter(new FileWriter(f));
				for (int i = 0; i < rec.length; i++) {
					if (rec[i] != null) {
						String s = i + "," + String.valueOf(rec[i].getX())
								+ "," + String.valueOf(rec[i].getY()) + ","
								+ String.valueOf(rec[i].getWidth()) + ","
								+ String.valueOf(rec[i].getHeight());
						write.write(s);
						write.newLine();
					}
				}
				write.close();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public MultiCameraViewer(String[] ips) throws IOException {
		// Create frame
		super("Multiple Camera Viewer");
		setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
		setIgnoreRepaint(true);
		addMouseListener(this);
		setSize(640, 480);
		setLocation(0, 0);
		setVisible(true);

		// Build URLs
		buildURLs(ips);

		// Load rectangles
		loadRectangles();

		// Get Joystick
		joy = getJoystick();

		// Buffer creation
		createBufferStrategy(2);
		buffer = getBufferStrategy();

		// Auth
		Authenticator.setDefault(new CameraAuthenticator("root", "1983"));

		// Start joystick poll loop
		joyPoller = new Thread() {
			public void run() {
				while (isVisible() && !disposing) {
					pollJoystick();
					yield();
				}
			}
		};
		joyPoller.setPriority(getPriority(-0.75f));
		joyPoller.start();

		// Start render loop
		renderThread = new Thread() {
			public void run() {
				while (isVisible() && !disposing) {
					render();
				}
				disposing = true;
				// Dispose Buffer
				buffer.dispose();

				if (!disposing)
					dispose();
			}
		};
		renderThread.setPriority(getPriority(-0.25f));
		renderThread.start();

		imageFetcher = new Thread() {
			public void run() {
				while (isVisible() && !disposing) {
					fetchImage();
				}
			}
		};
		imageFetcher.setPriority(getPriority(0f));
		imageFetcher.start();
	}

	public Controller getJoystick() {
		Controller[] cont = ControllerEnvironment.getDefaultEnvironment()
				.getControllers();
		for (Controller c : cont) {
			if (c.getType() == Type.STICK) {
				return c;
			}
		}
		return null;
	}

	public void buildURLs(String[] ips) throws MalformedURLException {
		start = new Point[ips.length];
		rec = new Rectangle2D[ips.length];
		urls = new URL[ips.length];
		for (int i = 0; i < ips.length; i++) {
			urls[i] = new URL("http://" + ips[i] + "/jpg/1/image.jpg");
		}
	}

	public void pollJoystick() {
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

	boolean freshImage = false;

	int lCurrent = -1;
	BufferedImage img;
	float baseX = -1, baseY, widthScale, heightScale;
	boolean ioError = false;
	BufferedImage cache = null;
	boolean cacheError = false;
	private static final int SOCK_TIMEOUT = 500;

	public BufferedImage getImage(final URL url) throws IOException {
		cache = null;
		cacheError = false;
		URLConnection conn = url.openConnection();
		conn.setReadTimeout(SOCK_TIMEOUT);
		conn.setConnectTimeout(SOCK_TIMEOUT);
		conn.connect();
		InputStream in = conn.getInputStream();
		try {
			cache = ImageIO.read(in);
			frames++;
			ioError = false;
		} catch (IOException e) {
			cacheError = true;
		} finally {
			in.close();
		}
		ioError = ioError || cacheError;
		return cache;
	}

	int rX = 0, rY = 0, rWidth = 0, rHeight = 0;
	int rIX = 0, rIY = 0, rIW = 0, rIH = 0;

	public void compileRectangle() {
		if (rec[current] != null) {
			rX = Math.round(((float) rec[current].getX() * widthScale) + baseX);
			rY = Math
					.round(((float) rec[current].getY() * heightScale) + baseY);
			rWidth = Math.round((float) rec[current].getWidth() * widthScale);
			rHeight = Math
					.round((float) rec[current].getHeight() * heightScale);
		}
	}

	public void compileImage(BufferedImage img) {
		if (img != null) {
			float aspect = ((float) img.getWidth()) / ((float) img.getHeight());
			int estHeight = (int) (getWidth() / aspect);
			int estWidth = (int) (getHeight() * aspect);
			if (estHeight > getHeight()) {
				baseX = (getWidth() / 2F) - (estWidth / 2F);
				baseY = 0;
				widthScale = estWidth;
				heightScale = getHeight();
			} else {
				baseX = 0;
				baseY = (getHeight() / 2F) - (estHeight / 2F);
				widthScale = getWidth();
				heightScale = estHeight;
			}
			rIX = Math.round(baseX);
			rIY = Math.round(baseY);
			rIW = Math.round(widthScale);
			rIH = Math.round(heightScale);
		}
	}

	public void fetchImage() {
		BufferedImage img = null;
		try {
			img = getImage(urls[current]);
		} catch (IOException e) {
		} finally {
			if (img != null && img != this.img) {
				synchronized (imageSemaphore) {
					this.img = img;
					if (img != null) {
						if (lCurrent != current || rIW <= 0 || rIH <= 0) {
							lCurrent = current;
							compileImage(img);
							compileRectangle();
						}
					}
					freshImage = true;
					imageSemaphore.notify();
				}
			}
		}
	}

	public void render() {
		synchronized (imageSemaphore) {
			if (freshImage) {
				freshImage = false;
				Graphics g = buffer.getDrawGraphics();
				g.clearRect(0, 0, getWidth(), getHeight());
				if (img != null) {
					if (rIW > 0 && rIH > 0) {
						g.drawImage(img, rIX, rIY, rIW, rIH, null);
					}
					g.setColor(Color.RED);
					if (rec[current] != null && rWidth > 0 && rHeight > 0) {
						g.drawRect(rX, rY, rWidth, rHeight);
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
				if (ioError) {
					g.setColor(Color.RED);
					g.drawString("I/O Error!", 50, 90);
				}
				g.dispose();
				buffer.show();
			} else {
				try {
					imageSemaphore.wait();
				} catch (InterruptedException e) {
				}
			}
		}
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
		saveRectangles();
		disposing = true;
		if (renderThread != null && renderThread.isAlive()) {
			try {
				renderThread.join();
			} catch (InterruptedException e) {
				renderThread.stop();
			}
		}
		if (joyPoller != null && joyPoller.isAlive()) {
			try {
				joyPoller.join();
			} catch (InterruptedException e) {
				joyPoller.stop();
			}
		}
		if (imageFetcher != null && imageFetcher.isAlive()) {
			try {
				imageFetcher.join();
			} catch (InterruptedException e) {
				imageFetcher.stop();
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
		float minX = Math.min(e.getX(), start[current].x);
		float minY = Math.min(e.getY(), start[current].y);
		float maxX = Math.max(e.getX(), start[current].x);
		float maxY = Math.max(e.getY(), start[current].y);
		start[current] = null;
		if (img != null && baseX != -1) {
			rec[current] = new Rectangle2D.Float((minX - baseX) / widthScale,
					(minY - baseY) / heightScale, (maxX - minX) / widthScale,
					(maxY - minY) / heightScale);
			compileRectangle();
		}
	}
}
