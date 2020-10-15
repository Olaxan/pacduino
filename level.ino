
// Example to read a two dimensional array.
//

/*
 * Read a file one field at a time.
 *
 * file - File to read.
 *
 * str - Character array for the field.
 *
 * size - Size of str array.
 *
 * delim - String containing field delimiters.
 *
 * return - length of field including terminating delimiter.
 *
 * Note, the last character of str will not be a delimiter if
 * a read error occurs, the field is too long, or the file
 * does not end with a delimiter.	Consider this an error
 * if not at end-of-file.
 *
 */
size_t read_field(File* file, char* str, size_t size, char* delim) 
{
	char ch;
	size_t n = 0;
	while ((n + 1) < size && file->read(&ch, 1) == 1) 
	{
		// Delete CR.
		if (ch == '\r') 
			continue;

		str[n++] = ch;
		if (strchr(delim, ch)) 
			break;
	}
	str[n] = '\0';
	return n;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void read_level(String path, int columns, int rows, int array[]) 
{
	Serial.println("Reading level data from " + path + ".");
	
	// Create or open the file.
	File file = SD.open(path, FILE_WRITE);
	if (!file) 
		error("Failed to open level file.");
	
	// Rewind file so test data is not appended.
	file.seek(0);

	// Array for data.
	int i = 0;		// First array index.
	int j = 0;		// Second array index
	size_t n;		// Length of returned field with delimiter.
	char str[20];		// Must hold longest field with delimiter and zero byte.
	char *ptr;		// Test for valid field.

	// Read the file and store the data.
	
	for (i = 0; i < rows; i++) 
	{
		for (j = 0; j < columns; j++) 
		{
			n = read_field(&file, str, sizeof(str), ",\n");
			if (n == 0) 
				error("Unexpected line count in level file.");

			array[i * columns + j] = strtol(str, &ptr, 10);
			if (ptr == str) 
				error("Invalid tile index in level file.");

			if (j < (columns-1) && str[n-1] != ',') 
				error("Invalid field length in level file.");

		}

		// Allow missing endl at eof.
		if (str[n-1] != '\n' && file.available()) 
			error("missing endl");

	}

	Serial.println("Read map " + path + " from SD card.");
	file.close();
}
//------------------------------------------------------------------------------
