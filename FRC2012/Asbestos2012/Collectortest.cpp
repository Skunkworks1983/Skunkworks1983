bool lowToMid, midToTop;

void update() 
{
	bool m_midToTop,m_lowToMid;
	
	m_midToTop = !TOPSLOT && MIDSLOT;
	m_lowToMid = !MIDSLOT && LOWSLOT;
	
	if(midToTop)
	{
		collectorVicTop.Set(1.0);
		collectorVicBottom.Set(1.0);
	}
	else if(lowToMid)
	{
		collectorVicTop.Set(0.0);
		collectorVicBottom.Set(1.0);
		if(MIDSLOT)
	}
	else
	{
		collectorVicTop.Set(0.0);
		collectorVicBottom.Set(0.0);
		midToTop = !TOPSLOT && MIDSLOT;
		lowToMid = !MIDSLOT && LOWSLOT;
	}
}
