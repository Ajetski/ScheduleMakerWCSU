

bool val_in(unsigned int val, unsigned int list[], unsigned int length) {
	for (unsigned int i = 0; i < length; i++) {
		if (val == list[i]) return true;
	}
	return false;
}
