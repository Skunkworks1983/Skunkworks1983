package com.pi.glyph;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.JTabbedPane;
import javax.swing.JToggleButton;
import javax.swing.border.EmptyBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.JTextField;

public class GlyphConfig extends JFrame {
    private static final long serialVersionUID = 1L;

    public static final long getAutoPoseTimeout = 10000;
    
    private JPanel contentPane;
    private GlyphCam glyphCam;

    private JLabel lblThreshold;
    private JSlider threshold;
    private JLabel lblMaxLines;
    private JSlider maxLines;
    private JLabel lblMinLength;
    private JSlider minLength;
    private JLabel lblMaxMajorPair;
    private JSlider maxMinorPair;
    private JSlider maxMajorPair;
    private JLabel lblMaxMinorPair;
    private JPanel controlPane;
    private JToggleButton btnStart;
    private JLabel lblParallax;
    private JSlider parallax;
    private JLabel lblBasketOffset;
    private JTextField basketOffset;
    private JLabel lblPixHeight;
    private JTextField pixHeight;
    private JLabel lblPixelWidth;
    private JTextField pixWidth;
    private JButton btnReset_pose;
    private JButton btnApply_pose;
    private JPanel autoPosePane;
    private JLabel lblCurrentDepth;
    private JTextField currentDepth;
    private JLabel lblCurrentYaw;
    private JTextField currYaw;
    private JButton btnCalculate;
    private JLabel lblCurrentPixelHeight;
    private JButton btnGetPixelValues;
    private JTextField pixelHeight;
    private JLabel lblCurrentPixelWidth;
    private JTextField pixelWidth;
    private JLabel lblWidthCalibrate;
    private JTextField calibrateWidth;
    private JLabel lblHeightCalibrate;
    private JTextField calibrateHeight;
    private Component rigidArea_1;
    private JButton btnApply_autopose;

    /**
     * Create the frame.
     */
    public GlyphConfig(GlyphCam tGlyphCam) {
	setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	setBounds(100, 100, 450, 300);
	this.glyphCam = tGlyphCam;

	contentPane = new JPanel();
	contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
	contentPane.setLayout(new BorderLayout(0, 0));
	setContentPane(contentPane);

	JTabbedPane tabbedPane = new JTabbedPane(JTabbedPane.TOP);
	contentPane.add(tabbedPane, BorderLayout.CENTER);

	controlPane = new JPanel();
	tabbedPane.addTab("Control", null, controlPane, null);
	controlPane.setLayout(new GridLayout(4, 2, 0, 0));

	Component rigidArea = Box.createRigidArea(new Dimension(20, 20));
	controlPane.add(rigidArea);

	btnStart = new JToggleButton("Start");
	btnStart.addMouseListener(new MouseAdapter() {
	    @Override
	    public void mouseClicked(MouseEvent e) {
		glyphCam.pause(!glyphCam.isPaused());
		btnStart.setSelected(glyphCam.isPaused());
		btnStart.setText(glyphCam.isPaused() ? "Resume" : "Pause");
	    }
	});
	btnStart.setSelected(!glyphCam.isPaused());
	btnStart.setText(glyphCam.isPaused() ? "Resume" : "Pause");
	controlPane.add(btnStart);

	JPanel filterPane = new JPanel();
	tabbedPane.addTab("Filter", null, filterPane, null);
	filterPane.setLayout(new GridLayout(6, 2, 0, 0));

	lblThreshold = new JLabel("Threshold (0-255)");
	filterPane.add(lblThreshold);

	threshold = new JSlider();
	threshold.addChangeListener(new ChangeListener() {
	    @Override
	    public void stateChanged(ChangeEvent e) {
		lblThreshold.setText("Threshold (" + threshold.getValue() + ")");
	    }
	});

	threshold.setPaintTicks(true);
	threshold.setMinorTickSpacing(10);
	threshold.setMajorTickSpacing(50);
	threshold.setMaximum(255);
	filterPane.add(threshold);

	lblMaxLines = new JLabel("Max Lines (0-250)");
	filterPane.add(lblMaxLines);

	maxLines = new JSlider();
	maxLines.addChangeListener(new ChangeListener() {
	    @Override
	    public void stateChanged(ChangeEvent e) {
		lblMaxLines.setText("Max Lines (" + maxLines.getValue() + ")");
	    }
	});
	maxLines.setPaintTicks(true);
	maxLines.setMinorTickSpacing(10);
	maxLines.setMajorTickSpacing(50);
	maxLines.setMaximum(250);
	filterPane.add(maxLines);

	lblMinLength = new JLabel("Min Length (0-100)");
	filterPane.add(lblMinLength);

	minLength = new JSlider();
	minLength.addChangeListener(new ChangeListener() {
	    @Override
	    public void stateChanged(ChangeEvent e) {
		lblMinLength.setText("Min Length (" + minLength.getValue()
			+ ")");
	    }
	});
	minLength.setPaintTicks(true);
	minLength.setMinorTickSpacing(5);
	minLength.setMajorTickSpacing(20);
	filterPane.add(minLength);

	lblMaxMinorPair = new JLabel("Max Minor Pair Dist (0-250)");
	filterPane.add(lblMaxMinorPair);

	maxMinorPair = new JSlider();
	maxMinorPair.addChangeListener(new ChangeListener() {
	    @Override
	    public void stateChanged(ChangeEvent e) {
		lblMaxMinorPair.setText("Max Minor Pair Dist ("
			+ maxMinorPair.getValue() + ")");
	    }
	});
	maxMinorPair.setPaintTicks(true);
	maxMinorPair.setMinorTickSpacing(10);
	maxMinorPair.setMajorTickSpacing(50);
	maxMinorPair.setMaximum(250);
	filterPane.add(maxMinorPair);

	lblMaxMajorPair = new JLabel("Max Major Pair Dist (0-250)");
	filterPane.add(lblMaxMajorPair);

	maxMajorPair = new JSlider();
	maxMajorPair.addChangeListener(new ChangeListener() {
	    @Override
	    public void stateChanged(ChangeEvent e) {
		lblMaxMajorPair.setText("Max Major Pair Dist ("
			+ maxMajorPair.getValue() + ")");
	    }
	});
	maxMajorPair.setPaintTicks(true);
	maxMajorPair.setMinorTickSpacing(10);
	maxMajorPair.setMajorTickSpacing(50);
	maxMajorPair.setMaximum(250);
	filterPane.add(maxMajorPair);

	JButton btnReset = new JButton("Reset");
	btnReset.addMouseListener(new MouseAdapter() {
	    @Override
	    public void mouseClicked(MouseEvent arg0) {
		resetFilter();
	    }
	});
	filterPane.add(btnReset);

	JButton btnApply = new JButton("Apply");
	btnApply.addMouseListener(new MouseAdapter() {
	    @Override
	    public void mouseClicked(MouseEvent e) {
		applyFilter();
	    }
	});
	filterPane.add(btnApply);

	JPanel posePane = new JPanel();
	tabbedPane.addTab("Pose Estimation", null, posePane, null);
	posePane.setLayout(new GridLayout(0, 2, 0, 0));

	lblParallax = new JLabel("Parallax Y (0-480)");
	lblParallax.setToolTipText("Y value where all depths are the same Y");
	posePane.add(lblParallax);

	parallax = new JSlider();
	parallax.setPaintTicks(true);
	parallax.setMinorTickSpacing(20);
	parallax.setMajorTickSpacing(100);
	parallax.setMaximum(480);
	parallax.addChangeListener(new ChangeListener() {
	    @Override
	    public void stateChanged(ChangeEvent e) {
		lblParallax.setText("Parallax Y (" + parallax.getValue() + ")");
	    }
	});
	posePane.add(parallax);

	lblBasketOffset = new JLabel("Basket Offset");
	posePane.add(lblBasketOffset);

	basketOffset = new JTextField();
	posePane.add(basketOffset);
	basketOffset.setColumns(10);

	lblPixHeight = new JLabel("Pixel Height At Distance of 1");
	posePane.add(lblPixHeight);

	pixHeight = new JTextField();
	posePane.add(pixHeight);
	pixHeight.setColumns(10);

	lblPixelWidth = new JLabel("Pixel Width At Distance of 1");
	posePane.add(lblPixelWidth);

	pixWidth = new JTextField();
	posePane.add(pixWidth);
	pixWidth.setColumns(10);

	btnReset_pose = new JButton("Reset");
	btnReset_pose.addMouseListener(new MouseAdapter() {
	    @Override
	    public void mouseClicked(MouseEvent e) {
		resetPose();
	    }
	});
	posePane.add(btnReset_pose);

	btnApply_pose = new JButton("Apply");
	btnApply_pose.addMouseListener(new MouseAdapter() {
	    @Override
	    public void mouseClicked(MouseEvent e) {
		applyPose();
	    }
	});
	posePane.add(btnApply_pose);

	autoPosePane = new JPanel();
	tabbedPane.addTab("Auto Pose Estimation", null, autoPosePane, null);
	autoPosePane.setLayout(new GridLayout(8, 2, 0, 0));

	lblCurrentDepth = new JLabel("Current Depth");
	autoPosePane.add(lblCurrentDepth);

	currentDepth = new JTextField();
	autoPosePane.add(currentDepth);
	currentDepth.setColumns(10);

	lblCurrentYaw = new JLabel("Current Yaw");
	autoPosePane.add(lblCurrentYaw);

	currYaw = new JTextField();
	currYaw.setText("0");
	autoPosePane.add(currYaw);
	currYaw.setColumns(10);

	lblCurrentPixelHeight = new JLabel("Current Pixel Height");
	autoPosePane.add(lblCurrentPixelHeight);

	pixelHeight = new JTextField();
	autoPosePane.add(pixelHeight);
	pixelHeight.setColumns(10);

	lblCurrentPixelWidth = new JLabel("Current Pixel Width");
	autoPosePane.add(lblCurrentPixelWidth);

	pixelWidth = new JTextField();
	autoPosePane.add(pixelWidth);
	pixelWidth.setColumns(10);

	btnGetPixelValues = new JButton("Get Pixel Values");
	btnGetPixelValues.addMouseListener(new MouseAdapter() {
	    @Override
	    public void mouseClicked(MouseEvent e) {
		getAutoPose();
	    }
	});
	autoPosePane.add(btnGetPixelValues);

	btnCalculate = new JButton("Calculate");
	btnCalculate.addMouseListener(new MouseAdapter() {
	    @Override
	    public void mouseClicked(MouseEvent e) {
		calculateAutoPose();
	    }
	});
	autoPosePane.add(btnCalculate);

	lblWidthCalibrate = new JLabel("Pixels Per Width Unit");
	autoPosePane.add(lblWidthCalibrate);

	calibrateWidth = new JTextField();
	autoPosePane.add(calibrateWidth);
	calibrateWidth.setColumns(10);

	lblHeightCalibrate = new JLabel("Pixels Per Height Unit");
	autoPosePane.add(lblHeightCalibrate);

	calibrateHeight = new JTextField();
	autoPosePane.add(calibrateHeight);
	calibrateHeight.setColumns(10);

	rigidArea_1 = Box.createRigidArea(new Dimension(20, 20));
	autoPosePane.add(rigidArea_1);

	btnApply_autopose = new JButton("Apply");
	btnApply_autopose.addMouseListener(new MouseAdapter() {
	    @Override
	    public void mouseClicked(MouseEvent e) {
		applyAutoPose();
	    }
	});
	autoPosePane.add(btnApply_autopose);
	
	resetFilter();
	resetPose();
    }

    public void resetPose() {
	parallax.setValue(glyphCam.pose.parallaxY);
	basketOffset.setText(glyphCam.pose.getBasketDepth() + "");
	pixHeight.setText(glyphCam.pose.pixelHeightAtDistance1 + "");
	pixWidth.setText(glyphCam.pose.pixelWidthAtDistance1 + "");
    }

    public void applyPose() {
	double basketOff, pixelW, pixelH;
	try {
	    basketOff = Double.valueOf(basketOffset.getText());
	} catch (NumberFormatException e) {
	    JOptionPane.showMessageDialog(this, basketOffset.getText()
		    + " is not a number!");
	    return;
	}
	try {
	    pixelW = Double.valueOf(pixWidth.getText());
	} catch (NumberFormatException e) {
	    JOptionPane.showMessageDialog(this, pixWidth.getText()
		    + " is not a number!");
	    return;
	}
	try {
	    pixelH = Double.valueOf(pixHeight.getText());
	} catch (NumberFormatException e) {
	    JOptionPane.showMessageDialog(this, pixHeight.getText()
		    + " is not a number!");
	    return;
	}
	glyphCam.pose.parallaxY = parallax.getValue();
	glyphCam.pose.setBasketDepth(basketOff);
	glyphCam.pose.pixelHeightAtDistance1 = pixelH;
	glyphCam.pose.pixelWidthAtDistance1 = pixelW;
    }

    public void resetFilter() {
	threshold.setValue(glyphCam.threshold);
	maxLines.setValue(glyphCam.maxLines);
	minLength.setValue(glyphCam.minLength);
	maxMinorPair.setValue(glyphCam.maxMinorPairDistance);
	maxMajorPair.setValue(glyphCam.maxMajorPairDistance);
    }

    public void applyFilter() {
	glyphCam.threshold = threshold.getValue();
	glyphCam.maxLines = maxLines.getValue();
	glyphCam.minLength = minLength.getValue();
	glyphCam.maxMajorPairDistance = maxMajorPair.getValue();
	glyphCam.maxMinorPairDistance = maxMinorPair.getValue();
    }

    public void getAutoPose() {
	boolean prev = glyphCam.isPaused();
	glyphCam.pause(false);
	glyphCam.calibrating = true;
	long startTime = System.currentTimeMillis();
	while (glyphCam.currentPixelWidth == -1 || glyphCam.currentPixelHeight == -1){
	    if (System.currentTimeMillis() - startTime >= getAutoPoseTimeout){
		JOptionPane.showMessageDialog(this, "Auto Glyph Pose Timout!");
		glyphCam.pause(prev);
		glyphCam.calibrating = false;
		return;
	    }
	}
	pixelWidth.setText("" + glyphCam.currentPixelWidth);
	pixelHeight.setText("" + glyphCam.currentPixelHeight);
	glyphCam.pause(prev);
	glyphCam.calibrating = false;
    }

    public void calculateAutoPose() {
	double pixWidth, pixHeight, yaw,depth;
	try {
	    pixWidth = Double.valueOf(pixelWidth.getText());
	} catch (NumberFormatException e) {
	    JOptionPane.showMessageDialog(this, pixelWidth.getText()
		    + " is not a number!");
	    return;
	}
	try {
	    pixHeight = Double.valueOf(pixelHeight.getText());
	} catch (NumberFormatException e) {
	    JOptionPane.showMessageDialog(this, pixelHeight.getText()
		    + " is not a number!");
	    return;
	}
	try {
	    yaw = Double.valueOf(currYaw.getText());
	} catch (NumberFormatException e) {
	    JOptionPane.showMessageDialog(this, currYaw.getText()
		    + " is not a number!");
	    return;
	}
	try {
	    depth = Double.valueOf(currentDepth.getText());
	} catch (NumberFormatException e) {
	    JOptionPane.showMessageDialog(this, currentDepth.getText()
		    + " is not a number!");
	    return;
	}
	
	calibrateHeight.setText("" + (pixHeight/depth));
	yaw = Math.abs(yaw);
	calibrateWidth.setText("" + ((Math.cos(Math.toRadians(yaw)) * pixWidth) / depth));
    }

    public void applyAutoPose() {
	double cWidth,cHeight;
	try {
	    cWidth = Double.valueOf(calibrateWidth.getText());
	} catch (NumberFormatException e) {
	    JOptionPane.showMessageDialog(this, calibrateWidth.getText()
		    + " is not a number!");
	    return;
	}
	try {
	    cHeight = Double.valueOf(calibrateHeight.getText());
	} catch (NumberFormatException e) {
	    JOptionPane.showMessageDialog(this, calibrateHeight.getText()
		    + " is not a number!");
	    return;
	}
	glyphCam.pose.pixelHeightAtDistance1 = cHeight;
	glyphCam.pose.pixelWidthAtDistance1 = cWidth;
    }
}
