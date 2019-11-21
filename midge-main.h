int
main()
{
	if (midge_init())
		return 1;
	while (midge_next() == 0) {
		midge_dispatch();
	}
	midge_done();
	return 0;
}
