#pragma once
struct LandscapeLine {
	Vector2 p1, p2;
	bool landable;
	int multiplier;

	LandscapeLine(Vector2 a, Vector2 b) : p1(a), p2(b), landable(a.y == b.y), multiplier(1) {}
};