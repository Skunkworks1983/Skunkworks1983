package com.pi.glyph;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.List;

import javax.imageio.ImageIO;

import com.pi.glyph.filter.FastMath;
import com.pi.glyph.filter.LineFilter;
import com.pi.glyph.filter.LinePairFinder;
import com.pi.glyph.filter.VerticalLines;

public class GlyphTest {
    public static int minLength = 25;
    public static int threshold = 225;
    public static int maxLines = 5;

    public static void main(String[] args) throws IOException {
	BufferedImage image = ImageIO.read(new File(
		"/home/westin/java-workspace/GlyphTracking/IR sensor.png"));
	BufferedImage outI = new BufferedImage(image.getWidth(),
		image.getHeight(), BufferedImage.TYPE_INT_RGB);
	long start = System.nanoTime();
	Point[] lines = VerticalLines.getVerticalLines(image, threshold,
		maxLines);
	System.out.println((System.nanoTime() - start) / 1000000d);
	start = System.nanoTime();
	List<Point[]> lines2 = LineFilter.filterLines(lines, minLength,
		minLength);
	System.out.println((System.nanoTime() - start) / 1000000d);
	start = System.nanoTime();
	Point[] topPair = LinePairFinder.getTopPair(lines2, 50);
	System.out.println((System.nanoTime() - start) / 1000000d);
	start = System.nanoTime();
	Graphics g = outI.getGraphics();
	g.setColor(Color.WHITE);
	for (int i = 0; i < lines2.size(); i++) {
	    Point[] line = lines2.get(i);
	    g.drawString(i + "", line[0].x, line[0].y);
	    g.drawLine(line[0].x, line[0].y, line[1].x, line[1].y);
	}
	g.setColor(Color.GREEN);
	if (topPair != null) {
	    g.drawLine(topPair[0].x, topPair[0].y, topPair[1].x, topPair[1].y);
	}

	g.dispose();
	ImageIO.write(outI, "PNG", new File(
		"/home/westin/java-workspace/GlyphTracking/IR sensor_edge.png"));
    }

    public static int distanceX(Point a, Point b) {
	return FastMath.abs(a.x - b.x);
    }

    public static int distanceY(Point a, Point b) {
	return FastMath.abs(a.y - b.y);
    }
}
