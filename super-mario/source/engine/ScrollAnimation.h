#ifndef _SCROLLANIMATION_H_
#define _SCROLLANIMATION_H_

#include <iostream>
#include <vector>

#include "./Animation.h"

struct ScrollEntry {
	int dx = 0;
	int dy = 0;
	unsigned delay = 0;
};

class ScrollAnimation : public Animation {
public:
	using Scroll = std::vector<ScrollEntry>;
private:
	Scroll scroll;
public:
	const Scroll& GetScroll(void) const;
	void SetScroll(const Scroll& p);
	Animation* Clone(void) const override;
	ScrollAnimation(const std::string& _id, const Scroll& _scroll);
};

#endif _SCROLLANIMATION_H_