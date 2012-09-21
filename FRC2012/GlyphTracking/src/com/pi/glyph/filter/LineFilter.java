package com.pi.glyph.filter;

import java.awt.Point;
import java.util.LinkedList;
import java.util.List;

public class LineFilter {
    public static List<Point[]> filterLines(Point[] lines, int minYLength,
	    int maxPairDist) {
	List<Point[]> newLines = new LinkedList<Point[]>();
	int lineCount = lines.length / Constants.POINT_STRIDE;
	boolean[] exclude = new boolean[lineCount];
	for (int i = 0, r = 0; i < lines.length; i += Constants.POINT_STRIDE, r++) {
	    int mDist = FastMath.abs(lines[i].y - lines[i + 1].y);
	    if (mDist <= minYLength || exclude[r]) {
		continue;
	    }
	    double bestD = maxPairDist;// Double.MAX_VALUE;
	    int bestI = -1;
	    int bestR = -1;
	    for (int iD = 0, iR = 0; iD < lines.length; iD += Constants.POINT_STRIDE, iR++) {
		if (iD != i && !exclude[iR]) {
		    int tDist = FastMath.abs(lines[iD].y - lines[iD + 1].y);
		    if (tDist > minYLength) {
			int nD = FastMath.abs(lines[i].x - lines[iD].x)
				+ FastMath.abs(lines[i].y - lines[iD].y);
			double score = /*
				        * LinePairFinder.parallelCompare(line,
				        * oL) +
				        */nD;
			if (nD < maxPairDist && score < bestD) {
			    bestD = score;
			    // oDist = tDist;
			    bestI = iD;
			    bestR = iR;
			}
		    }
		}
	    }
	    if (bestI != -1) {
		exclude[r] = true;
		exclude[bestR] = true;
		Point[] lCopy = new Point[Constants.POINT_STRIDE + Constants.POINT_STRIDE];
		if (lines[i].y < lines[bestI].y) {
		    System.arraycopy(lines, i, lCopy, 0, 2);
		    System.arraycopy(lines, bestI, lCopy, 2, 2);
		} else {
		    System.arraycopy(lines, bestI, lCopy, 0, 2);
		    System.arraycopy(lines, i, lCopy, 2, 2);
		}
		newLines.add(lCopy);
	    }
	}
	return newLines;
    }
}
