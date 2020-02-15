/* midge-main.h - default main() entry point for Midge
 *
 * Written in 2019 by Jon Mayo <jon@rm-f.net>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide.  This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along with
 * this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */
#ifndef MIDGE_MAIN_H_
#define MIDGE_MAIN_H_

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

#endif
