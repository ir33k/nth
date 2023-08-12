#include "walter.h"

TEST("Should print help message")
{
	RUN("./nth -h",     0, "snap/help", "snap/empty", 0);
	RUN("./nth -help",  0, "snap/help", "snap/empty", 0);
	RUN("./nth --help", 0, "snap/help", "snap/empty", 0);
	RUN("./nth --h",    0, "snap/help", "snap/empty", 0);
}

TEST("Should print lines from top")
{
	RUN("./nth  1", "snap/input", "snap/input01", 0, 0);
	RUN("./nth  5", "snap/input", "snap/input05", 0, 0);
	RUN("./nth 15", "snap/input", "snap/input15", 0, 0);
}

TEST("Should print lines from bottom")
{
	RUN("./nth  -1", "snap/input", "snap/input15", 0, 0);
	RUN("./nth -11", "snap/input", "snap/input05", 0, 0);
	RUN("./nth -15", "snap/input", "snap/input01", 0, 0);
}

TEST("Should work with stdin and input file")
{
	RUN("./nth 1", "snap/input", "snap/input01", 0, 0);
	RUN("./nth 1 snap/input", 0, "snap/input01", 0, 0);
}

TEST("Should print last line by default")
{
	RUN("./nth", "snap/input", "snap/input15", 0, 0);
}

TEST("Should print nothing when line number is too big")
{
	RUN("./nth  20", "snap/input", "snap/empty", 0, 1);
	RUN("./nth -20", "snap/input", "snap/empty", 0, 1);
}
