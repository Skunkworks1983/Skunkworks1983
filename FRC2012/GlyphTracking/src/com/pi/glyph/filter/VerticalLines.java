package com.pi.glyph.filter;

import java.awt.Point;
import java.awt.image.BufferedImage;
import java.awt.image.Raster;

public class VerticalLines {
    // private static final int broken = 1;

    public static Point[] getVerticalLines(BufferedImage image, int threshold,
	    int maxLines) {
	int height = image.getHeight();
	int width = image.getWidth();
	boolean[][] out = new boolean[width][height];
	Raster rast = image.getRaster();

	boolean prev = false;
	for (int y = 0; y < height; y++) {
	    for (int x = 0; x < width; x++) {
		int grey = rast.getSample(x, y, 1);
		boolean val = grey >= threshold;
		out[x][y] = (x > 0 && val != prev);
		prev = val;
		if (out[x][y] && y > 0) {
		    // Stupid issues: broken
		    // if (broken == 1) {

		    /*
		     * for (int side = -1; side <= 1; side += 2) { if (x + side
		     * < 0 || x + side >= width) continue; boolean isGood =
		     * false; int off; for (off = 1; off < 4; off++) { if (y -
		     * off < 0) break; else if (out[x][y - off]) { isGood =
		     * true; break; } else if (!out[x + side][y - off]) break; }
		     * if (isGood) { for (int yO = 1; y <= off; y++) { out[x +
		     * side][y - yO] = false; out[x][y - yO] = true; } break; }
		     * } /* } else if (broken == 2) { for (int yO = 1; yO < 4;
		     * yO++) { if (y < yO + 1 || !out[x][y - yO - 1]) break; if
		     * (x > 0) { boolean good = true; for (int off = 1; off <=
		     * yO; off++) { if (!out[x - 1][y - off]) { good = false;
		     * break; } } if (good) { for (int off = 1; off <= yO;
		     * off++) { out[x][y - off] = true; out[x - 1][y - off] =
		     * false; } } } if (x < width - 1) { boolean good = true;
		     * for (int off = 1; off <= yO; off++) { if (!out[x + 1][y -
		     * off]) { good = false; break; } } if (good) { for (int off
		     * = 1; off <= yO && good; off++) { out[x][y - off] = true;
		     * out[x + 1][y - off] = false; } } } } }
		     */

		}
	    }
	}
	byte[][] lineDir = new byte[width][height];
	int[][] lineMap = new int[width][height];
	Point[] lines = new Point[maxLines * Constants.POINT_STRIDE];
	int lineCount = 0;
	boolean edit = false;

	for (int y = 1; y < height; y++) {
	    edit = false;
	    int yM1 = y - 1;
	    for (int x = 2; x < width - 2; x++) {
		if (/* y > 0 && */out[x][y]) {
		    for (byte xO = -1; xO <= 1; xO++) {
			int xN = x + xO;
			// if (xN >= 0 && xN < width - 1) {
			if ((out[xN][yM1] || out[xN + xO][yM1])
				&& (lineDir[xN][yM1] == 0 || xO == 0 || lineDir[xN][yM1] == xO)) {
			    lineDir[x][y] = xO == 0 ? lineDir[xN][yM1] : xO;
			    if (lineMap[xN][yM1] == 0) {
				if (lineCount >= lines.length - 1) {
				    break;
				}
				lines[lineCount] = new Point(xN, yM1);
				lineMap[xN][yM1] = lineCount + 1;
				lineCount += Constants.POINT_STRIDE;
			    }
			    lineMap[x][y] = lineMap[xN][yM1];
			    lines[lineMap[xN][yM1]] = new Point(x, y);
			    edit = true;
			    break;
			}
			// }
		    }
		}
	    }
	    if (!edit && lineCount >= lines.length - 1) {
		return lines;
	    }
	}
	if (lineCount < lines.length - 1) {
	    Point[] nLines = new Point[lineCount];
	    System.arraycopy(lines, 0, nLines, 0, lineCount);
	    return nLines;
	}
	return lines;
    }
}
