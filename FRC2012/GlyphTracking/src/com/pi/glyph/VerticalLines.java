package com.pi.glyph;

import java.awt.Color;
import java.awt.Point;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.imageio.ImageIO;

public class VerticalLines {
    private static final int broken = 1;

    public static List<Point[]> getVerticalLines(BufferedImage image,
	    int threshold, int maxLines) {
	List<Point[]> lines = new ArrayList<Point[]>(maxLines);

	int height = image.getHeight();
	int width = image.getWidth();
	boolean[][] out = new boolean[width][height];

	boolean prev = false;
	for (int y = 0; y < height; y++) {
	    for (int x = 0; x < width; x++) {
		Color col = new Color(image.getRGB(x, y));
		int grey = (col.getRed() + col.getBlue() + col.getGreen()) / 3;
		boolean val = grey >= threshold;
		out[x][y] = (x > 0 ? (val != prev) : false);
		prev = val;
		if (out[x][y] && y > 0) {
		    // Stupid issues: broken
		    if (broken == 1) {
			for (int side = -1; side <= 1; side += 2) {
			    if (x + side < 0 || x + side >= width)
				continue;
			    boolean isGood = false;
			    int off;
			    for (off = 1; off < 4; off++) {
				if (y - off < 0)
				    break;
				else if (out[x][y - off]) {
				    isGood = true;
				    break;
				} else if (!out[x + side][y - off])
				    break;
			    }
			    if (isGood) {
				for (int yO = 1; y <= off; y++) {
				    out[x + side][y - yO] = false;
				    out[x][y - yO] = true;
				}
				break;
			    }
			}
		    } else if (broken == 2) {
			for (int yO = 1; yO < 4; yO++) {
			    if (y < yO + 1 || !out[x][y - yO - 1])
				break;
			    if (x > 0) {
				boolean good = true;
				for (int off = 1; off <= yO; off++) {
				    if (!out[x - 1][y - off]) {
					good = false;
					break;
				    }
				}
				if (good) {
				    for (int off = 1; off <= yO; off++) {
					out[x][y - off] = true;
					out[x - 1][y - off] = false;
				    }
				}
			    }
			    if (x < width - 1) {
				boolean good = true;
				for (int off = 1; off <= yO; off++) {
				    if (!out[x + 1][y - off]) {
					good = false;
					break;
				    }
				}
				if (good) {
				    for (int off = 1; off <= yO && good; off++) {
					out[x][y - off] = true;
					out[x + 1][y - off] = false;
				    }
				}
			    }
			}
		    }
		}
	    }
	}
	byte[][] lineDir = new byte[width][height];
	int[][] lineMap = new int[width][height];

	out: for (int y = 0; y < height; y++) {
	    for (int x = 0; x < width; x++) {
		if (y > 0 && out[x][y]) {
		    for (byte xO = -1; xO <= 1; xO++) {
			int xN = x + xO;
			if (xN >= 0 && xN < width - 1) {
			    if (out[xN][y - 1]
				    && (lineDir[xN][y - 1] == 0 || xO == 0 || lineDir[xN][y - 1] == xO)) {
				lineDir[x][y] = xO == 0 ? lineDir[xN][y - 1]
					: xO;
				Point[] line;
				if (lineMap[xN][y - 1] == 0) {
				    lineMap[xN][y - 1] = lines.size() + 1;
				    lines.add(line = new Point[2]);
				    line[0] = new Point(xN, y - 1);
				} else {
				    line = lines.get(lineMap[xN][y - 1] - 1);
				}
				lineMap[x][y] = lineMap[xN][y - 1];
				line[1] = new Point(x, y);
				break;
			    }
			}
		    }
		}
		if (lines.size() > maxLines) {
		    break out;
		}
	    }
	}
	return lines;
    }
}
