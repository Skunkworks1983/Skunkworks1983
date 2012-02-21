package com.pi.glyph;

import java.awt.Point;
import java.util.LinkedList;
import java.util.List;

public class LineFilter {
    public static List<Point[]> filterLines(List<Point[]> lines,
	    int minYLength, int maxPairDist) {
	List<Point[]> newLines = new LinkedList<Point[]>();
	boolean[] exclude = new boolean[lines.size()];
	for (int i = 0; i < lines.size(); i++) {
	    Point[] line = lines.get(i);
	    int mDist = Math.abs(line[0].y - line[1].y);
	    if (mDist <= minYLength || exclude[i]) {
		continue;
	    }
	    double bestD = maxPairDist;//Double.MAX_VALUE;
	    // int oDist = -1;
	    Point[] other = null;
	    int bestI = -1;
	    for (int iD = 0; iD < lines.size(); iD++) {
		if (iD != i && !exclude[iD]) {
		    Point[] oL = lines.get(iD);
		    int tDist = Math.abs(oL[0].y - oL[1].y);
		    if (tDist > minYLength) {
			int nD = Math.abs(line[0].x - oL[0].x)
				+ Math.abs(line[0].y - oL[0].y);
			double score = /*LinePairFinder.parallelCompare(line, oL) + */nD;
			if (nD < maxPairDist && score < bestD) {
			    bestD = score;
			    // oDist = tDist;
			    bestI = iD;
			    other = oL;
			}
		    }
		}
	    }
	    if (bestI != -1) {
		exclude[i] = true;
		exclude[bestI] = true;
		if (line[0].y < other[0].y) {
		    newLines.add(line);
		} else {
		    newLines.add(other);
		}
	    }
	}
	return newLines;
    }
}
