package com.pi.glyph.filter;

import java.awt.Point;
import java.util.ArrayList;
import java.util.List;

public class FindQuads {
    public static List<Point[]> findQuads(List<Point[]> lines) {
	List<Point[]> quads = new ArrayList<Point[]>();
	boolean[] exclude = new boolean[quads.size()];
	for (int i = 0; i < lines.size(); i++) {
	    if (exclude[i])
		continue;
	    Point[] line = lines.get(i); // Find the closest parallel
	    double bDSum = 999999;
	    int bPair = -1;
	    Point[] pairL = null;
	    for (int iP = 0; iP < lines.size(); iP++) {
		if (i == iP || exclude[iP])
		    continue;
		Point[] oL = lines.get(iP);
		double dSum = FastMath.abs(line[0].x - oL[0].x)
			+ FastMath.abs(line[1].x - oL[1].x);
		if (dSum < bDSum) {
		    bDSum = dSum;
		    bPair = iP;
		    pairL = oL;
		}
	    }
	    if (pairL == null || line == null)
		continue;
	    exclude[i] = true;
	    exclude[bPair] = true;
	    quads.add(new Point[] { line[0], line[1], pairL[1], pairL[0] });
	}
	return quads;
    }
}
