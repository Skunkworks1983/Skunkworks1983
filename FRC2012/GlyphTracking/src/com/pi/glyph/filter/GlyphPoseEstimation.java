package com.pi.glyph.filter;

import java.awt.Point;

public class GlyphPoseEstimation {
    // 12.333333333
    /**
     * The y value that when a line is made between the actual 3D location and
     * the camera lens is parallel to the ground.
     */
    public int parallaxY = 470;

    /**
     * The pixel height between the parallaxY and the top of the reflective
     * rectangle of the top hoop at the distance of 1
     */
    public double pixelHeightAtDistance1 = 26.047297297d;

    /**
     * The pixel width of the reflective tape on the top hoop the distance of 1
     */
    public double pixelWidthAtDistance1 = 9.72972973d;

    /**
     * The distance from the backboard to the center of the hoop
     */
    private double basketDepth = 1.333333333333333d;

    /**
     * The square of the basket depth for law of cosines
     */
    private double basketDepthSQ = basketDepth * basketDepth;

    /**
     * The camera's field of view horizontally
     */
    private double cameraFOV = 47D;

    public double getBasketDepth() {
	return basketDepth;
    }

    public void setBasketDepth(double bDepth) {
	this.basketDepth = bDepth;
	this.basketDepthSQ = bDepth * bDepth;
    }

    public double getDepth(Point... screen) {
	double screenY = 0;
	for (Point p : screen) {
	    screenY += p.y;
	}
	screenY /= ((double) screen.length);
	double height = ((double) parallaxY) - screenY;
	return height / pixelHeightAtDistance1;
    }

    public double getDistance(Point a, Point b) {
	int xO = a.x - b.x;
	int yO = a.y - b.y;
	return Math.sqrt((xO * xO) + (yO * yO));
    }

    /**
     * Get yaw in radians
     * 
     * @param topLine
     * @param depth
     * @return
     */
    public double getRobotYaw(Point[] topLine, double depth) {
	double myWidth = getDistance(topLine[0], topLine[1]);
	double expectedWidth = pixelWidthAtDistance1 * depth;
	if (myWidth >= expectedWidth)
	    return 0;
	double angle = Math.acos(myWidth / expectedWidth);
	if (topLine[0].y <= topLine[1].y) {
	    angle = -angle;
	}
	return angle;
    }

    /**
     * Get the depth of the basket itself
     * 
     * @param depth
     * @param yaw in radians
     * @return
     */
    public double getBasketDepth(double depth, double yaw) {
	if (yaw == 0)
	    return depth - basketDepth;
	// Law of cosines
	return Math.sqrt((depth * depth) + basketDepthSQ
		+ (2d * depth * basketDepth * Math.cos(yaw)));
    }

    /**
     * Gets the basket yaw
     * 
     * @param p
     * @return
     */
    public double getBasketYaw(Point[] p) {
	int avgX = (p[0].x + p[1].x) / 2;
	avgX -= 320; // Distance from center
	double normalX = ((double) avgX) / 320D; // Scale to 0-1
	return normalX * (cameraFOV / 2D);
    }

    /**
     * Gets the yaw of the basket
     * 
     * @param calcBasketDepth
     * @param originalYaw
     * @return radians
     */
    /*
     * public double getBasketYaw(double calcBasketDepth, double originalYaw) {
     * if (originalYaw == 0) return originalYaw; // Law of sines return
     * originalYaw + Math.asin((Math.sin(originalYaw) / basketDepth)
     * calcBasketDepth); }
     */
}
