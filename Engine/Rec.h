#pragma once
class Rec {
public:
	Rec(int l, int r, int t, int b)
		:
		left(l),
		right(r),
		top(t),
		bottom(b)
	{}
	int left;
	int right;
	int top;
	int bottom;
	bool Contains(int x, int y) {
		if (x > left && x < right && y < bottom && y > top)
			return true;
		else
			return false;
	}
};

