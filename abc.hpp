namespace {
	int one() {
		return 1;
	}
}

// static int one() {
// 	return 1;
// }

int two() {
	return one() + one();
}