package com.pi.glyph.filter;

import java.awt.Point;
import java.util.List;

public class LinePairFinder {
    public static Point[] getTopPair(List<Point[]> lines, int distanceLimit) {
	int bestIDX = -1;
	int bestY = Integer.MAX_VALUE;
	for (int i = 0; i < lines.size(); i++) {
	    Point[] dat = lines.get(i);
	    if (dat[0].y < bestY) {
		bestY = dat[0].y;
		bestIDX = i;
	    }
	}
	if (bestIDX == -1)
	    return null;
	if (bestIDX == lines.size() - 1)
	    return null;
	Point[] me = lines.get(bestIDX);
	int bestCompare = -1;
	double bestVal = Double.MAX_VALUE;
	for (int i = 0; i < lines.size(); i++) {
	    if (i == bestIDX)
		continue;
	    double cVal = compare(lines.get(i), me, distanceLimit);
	    if (cVal < bestVal) {
		bestVal = cVal;
		bestCompare = i;
	    }
	}
	if (bestCompare == -1)
	    return null;
	return new Point[] { me[0], lines.get(bestCompare)[0] };
    }

    public static double parallelCompare(Point[] lineA, Point[] lineB) {
	double slA = ((double) (lineA[0].y - lineA[1].y))
		/ ((double) (lineA[0].x - lineA[1].x));
	double slB = ((double) (lineB[0].y - lineB[1].y))
		/ ((double) (lineB[0].x - lineB[1].x));
	return Math.abs(slA - slB);
    }

    private static double compare(Point[] lineA, Point[] lineB,
	    int yDistanceLimit) {
	double parallaxCompare = parallelCompare(lineA, lineB) * 5;
	int dist = FastMath.abs(lineA[0].x - lineB[0].x)
		- FastMath.abs(lineA[1].x - lineB[1].x);
	int yDist = FastMath.abs(lineA[0].y - lineB[0].y);
	if (yDist > yDistanceLimit)
	    return Double.MAX_VALUE;
	return parallaxCompare + dist;

    }
}
