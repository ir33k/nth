#include "walter.h"

TEST("Should print help message")
{
	RUN("./nth  -h",    0, "snap/help", "snap/empty", 0);
	RUN("./nth  -help", 0, "snap/help", "snap/empty", 0);
	RUN("./nth --help", 0, "snap/help", "snap/empty", 0);
	RUN("./nth --h",    0, "snap/help", "snap/empty", 0);
}

TEST("Should print lines from top")
{
	RUN("./nth  1", "snap/input", "snap/input01", "snap/empty", 0);
	RUN("./nth  5", "snap/input", "snap/input05", "snap/empty", 0);
	RUN("./nth 15", "snap/input", "snap/input15", "snap/empty", 0);
}

TEST("Should print lines from bottom")
{
	RUN("./nth  -1", "snap/input", "snap/input15", "snap/empty", 0);
	RUN("./nth -11", "snap/input", "snap/input05", "snap/empty", 0);
	RUN("./nth -15", "snap/input", "snap/input01", "snap/empty", 0);
}

TEST("Should work with stdin and input file")
{
	RUN("./nth 1", "snap/input", "snap/input01", "snap/empty", 0);
	RUN("./nth 1 snap/input", 0, "snap/input01", "snap/empty", 0);
}

TEST("Should print last line by default")
{
	RUN("./nth", "snap/input", "snap/input15", "snap/empty", 0);
}

TEST("Should ignore invalid line number")
{
	/* It will think that "0" is a file path. */
	SRUN("./nth 0",   "Lorem ipsum", "", "fopen: No such file or directory\n", 1);
	SRUN("./nth aaa", "Lorem ipsum", "", "fopen: No such file or directory\n", 1);
	/* Last arg is taken as file path so in this case 0 is
	 * ignored, it should work like default case giving last
	 * line. */
	RUN("./nth 0   snap/input", 0, "snap/input15", "snap/empty", 0);
	RUN("./nth aaa snap/input", 0, "snap/input15", "snap/empty", 0);
}

TEST("Should print nothing when line number is too big")
{
	RUN("./nth  20", "snap/input", "snap/empty", "snap/empty", 1);
	RUN("./nth -20", "snap/input", "snap/empty", "snap/empty", 1);
}

TEST("Should return nothing when input is missing")
{
	RUN("./nth", "snap/empty", "snap/empty", "snap/empty", 1);
	RUN("./nth", 0,            "snap/empty", "snap/empty", 1);
}
