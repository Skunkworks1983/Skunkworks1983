package com.pi.glyph;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.List;

import javax.imageio.ImageIO;

public class GlyphTest {
    public static int minLength = 25;
    public static int threshold = 225;
    public static int maxLines = 5;

    public static void main(String[] args) throws IOException {
	BufferedImage image = ImageIO.read(new File(
		"/home/westin/Documents/IR sensor.bmp"));
	BufferedImage outI = new BufferedImage(image.getWidth(),
		image.getHeight(), BufferedImage.TYPE_INT_RGB);
	List<Point[]> lines = VerticalLines.getVerticalLines(image, threshold,
		maxLines);
	lines = LineFilter.filterLines(lines, minLength, minLength);
	Graphics g = outI.getGraphics();
	g.setColor(Color.WHITE);
	System.out.println(lines.size());
	for (int i = 0; i < lines.size(); i++) {
	    Point[] p = lines.get(i);
	    g.drawString(i + "", p[0].x, p[0].y);
	    g.drawLine(p[0].x, p[0].y, p[1].x, p[1].y);
	}
	g.dispose();
	ImageIO.write(outI, "PNG", new File(
		"/home/westin/Documents/IR sensor_edge.png"));
    }

    public static int distanceX(Point a, Point b) {
	return Math.abs(a.x - b.x);
    }

    public static int distanceY(Point a, Point b) {
	return Math.abs(a.y - b.y);
    }
}
