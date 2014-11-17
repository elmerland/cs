int func(int p0, int p1) {	
		int v0;
		if ((p0 - p1) <= 1)  {
			v0 = p0 + p1;
		}
		else {
			if ((p0 + p1) > 10) {
				v0 = 2 *p0;
			}
			else {
				v0 = 2*p1;
			}
		}
		return v0;
	}
