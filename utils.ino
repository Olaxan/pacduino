int wrap(int val, int const lower, int const upper)
{
	int range_size = upper - lower + 1;

	if (val < lower)
		val += range_size * ((lower - val) / range_size + 1);

	return lower + (val - lower) % range_size;
}

int read_axis(int axisNumber) 
{
	int distance = 0; 

	int reading = analogRead(axis[axisNumber]);

	if (reading < minima[axisNumber]) {
		minima[axisNumber] = reading;
	}
	if (reading > maxima[axisNumber]) {
		maxima[axisNumber] = reading;
	}

	reading = map(reading, minima[axisNumber], maxima[axisNumber], 0, range);

	if (abs(reading - center) > threshold) {
		distance = (reading - center);
	}

	if (axisNumber == 1) {
		distance = -distance;
	}

	return distance;
}

int sign(int val) 
{
	if (val < 0) return -1;
	if (val == 0) return 0;
	return 1;
}
